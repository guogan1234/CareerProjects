/* sodsLib 2.0 -- A C dynamic strings library
 *
 * Copyright (c) 2006-2015, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2015, Oran Agra
 * Copyright (c) 2015, Redis Labs, Inc
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include "sods.h"
#include "sodsalloc.h"

static inline int sodsHdrSize(char type) {
    switch(type&sods_TYPE_MASK) {
        case sods_TYPE_5:
            return sizeof(struct sodshdr5);
        case sods_TYPE_8:
            return sizeof(struct sodshdr8);
        case sods_TYPE_16:
            return sizeof(struct sodshdr16);
        case sods_TYPE_32:
            return sizeof(struct sodshdr32);
        case sods_TYPE_64:
            return sizeof(struct sodshdr64);
    }
    return 0;
}

static inline char sodsReqType(size_t string_size) {
    if (string_size < 1<<5)
        return sods_TYPE_5;
    if (string_size < 1<<8)
        return sods_TYPE_8;
    if (string_size < 1<<16)
        return sods_TYPE_16;
#if (LONG_MAX == LLONG_MAX)
    if (string_size < 1ll<<32)
        return sods_TYPE_32;
#endif
    return sods_TYPE_64;
}

/* Create a new sods string with the content specified by the 'init' pointer
 * and 'initlen'.
 * If NULL is used for 'init' the string is initialized with zero bytes.
 *
 * The string is always null-termined (all the sods strings are, always) so
 * even if you create an sods string with:
 *
 * mystring = sodsnewlen("abc",3);
 *
 * You can print the string with printf() as there is an implicit \0 at the
 * end of the string. However the string is binary safe and can contain
 * \0 characters in the middle, as the length is stored in the sods header. */
sods sodsnewlen(const void *init, size_t initlen) {
    void *sh;
    sods s;
    char type = sodsReqType(initlen);
    /* Empty strings are usually created in order to append. Use type 8
     * since type 5 is not good at this. */
    if (type == sods_TYPE_5 && initlen == 0) type = sods_TYPE_8;
    int hdrlen = sodsHdrSize(type);
    unsigned char *fp; /* flags pointer. */

    sh = s_malloc(hdrlen+initlen+1);
    if (!init)
        memset(sh, 0, hdrlen+initlen+1);
    if (sh == NULL) return NULL;
    s = (char*)sh+hdrlen;
    fp = ((unsigned char*)s)-1;
    switch(type) {
        case sods_TYPE_5: {
            *fp = type | (initlen << sods_TYPE_BITS);
            break;
        }
        case sods_TYPE_8: {
            sods_HDR_VAR(8,s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
            break;
        }
        case sods_TYPE_16: {
            sods_HDR_VAR(16,s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
            break;
        }
        case sods_TYPE_32: {
            sods_HDR_VAR(32,s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
            break;
        }
        case sods_TYPE_64: {
            sods_HDR_VAR(64,s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
            break;
        }
    }
    if (initlen && init)
        memcpy(s, init, initlen);
    s[initlen] = '\0';
    return s;
}

/* Create an empty (zero length) sods string. Even in this case the string
 * always has an implicit null term. */
sods sodsempty(void) {
    return sodsnewlen("",0);
}

/* Create a new sods string starting from a null terminated C string. */
sods sodsnew(const char *init) {
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sodsnewlen(init, initlen);
}

/* Duplicate an sods string. */
sods sodsdup(const sods s) {
    return sodsnewlen(s, sodslen(s));
}

/* Free an sods string. No operation is performed if 's' is NULL. */
void sodsfree(sods s) {
    if (s == NULL) return;
    s_free((char*)s-sodsHdrSize(s[-1]));
}

/* Set the sods string length to the length as obtained with strlen(), so
 * considering as content only up to the first null term character.
 *
 * This function is useful when the sods string is hacked manually in some
 * way, like in the following example:
 *
 * s = sodsnew("foobar");
 * s[2] = '\0';
 * sodsupdatelen(s);
 * printf("%d\n", sodslen(s));
 *
 * The output will be "2", but if we comment out the call to sodsupdatelen()
 * the output will be "6" as the string was modified but the logical length
 * remains 6 bytes. */
void sodsupdatelen(sods s) {
    size_t reallen = strlen(s);
    sodssetlen(s, reallen);
}

/* Modify an sods string in-place to make it empty (zero length).
 * However all the existing buffer is not discarded but set as free space
 * so that next append operations will not require allocations up to the
 * number of bytes previously available. */
void sodsclear(sods s) {
    sodssetlen(s, 0);
    s[0] = '\0';
}

/* Enlarge the free space at the end of the sods string so that the caller
 * is sure that after calling this function can overwrite up to addlen
 * bytes after the end of the string, plus one more byte for nul term.
 *
 * Note: this does not change the *length* of the sods string as returned
 * by sodslen(), but only the free buffer space we have. */
sods sodsMakeRoomFor(sods s, size_t addlen) {
    void *sh, *newsh;
    size_t avail = sodsavail(s);
    size_t len, newlen;
    char type, oldtype = s[-1] & sods_TYPE_MASK;
    int hdrlen;

    /* Return ASAP if there is enough space left. */
    if (avail >= addlen) return s;

    len = sodslen(s);
    sh = (char*)s-sodsHdrSize(oldtype);
    newlen = (len+addlen);
    if (newlen < sods_MAX_PREALLOC)
        newlen *= 2;
    else
        newlen += sods_MAX_PREALLOC;

    type = sodsReqType(newlen);

    /* Don't use type 5: the user is appending to the string and type 5 is
     * not able to remember empty space, so sodsMakeRoomFor() must be called
     * at every appending operation. */
    if (type == sods_TYPE_5) type = sods_TYPE_8;

    hdrlen = sodsHdrSize(type);
    if (oldtype==type) {
        newsh = s_realloc(sh, hdrlen+newlen+1);
        if (newsh == NULL) return NULL;
        s = (char*)newsh+hdrlen;
    } else {
        /* Since the header size changes, need to move the string forward,
         * and can't use realloc */
        newsh = s_malloc(hdrlen+newlen+1);
        if (newsh == NULL) return NULL;
        memcpy((char*)newsh+hdrlen, s, len+1);
        s_free(sh);
        s = (char*)newsh+hdrlen;
        s[-1] = type;
        sodssetlen(s, len);
    }
    sodssetalloc(s, newlen);
    return s;
}

/* Reallocate the sods string so that it has no free space at the end. The
 * contained string remains not altered, but next concatenation operations
 * will require a reallocation.
 *
 * After the call, the passed sods string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
sods sodsRemoveFreeSpace(sods s) {
    void *sh, *newsh;
    char type, oldtype = s[-1] & sods_TYPE_MASK;
    int hdrlen, oldhdrlen = sodsHdrSize(oldtype);
    size_t len = sodslen(s);
    sh = (char*)s-oldhdrlen;

    /* Check what would be the minimum sods header that is just good enough to
     * fit this string. */
    type = sodsReqType(len);
    hdrlen = sodsHdrSize(type);

    /* If the type is the same, or at least a large enough type is still
     * required, we just realloc(), letting the allocator to do the copy
     * only if really needed. Otherwise if the change is huge, we manually
     * reallocate the string to use the different header type. */
    if (oldtype==type || type > sods_TYPE_8) {
        newsh = s_realloc(sh, oldhdrlen+len+1);
        if (newsh == NULL) return NULL;
        s = (char*)newsh+oldhdrlen;
    } else {
        newsh = s_malloc(hdrlen+len+1);
        if (newsh == NULL) return NULL;
        memcpy((char*)newsh+hdrlen, s, len+1);
        s_free(sh);
        s = (char*)newsh+hdrlen;
        s[-1] = type;
        sodssetlen(s, len);
    }
    sodssetalloc(s, len);
    return s;
}

/* Return the total size of the allocation of the specifed sods string,
 * including:
 * 1) The sods header before the pointer.
 * 2) The string.
 * 3) The free buffer at the end if any.
 * 4) The implicit null term.
 */
size_t sodsAllocSize(sods s) {
    size_t alloc = sodsalloc(s);
    return sodsHdrSize(s[-1])+alloc+1;
}

/* Return the pointer of the actual sods allocation (normally sods strings
 * are referenced by the start of the string buffer). */
void *sodsAllocPtr(sods s) {
    return (void*) (s-sodsHdrSize(s[-1]));
}

/* Increment the sods length and decrements the left free space at the
 * end of the string according to 'incr'. Also set the null term
 * in the new end of the string.
 *
 * This function is used in order to fix the string length after the
 * user calls sodsMakeRoomFor(), writes something after the end of
 * the current string, and finally needs to set the new length.
 *
 * Note: it is possible to use a negative increment in order to
 * right-trim the string.
 *
 * Usage example:
 *
 * Using sodsIncrLen() and sodsMakeRoomFor() it is possible to mount the
 * following schema, to cat bytes coming from the kernel to the end of an
 * sods string without copying into an intermediate buffer:
 *
 * oldlen = sodslen(s);
 * s = sodsMakeRoomFor(s, BUFFER_SIZE);
 * nread = read(fd, s+oldlen, BUFFER_SIZE);
 * ... check for nread <= 0 and handle it ...
 * sodsIncrLen(s, nread);
 */
void sodsIncrLen(sods s, ssize_t incr) {
    unsigned char flags = s[-1];
    size_t len;
    switch(flags&sods_TYPE_MASK) {
        case sods_TYPE_5: {
            unsigned char *fp = ((unsigned char*)s)-1;
            unsigned char oldlen = sods_TYPE_5_LEN(flags);
            assert((incr > 0 && oldlen+incr < 32) || (incr < 0 && oldlen >= (unsigned int)(-incr)));
            *fp = sods_TYPE_5 | ((oldlen+incr) << sods_TYPE_BITS);
            len = oldlen+incr;
            break;
        }
        case sods_TYPE_8: {
            sods_HDR_VAR(8,s);
            assert((incr >= 0 && sh->alloc-sh->len >= incr) || (incr < 0 && sh->len >= (unsigned int)(-incr)));
            len = (sh->len += incr);
            break;
        }
        case sods_TYPE_16: {
            sods_HDR_VAR(16,s);
            assert((incr >= 0 && sh->alloc-sh->len >= incr) || (incr < 0 && sh->len >= (unsigned int)(-incr)));
            len = (sh->len += incr);
            break;
        }
        case sods_TYPE_32: {
            sods_HDR_VAR(32,s);
            assert((incr >= 0 && sh->alloc-sh->len >= (unsigned int)incr) || (incr < 0 && sh->len >= (unsigned int)(-incr)));
            len = (sh->len += incr);
            break;
        }
        case sods_TYPE_64: {
            sods_HDR_VAR(64,s);
            assert((incr >= 0 && sh->alloc-sh->len >= (uint64_t)incr) || (incr < 0 && sh->len >= (uint64_t)(-incr)));
            len = (sh->len += incr);
            break;
        }
        default: len = 0; /* Just to avoid compilation warnings. */
    }
    s[len] = '\0';
}

/* Grow the sods to have the specified length. Bytes that were not part of
 * the original length of the sods will be set to zero.
 *
 * if the specified length is smaller than the current length, no operation
 * is performed. */
sods sodsgrowzero(sods s, size_t len) {
    size_t curlen = sodslen(s);

    if (len <= curlen) return s;
    s = sodsMakeRoomFor(s,len-curlen);
    if (s == NULL) return NULL;

    /* Make sure added region doesn't contain garbage */
    memset(s+curlen,0,(len-curlen+1)); /* also set trailing \0 byte */
    sodssetlen(s, len);
    return s;
}

/* Append the specified binary-safe string pointed by 't' of 'len' bytes to the
 * end of the specified sods string 's'.
 *
 * After the call, the passed sods string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
sods sodscatlen(sods s, const void *t, size_t len) {
    size_t curlen = sodslen(s);

    s = sodsMakeRoomFor(s,len);
    if (s == NULL) return NULL;
    memcpy(s+curlen, t, len);
    sodssetlen(s, curlen+len);
    s[curlen+len] = '\0';
    return s;
}

/* Append the specified null termianted C string to the sods string 's'.
 *
 * After the call, the passed sods string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
sods sodscat(sods s, const char *t) {
    return sodscatlen(s, t, strlen(t));
}

/* Append the specified sods 't' to the existing sods 's'.
 *
 * After the call, the modified sods string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
sods sodscatsods(sods s, const sods t) {
    return sodscatlen(s, t, sodslen(t));
}

/* Destructively modify the sods string 's' to hold the specified binary
 * safe string pointed by 't' of length 'len' bytes. */
sods sodscpylen(sods s, const char *t, size_t len) {
    if (sodsalloc(s) < len) {
        s = sodsMakeRoomFor(s,len-sodslen(s));
        if (s == NULL) return NULL;
    }
    memcpy(s, t, len);
    s[len] = '\0';
    sodssetlen(s, len);
    return s;
}

/* Like sodscpylen() but 't' must be a null-termined string so that the length
 * of the string is obtained with strlen(). */
sods sodscpy(sods s, const char *t) {
    return sodscpylen(s, t, strlen(t));
}

/* Helper for sodscatlonglong() doing the actual number -> string
 * conversion. 's' must point to a string with room for at least
 * sods_LLSTR_SIZE bytes.
 *
 * The function returns the length of the null-terminated string
 * representation stored at 's'. */
#define sods_LLSTR_SIZE 21
int sodsll2str(char *s, long long value) {
    char *p, aux;
    unsigned long long v;
    size_t l;

    /* Generate the string representation, this method produces
     * an reversed string. */
    v = (value < 0) ? -value : value;
    p = s;
    do {
        *p++ = '0'+(v%10);
        v /= 10;
    } while(v);
    if (value < 0) *p++ = '-';

    /* Compute length and add null term. */
    l = p-s;
    *p = '\0';

    /* Reverse the string. */
    p--;
    while(s < p) {
        aux = *s;
        *s = *p;
        *p = aux;
        s++;
        p--;
    }
    return l;
}

/* Identical sodsll2str(), but for unsigned long long type. */
int sodsull2str(char *s, unsigned long long v) {
    char *p, aux;
    size_t l;

    /* Generate the string representation, this method produces
     * an reversed string. */
    p = s;
    do {
        *p++ = '0'+(v%10);
        v /= 10;
    } while(v);

    /* Compute length and add null term. */
    l = p-s;
    *p = '\0';

    /* Reverse the string. */
    p--;
    while(s < p) {
        aux = *s;
        *s = *p;
        *p = aux;
        s++;
        p--;
    }
    return l;
}

/* Create an sods string from a long long value. It is much faster than:
 *
 * sodscatprintf(sodsempty(),"%lld\n", value);
 */
sods sodsfromlonglong(long long value) {
    char buf[sods_LLSTR_SIZE];
    int len = sodsll2str(buf,value);

    return sodsnewlen(buf,len);
}

/* Like sodscatprintf() but gets va_list instead of being variadic. */
sods sodscatvprintf(sods s, const char *fmt, va_list ap) {
    va_list cpy;
    char staticbuf[1024], *buf = staticbuf, *t;
    size_t buflen = strlen(fmt)*2;

    /* We try to start using a static buffer for speed.
     * If not possible we revert to heap allocation. */
    if (buflen > sizeof(staticbuf)) {
        buf = s_malloc(buflen);
        if (buf == NULL) return NULL;
    } else {
        buflen = sizeof(staticbuf);
    }

    /* Try with buffers two times bigger every time we fail to
     * fit the string in the current buffer size. */
    while(1) {
        buf[buflen-2] = '\0';
        va_copy(cpy,ap);
        vsnprintf(buf, buflen, fmt, cpy);
        va_end(cpy);
        if (buf[buflen-2] != '\0') {
            if (buf != staticbuf) s_free(buf);
            buflen *= 2;
            buf = s_malloc(buflen);
            if (buf == NULL) return NULL;
            continue;
        }
        break;
    }

    /* Finally concat the obtained string to the sods string and return it. */
    t = sodscat(s, buf);
    if (buf != staticbuf) s_free(buf);
    return t;
}

/* Append to the sods string 's' a string obtained using printf-alike format
 * specifier.
 *
 * After the call, the modified sods string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call.
 *
 * Example:
 *
 * s = sodsnew("Sum is: ");
 * s = sodscatprintf(s,"%d+%d = %d",a,b,a+b).
 *
 * Often you need to create a string from scratch with the printf-alike
 * format. When this is the need, just use sodsempty() as the target string:
 *
 * s = sodscatprintf(sodsempty(), "... your format ...", args);
 */
sods sodscatprintf(sods s, const char *fmt, ...) {
    va_list ap;
    char *t;
    va_start(ap, fmt);
    t = sodscatvprintf(s,fmt,ap);
    va_end(ap);
    return t;
}

/* This function is similar to sodscatprintf, but much faster as it does
 * not rely on sprintf() family functions implemented by the libc that
 * are often very slow. Moreover directly handling the sods string as
 * new data is concatenated provides a performance improvement.
 *
 * However this function only handles an incompatible subset of printf-alike
 * format specifiers:
 *
 * %s - C String
 * %S - sods string
 * %i - signed int
 * %I - 64 bit signed integer (long long, int64_t)
 * %u - unsigned int
 * %U - 64 bit unsigned integer (unsigned long long, uint64_t)
 * %% - Verbatim "%" character.
 */
sods sodscatfmt(sods s, char const *fmt, ...) {
    size_t initlen = sodslen(s);
    const char *f = fmt;
    long i;
    va_list ap;

    va_start(ap,fmt);
    f = fmt;    /* Next format specifier byte to process. */
    i = initlen; /* Position of the next byte to write to dest str. */
    while(*f) {
        char next, *str;
        size_t l;
        long long num;
        unsigned long long unum;

        /* Make sure there is always space for at least 1 char. */
        if (sodsavail(s)==0) {
            s = sodsMakeRoomFor(s,1);
        }

        switch(*f) {
        case '%':
            next = *(f+1);
            f++;
            switch(next) {
            case 's':
            case 'S':
                str = va_arg(ap,char*);
                l = (next == 's') ? strlen(str) : sodslen(str);
                if (sodsavail(s) < l) {
                    s = sodsMakeRoomFor(s,l);
                }
                memcpy(s+i,str,l);
                sodsinclen(s,l);
                i += l;
                break;
            case 'i':
            case 'I':
                if (next == 'i')
                    num = va_arg(ap,int);
                else
                    num = va_arg(ap,long long);
                {
                    char buf[sods_LLSTR_SIZE];
                    l = sodsll2str(buf,num);
                    if (sodsavail(s) < l) {
                        s = sodsMakeRoomFor(s,l);
                    }
                    memcpy(s+i,buf,l);
                    sodsinclen(s,l);
                    i += l;
                }
                break;
            case 'u':
            case 'U':
                if (next == 'u')
                    unum = va_arg(ap,unsigned int);
                else
                    unum = va_arg(ap,unsigned long long);
                {
                    char buf[sods_LLSTR_SIZE];
                    l = sodsull2str(buf,unum);
                    if (sodsavail(s) < l) {
                        s = sodsMakeRoomFor(s,l);
                    }
                    memcpy(s+i,buf,l);
                    sodsinclen(s,l);
                    i += l;
                }
                break;
            default: /* Handle %% and generally %<unknown>. */
                s[i++] = next;
                sodsinclen(s,1);
                break;
            }
            break;
        default:
            s[i++] = *f;
            sodsinclen(s,1);
            break;
        }
        f++;
    }
    va_end(ap);

    /* Add null-term */
    s[i] = '\0';
    return s;
}

/* Remove the part of the string from left and from right composed just of
 * contiguous characters found in 'cset', that is a null terminted C string.
 *
 * After the call, the modified sods string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call.
 *
 * Example:
 *
 * s = sodsnew("AA...AA.a.aa.aHelloWorld     :::");
 * s = sodstrim(s,"Aa. :");
 * printf("%s\n", s);
 *
 * Output will be just "Hello World".
 */
sods sodstrim(sods s, const char *cset) {
    char *start, *end, *sp, *ep;
    size_t len;

    sp = start = s;
    ep = end = s+sodslen(s)-1;
    while(sp <= end && strchr(cset, *sp)) sp++;
    while(ep > sp && strchr(cset, *ep)) ep--;
    len = (sp > ep) ? 0 : ((ep-sp)+1);
    if (s != sp) memmove(s, sp, len);
    s[len] = '\0';
    sodssetlen(s,len);
    return s;
}

/* Turn the string into a smaller (or equal) string containing only the
 * substring specified by the 'start' and 'end' indexes.
 *
 * start and end can be negative, where -1 means the last character of the
 * string, -2 the penultimate character, and so forth.
 *
 * The interval is inclusive, so the start and end characters will be part
 * of the resulting string.
 *
 * The string is modified in-place.
 *
 * Example:
 *
 * s = sodsnew("Hello World");
 * sodsrange(s,1,-1); => "ello World"
 */
void sodsrange(sods s, ssize_t start, ssize_t end) {
    size_t newlen, len = sodslen(s);

    if (len == 0) return;
    if (start < 0) {
        start = len+start;
        if (start < 0) start = 0;
    }
    if (end < 0) {
        end = len+end;
        if (end < 0) end = 0;
    }
    newlen = (start > end) ? 0 : (end-start)+1;
    if (newlen != 0) {
        if (start >= (ssize_t)len) {
            newlen = 0;
        } else if (end >= (ssize_t)len) {
            end = len-1;
            newlen = (start > end) ? 0 : (end-start)+1;
        }
    } else {
        start = 0;
    }
    if (start && newlen) memmove(s, s+start, newlen);
    s[newlen] = 0;
    sodssetlen(s,newlen);
}

/* Apply tolower() to every character of the sods string 's'. */
void sodstolower(sods s) {
    size_t len = sodslen(s), j;

    for (j = 0; j < len; j++) s[j] = tolower(s[j]);
}

/* Apply toupper() to every character of the sods string 's'. */
void sodstoupper(sods s) {
    size_t len = sodslen(s), j;

    for (j = 0; j < len; j++) s[j] = toupper(s[j]);
}

/* Compare two sods strings s1 and s2 with memcmp().
 *
 * Return value:
 *
 *     positive if s1 > s2.
 *     negative if s1 < s2.
 *     0 if s1 and s2 are exactly the same binary string.
 *
 * If two strings share exactly the same prefix, but one of the two has
 * additional characters, the longer string is considered to be greater than
 * the smaller one. */
int sodscmp(const sods s1, const sods s2) {
    size_t l1, l2, minlen;
    int cmp;

    l1 = sodslen(s1);
    l2 = sodslen(s2);
    minlen = (l1 < l2) ? l1 : l2;
    cmp = memcmp(s1,s2,minlen);
    if (cmp == 0) return l1>l2? 1: (l1<l2? -1: 0);
    return cmp;
}

/* Split 's' with separator in 'sep'. An array
 * of sods strings is returned. *count will be set
 * by reference to the number of tokens returned.
 *
 * On out of memory, zero length string, zero length
 * separator, NULL is returned.
 *
 * Note that 'sep' is able to split a string using
 * a multi-character separator. For example
 * sodssplit("foo_-_bar","_-_"); will return two
 * elements "foo" and "bar".
 *
 * This version of the function is binary-safe but
 * requires length arguments. sodssplit() is just the
 * same function but for zero-terminated strings.
 */
sods *sodssplitlen(const char *s, ssize_t len, const char *sep, int seplen, int *count) {
    int elements = 0, slots = 5;
    long start = 0, j;
    sods *tokens;

    if (seplen < 1 || len < 0) return NULL;

    tokens = s_malloc(sizeof(sods)*slots);
    if (tokens == NULL) return NULL;

    if (len == 0) {
        *count = 0;
        return tokens;
    }
    for (j = 0; j < (len-(seplen-1)); j++) {
        /* make sure there is room for the next element and the final one */
        if (slots < elements+2) {
            sods *newtokens;

            slots *= 2;
            newtokens = s_realloc(tokens,sizeof(sods)*slots);
            if (newtokens == NULL) goto cleanup;
            tokens = newtokens;
        }
        /* search the separator */
        if ((seplen == 1 && *(s+j) == sep[0]) || (memcmp(s+j,sep,seplen) == 0)) {
            tokens[elements] = sodsnewlen(s+start,j-start);
            if (tokens[elements] == NULL) goto cleanup;
            elements++;
            start = j+seplen;
            j = j+seplen-1; /* skip the separator */
        }
    }
    /* Add the final element. We are sure there is room in the tokens array. */
    tokens[elements] = sodsnewlen(s+start,len-start);
    if (tokens[elements] == NULL) goto cleanup;
    elements++;
    *count = elements;
    return tokens;

cleanup:
    {
        int i;
        for (i = 0; i < elements; i++) sodsfree(tokens[i]);
        s_free(tokens);
        *count = 0;
        return NULL;
    }
}

/* Free the result returned by sodssplitlen(), or do nothing if 'tokens' is NULL. */
void sodsfreesplitres(sods *tokens, int count) {
    if (!tokens) return;
    while(count--)
        sodsfree(tokens[count]);
    s_free(tokens);
}

/* Append to the sods string "s" an escaped string representation where
 * all the non-printable characters (tested with isprint()) are turned into
 * escapes in the form "\n\r\a...." or "\x<hex-number>".
 *
 * After the call, the modified sods string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
sods sodscatrepr(sods s, const char *p, size_t len) {
    s = sodscatlen(s,"\"",1);
    while(len--) {
        switch(*p) {
        case '\\':
        case '"':
            s = sodscatprintf(s,"\\%c",*p);
            break;
        case '\n': s = sodscatlen(s,"\\n",2); break;
        case '\r': s = sodscatlen(s,"\\r",2); break;
        case '\t': s = sodscatlen(s,"\\t",2); break;
        case '\a': s = sodscatlen(s,"\\a",2); break;
        case '\b': s = sodscatlen(s,"\\b",2); break;
        default:
            if (isprint(*p))
                s = sodscatprintf(s,"%c",*p);
            else
                s = sodscatprintf(s,"\\x%02x",(unsigned char)*p);
            break;
        }
        p++;
    }
    return sodscatlen(s,"\"",1);
}

/* Helper function for sodssplitargs() that returns non zero if 'c'
 * is a valid hex digit. */
int is_hex_digit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

/* Helper function for sodssplitargs() that converts a hex digit into an
 * integer from 0 to 15 */
int hex_digit_to_int(char c) {
    switch(c) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': case 'A': return 10;
    case 'b': case 'B': return 11;
    case 'c': case 'C': return 12;
    case 'd': case 'D': return 13;
    case 'e': case 'E': return 14;
    case 'f': case 'F': return 15;
    default: return 0;
    }
}

/* Split a line into arguments, where every argument can be in the
 * following programming-language REPL-alike form:
 *
 * foo bar "newline are supported\n" and "\xff\x00otherstuff"
 *
 * The number of arguments is stored into *argc, and an array
 * of sods is returned.
 *
 * The caller should free the resulting array of sods strings with
 * sodsfreesplitres().
 *
 * Note that sodscatrepr() is able to convert back a string into
 * a quoted string in the same format sodssplitargs() is able to parse.
 *
 * The function returns the allocated tokens on success, even when the
 * input string is empty, or NULL if the input contains unbalanced
 * quotes or closed quotes followed by non space characters
 * as in: "foo"bar or "foo'
 */
sods *sodssplitargs(const char *line, int *argc) {
    const char *p = line;
    char *current = NULL;
    char **vector = NULL;

    *argc = 0;
    while(1) {
        /* skip blanks */
        while(*p && isspace(*p)) p++;
        if (*p) {
            /* get a token */
            int inq=0;  /* set to 1 if we are in "quotes" */
            int insq=0; /* set to 1 if we are in 'single quotes' */
            int done=0;

            if (current == NULL) current = sodsempty();
            while(!done) {
                if (inq) {
                    if (*p == '\\' && *(p+1) == 'x' &&
                                             is_hex_digit(*(p+2)) &&
                                             is_hex_digit(*(p+3)))
                    {
                        unsigned char byte;

                        byte = (hex_digit_to_int(*(p+2))*16)+
                                hex_digit_to_int(*(p+3));
                        current = sodscatlen(current,(char*)&byte,1);
                        p += 3;
                    } else if (*p == '\\' && *(p+1)) {
                        char c;

                        p++;
                        switch(*p) {
                        case 'n': c = '\n'; break;
                        case 'r': c = '\r'; break;
                        case 't': c = '\t'; break;
                        case 'b': c = '\b'; break;
                        case 'a': c = '\a'; break;
                        default: c = *p; break;
                        }
                        current = sodscatlen(current,&c,1);
                    } else if (*p == '"') {
                        /* closing quote must be followed by a space or
                         * nothing at all. */
                        if (*(p+1) && !isspace(*(p+1))) goto err;
                        done=1;
                    } else if (!*p) {
                        /* unterminated quotes */
                        goto err;
                    } else {
                        current = sodscatlen(current,p,1);
                    }
                } else if (insq) {
                    if (*p == '\\' && *(p+1) == '\'') {
                        p++;
                        current = sodscatlen(current,"'",1);
                    } else if (*p == '\'') {
                        /* closing quote must be followed by a space or
                         * nothing at all. */
                        if (*(p+1) && !isspace(*(p+1))) goto err;
                        done=1;
                    } else if (!*p) {
                        /* unterminated quotes */
                        goto err;
                    } else {
                        current = sodscatlen(current,p,1);
                    }
                } else {
                    switch(*p) {
                    case ' ':
                    case '\n':
                    case '\r':
                    case '\t':
                    case '\0':
                        done=1;
                        break;
                    case '"':
                        inq=1;
                        break;
                    case '\'':
                        insq=1;
                        break;
                    default:
                        current = sodscatlen(current,p,1);
                        break;
                    }
                }
                if (*p) p++;
            }
            /* add the token to the vector */
            vector = s_realloc(vector,((*argc)+1)*sizeof(char*));
            vector[*argc] = current;
            (*argc)++;
            current = NULL;
        } else {
            /* Even on empty input string return something not NULL. */
            if (vector == NULL) vector = s_malloc(sizeof(void*));
            return vector;
        }
    }

err:
    while((*argc)--)
        sodsfree(vector[*argc]);
    s_free(vector);
    if (current) sodsfree(current);
    *argc = 0;
    return NULL;
}

/* Modify the string substituting all the occurrences of the set of
 * characters specified in the 'from' string to the corresponding character
 * in the 'to' array.
 *
 * For instance: sodsmapchars(mystring, "ho", "01", 2)
 * will have the effect of turning the string "hello" into "0ell1".
 *
 * The function returns the sods string pointer, that is always the same
 * as the input pointer since no resize is needed. */
sods sodsmapchars(sods s, const char *from, const char *to, size_t setlen) {
    size_t j, i, l = sodslen(s);

    for (j = 0; j < l; j++) {
        for (i = 0; i < setlen; i++) {
            if (s[j] == from[i]) {
                s[j] = to[i];
                break;
            }
        }
    }
    return s;
}

/* Join an array of C strings using the specified separator (also a C string).
 * Returns the result as an sods string. */
sods sodsjoin(char **argv, int argc, char *sep) {
    sods join = sodsempty();
    int j;

    for (j = 0; j < argc; j++) {
        join = sodscat(join, argv[j]);
        if (j != argc-1) join = sodscat(join,sep);
    }
    return join;
}

/* Like sodsjoin, but joins an array of sods strings. */
sods sodsjoinsods(sods *argv, int argc, const char *sep, size_t seplen) {
    sods join = sodsempty();
    int j;

    for (j = 0; j < argc; j++) {
        join = sodscatsods(join, argv[j]);
        if (j != argc-1) join = sodscatlen(join,sep,seplen);
    }
    return join;
}

/* Wrappers to the allocators used by sods. Note that sods will actually
 * just use the macros defined into sodsalloc.h in order to avoid to pay
 * the overhead of function calls. Here we define these wrappers only for
 * the programs sods is linked to, if they want to touch the sods internals
 * even if they use a different allocator. */
void *sods_malloc(size_t size) { return s_malloc(size); }
void *sods_realloc(void *ptr, size_t size) { return s_realloc(ptr,size); }
void sods_free(void *ptr) { s_free(ptr); }

#if defined(sods_TEST_MAIN)
#include <stdio.h>
#include "testhelp.h"
#include "limits.h"

#define UNUSED(x) (void)(x)
int sodsTest(void) {
    {
        sods x = sodsnew("foo"), y;

        test_cond("Create a string and obtain the length",
            sodslen(x) == 3 && memcmp(x,"foo\0",4) == 0)

        sodsfree(x);
        x = sodsnewlen("foo",2);
        test_cond("Create a string with specified length",
            sodslen(x) == 2 && memcmp(x,"fo\0",3) == 0)

        x = sodscat(x,"bar");
        test_cond("Strings concatenation",
            sodslen(x) == 5 && memcmp(x,"fobar\0",6) == 0);

        x = sodscpy(x,"a");
        test_cond("sodscpy() against an originally longer string",
            sodslen(x) == 1 && memcmp(x,"a\0",2) == 0)

        x = sodscpy(x,"xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk");
        test_cond("sodscpy() against an originally shorter string",
            sodslen(x) == 33 &&
            memcmp(x,"xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk\0",33) == 0)

        sodsfree(x);
        x = sodscatprintf(sodsempty(),"%d",123);
        test_cond("sodscatprintf() seems working in the base case",
            sodslen(x) == 3 && memcmp(x,"123\0",4) == 0)

        sodsfree(x);
        x = sodsnew("--");
        x = sodscatfmt(x, "Hello %s World %I,%I--", "Hi!", LLONG_MIN,LLONG_MAX);
        test_cond("sodscatfmt() seems working in the base case",
            sodslen(x) == 60 &&
            memcmp(x,"--Hello Hi! World -9223372036854775808,"
                     "9223372036854775807--",60) == 0)
        printf("[%s]\n",x);

        sodsfree(x);
        x = sodsnew("--");
        x = sodscatfmt(x, "%u,%U--", UINT_MAX, ULLONG_MAX);
        test_cond("sodscatfmt() seems working with unsigned numbers",
            sodslen(x) == 35 &&
            memcmp(x,"--4294967295,18446744073709551615--",35) == 0)

        sodsfree(x);
        x = sodsnew(" x ");
        sodstrim(x," x");
        test_cond("sodstrim() works when all chars match",
            sodslen(x) == 0)

        sodsfree(x);
        x = sodsnew(" x ");
        sodstrim(x," ");
        test_cond("sodstrim() works when a single char remains",
            sodslen(x) == 1 && x[0] == 'x')

        sodsfree(x);
        x = sodsnew("xxciaoyyy");
        sodstrim(x,"xy");
        test_cond("sodstrim() correctly trims characters",
            sodslen(x) == 4 && memcmp(x,"ciao\0",5) == 0)

        y = sodsdup(x);
        sodsrange(y,1,1);
        test_cond("sodsrange(...,1,1)",
            sodslen(y) == 1 && memcmp(y,"i\0",2) == 0)

        sodsfree(y);
        y = sodsdup(x);
        sodsrange(y,1,-1);
        test_cond("sodsrange(...,1,-1)",
            sodslen(y) == 3 && memcmp(y,"iao\0",4) == 0)

        sodsfree(y);
        y = sodsdup(x);
        sodsrange(y,-2,-1);
        test_cond("sodsrange(...,-2,-1)",
            sodslen(y) == 2 && memcmp(y,"ao\0",3) == 0)

        sodsfree(y);
        y = sodsdup(x);
        sodsrange(y,2,1);
        test_cond("sodsrange(...,2,1)",
            sodslen(y) == 0 && memcmp(y,"\0",1) == 0)

        sodsfree(y);
        y = sodsdup(x);
        sodsrange(y,1,100);
        test_cond("sodsrange(...,1,100)",
            sodslen(y) == 3 && memcmp(y,"iao\0",4) == 0)

        sodsfree(y);
        y = sodsdup(x);
        sodsrange(y,100,100);
        test_cond("sodsrange(...,100,100)",
            sodslen(y) == 0 && memcmp(y,"\0",1) == 0)

        sodsfree(y);
        sodsfree(x);
        x = sodsnew("foo");
        y = sodsnew("foa");
        test_cond("sodscmp(foo,foa)", sodscmp(x,y) > 0)

        sodsfree(y);
        sodsfree(x);
        x = sodsnew("bar");
        y = sodsnew("bar");
        test_cond("sodscmp(bar,bar)", sodscmp(x,y) == 0)

        sodsfree(y);
        sodsfree(x);
        x = sodsnew("aar");
        y = sodsnew("bar");
        test_cond("sodscmp(bar,bar)", sodscmp(x,y) < 0)

        sodsfree(y);
        sodsfree(x);
        x = sodsnewlen("\a\n\0foo\r",7);
        y = sodscatrepr(sodsempty(),x,sodslen(x));
        test_cond("sodscatrepr(...data...)",
            memcmp(y,"\"\\a\\n\\x00foo\\r\"",15) == 0)

        {
            unsigned int oldfree;
            char *p;
            int step = 10, j, i;

            sodsfree(x);
            sodsfree(y);
            x = sodsnew("0");
            test_cond("sodsnew() free/len buffers", sodslen(x) == 1 && sodsavail(x) == 0);

            /* Run the test a few times in order to hit the first two
             * sods header types. */
            for (i = 0; i < 10; i++) {
                int oldlen = sodslen(x);
                x = sodsMakeRoomFor(x,step);
                int type = x[-1]&sods_TYPE_MASK;

                test_cond("sodsMakeRoomFor() len", sodslen(x) == oldlen);
                if (type != sods_TYPE_5) {
                    test_cond("sodsMakeRoomFor() free", sodsavail(x) >= step);
                    oldfree = sodsavail(x);
                }
                p = x+oldlen;
                for (j = 0; j < step; j++) {
                    p[j] = 'A'+j;
                }
                sodsIncrLen(x,step);
            }
            test_cond("sodsMakeRoomFor() content",
                memcmp("0ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ",x,101) == 0);
            test_cond("sodsMakeRoomFor() final length",sodslen(x)==101);

            sodsfree(x);
        }
    }
    test_report()
    return 0;
}
#endif

#ifdef sods_TEST_MAIN
int main(void) {
    return sodsTest();
}
#endif

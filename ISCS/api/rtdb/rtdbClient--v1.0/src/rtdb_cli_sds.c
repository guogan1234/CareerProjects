#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtdb_cli_sds.h"

sds sdsnewlen(const void* init, size_t initlen) {
    sdshdr* sh;

    if (init) {
        sh = malloc(sizeof(sdshdr)+initlen+1);
    } else {
        sh = calloc(sizeof(sdshdr)+initlen+1, 1);
    }
    if (NULL == sh) return NULL;
    sh->len = initlen;
    sh->free = 0;
    if (initlen && init) memcpy(sh->buf, init, initlen);
    sh->buf[initlen] = '\0';
    return (char*)sh->buf;
}

sds sdsempty(void) {
    return sdsnewlen("",0);
}

sds sdsnew(const char* init) {
    size_t initlen = (NULL == init) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}

sds sdsdup(const sds s) {
    return sdsnewlen(s, sdslen(s));
}

void sdsfree(sds s) {
    if (NULL == s) return;
    free(s-sizeof(sdshdr));
}

void sdsupdatelen(sds s) {
    sdshdr* sh = (void*)(s-sizeof(sdshdr));
    int reallen = strlen(s);
    sh->free += (sh->len-reallen);
    sh->len = reallen;
}

void sdsclear(sds s) {
    sdshdr* sh = (void*)(s-sizeof(sdshdr));
    sh->free += sh->len;
    sh->len = 0;
    sh->buf[0] = '\0';
}

sds sdsMakeRoomFor(sds s, size_t addlen) {
    sdshdr* sh;
    sdshdr* newsh;
    size_t len;
    size_t newlen;
    size_t free = sdsavail(s);
    
    if (free >= addlen) return s;
    len = sdslen(s);
    sh = (void*)(s-sizeof(sdshdr));
    newlen = (len+addlen);
    if (newlen < SDS_MAX_PREALLOC) {
        newlen *= 2;
    } else {
        newlen += SDS_MAX_PREALLOC;
    }
    newsh = realloc(sh, sizeof(sdshdr)+newlen+1);
    if (NULL == newsh) return NULL;
    newsh->free = newlen - len;
    
    return newsh->buf;
}

sds sdscatlen(sds s, const void* t, size_t len) {
    sdshdr* sh;
    size_t curlen = sdslen(s);

    s = sdsMakeRoomFor(s,len);
    if (NULL == s) return NULL;
    sh = (void*)(s-sizeof(sdshdr));
    memcpy(s+curlen, t, len);
    sh->len = curlen+len;
    sh->free = sh->free-len;
    s[curlen+len] = '\0';
    return s;
}

sds sdscat(sds s, const char* t) {
    return sdscatlen(s, t, strlen(t));
}

sds sdscatvprintf(sds s, const char* fmt, va_list ap) {
    va_list cpy;
    char staticbuf[1024];
    char* buf = staticbuf;
    char* t;
    size_t buflen = strlen(fmt)*2;

    if (buflen > sizeof(staticbuf)) {
        buf = malloc(buflen);
        if (NULL == buf) return NULL;
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
            if (buf != staticbuf) free(buf);
            buflen *= 2;
            buf = malloc(buflen);
            if (NULL == buf) return NULL;
            continue;
        }
        break;
    }

    /* Finally concat the obtained string to the SDS string and return it. */
    t = sdscat(s, buf);
    if (buf != staticbuf) free(buf);
    return t;
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
 * s = sdsnew("Hello World");
 * sdsrange(s,1,-1); => "ello World"
 */
void sdsrange(sds s, int start, int end) {
    sdshdr* sh = (void*)(s-sizeof(sdshdr));
    size_t newlen;
    size_t len = sdslen(s);

    if (0 == len) return;
    if (0 > start) {
        start = len+start;
        if (0 > start) start = 0;
    }
    if (0 > end) {
        end = len+end;
        if (0 > end) end = 0;
    }
    newlen = (start > end) ? 0 : (end-start)+1;
    if (0 != newlen) {
        if (start >= (signed)len) {
            newlen = 0;
        } else if (end >= (signed)len) {
            end = len-1;
            newlen = (start > end) ? 0 : (end-start)+1;
        }
    } else {
        start = 0;
    }
    if (start && newlen) memmove(sh->buf, sh->buf+start, newlen);
    sh->buf[newlen] = 0;
    sh->free = sh->free+(sh->len-newlen);
    sh->len = newlen;
}

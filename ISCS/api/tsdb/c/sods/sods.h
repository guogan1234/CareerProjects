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

#ifndef __sods_H
#define __sods_H

#define sods_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>
#include <stdint.h>

typedef char *sods;

/* Note: sodshdr5 is never used, we just access the flags byte directly.
 * However is here to document the layout of type 5 sods strings. */
struct __attribute__ ((__packed__)) sodshdr5 {
    unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
    char buf[];
};
struct __attribute__ ((__packed__)) sodshdr8 {
    uint8_t len; /* used */
    uint8_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
struct __attribute__ ((__packed__)) sodshdr16 {
    uint16_t len; /* used */
    uint16_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
struct __attribute__ ((__packed__)) sodshdr32 {
    uint32_t len; /* used */
    uint32_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
struct __attribute__ ((__packed__)) sodshdr64 {
    uint64_t len; /* used */
    uint64_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};

#define sods_TYPE_5  0
#define sods_TYPE_8  1
#define sods_TYPE_16 2
#define sods_TYPE_32 3
#define sods_TYPE_64 4
#define sods_TYPE_MASK 7
#define sods_TYPE_BITS 3
#define sods_HDR_VAR(T,s) struct sodshdr##T *sh = (void*)((s)-(sizeof(struct sodshdr##T)));
#define sods_HDR(T,s) ((struct sodshdr##T *)((s)-(sizeof(struct sodshdr##T))))
#define sods_TYPE_5_LEN(f) ((f)>>sods_TYPE_BITS)

static inline size_t sodslen(const sods s) {
    unsigned char flags = s[-1];
    switch(flags&sods_TYPE_MASK) {
        case sods_TYPE_5:
            return sods_TYPE_5_LEN(flags);
        case sods_TYPE_8:
            return sods_HDR(8,s)->len;
        case sods_TYPE_16:
            return sods_HDR(16,s)->len;
        case sods_TYPE_32:
            return sods_HDR(32,s)->len;
        case sods_TYPE_64:
            return sods_HDR(64,s)->len;
    }
    return 0;
}

static inline size_t sodsavail(const sods s) {
    unsigned char flags = s[-1];
    switch(flags&sods_TYPE_MASK) {
        case sods_TYPE_5: {
            return 0;
        }
        case sods_TYPE_8: {
            sods_HDR_VAR(8,s);
            return sh->alloc - sh->len;
        }
        case sods_TYPE_16: {
            sods_HDR_VAR(16,s);
            return sh->alloc - sh->len;
        }
        case sods_TYPE_32: {
            sods_HDR_VAR(32,s);
            return sh->alloc - sh->len;
        }
        case sods_TYPE_64: {
            sods_HDR_VAR(64,s);
            return sh->alloc - sh->len;
        }
    }
    return 0;
}

static inline void sodssetlen(sods s, size_t newlen) {
    unsigned char flags = s[-1];
    switch(flags&sods_TYPE_MASK) {
        case sods_TYPE_5:
            {
                unsigned char *fp = ((unsigned char*)s)-1;
                *fp = sods_TYPE_5 | (newlen << sods_TYPE_BITS);
            }
            break;
        case sods_TYPE_8:
            sods_HDR(8,s)->len = newlen;
            break;
        case sods_TYPE_16:
            sods_HDR(16,s)->len = newlen;
            break;
        case sods_TYPE_32:
            sods_HDR(32,s)->len = newlen;
            break;
        case sods_TYPE_64:
            sods_HDR(64,s)->len = newlen;
            break;
    }
}

static inline void sodsinclen(sods s, size_t inc) {
    unsigned char flags = s[-1];
    switch(flags&sods_TYPE_MASK) {
        case sods_TYPE_5:
            {
                unsigned char *fp = ((unsigned char*)s)-1;
                unsigned char newlen = sods_TYPE_5_LEN(flags)+inc;
                *fp = sods_TYPE_5 | (newlen << sods_TYPE_BITS);
            }
            break;
        case sods_TYPE_8:
            sods_HDR(8,s)->len += inc;
            break;
        case sods_TYPE_16:
            sods_HDR(16,s)->len += inc;
            break;
        case sods_TYPE_32:
            sods_HDR(32,s)->len += inc;
            break;
        case sods_TYPE_64:
            sods_HDR(64,s)->len += inc;
            break;
    }
}

/* sodsalloc() = sodsavail() + sodslen() */
static inline size_t sodsalloc(const sods s) {
    unsigned char flags = s[-1];
    switch(flags&sods_TYPE_MASK) {
        case sods_TYPE_5:
            return sods_TYPE_5_LEN(flags);
        case sods_TYPE_8:
            return sods_HDR(8,s)->alloc;
        case sods_TYPE_16:
            return sods_HDR(16,s)->alloc;
        case sods_TYPE_32:
            return sods_HDR(32,s)->alloc;
        case sods_TYPE_64:
            return sods_HDR(64,s)->alloc;
    }
    return 0;
}

static inline void sodssetalloc(sods s, size_t newlen) {
    unsigned char flags = s[-1];
    switch(flags&sods_TYPE_MASK) {
        case sods_TYPE_5:
            /* Nothing to do, this type has no total allocation info. */
            break;
        case sods_TYPE_8:
            sods_HDR(8,s)->alloc = newlen;
            break;
        case sods_TYPE_16:
            sods_HDR(16,s)->alloc = newlen;
            break;
        case sods_TYPE_32:
            sods_HDR(32,s)->alloc = newlen;
            break;
        case sods_TYPE_64:
            sods_HDR(64,s)->alloc = newlen;
            break;
    }
}

sods sodsnewlen(const void *init, size_t initlen);
sods sodsnew(const char *init);
sods sodsempty(void);
sods sodsdup(const sods s);
void sodsfree(sods s);
sods sodsgrowzero(sods s, size_t len);
sods sodscatlen(sods s, const void *t, size_t len);
sods sodscat(sods s, const char *t);
sods sodscatsods(sods s, const sods t);
sods sodscpylen(sods s, const char *t, size_t len);
sods sodscpy(sods s, const char *t);

sods sodscatvprintf(sods s, const char *fmt, va_list ap);
#ifdef __GNUC__
sods sodscatprintf(sods s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sods sodscatprintf(sods s, const char *fmt, ...);
#endif

sods sodscatfmt(sods s, char const *fmt, ...);
sods sodstrim(sods s, const char *cset);
void sodsrange(sods s, ssize_t start, ssize_t end);
void sodsupdatelen(sods s);
void sodsclear(sods s);
int sodscmp(const sods s1, const sods s2);
sods *sodssplitlen(const char *s, ssize_t len, const char *sep, int seplen, int *count);
void sodsfreesplitres(sods *tokens, int count);
void sodstolower(sods s);
void sodstoupper(sods s);
sods sodsfromlonglong(long long value);
sods sodscatrepr(sods s, const char *p, size_t len);
sods *sodssplitargs(const char *line, int *argc);
sods sodsmapchars(sods s, const char *from, const char *to, size_t setlen);
sods sodsjoin(char **argv, int argc, char *sep);
sods sodsjoinsods(sods *argv, int argc, const char *sep, size_t seplen);

/* Low level functions exposed to the user API */
sods sodsMakeRoomFor(sods s, size_t addlen);
void sodsIncrLen(sods s, ssize_t incr);
sods sodsRemoveFreeSpace(sods s);
size_t sodsAllocSize(sods s);
void *sodsAllocPtr(sods s);

/* Export the allocator used by sods to the program using sods.
 * Sometimes the program sods is linked to, may use a different set of
 * allocators, but may want to allocate or free things that sods will
 * respectively free or allocate. */
void *sods_malloc(size_t size);
void *sods_realloc(void *ptr, size_t size);
void sods_free(void *ptr);

#ifdef REDIS_TEST
int sodsTest(int argc, char *argv[]);
#endif

#endif

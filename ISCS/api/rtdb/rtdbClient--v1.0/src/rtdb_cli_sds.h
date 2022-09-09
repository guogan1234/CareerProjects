#ifndef RTDB_CLI_SDS_H
#define RTDB_CLI_SDS_H

#include <sys/types.h>
#include <stdarg.h>

#define SDS_MAX_PREALLOC (1024*1024)

typedef char* sds;

typedef struct sdshdr {
    unsigned int len;
    unsigned int free;
    char buf[];
} sdshdr;

#ifdef _WIN32
static __inline size_t sdslen(const sds s) {
    struct sdshdr* sh = (void*)(s-(sizeof(sdshdr)));
    return sh->len;
}
static __inline size_t sdsavail(const sds s) {
    struct sdshdr* sh = (void*)(s-(sizeof(sdshdr)));
    return sh->free;
}
#else
static inline size_t sdslen(const sds s) {
    struct sdshdr* sh = (void*)(s-(sizeof(sdshdr)));
    return sh->len;
}
static inline size_t sdsavail(const sds s) {
    struct sdshdr* sh = (void*)(s-(sizeof(sdshdr)));
    return sh->free;
}
#endif

sds sdsnewlen(const void* init, size_t initlen);
sds sdsnew(const char* init);
sds sdsempty();
size_t sdslen(const sds s);
sds sdsdup(const sds s);
void sdsfree(sds s);
size_t sdsavail(const sds s);
sds sdscatlen(sds s, const void* t, size_t len);
sds sdscat(sds s, const char* t);
sds sdscatvprintf(sds s, const char* fmt, va_list ap);
void sdsrange(sds s, int start, int end);
void sdsupdatelen(sds s);
void sdsclear(sds s);

/* Low level functions exposed to the user API */
sds sdsMakeRoomFor(sds s, size_t addlen);

#endif

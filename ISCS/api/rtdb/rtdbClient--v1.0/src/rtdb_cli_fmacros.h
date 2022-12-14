/*
 @Copyright Reserved by .
 Define compiler option macros.
 Create by KanWenDi, 2018.03.23.
 Histories:

 */
#ifndef RTDB_CLI_FMACROS_H
#define RTDB_CLI_FMACROS_H
#ifdef __cplusplus
extern "C" {
#endif

#define _BSD_SOURCE

#if defined(__linux__)
#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#endif

#if defined(_AIX)
#define _ALL_SOURCE
#endif

#if defined(__linux__) || defined(__OpenBSD__)
#define _XOPEN_SOURCE 700
/*
 * On NetBSD, _XOPEN_SOURCE undefines _NETBSD_SOURCE and
 * thus hides inet_aton etc.
 */
#elif !defined(__NetBSD__)
#define _XOPEN_SOURCE
#endif

#if defined(__sun)
#define _POSIX_C_SOURCE 199506L
#endif

#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

#ifdef __cplusplus
}
#endif

#endif

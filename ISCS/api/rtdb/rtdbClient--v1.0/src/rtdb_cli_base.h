#ifndef RTDB_CLI_BASE_H
#define RTDB_CLI_BASE_H
#include <stdio.h> 
#include <stdarg.h> 

#define RTDB_ERR -1
#define RTDB_OK 0

#define RTDB_ERR_IO 1 /* Error in read or write */
#define RTDB_ERR_EOF 3 /* End of file */
#define RTDB_ERR_PROTOCOL 4 /* Protocol error */
#define RTDB_ERR_OOM 5 /* Out of memory */
#define RTDB_ERR_OTHER 2 /* Everything else... */

#define RTDB_BLOCK 0x1
#define RTDB_CONNECTED 0x2

#define RTDB_REPLY_STRING 1
#define RTDB_REPLY_ARRAY 2
#define RTDB_REPLY_INTEGER 3
#define RTDB_REPLY_NIL 4
#define RTDB_REPLY_STATUS 5
#define RTDB_REPLY_ERROR 6

#define RTDB_READER_MAX_BUF (1024*16)  /* Default max unused reader buffer. */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rtdb_cli_reply {
    int type; /* RTDB_REPLY_* */
    long long integer; /* The integer when type is RTDB_REPLY_INTEGER */
    int len; /* Length of string */
    char *str; /* Used for both RTDB_REPLY_ERROR and RTDB_REPLY_STRING */
    size_t elements; /* number of elements, for RTDB_REPLY_ARRAY */
    struct rtdb_cli_reply **element; /* elements vector for RTDB_REPLY_ARRAY */
} rtdb_cli_reply;

typedef struct rtdb_cli_read_task {
    int type;
    int elements; /* number of elements in multibulk container */
    int idx; /* index in parent (array) object */
    void *obj; /* holds user-generated value for a read task */
    struct rtdb_cli_read_task *parent; /* parent task */
    void *privdata; /* user-settable arbitrary field */
} rtdb_cli_read_task;

typedef struct rtdb_cli_reply_obj_funs {
    void *(*create_string)(const rtdb_cli_read_task*, char*, size_t);
    void *(*create_array)(const rtdb_cli_read_task*, int);
    void *(*create_integer)(const rtdb_cli_read_task*, long long);
    void *(*create_nil)(const rtdb_cli_read_task*);
    void (*free_object)(void*);
} rtdb_cli_reply_obj_funs;

typedef struct rtdb_cli_reader {
    int err; /* Error flags, 0 when there is no error */
    char errstr[128]; /* String representation of error when applicable */

    char *buf; /* Read buffer */
    size_t pos; /* Buffer cursor */
    size_t len; /* Buffer length */
    size_t maxbuf; /* Max length of unused buffer */

    rtdb_cli_read_task rstack[9];
    int ridx; /* Index of current read task */
    void *reply; /* Temporary reply pointer */

    rtdb_cli_reply_obj_funs *fn;
    void *privdata;
} rtdb_cli_reader;

rtdb_cli_reader *rtdb_cli_reader_create(void);
void rtdb_cli_reader_free(rtdb_cli_reader *r);
int rtdb_cli_reader_feed(rtdb_cli_reader *r, const char *buf, size_t len);
int rtdb_cli_reader_get_reply(rtdb_cli_reader *r, void **reply);
void free_reply_object(void *reply);
int rtdb_cli_vformat_command(char **target, const char *format, va_list ap);

typedef struct rtdb_cli_context {
    int err; /* Error flags, 0 when there is no error */
    char errstr[128]; /* String representation of error when applicable */
    int fd;
    int flags;
    char *obuf; /* Write buffer */
    rtdb_cli_reader *reader; /* Protocol reader */
} rtdb_cli_context;

rtdb_cli_context *rtdb_cli_connect(const char *ip, int port);
void rtdb_cli_contex_free(rtdb_cli_context *c);
int rtdb_cli_buffer_read(rtdb_cli_context *c);
int rtdb_cli_buffer_write(rtdb_cli_context *c, int *done);
int rtdb_cli_get_reply(rtdb_cli_context *c, void **reply);
int rtdb_cli_get_reply_from_reader(rtdb_cli_context *c, void **reply);
int rtdb_cli_append_vcommand(rtdb_cli_context *c, const char *format, va_list ap);
int rtdb_cli_append_command(rtdb_cli_context *c, const char *format, ...);
void *rtdb_cli_vcommand(rtdb_cli_context *c, const char *format, va_list ap);
void *rtdb_client_command(rtdb_cli_context *c, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#ifdef __linux__
#include <sys/time.h> 
#else
#include <WinSock2.h> 
#endif

#include "rtdb_cli_cmn.h"
#include "rtdb_cli_base.h"
#include "rtdb_cli_sds.h"
#include "rtdb_cli_net.h"

static rtdb_cli_reply *create_reply_object(int type);
static void *create_string_object(const rtdb_cli_read_task *task, char *str, size_t len);
static void *create_array_object(const rtdb_cli_read_task *task, int elements);
static void *create_integer_object(const rtdb_cli_read_task *task, long long value);
static void *create_nil_object(const rtdb_cli_read_task *task);

/* Default set of functions to build the reply. Keep in mind that such a
 * function returning NULL is interpreted as OOM. */
static rtdb_cli_reply_obj_funs defaultFunctions = {
    create_string_object,
    create_array_object,
    create_integer_object,
    create_nil_object,
    free_reply_object
};

static rtdb_cli_reply *create_reply_object(int type) {
    rtdb_cli_reply *r = calloc(1,sizeof(*r));

    if (r == NULL) return NULL;
    r->type = type;
    return r;
}

void free_reply_object(void *reply) 
{
    rtdb_cli_reply *r = reply;
    size_t j;

    if (NULL == r) return;
    switch(r->type) 
    {
        case RTDB_REPLY_INTEGER:
            break; /* Nothing to free */
        case RTDB_REPLY_ARRAY:
            if (r->element != NULL) 
            {
                for (j = 0; j < r->elements; j++) 
                {
                    if (r->element[j] != NULL) 
                    {
                        free_reply_object(r->element[j]);
                    }
                }
                free(r->element);
            }
            break;
        case RTDB_REPLY_ERROR:
        case RTDB_REPLY_STATUS:
        case RTDB_REPLY_STRING:
            if (r->str != NULL) free(r->str);
            break;
    }
    free(r);
}

static void *create_string_object(const rtdb_cli_read_task *task, char *str, size_t len) 
{
    rtdb_cli_reply *r, *parent;
    char *buf;

    r = create_reply_object(task->type);
    if (r == NULL) return NULL;

    buf = malloc(len+1);
    if (buf == NULL) 
    {
        free_reply_object(r);
        return NULL;
    }

    assert(task->type == RTDB_REPLY_ERROR  ||
           task->type == RTDB_REPLY_STATUS ||
           task->type == RTDB_REPLY_STRING);

    /* Copy string value */
    memcpy(buf,str,len);
    buf[len] = '\0';
    r->str = buf;
    r->len = len;

    if (task->parent) 
    {
        parent = task->parent->obj;
        assert(parent->type == RTDB_REPLY_ARRAY);
        parent->element[task->idx] = r;
    }
    return r;
}

static void *create_array_object(const rtdb_cli_read_task *task, int elements) 
{
    rtdb_cli_reply *r, *parent;

    r = create_reply_object(RTDB_REPLY_ARRAY);
    if (r == NULL) return NULL;

    if (elements > 0) 
    {
        r->element = calloc(elements,sizeof(rtdb_cli_reply*));
        if (r->element == NULL) {
            free_reply_object(r);
            return NULL;
        }
    }
    r->elements = elements;

    if (task->parent) 
    {
        parent = task->parent->obj;
        assert(parent->type == RTDB_REPLY_ARRAY);
        parent->element[task->idx] = r;
    }
    return r;
}

static void *create_integer_object(const rtdb_cli_read_task *task, long long value) 
{
    rtdb_cli_reply *r, *parent;

    r = create_reply_object(RTDB_REPLY_INTEGER);
    if (r == NULL) return NULL;

    r->integer = value;
    if (task->parent) 
    {
        parent = task->parent->obj;
        assert(parent->type == RTDB_REPLY_ARRAY);
        parent->element[task->idx] = r;
    }
    return r;
}

static void *create_nil_object(const rtdb_cli_read_task *task) 
{
    rtdb_cli_reply *r, *parent;

    r = create_reply_object(RTDB_REPLY_NIL);
    if (r == NULL) return NULL;

    if (task->parent) 
    {
        parent = task->parent->obj;
        assert(parent->type == RTDB_REPLY_ARRAY);
        parent->element[task->idx] = r;
    }
    return r;
}

static void __rtdb_cli_reader_set_error(rtdb_cli_reader *r, int type, const char *str) 
{
    size_t len;

    if (r->reply != NULL && r->fn && r->fn->free_object) 
    {
        r->fn->free_object(r->reply);
        r->reply = NULL;
    }
    
    if (r->buf != NULL) 
    {/* Clear input buffer on errors. */
        sdsfree(r->buf);
        r->buf = NULL;
        r->pos = r->len = 0;
    }

    /* Reset task stack. */
    r->ridx = -1;

    /* Set error. */
    r->err = type;
    len = strlen(str);
    len = len < (sizeof(r->errstr)-1) ? len : (sizeof(r->errstr)-1);
    memcpy(r->errstr,str,len);
    r->errstr[len] = '\0';
}

static size_t chrtos(char *buf, size_t size, char byte) 
{
    size_t len = 0;

    switch(byte) 
    {
        case '\\':
        case '"':
            len = snprintf(buf,size,"\"\\%c\"",byte);
            break;
        case '\n': 
            len = snprintf(buf,size,"\"\\n\""); 
            break;
        case '\r': 
            len = snprintf(buf,size,"\"\\r\""); 
            break;
        case '\t': 
            len = snprintf(buf,size,"\"\\t\""); 
            break;
        case '\a': 
            len = snprintf(buf,size,"\"\\a\""); 
            break;
        case '\b': 
            len = snprintf(buf,size,"\"\\b\""); 
            break;
        default:
            if (isprint(byte))
                len = snprintf(buf,size,"\"%c\"",byte);
            else
                len = snprintf(buf,size,"\"\\x%02x\"",(unsigned char)byte);
            break;
    }

    return len;
}

static void __rtdb_cli_reader_set_error_protocol_byte(rtdb_cli_reader *r, char byte) 
{
    char cbuf[8], sbuf[128];

    chrtos(cbuf,sizeof(cbuf),byte);
    snprintf(sbuf,sizeof(sbuf), "Protocol error, got %s as reply type byte", cbuf);
    __rtdb_cli_reader_set_error(r,RTDB_ERR_PROTOCOL,sbuf);
}

static void __rtdb_cli_reader_set_error_oom(rtdb_cli_reader *r) 
{
    __rtdb_cli_reader_set_error(r,RTDB_ERR_OOM,"Out of memory");
}

static char *read_bytes(rtdb_cli_reader *r, unsigned int bytes) 
{
    char *p;
    if (r->len-r->pos >= bytes) 
    {
        p = r->buf+r->pos;
        r->pos += bytes;
        return p;
    }
    return NULL;
}

/* Find pointer to \r\n. */
static char *seek_newline(char *s, size_t len) 
{
    int pos = 0;
    int _len = len-1;

    /* Position should be < len-1 because the character at "pos" should be
     * followed by a \n. Note that strchr cannot be used because it doesn't
     * allow to search a limited length and the buffer that is being searched
     * might not have a trailing NULL character. */
    while (pos < _len) 
    {
        while(pos < _len && s[pos] != '\r') pos++;
        if (s[pos] != '\r') 
        {
            /* Not found. */
            return NULL;
        } 
        else 
        {
            if (s[pos+1] == '\n') 
            {
                /* Found. */
                return s+pos;
            } 
            else 
            {
                /* Continue searching. */
                pos++;
            }
        }
    }
    return NULL;
}

/* Read a long long value starting at *s, under the assumption that it will be
 * terminated by \r\n. Ambiguously returns -1 for unexpected input. */
static long long readLongLong(char *s) 
{
    long long v = 0;
    int dec, mult = 1;
    char c;

    if (*s == '-') 
    {
        mult = -1;
        s++;
    } 
    else if (*s == '+') 
    {
        mult = 1;
        s++;
    }

    while ((c = *(s++)) != '\r') 
    {
        dec = c - '0';
        if (dec >= 0 && dec < 10) 
        {
            v *= 10;
            v += dec;
        } 
        else 
        {
            /* Should not happen... */
            return -1;
        }
    }

    return mult*v;
}

static char *readLine(rtdb_cli_reader *r, int *_len) 
{
    char *p, *s;
    int len;

    p = r->buf+r->pos;
    s = seek_newline(p,(r->len-r->pos));
    if (s != NULL) 
    {
        len = s-(r->buf+r->pos);
        r->pos += len+2; /* skip \r\n */
        if (_len) *_len = len;
        return p;
    }
    return NULL;
}

static void moveToNextTask(rtdb_cli_reader *r) 
{
    rtdb_cli_read_task *cur, *prv;
    while (r->ridx >= 0) 
    {
        /* Return a.s.a.p. when the stack is now empty. */
        if (r->ridx == 0) 
        {
            r->ridx--;
            return;
        }

        cur = &(r->rstack[r->ridx]);
        prv = &(r->rstack[r->ridx-1]);
        assert(prv->type == RTDB_REPLY_ARRAY);
        if (cur->idx == prv->elements-1) 
        {
            r->ridx--;
        } 
        else 
        {
            /* Reset the type because the next item can be anything */
            assert(cur->idx < prv->elements);
            cur->type = -1;
            cur->elements = -1;
            cur->idx++;
            return;
        }
    }
}

static int processLineItem(rtdb_cli_reader *r) 
{
    rtdb_cli_read_task *cur = &(r->rstack[r->ridx]);
    void *obj;
    char *p;
    int len;

    if ((p = readLine(r,&len)) != NULL) 
    {
        if (cur->type == RTDB_REPLY_INTEGER) 
        {
            if (r->fn && r->fn->create_integer)
                obj = r->fn->create_integer(cur,readLongLong(p));
            else
                obj = (void*)RTDB_REPLY_INTEGER;
        } 
        else 
        {
            /* Type will be error or status. */
            if (r->fn && r->fn->create_string)
                obj = r->fn->create_string(cur,p,len);
            else
                obj = (void*)(size_t)(cur->type);
        }

        if (obj == NULL) 
        {
            __rtdb_cli_reader_set_error_oom(r);
            return RTDB_ERR;
        }

        /* Set reply if this is the root object. */
        if (r->ridx == 0) r->reply = obj;
        moveToNextTask(r);
        return RTDB_OK;
    }

    return RTDB_ERR;
}

static int processBulkItem(rtdb_cli_reader *r) 
{
    rtdb_cli_read_task *cur = &(r->rstack[r->ridx]);
    void *obj = NULL;
    char *p, *s;
    long len;
    unsigned long bytelen;
    int success = 0;

    p = r->buf+r->pos;
    s = seek_newline(p,r->len-r->pos);
    if (s != NULL) 
    {
        p = r->buf+r->pos;
        bytelen = s-(r->buf+r->pos)+2; /* include \r\n */
        len = (long)readLongLong(p);

        if (len < 0) 
        {
            /* The nil object can always be created. */
            if (r->fn && r->fn->create_nil)
                obj = r->fn->create_nil(cur);
            else
                obj = (void*)RTDB_REPLY_NIL;
            success = 1;
        } 
        else 
        {
            /* Only continue when the buffer contains the entire bulk item. */
            bytelen += len+2; /* include \r\n */
            if (r->pos+bytelen <= r->len) 
            {
                if (r->fn && r->fn->create_string)
                    obj = r->fn->create_string(cur,s+2,len);
                else
                    obj = (void*)RTDB_REPLY_STRING;
                success = 1;
            }
        }

        /* Proceed when obj was created. */
        if (success) 
        {
            if (obj == NULL) 
            {
                __rtdb_cli_reader_set_error_oom(r);
                return RTDB_ERR;
            }

            r->pos += bytelen;

            /* Set reply if this is the root object. */
            if (r->ridx == 0) r->reply = obj;
            moveToNextTask(r);
            return RTDB_OK;
        }
    }

    return RTDB_ERR;
}

static int processMultiBulkItem(rtdb_cli_reader *r) 
{
    rtdb_cli_read_task *cur = &(r->rstack[r->ridx]);
    void *obj;
    char *p;
    long elements;
    int root = 0;

    /* Set error for nested multi bulks with depth > 7 */
    if (r->ridx == 8) 
    {
        __rtdb_cli_reader_set_error(r,RTDB_ERR_PROTOCOL, "No support for nested multi bulk replies with depth > 7");
        return RTDB_ERR;
    }

    if ((p = readLine(r,NULL)) != NULL) 
    {
        elements = (long)readLongLong(p);
        root = (r->ridx == 0);

        if (elements == -1) 
        {
            if (r->fn && r->fn->create_nil)
                obj = r->fn->create_nil(cur);
            else
                obj = (void*)RTDB_REPLY_NIL;

            if (obj == NULL) 
            {
                __rtdb_cli_reader_set_error_oom(r);
                return RTDB_ERR;
            }

            moveToNextTask(r);
        } 
        else 
        {
            if (r->fn && r->fn->create_array)
                obj = r->fn->create_array(cur,elements);
            else
                obj = (void*)RTDB_REPLY_ARRAY;

            if (obj == NULL) 
            {
                __rtdb_cli_reader_set_error_oom(r);
                return RTDB_ERR;
            }

            /* Modify task stack when there are more than 0 elements. */
            if (elements > 0) 
            {
                cur->elements = elements;
                cur->obj = obj;
                r->ridx++;
                r->rstack[r->ridx].type = -1;
                r->rstack[r->ridx].elements = -1;
                r->rstack[r->ridx].idx = 0;
                r->rstack[r->ridx].obj = NULL;
                r->rstack[r->ridx].parent = cur;
                r->rstack[r->ridx].privdata = r->privdata;
            } 
            else 
            {
                moveToNextTask(r);
            }
        }

        /* Set reply if this is the root object. */
        if (root) r->reply = obj;
        return RTDB_OK;
    }

    return RTDB_ERR;
}

static int processItem(rtdb_cli_reader *r) 
{
    rtdb_cli_read_task *cur = &(r->rstack[r->ridx]);
    char *p;

    /* check if we need to read type */
    if (cur->type < 0) 
    {
        if ((p = read_bytes(r,1)) != NULL) 
        {
            switch (p[0]) 
            {
                case '-':
                    cur->type = RTDB_REPLY_ERROR;
                    break;
                case '+':
                    cur->type = RTDB_REPLY_STATUS;
                    break;
                case ':':
                    cur->type = RTDB_REPLY_INTEGER;
                    break;
                case '$':
                    cur->type = RTDB_REPLY_STRING;
                    break;
                case '*':
                    cur->type = RTDB_REPLY_ARRAY;
                    break;
                default:
                    __rtdb_cli_reader_set_error_protocol_byte(r,*p);
                    return RTDB_ERR;
            }
        }
        else 
        {
            /* could not consume 1 byte */
            return RTDB_ERR;
        }
    }

    /* process typed item */
    switch(cur->type) 
    {
        case RTDB_REPLY_ERROR:
        case RTDB_REPLY_STATUS:
        case RTDB_REPLY_INTEGER:
            return processLineItem(r);
        case RTDB_REPLY_STRING:
            return processBulkItem(r);
        case RTDB_REPLY_ARRAY:
            return processMultiBulkItem(r);
        default:
            assert(NULL);
            return RTDB_ERR; /* Avoid warning. */
    }
}

rtdb_cli_reader *rtdb_cli_reader_create(void) 
{
    rtdb_cli_reader *r;

    r = calloc(sizeof(rtdb_cli_reader),1);
    if (r == NULL) return NULL;

    r->err = 0;
    r->errstr[0] = '\0';
    r->fn = &defaultFunctions;
    r->buf = sdsempty();
    r->maxbuf = RTDB_READER_MAX_BUF;
    if (r->buf == NULL) 
    {
        free(r);
        return NULL;
    }

    r->ridx = -1;
    return r;
}

void rtdb_cli_reader_free(rtdb_cli_reader *r) 
{
    if (r->reply != NULL && r->fn && r->fn->free_object) 
    {
        r->fn->free_object(r->reply);
    }
    if (r->buf != NULL) sdsfree(r->buf);
    free(r);
}

int rtdb_cli_reader_feed(rtdb_cli_reader *r, const char *buf, size_t len) 
{
    sds newbuf;

    /* Return early when this reader is in an erroneous state. */
    if (r->err) return RTDB_ERR;

    /* Copy the provided buffer. */
    if (buf != NULL && len >= 1) 
    {
        /* Destroy internal buffer when it is empty and is quite large. */
        if (r->len == 0 && r->maxbuf != 0 && sdsavail(r->buf) > r->maxbuf) 
        {
            sdsfree(r->buf);
            r->buf = sdsempty();
            r->pos = 0;

            /* r->buf should not be NULL since we just free'd a larger one. */
            assert(r->buf != NULL);
        }
        newbuf = sdscatlen(r->buf,buf,len);
        if (newbuf == NULL) {
            __rtdb_cli_reader_set_error_oom(r);
            return RTDB_ERR;
        }

        r->buf = newbuf;
        r->len = sdslen(r->buf);
    }

    return RTDB_OK;
}

int rtdb_cli_reader_get_reply(rtdb_cli_reader *r, void **reply) 
{
    if (reply != NULL) *reply = NULL; /* Default target pointer to NULL. */
    if (r->err) return RTDB_ERR; /* Return early when this reader is in an erroneous state. */
    if (r->len == 0) return RTDB_OK; /* When the buffer is empty, there will never be a reply. */
    if (r->ridx == -1) 
    {/* Set first item to process when the stack is empty. */
        r->rstack[0].type = -1;
        r->rstack[0].elements = -1;
        r->rstack[0].idx = -1;
        r->rstack[0].obj = NULL;
        r->rstack[0].parent = NULL;
        r->rstack[0].privdata = r->privdata;
        r->ridx = 0;
    }

    /* Process items in reply. */
    while (r->ridx >= 0) 
    {
        if (processItem(r) != RTDB_OK) 
        {
            break;
        }
    }
    
    if (r->err) return RTDB_ERR; /* Return ASAP when an error occurred. */

    /* Discard part of the buffer when we've consumed at least 1k, to avoid
     * doing unnecessary calls to memmove() in sds.c. */
    if (r->pos >= 1024) 
    {
        sdsrange(r->buf,r->pos,-1);
        r->pos = 0;
        r->len = sdslen(r->buf);
    }

    /* Emit a reply when there is one. */
    if (r->ridx == -1) 
    {
        if (reply != NULL) *reply = r->reply;
        r->reply = NULL;
    }
    return RTDB_OK;
}

/* Calculate the number of bytes needed to represent an integer as string. */
static int intlen(int i) 
{
    int len = 0;
    if (i < 0) 
    {
        len++;
        i = -i;
    }
    do 
    {
        len++;
        i /= 10;
    } while(i);
    return len;
}

/* Helper that calculates the bulk length given a certain string length. */
static size_t bulklen(size_t len) 
{
    return 1+intlen(len)+2+len+2;
}

int rtdb_cli_vformat_command(char **target, const char *format, va_list ap) 
{
    const char *c = format;
    char *cmd = NULL; /* final command */
    int pos; /* position in final command */
    sds curarg, newarg; /* current argument */
    int touched = 0; /* was the current argument touched? */
    char **curargv = NULL, **newargv = NULL;
    int argc = 0;
    int totlen = 0;
    int j;

    /* Abort if there is not target to set */
    if (target == NULL) return -1;

    /* Build the command string accordingly to protocol */
    curarg = sdsempty();
    if (curarg == NULL) return -1;

    while(*c != '\0') 
    {
        if (*c != '%' || c[1] == '\0') 
        {
            if (*c == ' ') 
            {
                if (touched) 
                {
                    newargv = realloc(curargv,sizeof(char*)*(argc+1));
                    if (newargv == NULL) goto err;
                    curargv = newargv;
                    curargv[argc++] = curarg;
                    totlen += bulklen(sdslen(curarg));

                    /* curarg is put in argv so it can be overwritten. */
                    curarg = sdsempty();
                    if (curarg == NULL) goto err;
                    touched = 0;
                }
            } 
            else 
            {
                newarg = sdscatlen(curarg,c,1);
                if (newarg == NULL) goto err;
                curarg = newarg;
                touched = 1;
            }
        } 
        else 
        {
            char *arg;
            size_t size;

            /* Set newarg so it can be checked even if it is not touched. */
            newarg = curarg;

            switch(c[1]) 
            {
                case 's':
                    arg = va_arg(ap,char*);
                    size = strlen(arg);
                    if (size > 0)
                        newarg = sdscatlen(curarg,arg,size);
                    break;
                case 'b':
                    arg = va_arg(ap,char*);
                    size = va_arg(ap,size_t);
                    if (size > 0)
                        newarg = sdscatlen(curarg,arg,size);
                    break;
                case '%':
                    newarg = sdscat(curarg,"%");
                    break;
                default:
                    /* Try to detect printf format */
                    {
                        static const char intfmts[] = "diouxX";
                        char _format[16];
                        const char *_p = c+1;
                        size_t _l = 0;
                        va_list _cpy;

                        /* Flags */
                        if (*_p != '\0' && *_p == '#') _p++;
                        if (*_p != '\0' && *_p == '0') _p++;
                        if (*_p != '\0' && *_p == '-') _p++;
                        if (*_p != '\0' && *_p == ' ') _p++;
                        if (*_p != '\0' && *_p == '+') _p++;

                        /* Field width */
                        while (*_p != '\0' && isdigit(*_p)) _p++;

                        /* Precision */
                        if (*_p == '.') 
                        {
                            _p++;
                            while (*_p != '\0' && isdigit(*_p)) _p++;
                        }

                        /* Copy va_list before consuming with va_arg */
                        va_copy(_cpy,ap);

                        /* Integer conversion (without modifiers) */
                        if (strchr(intfmts,*_p) != NULL) 
                        {
                            va_arg(ap,int);
                            goto fmt_valid;
                        }

                        /* Double conversion (without modifiers) */
                        if (strchr("eEfFgGaA",*_p) != NULL) 
                        {
                            va_arg(ap,double);
                            goto fmt_valid;
                        }

                        /* Size: char */
                        if (_p[0] == 'h' && _p[1] == 'h') 
                        {
                            _p += 2;
                            if (*_p != '\0' && strchr(intfmts,*_p) != NULL) 
                            {
                                va_arg(ap,int); /* char gets promoted to int */
                                goto fmt_valid;
                            }
                            goto fmt_invalid;
                        }

                        /* Size: short */
                        if (_p[0] == 'h') 
                        {
                            _p += 1;
                            if (*_p != '\0' && strchr(intfmts,*_p) != NULL) 
                            {
                                va_arg(ap,int); /* short gets promoted to int */
                                goto fmt_valid;
                            }
                            goto fmt_invalid;
                        }

                        /* Size: long long */
                        if (_p[0] == 'l' && _p[1] == 'l') 
                        {
                            _p += 2;
                            if (*_p != '\0' && strchr(intfmts,*_p) != NULL) 
                            {
                                va_arg(ap,long long);
                                goto fmt_valid;
                            }
                            goto fmt_invalid;
                        }

                        /* Size: long */
                        if (_p[0] == 'l') 
                        {
                            _p += 1;
                            if (*_p != '\0' && strchr(intfmts,*_p) != NULL) 
                            {
                                va_arg(ap,long);
                                goto fmt_valid;
                            }
                            goto fmt_invalid;
                        }

                    fmt_invalid:
                        va_end(_cpy);
                        goto err;

                    fmt_valid:
                        _l = (_p+1)-c;
                        if (_l < sizeof(_format)-2) 
                        {
                            memcpy(_format,c,_l);
                            _format[_l] = '\0';
                            newarg = sdscatvprintf(curarg,_format,_cpy);

                            /* Update current position (note: outer blocks
                             * increment c twice so compensate here) */
                            c = _p-1;
                        }

                        va_end(_cpy);
                        break;
                    }
            }

            if (newarg == NULL) goto err;
            curarg = newarg;

            touched = 1;
            c++;
        }
        c++;
    }

    /* Add the last argument if needed */
    if (touched) 
    {
        newargv = realloc(curargv,sizeof(char*)*(argc+1));
        if (newargv == NULL) goto err;
        curargv = newargv;
        curargv[argc++] = curarg;
        totlen += bulklen(sdslen(curarg));
    } 
    else 
    {
        sdsfree(curarg);
    }

    /* Clear curarg because it was put in curargv or was free'd. */
    curarg = NULL;

    /* Add bytes needed to hold multi bulk count */
    totlen += 1+intlen(argc)+2;

    /* Build the command at protocol level */
    cmd = malloc(totlen+1);
    if (cmd == NULL) goto err;

    pos = sprintf(cmd,"*%d\r\n",argc);
    for (j = 0; j < argc; j++) 
    {
#ifdef _WIN32
        pos += sprintf(cmd + pos, "$%Iu\r\n", sdslen(curargv[j]));
#else
        pos += sprintf(cmd + pos, "$%zu\r\n", sdslen(curargv[j]));
#endif
        memcpy(cmd+pos,curargv[j],sdslen(curargv[j]));
        pos += sdslen(curargv[j]);
        sdsfree(curargv[j]);
        cmd[pos++] = '\r';
        cmd[pos++] = '\n';
    }
    assert(pos == totlen);
    cmd[pos] = '\0';

    free(curargv);
    *target = cmd;
    return totlen;

err:
    while(argc--)
        sdsfree(curargv[argc]);
    free(curargv);

    if (curarg != NULL)
        sdsfree(curarg);

    /* No need to check cmd since it is the last statement that can fail,
     * but do it anyway to be as defensive as possible. */
    if (cmd != NULL)
        free(cmd);

    return -1;
}


void __rtdbSetError(rtdb_cli_context *c, int type, const char *str) 
{
    size_t len;

    c->err = type;
    if (type == RTDB_ERR_IO || type == RTDB_ERR_EOF) 
    {
        if (c->fd != NETWORK_ERROR) 
        {
            rtdb_cli_net_close(c->fd);
            c->fd = NETWORK_ERROR;
        }
    }
    if (str != NULL) 
    {
        len = strlen(str);
        len = len < (sizeof(c->errstr)-1) ? len : (sizeof(c->errstr)-1);
        memcpy(c->errstr,str,len);
        c->errstr[len] = '\0';
    } 
    else 
    { 
#ifdef _WIN32
        /*strerror_s(c->errstr, sizeof(c->errstr), errno);*/
#else
        /*strerror_r(errno,c->errstr,sizeof(c->errstr));*/
#endif
    }
}

void rtdb_cli_contex_free(rtdb_cli_context *c) 
{
    if(!c) return ;
    if (c->fd > 0)
        rtdb_cli_net_close(c->fd);
    if (c->obuf != NULL)
        sdsfree(c->obuf);
    if (c->reader != NULL)
        rtdb_cli_reader_free(c->reader);
    free(c);
    rtdb_cli_net_uninit();
}

rtdb_cli_context *rtdb_cli_connect(const char *ip, int port) 
{
    rtdb_cli_context *c;
    rtdb_cli_net_code code;

    rtdb_cli_net_init();
    c = malloc(sizeof(rtdb_cli_context));
    if(c)
    {
        memset(c, 0, sizeof(rtdb_cli_context));
        c->obuf = sdsempty();
        c->reader = rtdb_cli_reader_create();
    }
    if (c == NULL)
        return NULL;

    c->flags |= RTDB_BLOCK;
    code = rtdb_cli_net_settcpconn(&c->fd, ip, port, NULL);    
    if (CLIENT_OK != code) 
    {
        c->err = code;
        __rtdbSetError(c, RTDB_ERR_IO, NULL);
        rtdb_cli_contex_free(c);
        c = NULL;
        fprintf(stderr,"connect %s:%d error(code : %d)!\n",ip,port,code);
    } 
    else 
    {
        c->flags |= RTDB_CONNECTED;
    }
    
    return c;
}

int rtdb_cli_buffer_read(rtdb_cli_context *c) 
{
    char buf[1024*16];
    int nread;

    /* Return early when the context has seen an error. */
    if (c->err)
        return RTDB_ERR;

    nread = network_read(c->fd,buf,sizeof(buf));
    if (nread == -1) 
    {
        if ((errno == EAGAIN && !(c->flags & RTDB_BLOCK)) || (errno == EINTR)) 
        {
            /* Try again later */
        } 
        else 
        {
            __rtdbSetError(c,RTDB_ERR_IO,NULL);
            return RTDB_ERR;
        }
    } 
    else if (nread == 0) 
    {
        __rtdbSetError(c,RTDB_ERR_EOF,"Server closed the connection");
        return RTDB_ERR;
    } 
    else 
    {
        if (rtdb_cli_reader_feed(c->reader,buf,nread) != RTDB_OK) 
        {
            __rtdbSetError(c,c->reader->err,c->reader->errstr);
            return RTDB_ERR;
        }
    }
    return RTDB_OK;
}

int rtdb_cli_buffer_write(rtdb_cli_context *c, int *done) 
{
    int nwritten;

    /* Return early when the context has seen an error. */
    if (c->err)
        return RTDB_ERR;

    if (sdslen(c->obuf) > 0) 
    {
        nwritten = network_write(c->fd,c->obuf,sdslen(c->obuf));
        if (nwritten == -1) 
        {
            if ((errno == EAGAIN && !(c->flags & RTDB_BLOCK)) || (errno == EINTR)) {
                /* Try again later */
            } else {
                __rtdbSetError(c,RTDB_ERR_IO,NULL);
                return RTDB_ERR;
            }
        } 
        else if (nwritten > 0) 
        {
            if (nwritten == (signed)sdslen(c->obuf)) 
            {
                sdsfree(c->obuf);
                c->obuf = sdsempty();
            } 
            else 
            {
                sdsrange(c->obuf,nwritten,-1);
            }
        }
    }
    if (done != NULL) *done = (sdslen(c->obuf) == 0);
    return RTDB_OK;
}

int rtdb_cli_get_reply_from_reader(rtdb_cli_context *c, void **reply) 
{
    if (rtdb_cli_reader_get_reply(c->reader,reply) == RTDB_ERR) 
    {
        __rtdbSetError(c,c->reader->err,c->reader->errstr);
        return RTDB_ERR;
    }
    return RTDB_OK;
}

int rtdb_cli_get_reply(rtdb_cli_context *c, void **reply) 
{
    int wdone = 0;
    void *aux = NULL;

    /* Try to read pending replies */
    if (rtdb_cli_get_reply_from_reader(c,&aux) == RTDB_ERR)
        return RTDB_ERR;

    /* For the blocking context, flush output buffer and read reply */
    if (aux == NULL && c->flags & RTDB_BLOCK) 
    {
        /* Write until done */
        do 
        {
            if (rtdb_cli_buffer_write(c,&wdone) == RTDB_ERR)
                return RTDB_ERR;
        } while (!wdone);

        /* Read until there is a reply */
        do 
        {
            if (rtdb_cli_buffer_read(c) == RTDB_ERR)
                return RTDB_ERR;
            if (rtdb_cli_get_reply_from_reader(c,&aux) == RTDB_ERR)
                return RTDB_ERR;
        } while (aux == NULL);
    }

    /* Set reply object */
    if (reply != NULL) *reply = aux;
    return RTDB_OK;
}

int __rtdb_cli_append_command(rtdb_cli_context *c, const char *cmd, size_t len) 
{
    sds newbuf;

    newbuf = sdscatlen(c->obuf,cmd,len);
    if (newbuf == NULL) 
    {
        __rtdbSetError(c,RTDB_ERR_OOM,"Out of memory");
        return RTDB_ERR;
    }

    c->obuf = newbuf;
    return RTDB_OK;
}


int rtdb_cli_append_vcommand(rtdb_cli_context *c, const char *format, va_list ap) 
{
    char *cmd;
    int len;

    len = rtdb_cli_vformat_command(&cmd,format,ap);
    if (len == -1) 
    {
        __rtdbSetError(c,RTDB_ERR_OOM,"Out of memory");
        return RTDB_ERR;
    }

    if (__rtdb_cli_append_command(c,cmd,len) != RTDB_OK) 
    {
        free(cmd);
        return RTDB_ERR;
    }

    free(cmd);
    return RTDB_OK;
}

int rtdb_cli_append_command(rtdb_cli_context *c, const char *format, ...) 
{
    va_list ap;
    int ret;

    va_start(ap,format);
    ret = rtdb_cli_append_vcommand(c,format,ap);
    va_end(ap);
    return ret;
}

static void *__rtdb_block_for_reply(rtdb_cli_context *c) 
{
    void *reply;
    if (c->flags & RTDB_BLOCK) 
    {
        if (rtdb_cli_get_reply(c,&reply) != RTDB_OK)
            return NULL;
        return reply;
    }
    return NULL;
}

void *rtdb_cli_vcommand(rtdb_cli_context *c, const char *format, va_list ap) 
{
    if (rtdb_cli_append_vcommand(c,format,ap) != RTDB_OK)
        return NULL;
    return __rtdb_block_for_reply(c);
}

void *rtdb_client_command(rtdb_cli_context *c, const char *format, ...) 
{
    va_list ap;
    void *reply = NULL;
    va_start(ap,format);
    reply = rtdb_cli_vcommand(c,format,ap);
    va_end(ap);
    return reply;
}


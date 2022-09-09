#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <pthread.h>

#include "curl/curl.h"

#include "confcmn.h"
#include "recordset.h"
#include "conf_parser.h"

#define CONFDB_STRING_INCR       1024
#define CONFDB_HTTP_TIMEOUT      60000

#define CONFDB_SQL_SELECT            "SELECT "
#define CONFDB_SQL_UPDATE            "UPDATE"
#define CONFDB_SQL_DELETE            "DELETE"
#define CONFDB_SQL_INSERT            "INSERT"

static char* g_queryUrl = NULL;
static char* g_execUrl  = NULL;

static int confdb_url_build(const char* sql, const struct Pageable* page, char* url, size_t* size);
static size_t confdb_curl_write_data(char *buf, size_t size, size_t nmemb, void *userdata);
static int confdb_curl_impl(const char* url, const char* reqType, const int timeout, char** response);
static int confdb_curl_succeed(const int status);
static int confdb_strncat(char* dest, char* src, size_t* size);
static const char *strcasestr(const char *s1, const char *s2);
static int is_excutable(const char* sql);

int confdb_init() {
    int result = 0;

    //防止重入
    if(g_queryUrl || g_queryUrl) {
        return 0;
    }

    sys_conf_confdb_domain* confdb = sys_conf_get_confdb_domain();
    if(confdb) {
        sys_conf_confdb_server* local = confdb->first;

        if(local && local->http_url) {
            // query
            g_queryUrl = calloc(CONFDB_STRING_INCR, sizeof(char));
            size_t sizeQuery = CONFDB_STRING_INCR;
            confdb_strncat(g_queryUrl, local->http_url, &sizeQuery);
            confdb_strncat(g_queryUrl, "/getPageList", &sizeQuery);

            //exec
            g_execUrl = calloc(CONFDB_STRING_INCR, sizeof(char));
            size_t sizeExec = CONFDB_STRING_INCR;
            confdb_strncat(g_execUrl, local->http_url, &sizeExec);
            confdb_strncat(g_execUrl, "/execSql", &sizeExec);
        }

        sys_conf_free_confdb_domain(confdb);
    } else {
        g_queryUrl = strdup("http://127.0.0.1:7777/conf/getPageList");
        g_execUrl  = strdup("http://127.0.0.1:7777/conf/execSql");
        result = -1;
    }

    return result;
}


int confdb_release() {
    int result = 0;

    if(g_queryUrl) free(g_queryUrl);

    if(g_execUrl) free(g_execUrl);

    return result;
}

struct RecordSet* confdb_query_page(const char* sql, const ConfDbPageable* page) {
    if(!sql) return NULL;
    if(!strcasestr(sql, CONFDB_SQL_SELECT)) return NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    struct RecordSet* rs  = NULL;
    size_t size = strlen(g_queryUrl) + CONFDB_STRING_INCR;
    char* url = calloc(size, sizeof(char));
    *url = '\0';
    strcpy(url, g_queryUrl);
    if(confdb_url_build(sql, page, url, &size) == 0){
        char* buffer = calloc(1, sizeof(char));
        *buffer = '\0';
        if(confdb_curl_succeed(confdb_curl_impl(url, "GET", CONFDB_HTTP_TIMEOUT, &buffer))) {
            rs = confdb_recordset_create(buffer);
        }
        free(buffer);
    }
    free(url);

    curl_global_cleanup();

    return rs;
}

struct RecordSet* confdb_query(const char* sql) {
    return confdb_query_page(sql, NULL);
}

//do insert/update/delete
int confdb_exec(const char* sql) {
    if(!sql) return -1;
    if(!is_excutable(sql)) return -1;

    curl_global_init(CURL_GLOBAL_ALL);

    int result = -1;
    size_t size = strlen(g_execUrl) + CONFDB_STRING_INCR;
    char* url = calloc(size, sizeof(char));
    *url = '\0';
    strcpy(url, g_execUrl);
    if(confdb_url_build(sql, NULL, url, &size) == 0){
        char* buffer = calloc(1, sizeof(char));
        *buffer = '\0';
        if(confdb_curl_succeed(confdb_curl_impl(url, "GET", CONFDB_HTTP_TIMEOUT, &buffer)))
            result = 0;
        free(buffer);
    }
    free(url);

    curl_global_cleanup();

    return result;
}

/**
 * Build real URL to the API using given client config and parameters
 */
static int confdb_url_build(const char* sql, const struct Pageable* page, char* url, size_t* size) {
    if(!url || !size || !(*size) || !sql) return -1;

    //sql 
    char* escape = curl_easy_escape(NULL, sql, 0);
    if(!escape)  return -1;
    confdb_strncat(url, "?sql=", size);
    confdb_strncat(url, escape, size);
    free(escape);

    //page
    if(page) {
        char p[10];
        confdb_strncat(url, "&pageSize=", size);
        sprintf(p, "%d", page->pageSize);
        confdb_strncat(url, p, size);

        confdb_strncat(url, "&pageNumber=", size);
        sprintf(p, "%d", page->pageNumber);
        confdb_strncat(url, p, size);
    }

    return 0;
}

/**
 * Performing real HTTP request
 */
static int confdb_curl_impl(const char* url, const char* reqType, const int timeout, char** response) {
    CURLcode c;
    CURL *handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
    if (reqType != NULL) {
        curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, reqType);
    }
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, confdb_curl_write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, response);

    printf("url -- %s\n",url);
    c = curl_easy_perform(handle);

    if (c == CURLE_OK) {
        long status_code = 0;
        if (curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &status_code) == CURLE_OK)
            c = status_code;
    }

    curl_easy_cleanup(handle);

    return c;
}

/**
 * CURL Callback reading data to userdata buffer
 */
static size_t confdb_curl_write_data(char *buf, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    if (userdata != NULL){
        char **buffer = userdata;

        *buffer = realloc(*buffer, strlen(*buffer) + realsize + 1);

        strncat(*buffer, buf, realsize);
    }
    return realsize;
}

static int confdb_curl_succeed(const int status) {
    return (status >= 200 && status < 300) ? 1:0;
}

static int confdb_strncat(char* dest, char* src, size_t* size) {
    if(!dest || !src || !size) return -1;

    if(strlen(dest) + strlen(src) >= *size) {
        *size = *size + strlen(src) + sizeof(char) * CONFDB_STRING_INCR;
        dest = realloc(dest, *size);
    }
    strncat(dest, src, *size);
    return 0;
}

char* strupr(char* src) {
    char* orign = src;
    for(; *src != '\0'; src++) {
        *src = toupper(*src);
    }

    return orign;
}

const char *strcasestr(const char *s1, const char *s2) {
    // if either pointer is null
    if (s1 == NULL || s2 == NULL)
        return NULL;
    if(s1 == s2) return s1;

    char* upr = strupr(strdup(s1));
    char* result = strstr(upr, s2);
    free(upr);

    return (result ? s1 + (result - upr) : NULL);
}

int is_excutable(const char* sql) {
    int result  = 1;

    if(strcasestr(sql, CONFDB_SQL_SELECT) != NULL)
        return 0;

    return result;
}

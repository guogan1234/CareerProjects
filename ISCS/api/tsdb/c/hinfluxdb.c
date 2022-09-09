#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sods/sods.h>

#include "curl/curl.h"
#include "conf_parser.h"
#include "hinfluxdb.h"

#define TSDB_URL_ADDITION     256

static sods influx_url_build(InfluxDbContext *client, const char *path, const char *sql);

static int influx_curl_impl(InfluxDbContext *client, const char *url,
                            const char *reqType, const int timeout, JSON_Value *body, char **buffer);

static int influx_curl_str_impl(InfluxDbContext *client, const char *url,
                                const char *reqType, const int timeout, const char *body, char **buffer);

static size_t influx_curl_write_data(char *buf, size_t size, size_t nmemb, void *userdata);

static int influx_curl_succeed(const int status);

static pthread_mutex_t g_curlLock = PTHREAD_MUTEX_INITIALIZER;
static int g_curlInited = 0;

InfluxDbContext *tsdb_influx_create() {
    sys_conf_tsdb_domain *data = sys_conf_get_tsdb_domain();
    if (data == NULL || data->first == NULL) {
        if (data != NULL) {
            sys_conf_free_tsdb_domain(data);
        }
        return tsdb_influx_create0("http", "127.0.0.1:8086", "tsdb_iscs", "admin", "123456", 60000, 0, NULL);
    }

    char schema[TSDB_URL_ADDITION], host[TSDB_URL_ADDITION], database[TSDB_URL_ADDITION],
            username[TSDB_URL_ADDITION], password[TSDB_URL_ADDITION];
    snprintf(schema, TSDB_URL_ADDITION, "%s", data->first->schema);
    snprintf(host, TSDB_URL_ADDITION, "%s:%d", data->first->ip_1, data->first->port);
    snprintf(database, TSDB_URL_ADDITION, "%s", data->first->database);
    snprintf(username, TSDB_URL_ADDITION, "%s", data->first->username);
    snprintf(password, TSDB_URL_ADDITION, "%s", data->first->password);
    int timeout = data->first->timeout;

    InfluxDbContext *ret = tsdb_influx_create0(schema, host, database, username, password, timeout, 0, data->log);

    sys_conf_free_tsdb_domain(data);

    return ret;
}

InfluxDbContext *tsdb_influx_create0(const char *schema, const char *host, const char *database, const char *username,
                                     const char *password, const int timeout, const int domain, sys_conf_log *log) {
    pthread_mutex_lock(&g_curlLock);
    if (!g_curlInited) {
        curl_global_init(CURL_GLOBAL_ALL);
        g_curlInited = !0;
    }
    pthread_mutex_unlock(&g_curlLock);

    InfluxDbContext *client = malloc(sizeof(InfluxDbContext));
    if (client) {
        client->schema = strdup(schema);
        client->host = strdup(host);
        client->database = strdup(database);
        client->username = strdup(username);
        client->password = strdup(password);
        client->ssl = 0;
        client->timeout = timeout;
        client->maxQueryRecords = 0;
        client->curl = NULL;
        client->domain = domain;
        client->logger = NULL;

        if (log != NULL) {
            client->logger = cmn_logger_init(log->file, log->level, log->max_size, log->roll_num);
        }
    }

    return client;
}

int tsdb_influx_destroy(InfluxDbContext *client) {
    if (client) {
        if (client->schema) free(client->schema);
        if (client->host) free(client->host);
        if (client->username) free(client->username);
        if (client->password) free(client->password);
        if (client->database) free(client->database);
        if (client->curl) curl_easy_cleanup(client->curl);
        free(client);

        pthread_mutex_lock(&g_curlLock);
        if (g_curlInited) {
            curl_global_cleanup();
            g_curlInited = 0;
        }
        pthread_mutex_unlock(&g_curlLock);

        return 0;
    }

    return -1;
}

JSON_Value *tsdb_influx_ping(InfluxDbContext *client) {
    JSON_Value *resp = NULL;
    sods url = influx_url_build(client, "/ping", NULL);
    if (url) {
        //调用curl http/https功能
        char *buffer = calloc(1, sizeof(char));
        if (influx_curl_succeed(influx_curl_impl(client, url, "GET", client->timeout, NULL, &buffer))) {
            resp = json_parse_string(buffer);
        }
        free(buffer);
        sodsfree(url);
    }

    return resp;
}

JSON_Value *tsdb_influx_query(InfluxDbContext *client, const char *reqType, const char *sql) {
    JSON_Value *resp = NULL;

    sods url = influx_url_build(client, "/query", sql);
    if (url) {
        //调用curl http/https功能
        char *buffer = calloc(1, sizeof(char));
        if (influx_curl_succeed(influx_curl_impl(client, url, reqType, client->timeout, NULL, &buffer))) {
            resp = json_parse_string(buffer);
        }
        free(buffer);
        sodsfree(url);
    }

    return resp;
}

int tsdb_influx_write_str(InfluxDbContext *client, const char *reqType, const char *body) {
    int status = 0;
    sods url = influx_url_build(client, "/write", NULL);
    if (url) {
        //调用curl http/https功能
        char *buffer = calloc(1, sizeof(char));
        status = influx_curl_str_impl(client, url, reqType, client->timeout, body, &buffer);
        free(buffer);
        sodsfree(url);
    }

    return influx_curl_succeed(status) ? 0 : -1;
}

int tsdb_influx_free_reply(InfluxDbContext *client, JSON_Value *reply) {
    if (client) {
        if (reply) {
            json_value_free(reply);
            return 0;
        }
    }

    return -1;
}

/**
 * Build real URL to the API using given client config and parameters
 */
sods influx_url_build(InfluxDbContext *client, const char *path, const char *sql) {
    sods url = sodsempty();
    url = sodscat(url, client->schema);
    url = sodscat(url, "://");
    url = sodscat(url, client->host); //ip:port
    url = sodscat(url, path); // eg: /query或/write

    url = sodscat(url, "?");

    //db
    url = sodscat(url, "db=");
    url = sodscat(url, client->database);

    //auth
    if (strlen(client->username)) {
        url = sodscat(url, "&u=");
        url = sodscat(url, client->username);
    }
    if (strlen(client->password)) {
        url = sodscat(url, "&p=");
        url = sodscat(url, client->password);
    }

    //sql
    if (sql) {
        char *escape = curl_easy_escape(NULL, sql, 0);
        if (escape) {
            url = sodscat(url, "&q=");
            url = sodscat(url, escape);
            free(escape);
        }
    }

    CMN_LOGGER_DEBUG(client->logger, "URL = %s", url);

    return url;
}

/**
 * Performing real HTTP request
 */
int influx_curl_impl(InfluxDbContext *client, const char *url, const char *reqType,
                     const int timeout, JSON_Value *body, char **response) {
    char *form = body != NULL ? json_serialize_to_string(body) : NULL;

    return influx_curl_str_impl(client, url, reqType, timeout, form, response);
}

int influx_curl_str_impl(InfluxDbContext *client, const char *url, const char *reqType,
                         const int timeout, const char *body, char **response) {
    CURLcode c;
    if (client->curl == NULL) {
        client->curl = curl_easy_init();
    }
    CURL *handle = client->curl;

    char domain[20] = {0};
    sprintf(domain, "domain: %d", client->domain);
    struct curl_slist *header = NULL;
    header = curl_slist_append(header, domain);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);
    CMN_LOGGER_DEBUG(client->logger, "Add header %s", domain);

    curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
    if (reqType != NULL) {
        curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, reqType);
    }
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, influx_curl_write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, response);
    if (body != NULL) {
        CMN_LOGGER_DEBUG(client->logger, "Add body %s", body);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, body);
    }

    c = curl_easy_perform(handle);
    curl_slist_free_all(header);
	CMN_LOGGER_DEBUG(client->logger, "curl_easy_perform code(%ld)", c);
    if (c == CURLE_OK) {
        long status_code = 0;
        if (curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &status_code) == CURLE_OK)
            c = status_code;
        CMN_LOGGER_DEBUG(client->logger, "curl_easy_getinfo code(%ld)", status_code);
    }
	CMN_LOGGER_DEBUG(client->logger, "done code(%ld)", c);
    return c;
}

/**
 * CURL Callback reading data to userdata buffer
 */
size_t influx_curl_write_data(char *buf, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    if (userdata != NULL) {
        char **buffer = userdata;

        *buffer = realloc(*buffer, strlen(*buffer) + realsize + 1);

        strncat(*buffer, buf, realsize);
    }
    return realsize;
}

int influx_curl_succeed(const int status) {
    return (status >= 200 && status < 300) ? 1 : 0;
}

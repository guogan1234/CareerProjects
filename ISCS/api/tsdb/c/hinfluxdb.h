/*
 @Copyright Reserved.
 HINFLUXDB.
 Define InfluxDb Http interface, it depends on libcurl4-gnutls-dev.
 Create by Boris.F, 2018.04.23.
 Histories:

 */
#ifndef TSDB_HINFLUXDB_H
#define TSDB_HINFLUXDB_H

#include "cmn_logger.h"
#include "parson.h"
#include "curl/curl.h"

#include "conf_parser.h"

typedef struct InfluxDbContext {
    char *schema;
    char *host;
    char *username;
    char *password;
    char *database;
    int  timeout;
    char ssl; // reserved
    int  maxQueryRecords; //reserved,limit by maxRecords
    int domain;
    CURL * curl;
    cmn_logger* logger;
} InfluxDbContext;

InfluxDbContext *tsdb_influx_create0(const char *schema, const char *host, const char *database, const char *user, const char *password,
                                     const int timeout, const int domain, sys_conf_log *pLog);
InfluxDbContext* tsdb_influx_create();
int tsdb_influx_destroy(InfluxDbContext* client);

//https://docs.influxdata.com/influxdb/v1.5/tools/api/
//operate /ping
JSON_Value* tsdb_influx_ping(InfluxDbContext* client);
//operate /query
JSON_Value* tsdb_influx_query(InfluxDbContext* client, const char* reqType, const char* sql);

int tsdb_influx_write_str(InfluxDbContext *client, const char *reqType, const char *body);

int tsdb_influx_free_reply(InfluxDbContext* client, JSON_Value* reply);

#endif // TSDB_HINFLUXDB_H

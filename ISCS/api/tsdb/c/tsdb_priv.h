#ifndef TSDB_PRIV
#define TSDB_PRIV

#include <sods/sods.h>
#include "tsdb_types.h"


Record *TsdbRecordNew();

void TsdbRecordFree(Record *record);

void TsdbResultRecordFree(ResultRecord *record);

ResultRecord *JsonValueToResultRecord(JSON_Value *value);

sods RecordToString(const char *table, const Record *record);

void CopyRecordToTsdbData(const TsdbResultData *result, TsdbData *data);

char * TsdbDataToQuery(const TsdbData *data);

#endif
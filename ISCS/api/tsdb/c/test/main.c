#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tsdb.h"

int main(int argc, char **argv) {
    printf("test tsdb begin\n");
    TsdbDao *tsdb = tsdb_create("");
    printf("tsdb_create done\n");

    // TsdbData
    TsdbData *sta = tsdb_new_record(tsdb);
    RecordSetString(sta->data, "cpu", "C", 1);
    RecordSetDouble(sta->data, "u", 20, 0);

    TsdbData *sta2 = tsdb_new_record(tsdb);
    RecordSetString(sta2->data, "cpu", "D", 1);
    RecordSetDouble(sta2->data, "u", 30, 0);

    TsdbData *staArr[] = {sta, sta2};
    tsdb_save_record(tsdb, "test", (const TsdbData **) staArr, 2);
    tsdb_free_record(tsdb, sta);
    tsdb_free_record(tsdb, sta2);

    TsdbResultData *stas = NULL;
    size_t stasSize = 0;
    tsdb_select_record(tsdb, "SELECT * FROM test", &stasSize, &stas);
    for (int i = 0; i < stasSize; i++) {
        char *cpu;
        ResultRecordGetString(stas[i].data, "cpu", &cpu);
        puts(cpu);
    }

    tsdb_free_result(tsdb, stas, stasSize);
    TsdbData *update = tsdb_new_record(tsdb);
    update->timestamp = 1546085933502162754;
    RecordSetString(update->data, "cpu", "X", 1);
    RecordSetDouble(update->data, "u", 200.45678, 0);
    tsdb_update_record(tsdb, "test", (const TsdbData **) &update, 1);

    TsdbData *delete = tsdb_new_record(tsdb);
    delete->timestamp = 1528032769952420459;
    tsdb_delete_record(tsdb, "test", (const TsdbData **) &delete, 1);

    TsdbData *update2 = tsdb_new_record(tsdb);
    RecordSetString(update2->data, "cpu", "X", 1);
    RecordSetDouble(update2->data, "u", 123.456, 0);
    RecordSetDouble(update2->data, "v", 456.123, 0);
    tsdb_save_record(tsdb, "test", (const TsdbData **) &update2, 1);

    stas = NULL;
    stasSize = 0;
    tsdb_select_record(tsdb, "SELECT * FROM test", &stasSize, &stas);
    for (int i = 0; i < stasSize; i++) {
        char *cpu;
        double u, v;
        ResultRecordGetString(stas[i].data, "cpu", &cpu);
        ResultRecordGetDouble(stas[i].data, "u", &u);
        ResultRecordGetDouble(stas[i].data, "v", &v);
        printf("cpu=%s, u=%lf, v=%lf\n", cpu, u, v);
    }

    puts("");

    RecordSetDouble(update2->data, "v", 789.910, 0);
    tsdb_update_record(tsdb, "test", (const TsdbData **) &update2, 1);
    stas = NULL;
    stasSize = 0;
    tsdb_select_record(tsdb, "SELECT * FROM test", &stasSize, &stas);
    for (int i = 0; i < stasSize; i++) {
        char *cpu;
        double u, v;
        ResultRecordGetString(stas[i].data, "cpu", &cpu);
        ResultRecordGetDouble(stas[i].data, "u", &u);
        ResultRecordGetDouble(stas[i].data, "v", &v);
        printf("cpu=%s, u=%lf, v=%lf\n", cpu, u, v);
    }

    tsdb_delete_records(tsdb, "DELETE FROM test WHERE cpu='A'");

    tsdb_close(tsdb);
    printf("tsdb_close done\n");

    return 0;
}
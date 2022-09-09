#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "tsdb.h"

void bench_tv_int(size_t count, size_t batch_size);

void bench_tv_20column(size_t count, size_t batch_size);

void bench_select(size_t count, size_t batch_size);


int main(int argc, char **argv) {
    size_t count = 10000, batch = 1000;

    if (argc >= 2) {
        int n = atoi(argv[1]);
        if (n > 0 && n <= 100000000) {
            count = (size_t) n;
        }
    }

    if (argc >= 3) {
        int n = atoi(argv[2]);
        if (n > 0 && n <= 10000) {
            batch = (size_t) n;
        }
    }

    bench_select(count, batch);
    bench_tv_int(count, batch);
    bench_tv_20column(count, batch);

    return 0;
}

typedef void fill(const TsdbData *v);

static void bench_run(const char *name, const char *table, const size_t count, const size_t batch_size, fill *pf) {
    size_t remain = count;
    time_t t0 = time(NULL);
    TsdbData **data = (TsdbData **) malloc(sizeof(TsdbData *) * batch_size);
    uint64_t now = (uint64_t) (time(NULL) * 1000000000);
    TsdbDao *tsdb = tsdb_create("");
    while (1) {
        size_t n = remain >= batch_size ? batch_size : remain;
        for (size_t i = 0; i < n; i++) {
            TsdbData *v = data[i] = tsdb_new_record(tsdb);
            v->timestamp = now++;
            pf(v);

        }
        tsdb_save_record(tsdb, table, (const TsdbData **) data, n);
        for (size_t i = 0; i < n; i++) {
            tsdb_free_record(tsdb, data[i]);
        }

        if (remain > batch_size) {
            remain -= batch_size;
        } else {
            break;
        }
    }
    free(data);
    tsdb_close(tsdb);
    time_t t1 = time(NULL);
    time_t cost = t1 - t0;
    printf("%s count=%ld, batch=%ld, cost=%lds\n", name, count, batch_size, cost);
}

void fill_int(const TsdbData *v) {
    RecordSetString(v->data, "tag", "FOO ,=", 1);
    RecordSetInt32(v->data, "u", 1, 0);
}

void fill_20column(const TsdbData *v) {
    RecordSetString(v->data, "tag", "X", 1);
    RecordSetInt32(v->data, "c0", 0, 0);
    RecordSetInt32(v->data, "c1", 1, 0);
    RecordSetInt32(v->data, "c2", 2, 0);
    RecordSetInt32(v->data, "c3", 3, 0);
    RecordSetInt32(v->data, "c4", 4, 0);
    RecordSetInt32(v->data, "c5", 5, 0);
    RecordSetInt32(v->data, "c6", 6, 0);
    RecordSetInt32(v->data, "c7", 7, 0);
    RecordSetInt32(v->data, "c8", 8, 0);
    RecordSetInt32(v->data, "c9", 9, 0);
    RecordSetInt32(v->data, "d0", 0, 0);
    RecordSetInt32(v->data, "d1", 1, 0);
    RecordSetInt32(v->data, "d2", 2, 0);
    RecordSetInt32(v->data, "d3", 3, 0);
    RecordSetString(v->data, "d4", "一大波测试数据", 0);
    RecordSetString(v->data, "d5", "二大波测试数据", 0);
    RecordSetString(v->data, "d6", "三大波测试数据", 0);
    RecordSetString(v->data, "d7", "四大波测试数据", 0);
}

void bench_tv_int(const size_t count, const size_t batch_size) {
    bench_run(__FUNCTION__, "bench1", count, batch_size, fill_int);
}

void bench_tv_20column(size_t count, size_t batch_size) {
    bench_run(__FUNCTION__, "bench2", count, batch_size, fill_20column);
}

void bench_select(size_t count, size_t batch_size) {
    char buf[255] = {0};
    time_t t0 = time(NULL);
    TsdbDao *tsdb = tsdb_create("");
    size_t remain = count;
    size_t offset = 0;
    while (1) {
        size_t n = remain >= batch_size ? batch_size : remain;
        sprintf(buf, "SELECT * FROM bench1 LIMIT %ld OFFSET %ld", n, offset);
        TsdbResultData *stas = NULL;
        size_t stasSize = 0;
        tsdb_select_record(tsdb, buf, &stasSize, &stas);
        tsdb_free_result(tsdb, stas, stasSize);

        if (remain > batch_size) {
            remain -= batch_size;
        } else {
            break;
        }
    }
    tsdb_close(tsdb);
    time_t t1 = time(NULL);
    time_t cost = t1 - t0;
    printf("%s count=%ld, cost=%lds\n", __FUNCTION__, count, cost);
}
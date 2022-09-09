
// test/test.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "confcmn.h"

static int query_test() {
    //select * from conf.dev_tp_tbl
    ConfDbRecordSet* rs = confdb_query("select * from conf.station_tbl where domain_id=99");
    if(rs) {
        char** columns;
        int size;
        confdb_recordset_get_columns(rs, &columns, &size);

        int count = confdb_recordset_get_count(rs);
        for(int i = 0; i < count; i++) {
            printf("dev_tp_tbl(%d):\n", i);
            for(int j = 0; j < size; j++) {
                const char* val = confdb_recordset_get(rs, i, columns[j]);
                printf("====%s = %s\n", columns[j], val);
            }

            const char** values = confdb_recordset_multi_get(rs, i, (const char**)columns, size);
            confdb_recordset_multi_free(rs, (char **)values);
        }

        int code = confdb_recordset_get_code(rs);
        printf("code(%d)\n",code);

        confdb_recordset_free(rs);
    }
    return 0;
}

static int query_page_test() {
    ConfDbPageable page = {10, 0};
    ConfDbRecordSet* rs = confdb_query_page("select * from conf.dev_tbl order by operate_time", &page);
    if(rs) {
        char** columns;
        int size;
        confdb_recordset_get_columns(rs, &columns, &size);

        int count = confdb_recordset_get_count(rs);
        for(int i = 0; i < count; i++) {
            printf("dev_tbl(%d):\n", i);
            for(int j = 0; j < size; j++) {
                const char* val = confdb_recordset_get(rs, i, columns[j]);
                printf("====%s = %s\n", columns[j], val);
            }
        }

        ConfDbPage* p = confdb_recordset_get_page(rs);
        if(p) {
            printf("query page num=%d, size=%d, totalElements=%ld\n", p->number, p->size, p->totalElements);
        }

        int code = confdb_recordset_get_code(rs);
        printf("code(%d)\n",code);

        confdb_recordset_free(rs);
    }
    return 0;
}


static int exec_test() {
    int result = -1;
    // table not exixt
    result = confdb_exec("update conf.unkown set name='xx' where id=1");

    // tabale and data is valid
    result = confdb_exec("update conf.di_tbl set operate_time=now() where di_label='11.2.adaa.1.1'");

    // table insert & update
    result = confdb_exec("insert into sys_user values(100,'ch中文ch','管理员@123.com');"
                         "update sys_user set username='U管理员U@123.com' where id=100");
    ConfDbRecordSet* rs = confdb_query("select username,id,password from sys_user where id=100");
    if(rs && confdb_recordset_get_count(rs) > 0) {
        const char *username = confdb_recordset_get(rs, 0, "username");
        const char *passwd = confdb_recordset_get(rs, 0, "password");;
        printf("username = %s, passwd = %s\n", username, passwd);

        result = confdb_exec("delete from sys_user where id=100");

        int code = confdb_recordset_get_code(rs);
        printf("code(%d)\n",code);
    }

    return result;
}

int main(int argc, char **argv) {
    printf("test confdb cmn begin\n");

    confdb_init();

    query_test();

    query_page_test();

    exec_test();

    confdb_release();

    printf("confdb done\n");
    return 0;
}




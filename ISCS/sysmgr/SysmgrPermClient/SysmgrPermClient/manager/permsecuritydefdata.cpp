#include "permsecuritydefdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermSecurityDefData::PermSecurityDefData()
{

}

PermSecurityDef *PermSecurityDefData::getPermSecurityInfo(int *ret)
{
    char* sql = (char*)"select * from conf.perm_security_def";
    printf("sql - %s\n",sql);
    return getDataBySql(sql,ret);
}

PermSecurityDef *PermSecurityDefData::getDataBySql(char *sql, int *ret)
{
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        *ret = -1;
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        PermSecurityDef* temp = new PermSecurityDef();
        temp->permSecurityId = strdup(confdb_recordset_get(rs,0,"perm_security_id"));
        temp->permPWDMinLength = strdup(confdb_recordset_get(rs,0,"perm_password_min_length"));
        temp->permPWDMinLetter = strdup(confdb_recordset_get(rs,0,"perm_password_min_letr"));
        temp->permPWDMinNumber = strdup(confdb_recordset_get(rs,0,"perm_password_min_num"));
        temp->permAccountMaxLock = strdup(confdb_recordset_get(rs,0,"perm_account_max_lock"));
        temp->permAccountLockTime = strdup(confdb_recordset_get(rs,0,"perm_account_lock_timer"));
        temp->permAccountInvalidTime = strdup(confdb_recordset_get(rs,0,"perm_account_invalid_timer"));
        printf("PermSecurityDef[permSecurityId] - %s\n",temp->permSecurityId);
        printf("PermSecurityDef[permPWDMinLength] - %s\n",temp->permPWDMinLength);
        printf("PermSecurityDef[permPWDMinLetter] - %s\n",temp->permPWDMinLetter);
        printf("PermSecurityDef[permPWDMinNumber] - %s\n",temp->permPWDMinNumber);
        printf("PermSecurityDef[permAccountMaxLock] - %s\n",temp->permAccountMaxLock);
        printf("PermSecurityDef[permAccountLockTime] - %s\n",temp->permAccountLockTime);
        printf("PermSecurityDef[permAccountInvalidTime] - %s\n",temp->permAccountInvalidTime);

        *ret = 0;
        return temp;
    }
}

#ifndef PERMUSERDEFDATA_H
#define PERMUSERDEFDATA_H

#include "data/permdefine.h"
#include <list>
using namespace std;

class PermUserDefData
{
public:
    PermUserDefData();

    list<PermUserDef*> * loadAll(int *ret);
    list<PermUserDef*> * loadUserInfoById(int userId,int* ret);
    list<PermUserDef*> * loadUserInfoByName(const char* userName,int* ret);
    int delUserById(int userId,int* ret);
    int modifyUserPWD(int userId,const char* password);
    void releaseUserInfo(list<PermUserDef*> * datas);
    bool userUnlock(int userId,const char* password);
    bool userUnlock(const char* userName,const char* password);
  private:
    list<PermUserDef*> * loadDataBySql(const char* sql,int *ret);
    int simpleExecSql(const char* sql,int *ret);
};

#endif // PERMUSERDEFDATA_H

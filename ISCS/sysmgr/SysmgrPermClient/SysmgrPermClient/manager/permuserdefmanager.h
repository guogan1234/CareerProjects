#ifndef PERMUSERDEFMANAGER_H
#define PERMUSERDEFMANAGER_H

#include "permuserdefdata.h"

class PermUserDefManager
{
public:
    PermUserDefManager();
    ~PermUserDefManager();

    list<PermUserDef*> * loadAll(int *ret);
    list<PermUserDef*> * loadUserInfoById(int userId,int* ret);
    list<PermUserDef*> * loadUserInfoByName(const char* userName,int* ret);
    int delUserById(int userId,int* ret);
    int modifyUserPWD(int userId,const char* password);
    void releaseUserInfo(list<PermUserDef*> * datas);
    bool userUnlock(int userId,const char* password);
    bool userUnlock(const char* userName,const char* password);
private:
    PermUserDefData* data;

    void init();
    void initParam();
};

#endif // PERMUSERDEFMANAGER_H

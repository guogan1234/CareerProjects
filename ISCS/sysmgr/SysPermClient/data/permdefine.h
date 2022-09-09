#ifndef PERMDEFINE_H
#define PERMDEFINE_H

struct PermUserDef{
    char* userID;
    char* userName;
    char* userAlias;
    char* userPassword;
    char* userCreateDate;
    char* userExpireDate;
    char* userDesc;
    char* userIfLock;
    char* userLockTime;
};

struct PermGroupDef{
    char* groupId;
    char* groupName;
    char* groupLevel;
    char* groupDesc;
    char* groupPic1;
    char* groupPic2;
    char* groupPic3;
};

struct PermSecurityDef{
    char* permSecurityId;
    char* permPWDMinLength;
    char* permPWDMinLetter;
    char* permPWDMinNumber;
    char* permAccountMaxLock;
    char* permAccountLockTime;
    char* permAccountInvalidTime;
};

struct PermFuncDef{
    char* permId;
    char* permName;
    char* permDefine;
    char* permTypeName;
};

#endif // PERMDEFINE_H

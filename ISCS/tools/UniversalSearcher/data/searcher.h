#ifndef SEARCHER_H
#define SEARCHER_H

struct Domain{
//    int id;
    char* id;
    char* label;
    char* name;
    char* type;
//    int type;
};
typedef struct Domain Domain;

struct Station{
    char* id;
    char* label;
    char* name;
    char* pid;
    char* domainId;
};
typedef struct Station Station;

struct ProSystem{
    char* id;
    char* label;
    char* name;
};
typedef struct ProSystem ProSystem;

struct DevType{
    char* label;
    char* name;
    char* systemId;
};
typedef struct DevType DevType;

struct Dev{
    char* devLabel;
    char* devName;
};
typedef struct Dev Dev;

struct Acc{
    char* accLabel;
    char* accName;
};
typedef struct Acc Acc;

struct Di{
    char* diLabel;
    char* diName;
};
typedef struct Di Di;

struct Ai{
    char* aiLabel;
    char* aiName;
};
typedef struct Ai Ai;

#endif // SEARCHER_H

#ifndef SEARCHER_H
#define SEARCHER_H

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

#endif // SEARCHER_H

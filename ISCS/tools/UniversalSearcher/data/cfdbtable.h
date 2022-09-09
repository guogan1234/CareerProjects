#ifndef CFDBTABLE_H
#define CFDBTABLE_H

struct AccDef{
    char* label;
    char* accBase_1;
    char* accBase_2;
    char* accBase_3;
    char* accBase_4;
};
typedef struct AccDef AccDef;

struct AccLimit{
    char* label;
    char* alarmDelayTime;
    char* alarmLevel;
    char* domainId;
    char* isDelOnAck;
};
typedef struct AccLimit AccLimit;

struct AccTp{
    char* label;
    char* accBase_1;
    char* accBase_2;
    char* accBase_3;
    char* accBase_4;
    char* accCoeff_1;
    char* accCoeff_2;
    char* accCoeff_3;
    char* accCoeff_4;
};
typedef struct AccTp AccTp;

#endif // CFDBTABLE_H

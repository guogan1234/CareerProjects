
struct WorkThreadArg{
    int interval;
};
typedef struct WorkThreadArg WorkThreadArg;

int getWorkInterval();
int getWorkArgs(WorkThreadArg* workArgs);

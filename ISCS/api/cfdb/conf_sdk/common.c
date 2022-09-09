#include "common.h"
#include <time.h>

long get_time_t_msec(){
    time_t t;
    time(&t);
    return t*1000;

//    long l = 1;
//    return l;//test
}

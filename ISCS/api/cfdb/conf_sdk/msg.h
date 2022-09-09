#ifndef MSG_H
#define MSG_H

typedef struct msg_head{
    char start[2];
    long secends;
    int table_len;
    char table_name[20];
    int total_len;
}Msg_head;

typedef struct msg_body{
    int page;
    int page_size;
}Msg_body;

typedef struct msg_tail{
    char crc[2];
    char end[2];
}Msg_tail;

typedef struct msg_full{
    Msg_head head;
    Msg_body body;
    Msg_tail tail;
}Msg_full;

typedef struct msg_data
{
    short b_next;
//    Dev dev;
}Msg_data;

typedef struct msg_full_data{
    Msg_head head;
    Msg_data data;
    Msg_tail tail;
}Msg_full_data;
#endif // MSG_H

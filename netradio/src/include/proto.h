#pragma once

#include"site_type.h"

#define     DEFAULT_RCV_PORT         "1996"
#define     DEFAULT_MGROUP          "224.2.2.2"

#define     MSG_CHANNLE_MAX         (65536-20-8)
#define     MSG_CHANNLE_DATA_MAx    (MSG_CHANNLE_MAX-sizeof(chnid_t))

#define     MSG_LIST_MAX            (65536-20-8)
#define     MSG_LIST_DESC_MAX       (MSG_LIST_MAX-sizeof(chnid_t))

#define     CHANNLE_NUM             200
#define     CHANNLE_LIST_ID         0
#define     CHANNLE_MAX_ID          (CHANNLE_NUM-1)
#define     CHANNLE_MIN_ID          (CHANNLE_LIST_ID+1)


struct msg_channle_st{
    chnid_t chnid;
    uint8_t data[1];                      
}__attribute__((packed));

struct msg_listdesc_st{
    chnid_t chnid;
    uint8_t description[1];
}__attribute__((packed));

struct msg_list_st{
    chnid_t chnid;
    struct msg_listdesc_st list_description[1];
}__attribute__((packed));


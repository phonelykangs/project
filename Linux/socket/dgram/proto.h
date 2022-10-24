#pragma once
#include<stdint.h>

#define  RCV_PORT      "1996"
#define  NAME_MAX      512-8-8

struct msg_st{
    uint32_t math;
    uint32_t chinese;
    uint8_t name[1];
}__attribute__((packed));
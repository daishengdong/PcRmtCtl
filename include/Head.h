//#include "../init/stm32f10x_it.h"
#include <stdbool.h>
#include "../include/hw_config.h"
#define Binary
//#define Json
//#define Xml
#define  DEV_STATE   bool
#pragma pack(push,1) //表示已字节对齐 
#define MAGIC1	'Q'
#define MAGIC2	'B'

typedef struct HeadForSend
{
  u8            magic1;
  u16           pkg_len;
  u8            magic2;
  u8			      content_type;
  u8			      reserve;
}HeadForSend,HeadForRecv;


#ifdef WIN32
typedef struct System{
    HeadForSend   head;
    S16           current_temp;//当前温度
    DEV_STATE     lamb1_state:1;
    DEV_STATE     lamb2_state:1;
    DEV_STATE     cdplayer_state:1;
}System;
#else
typedef struct System{
    HeadForSend   head;
    s16           current_temp;//当前温度
    DEV_STATE     lamb1_state;
    DEV_STATE     lamb2_state;
    DEV_STATE     cdplayer_state;
}System;
#endif//WIN32

//Rec struct
typedef enum DEVICE_ID{
    ID_LAMB1,
    ID_LAMB2,
    ID_CDPLAYER,
    ID_HEATER,
    ID_INDICATOR//连接指示灯
}DEVICE_ID;
typedef enum OPERTATING_TYPE{
    OP_OPEN,
    OP_CLOSE,
    OP_NOT,
    OP_SET
}OP_TYPE;
typedef struct CMD_PKG{
    OP_TYPE                op_type;
    DEVICE_ID              dev_id;
    u8                     op_data;
}CMD_PKG;

void Format_Head(HeadForSend *Head,u16 pkg_length);

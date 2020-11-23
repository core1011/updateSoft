#ifndef COMMONFUN_H
#define COMMONFUN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef unsigned  int u32;
typedef unsigned  char u8;
typedef unsigned  short u16;



#define OUT_INSITE_VALVE1  (0x01)//进站阀1//0X90
#define OUT_INSITE_VALVE2  (0x02)//进站阀2//0X92
#define OUT_LOW_VALVE1     (0x03)//下放阀1//0X94
#define OUT_LOW_VALVE2     (0x04)//下放阀2//0X96
#define OUT_LOW_VALVE3     (0x05)//下放阀3//0X98
#define OUT_LOW_VALVE4     (0x06)//下放阀4//0X9A
#define OUT_OUTSITE_VALVE  (0x07)//出站阀//0X9C
#define OUT_RISE_MOTO1     (0x08)//提升电机1//0X9E
#define OUT_RISE_MOTO2     (0x09)//提升电机2//0XA0
#define OUT_MAIN_MOTO      (0x0A)//主轨电机//0XA2
#define OUT_LED_RED        (0x0B)//红灯//0XA4
#define OUT_LED_GREEN      (0x0C)//绿灯//0XA6
#define OUT_LED_YELLOW     (0x0D)//黄灯//0XA8
#define OUT_LIGHT          (0x0E)//灯//0XAA

#define IN_FULL_LIMIT1     (0x01)//满站限位1//0XAC
#define IN_FULL_LIMIT2     (0x02)//满站限位2//0XAE
#define IN_RISEARM_TRIP1   (0x03)//提升臂1行程//0XB0
#define IN_RISEARM_TRIP2   (0x04)//提升臂2行程//0XB2
#define IN_OUTSITE_CAP     (0x05)//出站缓存点//0XB4
#define IN_MAIN_PUSH       (0x06)//主轨推杆//0XB6
#define IN_OUTSITE_BUTTON  (0x07)//出站按钮//0XB8
#define IN_AIR_DET         (0x08)//气压检测//0XBA
#define IN_TEN_DEVICE      (0x09)//张紧装置//0XBC
#define IN_STATION_CAP1    (0x0A)//站内缓存点1//0XBE
#define IN_STATION_CAP2    (0x0B)//站内缓存点2//0XC0
#define IN_STATION_CAP3    (0x0C)//站内缓存点3//0XC2


//=========================== Macro,Typestruct, & Class ===================//
#define MAIN_ASPECTS_EN  0   //1：大端模式, 0:小端模式

#if MAIN_ASPECTS_EN > 0    //大端模式
    typedef struct
    {
       unsigned int byte0:8;
       unsigned int byte1:8;
    }TYPE_INT16U;

    typedef struct
    {
       signed int byte0:8;
       signed int byte1:8;
    }TYPE_INT16S;

    typedef struct
    {
       unsigned long byte0:8;
       unsigned long byte1:8;
       unsigned long byte2:8;
       unsigned long byte3:8;
    }TYPE_INT32U;

    typedef struct
    {
       signed long byte0:8;
       signed long byte1:8;
       signed long byte2:8;
       signed long byte3:8;
    }TYPE_INT32S;
#else                       //小端模式
    typedef struct
    {
       unsigned int byte1:8;
       unsigned int byte0:8;
    }TYPE_INT16U;

    typedef struct
    {
       signed int byte1:8;
       signed int byte0:8;
    }TYPE_INT16S;

    typedef struct
    {
       unsigned long byte3:8;
       unsigned long byte2:8;
       unsigned long byte1:8;
       unsigned long byte0:8;
    }TYPE_INT32U;

    typedef struct
    {
       signed long byte3:8;
       signed long byte2:8;
       signed long byte1:8;
       signed long byte0:8;
    }TYPE_INT32S;
#endif

typedef union
{
    float          f_FloatData;
    signed long    lg_LongData;
    unsigned long  ul_UlongData;
    TYPE_INT32U   bit;
}TYPECHANGE;


typedef union
{
    signed int        in_IntData;
    unsigned int      uin_UintData;
    TYPE_INT16U   bit;
}TYPECHANGE2;

#define SEND_BUF_MAX 1024

#define BUF_MAX            (1400*4)    //数据缓冲区字节
#define MAX_ROUT           4	  //最长路径
#define MY_ADD             0xF0   //本机地址

#define CMD_READ           0x55   //读命令
#define CMD_WRITE          0x66   //写命令
#define CMD_R_ANSWER       0xAA   //读回应
#define CMD_W_ANSWER       0x99   //写回应

#define CMD_W_SUCCESS      0x88   //写成功
#define CMD_W_FALSE        0x77   //写失败

#define PACKTYPE_AC        0x01  //AC协议帧

#define COM_SCI1           0x01  //串口1
#define COM_SCI2           0x02  //串口2
#define COM_ETH            0x03  //以太网

#define COMTYPE_TX         (1)  //发送包
#define COMTYPE_RX         (2)  //接收包

#define STATE_IDLE         (1)
#define STATE_REV          (2)

#define MSG_SEND  (1)    //发送数据
//数据帧结构体
typedef struct
{
    u8   uch_used;           //是否使用
    u8   uch_head;           //帧头
    u8   uch_resv[4];        //保留字节
    u8   uch_sumrout;        //总路径长度
    u8   uch_nowrout;        //当前路径长度
    u8   uch_rout[MAX_ROUT]; //路径列表
    u8   uch_cmd;            //命令码
    u8   uch_excmd;          //命令类型
    u16  uin_datlen;         //数据区长度
    u8   uch_data[BUF_MAX];  //数据区
    u16  uin_crc;            //校验码
    u8   uch_tail;           //帧尾

    u16  uin_len;            //数据帧长度
    u8   *puch_data;         //数据指针
    u8   uch_type;           //包类型
    u8   uch_comtype;        //端口
    u8   uch_last;           //最近一个字符

    u8   src_ip[4];
    u16  src_port;
    u16  dst_port;

    u8   state;
}AC_FRAME;



#define  LIST_MAX    (50)
typedef	struct
{
    u32  id;                    //id号
    u8   used;                  //使用标志
}CARD;

//--------------------站位属性----------------------//
typedef	struct _SITEVAR
{
    u32  MyID;                  //本机唯一ID号
    //-----------------业务参数--------------------//
    u8   MyType;                //本机类型
         #define  TYPE_NORMAL (1)  //普通工位
         #define  TYPE_FIRST  (3)  //首站工位
         #define  TYPE_LAST   (4)  //尾站工位
    u16  lineid;                //线号
    u16  siteid;                //站号
    u16  insite_offset1;        //进站1偏移
    u16  insite_offset2;        //进站2偏移
    u16  insite_keep;           //进站保持
    u16  lvalve_delay;          //下放阀延时
    u16  risearm_delay;         //提升臂延时
    u16  risearm_trip;          //提升臂行程
    u16  outsite_offset;        //出站偏移
    u8   outsite_cap;           //出站缓冲容量
    u8   tworist_en;            //二次提升使能
    u8   high_en;               //高线使能
    //u16  autoout_per;           //自动出衣周期
    u8   insite_num;            //进站数量
    u8   resv;                  //保留
    //-----------------输出端口--------------------//
    u8   out_insite_valve1;     //进站阀1
    u8   out_insite_valve2;     //进站阀2
    u8   out_low_valve1;        //下放阀1
    u8   out_low_valve2;        //下放阀2
    u8   out_low_valve3;        //下放阀3
    u8   out_low_valve4;        //下放阀4
    u8   out_outsite_valve;     //出站阀
    u8   out_rise_moto1;        //提升电机1
    u8   out_rise_moto2;        //提升电机2
    u8   out_main_moto;         //主轨电机
    u8   out_led_red;           //红灯
    u8   out_led_green;         //绿灯
    u8   out_led_yellow;        //黄灯
    u8   out_light;             //照明灯
    //-----------------输入端口--------------------//
    u8   in_full_limit1;        //满站限位1
    u8   in_full_limit2;        //满站限位2
    u8   in_risearm_trip1;      //提升臂1行程
    u8   in_risearm_trip2;      //提升臂2行程
    u8   in_outsite_cap;        //出站缓存点
    u8   in_main_push;          //主轨推杆
    u8   in_outsite_button;     //出站按钮
    u8   in_air_det;            //气压检测
    u8   in_ten_device;         //张紧装置
    u8   in_station_cap1;       //站内缓存点1
    u8   in_station_cap2;       //站内缓存点2
    u8   in_station_cap3;       //站内缓存点3

    //-----------------实时数据--------------------//
    u8   State;                 //状态
         #define STATE_NORMAL  (1)  //正常
         #define STATE_MOTOERR (2)  //电机故障
         #define STATE_OUTERR  (3)  //出站异常
    u8   IfLogin;               //是否已登录

    u8   OutAck;                //请求出站回应
    u8   Nextbusy;              //下一站忙标志
    u32  NowID;                 //当前在做的id
    u32  disabblecnt;           //禁止下放计数
    u16  NowID_Tailor_Count;    //当前衣架停留时间计数
         #define  TAILOR_COUNT_START (1)
         #define  TAILOR_COUNT_END  (0)
    u8   NowID_Tailor_Keep_Count_Start_Flag; //衣架停留时间开始计数标志
    u8   NowID_Tailor_Keep_Count_End_Flag;   //衣架停留时间结束计数标志
    u8   in_full_limit_state;                //满站限位状态,光电开关检测
    u32  Ack_HangerId_63;
    u32  High_Station_Lock_NowID;            //高站衣架越过读卡器时，保存该衣架ID
    u8   Hnager_Repair_Auto_Up_Flag;         //执行返修命令，用来提升电机
    u32  Repair_HangerId_74;                 //返修衣架ID
    u32  card0cnt;                           //卡号0计数
}SITEVAR;

//--------------------全局变量----------------------//
typedef	struct _STATIONVAR
{
    //-----------------通用属性--------------------//
    u16  BeatFreq;              //心跳包频率
    u16  timeout;               //通信超时
    u8   initflag;				//程序初始化完成标志
    u8   IP[4];                 //IP地址
    u8   Subnet[4];             //子网掩码
    u8   GateWay[4];            //网关地址
    u8   ServerIp[4];			//服务端的IP地址
    u16  ServerPort;			//服务端的端口
    u8   NetState;              //网络状态
    u8   main_moto_on_flag;    //接收到主轨电机开启后标志
    u8   main_rail_count; //
    u8   air_pressure_statue; //气压监测
    //-----------------站点数据--------------------//
    SITEVAR site;            //站点数据
}STATIONVAR;


//========================= Variable,Object & Fuction =====================//
unsigned int  GetCrc16Bit(unsigned char *p_uch_Data,unsigned int uin_CrcDataLen);
void          EncodeFloat(float f1, unsigned char *p);
float         DecodeFloat(unsigned char *p);
void          EncodeUlong(unsigned long ul_Data, unsigned char *p);
unsigned long DecodeUlong(unsigned char *p);
void          EncodeLong(signed long lg_Data, unsigned char *p);
signed long   DecodeLong(unsigned char *p);
void          EncodeUint(unsigned int uin_Data, unsigned char *p);
unsigned int  DecodeUint(unsigned char *p);
void          EncodeInt(signed int in_Data, unsigned char *p);
signed int    DecodeInt(unsigned char *p);
void          CopyFromAToB(unsigned char *pAbuf,unsigned char *pBbuf,unsigned int uin_Len);

void          EncodeUlongB(unsigned long ul_Data, unsigned char *p);
void          EncodeUintB(unsigned int uin_Data, unsigned char *p);


unsigned int GetCrc16Bit(unsigned char *p_uch_Data, unsigned int uin_CrcDataLen );
unsigned char   Make_ComPack(AC_FRAME *pst_ComPack);
unsigned char Dec_ComPack(AC_FRAME *pst_ComPack);
void BigendianShortCopy(unsigned short src,unsigned char *dst,int size);
unsigned short BigendianShortTransform(unsigned char *p);
unsigned int BigendianIntTransform(unsigned char *p);

#endif // COMMONFUN_H

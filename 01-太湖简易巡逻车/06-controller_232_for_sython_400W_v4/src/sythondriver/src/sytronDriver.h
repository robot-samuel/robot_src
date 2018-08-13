#pragma once


#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include <numeric>
#include <string>
#include <iostream>
#include<map>
#include <boost/format.hpp>

#include "mbcrc.h"
#include "message.hpp"

class sytronDriver{

public:
	
    typedef      short                  int16; 
    typedef      short                  INT16; 
    typedef    unsigned short int       uint16; 
    typedef    unsigned short int       UINT16; 
    
    typedef    int                      int32; 
    typedef    int                      INT32; 
    typedef    unsigned int             uint32; 
    typedef    unsigned int             UINT32;

    enum BIT{
 	BIT0  = 0x0001,
	BIT1  = 0x0002,
	BIT2  = 0x0004,
	BIT3  = 0x0008,
	BIT4  = 0x0010,
	BIT5  = 0x0020,
	BIT6  = 0x0040,
	BIT7  = 0x0080,
	BIT8  = 0x0100,
	BIT9  = 0x0200,
	BIT10  = 0x0400,
	BIT11  = 0x0800,
	BIT12  = 0x1000,
	BIT13  = 0x2000,
	BIT14  = 0x4000,
	BIT15  = 0x8000,
        };

    enum CMDFUN{
 	Read_Holding = 3,
	Write_Single_Register= 6,

        };
    enum Register_address{
 	FN010_ENABLE = 10016,    //使能控制位
	DN02B_APPS2 = 0x002B,
	DN02B_APPS1 = 0x002C,      //应用层 状态字
	DN05A_CTRLS = 0x005A,
	FN003_CTRLMODE = 10003,     //控制模式  //2713
	FN011_CLEARERROR = 10017,   //清楚错误 2721
	FN012_EMERGENCY  = 10018,   //紧急停车 2722
	FN0E3_LIFEGUARD  = 10227,   //通信中断停车27f3

	DN000_ACTVelocity  = 0x0000,   //实时速度RPM
	DN007_CMDVelocity  = 0x0007,   //速度指令
	DN036_ERROR  = 0x0036,    //实时错误状态
	DN024_IPM_TEM  = 0x0024,    //功率器件温度
	DN032_DIO_IN  = 0x0032,//'数字量逻辑输入
	DN002_Current  = 0x0002,//电流千分比 
	DN01D_CurPos  = 0x001D,//累计位置
	DN080_CurUDC  = 0x0080,//累计位置
	DN030_DIN = 0x0030,//输入io物理输入状态
	CN000_CMDVelocity  = 20000,    //速度指令
	

        };
	
	struct DEVICE_DRIV_Status{		
		unsigned char bit0_isSRDY;		//系统上电自检通过，等待伺服使能（SON）信号
		unsigned char bit1_isENA_SRV;	 //伺服已经使能	
		unsigned char bit2_isRes1;	      //保留	
		unsigned char bit3_isFault;	     //严重故障，必须通过断电才能清除故障报警状态	
		unsigned char bit4_isRes2;	    // 保留	
		unsigned char bit5_EMGING;	   //伺服电机急停状态
		unsigned char bit6_isDIS_SRV;  //内部使能无效，当驱动器发生异常报警时有效	
		unsigned char bit7_Warning;	   //般警告，可以通过报警清除信号（ARST）清除报警状态			
		unsigned char bit8_isRes3; //		
		unsigned char bit9_isRes4;		
		unsigned char bit10_isTR;	 //位置（位置模式）、转速（速度模式）、转矩到达（转矩模式）任意一种有效时即有效；否则无效	
		unsigned char bit11_isINL;	  //	正转禁止或者反转禁止时有效；否则无效
		unsigned char bit12_isRes5;		
		unsigned char bit13_isRes6;		
		unsigned char bit14_isRes7;		
		unsigned char bit15_isRes8;		
		};
	
public:
//api
	sytronDriver();
	sytronDriver(unsigned char add);
	~sytronDriver();


public :

	void print_hex(unsigned char * data);
	void print_hex(std::string &data);

	void Set_add(unsigned char  add);
	unsigned char add();
	bool ConfigFaild();

	std::string Cmd_GetCruPos();
	std::string Cmd_GetVelocity();
	std::string Cmd_GetCurrent();
	std::string Cmd_GetUdc();
	std::string Cmd_GetError();
	std::string Cmd_GetIniTCtrlMode();
	std::string Cmd_GetStauts();
	std::string Cmd_GetCmdVelocity();
	std::string Cmd_GetDevTemp();
	std::string Cmd_GetDevName();
	std::string Cmd_GetDevserialN();
	std::string Cmd_GetDinStatues();

	std::string Cmd_SetEnable();
	std::string Cmd_SetRealse();

	std::string Cmd_SetVelocity(INT16 Velocity);
	std::string Cmd_SetStop();
	std::string Cmd_SetClearError();
	std::string Cmd_QuickStop();

	std::string Cmd_SetLifeGuard(INT16 TIMEOUT);

	

	 void set_dir(int dir);
	 int dir();
	 unsigned  int CruPos();
 	INT16 cru_speed();
 	INT16 cru_speedcmd();
 	INT16 Udc();
 	INT16 effectiveTorque_I();
       INT16 IpmTem();
	UINT16 DrivStatus(); 
	std::string GetStrError();
	void MyMemclear(const void *src,int count) ;
	
	int signleRecord(message &msg,unsigned short int cmd);
	void recordBitstauts(UINT16 statues);
	struct DEVICE_DRIV_Status m_BitStatus;
	UINT16 Dins(); 
	
private:

        unsigned char m_add;   //设备自身地址
        INT16 m_SetVelocity;   //目标速度 rpm
        
        INT16 m_ActVelocity;   //实际速度  rpm
        INT32 m_ActPos;    //实际位置
        INT16 m_ActCurrent;    //实际电流
        INT16 m_ActUdc;   //实际总线电压

	char m_ctrlmode;  //工作模式
	UINT16 m_error1;
	UINT16 m_error2;


	bool m_ConfigFaild;
	INT16 m_DevTemp;
	UINT16 m_Dins;
	UINT32 m_AVelocity;   //梯形加速度
	UINT32 m_DVelocity; //梯形减速度
	UINT16 m_DevStautes;  //工作状态

	 int  m_dir_flag;    //转动方向标志 顺时针1 逆时针-1
	std::map<int,std::string >errormap;

	
	
};




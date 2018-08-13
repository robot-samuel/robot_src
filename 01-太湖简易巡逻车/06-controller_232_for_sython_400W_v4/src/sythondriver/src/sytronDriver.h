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
 	FN010_ENABLE = 10016,    //ʹ�ܿ���λ
	DN02B_APPS2 = 0x002B,
	DN02B_APPS1 = 0x002C,      //Ӧ�ò� ״̬��
	DN05A_CTRLS = 0x005A,
	FN003_CTRLMODE = 10003,     //����ģʽ  //2713
	FN011_CLEARERROR = 10017,   //������� 2721
	FN012_EMERGENCY  = 10018,   //����ͣ�� 2722
	FN0E3_LIFEGUARD  = 10227,   //ͨ���ж�ͣ��27f3

	DN000_ACTVelocity  = 0x0000,   //ʵʱ�ٶ�RPM
	DN007_CMDVelocity  = 0x0007,   //�ٶ�ָ��
	DN036_ERROR  = 0x0036,    //ʵʱ����״̬
	DN024_IPM_TEM  = 0x0024,    //���������¶�
	DN032_DIO_IN  = 0x0032,//'�������߼�����
	DN002_Current  = 0x0002,//����ǧ�ֱ� 
	DN01D_CurPos  = 0x001D,//�ۼ�λ��
	DN080_CurUDC  = 0x0080,//�ۼ�λ��
	DN030_DIN = 0x0030,//����io��������״̬
	CN000_CMDVelocity  = 20000,    //�ٶ�ָ��
	

        };
	
	struct DEVICE_DRIV_Status{		
		unsigned char bit0_isSRDY;		//ϵͳ�ϵ��Լ�ͨ�����ȴ��ŷ�ʹ�ܣ�SON���ź�
		unsigned char bit1_isENA_SRV;	 //�ŷ��Ѿ�ʹ��	
		unsigned char bit2_isRes1;	      //����	
		unsigned char bit3_isFault;	     //���ع��ϣ�����ͨ���ϵ����������ϱ���״̬	
		unsigned char bit4_isRes2;	    // ����	
		unsigned char bit5_EMGING;	   //�ŷ������ͣ״̬
		unsigned char bit6_isDIS_SRV;  //�ڲ�ʹ����Ч���������������쳣����ʱ��Ч	
		unsigned char bit7_Warning;	   //�㾯�棬����ͨ����������źţ�ARST���������״̬			
		unsigned char bit8_isRes3; //		
		unsigned char bit9_isRes4;		
		unsigned char bit10_isTR;	 //λ�ã�λ��ģʽ����ת�٣��ٶ�ģʽ����ת�ص��ת��ģʽ������һ����Чʱ����Ч��������Ч	
		unsigned char bit11_isINL;	  //	��ת��ֹ���߷�ת��ֹʱ��Ч��������Ч
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

        unsigned char m_add;   //�豸�����ַ
        INT16 m_SetVelocity;   //Ŀ���ٶ� rpm
        
        INT16 m_ActVelocity;   //ʵ���ٶ�  rpm
        INT32 m_ActPos;    //ʵ��λ��
        INT16 m_ActCurrent;    //ʵ�ʵ���
        INT16 m_ActUdc;   //ʵ�����ߵ�ѹ

	char m_ctrlmode;  //����ģʽ
	UINT16 m_error1;
	UINT16 m_error2;


	bool m_ConfigFaild;
	INT16 m_DevTemp;
	UINT16 m_Dins;
	UINT32 m_AVelocity;   //���μ��ٶ�
	UINT32 m_DVelocity; //���μ��ٶ�
	UINT16 m_DevStautes;  //����״̬

	 int  m_dir_flag;    //ת�������־ ˳ʱ��1 ��ʱ��-1
	std::map<int,std::string >errormap;

	
	
};




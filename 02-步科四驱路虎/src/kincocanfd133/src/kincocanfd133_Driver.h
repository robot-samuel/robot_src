#pragma once


#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include <numeric>
#include <string>
#include <iostream>
#include<map>
#include <boost/format.hpp>

#include "message.hpp"

class kincors232Fd133Driver{

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

    enum READ_CMD{
 	RBYTE1  = 0x43,   //����������4һ���ֽ�
	RBYTE2  = 0x4b,   //����������2һ���ֽ�
	RBYTE4  = 0x4f,   //����������1һ���ֽ�
	RBYTEOK=0x60,    //д������ȷ
	RBYTEE=0x80,       //д���ݴ���
        };
	
    enum WRITE_CMD{
 	WBYTE1  = 0x23,  //����������4һ���ֽ�
	WBYTE2  = 0x2b,  //����������2һ���ֽ�
	WBYTE4  = 0x2f,  //����������1һ���ֽ�
        };

	struct DEVICE_DRIV_Status{
		unsigned char bit0_isReady_on;
		unsigned char bit1_isSwitched_on;
		unsigned char bit2_isOperation_enable;
		unsigned char bit3_isFault;
		unsigned char bit4_isVoltage_enable;
		unsigned char bit5_Quick_stop;
		unsigned char bit6_Switchon_disabled;
		unsigned char bit7_Warning;
		
		unsigned char bit8_Maunufacture;
		unsigned char bit9_Remote;
		unsigned char bit10_Target_reached;
		unsigned char bit11_Intlim_active;
		unsigned char bit12_Setpoint_Ack;
		unsigned char bit13_Fllowing_Error;
		unsigned char bit14_Commutation_Found;
		unsigned char bit15_Reference_Found;
		};
	struct DEVICE_DRIV_ERROR1{
		unsigned char bit0_Internal_error_alarm;
		unsigned char bit1_Encoder_ABZ_connection_alarm;
		unsigned char bit2_Encoder_UVW_connection_alarm;
		unsigned char bit3_Encoder_count_alarm;
		unsigned char bit4_Drive_high_temperature_alarm;
		unsigned char bit5_Drive_high_voltage_alarm;
		unsigned char bit6_Drive_low_voltage_alarm;
		unsigned char bit7_Drive_overcurrent_alarm;
		
		unsigned char bit8_Absorption_resistance_alarm;
		unsigned char bit9_Position_error_is_too_large_to_alarm;
		unsigned char bit10_Logical_low_voltage_alarm;
		unsigned char bit11_Motor_or_drive_IIT_alarm;
		unsigned char bit12_high_pulse_frequency;
		unsigned char bit13_High_temperature_alarm_for_motor;
		unsigned char bit14_Motor_excitation_alarm;
		unsigned char bit15_Memory_alarm;
		};
	struct DEVICE_DRIV_ERROR2{
		unsigned char bit0_current_sensor;
		unsigned char bit1_watch_dog;
		unsigned char bit2_interrupt_Abort;
		unsigned char bit3_MCU_ID;
		unsigned char bit4_Motor_configuration;
		unsigned char bit5_Logical_output;
		unsigned char bit6_STO1;
		unsigned char bit7_STO2;
		
		unsigned char bit8_External_enable;
		unsigned char bit9_Positive_limit;
		unsigned char bit10_Negative_limit;
		unsigned char bit11_SPI;
		unsigned char bit12_CAN_bus_interrupt;
		unsigned char bit13_Full_closed_loop_direction;
		unsigned char bit14_Main_encoder_ABZ;
		unsigned char bit15_Main_encoder_count;
		};

    enum KINCOCMD{
		GetCruPos = 0x606300,
		GetVelocity = 0x606c00,
		GetCurrent = 0x607800,
		GetUdc = 0x60f712,
		GetError  = 0x260100,    //ʵʱ����״̬1
		GetError2  = 0x260200, //ʵʱ����״̬2
		GetIniTCtrlMode = 0x606000,
		GetStauts = 0x604100,
		GetCmdVelocity = 0x60ff00,
		GetDevTemp = 0x60f70b,
		GetDevName = 0x100800,
		GetDevserialN = 0x2ff800,
		GetDinStatues = 0x60fD00,
		
		SetEnable =  0x604000,
		SetVelocity = 0x60fF00,
		SetAVelocity = 0x608300,  //�ٶ�ģʽ�µļ��ٶ�
		SetDVelocity = 0x608400,  //�ٶ�ģʽ�µļ��ٶ�
		
        };
public:
//api
	kincors232Fd133Driver();
	kincors232Fd133Driver(unsigned char add);
	~kincors232Fd133Driver();


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

	std::string Cmd_SetVelocity(INT32 Velocity);
	std::string Cmd_SetStop();
	std::string Cmd_SetClearError();
	std::string Cmd_QuickStop();

	

	 void set_dir(int dir);
	 int dir();
	 int CruPos();
	 short int cru_speed();
	 short int cru_speedcmd();
	 short int Udc();
	 short int effectiveTorque_I();
	 short int IpmTem();
	 unsigned short int DrivStatus();
	 
	 
	void  signleRecord(message &msg);
	void recordBitstauts(UINT16 statues);
	void recorderror1(UINT16 error1);
	void recorderror2(UINT16 statues);
	void MyMemclear(const void *src,int count) ;
	std::string GetStrError();
	
	struct DEVICE_DRIV_ERROR1 mb_ERROR1;
	struct DEVICE_DRIV_ERROR2 mb_ERROR2;
	struct DEVICE_DRIV_Status m_BitStatus;	
	
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
	char m_Dins;
	UINT32 m_AVelocity;   //���μ��ٶ�
	UINT32 m_DVelocity; //���μ��ٶ�
	UINT16 m_DevStautes;  //����״̬

	 int  m_dir_flag;    //ת�������־ ˳ʱ��1 ��ʱ��-1
	
	
};




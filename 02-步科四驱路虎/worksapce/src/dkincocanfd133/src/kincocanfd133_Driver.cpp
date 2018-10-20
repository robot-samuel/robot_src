 
#include "kincocanfd133_Driver.h"


 
#define  DEBUG_ECHO 0

kincors232Fd133Driver::kincors232Fd133Driver(){

	m_add=127;   //默认为广播地址
	m_ConfigFaild = true;

	
        m_SetVelocity=0;   //目标速度 rpm
        m_ActVelocity=0;   //实际速度  rpm
        m_ActPos = 0;    //实际位置
        m_ActCurrent = 0;    //实际电流
        m_ActUdc = 0;   //实际总线电压
	m_ctrlmode = 0;  //工作模式
	m_error1 = 0;
	m_error2 = 0;
	m_DevTemp = 0;
	m_Dins = 0;
	m_AVelocity = 0;   //梯形加速度
	m_DVelocity = 0; //梯形减速度
	m_DevStautes = 0;  //工作状态
	m_dir_flag = 1;    //转动方向标志 顺时针1 逆时针-1
	
	m_DevStautes=0;
	recordBitstauts(m_DevStautes);
	recorderror1(m_error1);

	

}
kincors232Fd133Driver::kincors232Fd133Driver(unsigned char add)
    :m_add(add)
{
	m_ConfigFaild = true;

        m_SetVelocity=0;   //目标速度 rpm
        m_ActVelocity=0;   //实际速度  rpm
        m_ActPos = 0;    //实际位置
        m_ActCurrent = 0;    //实际电流
        m_ActUdc = 0;   //实际总线电压
	m_ctrlmode = 0;  //工作模式
	m_error1 = 0;
	m_error2 = 0;
	m_DevTemp = 0;
	m_Dins = 0;
	m_AVelocity = 0;   //梯形加速度
	m_DVelocity = 0; //梯形减速度
	m_DevStautes = 0;  //工作状态
	m_dir_flag = 1;    //转动方向标志 顺时针1 逆时针-1
	
	m_DevStautes=0;
	recordBitstauts(m_DevStautes);
	recorderror1(m_error1);

}

kincors232Fd133Driver::~kincors232Fd133Driver(){



}

void kincors232Fd133Driver::MyMemclear(const void *src,int count)   
{   
 
    assert(src);   
    while (count--)   
    {   
       *(char *)src=0;   

        src = (char *)src + 1;   
    }   
} 

//查询实际位置
std::string kincors232Fd133Driver::Cmd_GetCruPos()
{
	//40 63 60 00 00 00 00 00   //实际位置
	#if DEBUG_ECHO
	std::cout<< "hello Cmd_GetCruPos "<<std::endl;
	#endif
	message msg;

	//msg.Set_add(m_add);
	//msg.Set_sizetype(0x40);

	//msg.Set_CMD(GetCruPos);

	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetCruPos;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}


//查询实际速度
std::string kincors232Fd133Driver::Cmd_GetVelocity()
{
	#if DEBUG_ECHO
	std::cout<< "hello Cmd_GetVelocity "<<std::endl;
	#endif
	//40 6C 60 00 00 00 00 00   //实际速度
	message msg;

	//msg.Set_add(m_add);
	//msg.Set_sizetype(0x40);

	//msg.Set_CMD(GetVelocity);

	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetVelocity;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}

//查询实际电流
std::string kincors232Fd133Driver::Cmd_GetCurrent()
{
	//40 78 60 00 00 00 00 00   //实际电流
	#if DEBUG_ECHO
	#endif
	message msg;

	//msg.Set_add(m_add);
	//msg.Set_sizetype(0x40);

	//msg.Set_CMD(GetCurrent);

	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetCurrent;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}

//查询直流总线电压
std::string kincors232Fd133Driver::Cmd_GetUdc()
{
	//40 F7 60 12 00 00 00 00   //直流总线电压
	//40 F7 12 60 00 00 00 00
	message msg;

	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetUdc;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);

	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}

//查询实时报警错误状态
std::string kincors232Fd133Driver::Cmd_GetError()
{
	//40 01 26 00 00 00 00 00   //实时报警错误状态

	message msg;

	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetError;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);

	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}

//查询工作模式
std::string kincors232Fd133Driver::Cmd_GetIniTCtrlMode()
{
	//40 60 60 00 00 00 00 00   //工作模式
	message msg;

	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetIniTCtrlMode;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);

	 std::string res = msg.to_string();
	print_hex(res);

	 return res;
	 
}

//查询驱动器状态字
std::string kincors232Fd133Driver::Cmd_GetStauts()
{
	//40 41 60 00 00 00 00 00   //驱动器状态字
	message msg;

	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetStauts;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}

//驱动器目标速度
std::string kincors232Fd133Driver::Cmd_GetCmdVelocity()
{
	//40 FF 60 00 00 00 00 00   //驱动器目标速度
	message msg;
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetCmdVelocity;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);

	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}

//驱动器温度
std::string kincors232Fd133Driver::Cmd_GetDevTemp()
{
	//40 F7 60 0B 00 00 00 00   //驱动器温度
	message msg;
	
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetDevTemp;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
	std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}


//设备名称
std::string kincors232Fd133Driver::Cmd_GetDevName()
{
	//40 08 10 00 00 00 00 00   //设备名称
	message msg;
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetDevName;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}



//硬件序列号
std::string kincors232Fd133Driver::Cmd_GetDevserialN()
{
	//40 F8 2F 00 00 00 00 00   //硬件序列号
	#if DEBUG_ECHO
	std::cout<< "hello Cmd_GetDevserialN "<<std::endl;
	#endif
	message msg;

	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	
	unsigned int data = GetDevserialN;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
     std::string res = msg.to_string();
	 
	print_hex(res);
	 return res;
	 
}

//输入开口状态
std::string kincors232Fd133Driver::Cmd_GetDinStatues()
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message msg;

	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(0x40,0);
	//msg.Set_CMD(GetDinStatues);
	
	unsigned int data = GetDinStatues;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);	
	
	
      msg.set_data_m_param3(0);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}


//使能伺服闭环
std::string kincors232Fd133Driver::Cmd_SetEnable()
{
	#if DEBUG_ECHO
	std::cout<< "hello Cmd_SetEnable "<<std::endl;
	#endif
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message msg;
	
	//msg.Set_add(m_add);
	//msg.Set_sizetype(WBYTE4);

	//msg.Set_CMD(SetEnable);
	//msg.Set_BYTE(0x0f);
	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(WBYTE4,0);
	//msg.Set_CMD(SetEnable);
	
	unsigned int data = SetEnable;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);	
	
      msg.set_data_m_param3(0x000f);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}


//使能伺服闭环
std::string kincors232Fd133Driver::Cmd_SetRealse()
{
	#if DEBUG_ECHO
	std::cout<< "hello Cmd_SetRealse "<<std::endl;
	#endif
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message msg;

	//msg.Set_add(m_add);
	//msg.Set_sizetype(WBYTE4);

	//msg.Set_CMD(SetEnable);
	//msg.Set_BYTE(0x06);
	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(WBYTE4,0);
	//msg.Set_CMD(SetEnable);
	unsigned int data = SetEnable;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);	

	
      msg.set_data_m_param3(0x0006);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}



//设置速度
std::string kincors232Fd133Driver::Cmd_SetVelocity(INT32 Velocity)
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message msg;

	//msg.Set_add(m_add);
	//msg.Set_sizetype(WBYTE1);

	//msg.Set_CMD(SetVelocity);
	//msg.Set_WWDATA((UINT32)(Velocity*2731)*dir());
	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(WBYTE1,0);
	//msg.Set_CMD(SetVelocity);
	unsigned int data = SetVelocity;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);	
	
      msg.set_data_m_param34((UINT32)(Velocity*2731)*dir());
      //msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}
//停车
std::string kincors232Fd133Driver::Cmd_SetStop()
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message msg;

	//msg.Set_add(m_add);
	//msg.Set_sizetype(WBYTE1);

	//msg.Set_CMD(SetVelocity);
	//msg.Set_WWDATA((UINT32)(0*2731));
	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(WBYTE1,0);
	//msg.Set_CMD(SetVelocity);

	unsigned int data = SetVelocity;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);	
	
      msg.set_data_m_param34((UINT32)(0*2731)*dir());
      //msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}
//急停车
std::string kincors232Fd133Driver::Cmd_QuickStop()
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message msg;

	//msg.Set_add(m_add);
	//msg.Set_sizetype(WBYTE4);

	//msg.Set_CMD(SetEnable);
	//msg.Set_BYTE(0x0B);
	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(WBYTE4,0);
	
	unsigned int data = SetEnable;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0x000B);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}

//清楚错误
std::string kincors232Fd133Driver::Cmd_SetClearError()
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message msg;
	//msg.Set_add(m_add);
	//msg.Set_sizetype(WBYTE4);

	//msg.Set_CMD(SetEnable);
	//msg.Set_BYTE(0x86);
	//msg.Set_check();
	msg.set_fram_info(0x08);
	msg.set_fram_ID(0x0600+add());

	msg.set_data_m_byte(WBYTE4,0);
	
	unsigned int data = SetEnable;
	unsigned char *p = (unsigned char *)&data;
	msg.set_data_m_byte(*(p+1),1);
	msg.set_data_m_byte(*(p+2),2);
	msg.set_data_m_byte(*(p+0),3);
	
      msg.set_data_m_param3(0x0086);
      msg.set_data_m_param4(0);
	 std::string res = msg.to_string();
	print_hex(res);
	 return res;
	 
}







void kincors232Fd133Driver::Set_add(unsigned char  add)
{
	m_add = add;
 }

unsigned char kincors232Fd133Driver::add()
{
	return m_add;
 }

bool kincors232Fd133Driver::ConfigFaild()
{
	return m_ConfigFaild;
 }



void kincors232Fd133Driver::print_hex(unsigned char * data)
{
	#if DEBUG_ECHO
	std::cout<< "Fd133:print_hex:";
	for(std::size_t index=0;index<10;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";
	#endif

 }

void kincors232Fd133Driver::print_hex(std::string &data)
{
	#if DEBUG_ECHO
	std::cout<< "Fd133:print_hex:";
	for(std::size_t index=0;index<13;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";
	#endif

 }

 void kincors232Fd133Driver::set_dir(int dir)
{
    this->m_dir_flag = dir;
}

int kincors232Fd133Driver::dir()
{
    return this->m_dir_flag ;
}


int kincors232Fd133Driver::CruPos(){ return m_ActPos;}   //'电机当前位置
 short int kincors232Fd133Driver::cru_speed(){ return m_ActVelocity;}   //电机当前运行速度
 short int kincors232Fd133Driver::cru_speedcmd(){ return m_SetVelocity;}   //'电机速度调节指令值
 short int kincors232Fd133Driver::Udc(){ return m_ActUdc;}  //母线电压值
 short int kincors232Fd133Driver::effectiveTorque_I(){ return m_ActCurrent;}  //'电机有效转矩率(绝对值): 电流法
short int kincors232Fd133Driver::IpmTem(){ return m_DevTemp;}  //'功率器件温度（单位：度）
unsigned short int kincors232Fd133Driver::DrivStatus(){ return m_DevStautes;} 

std::string kincors232Fd133Driver::GetStrError(){    
    std::string errstr;
    errstr.clear();
    

    if(mb_ERROR1.bit0_Internal_error_alarm){
        errstr+="bit0_Internal_error_alarm ";
    }
    if(mb_ERROR1.bit1_Encoder_ABZ_connection_alarm){
        errstr+="bit1_Encoder_ABZ_connection_alarm ";
    }
    if(mb_ERROR1.bit2_Encoder_UVW_connection_alarm){
        errstr+="bit2_Encoder_UVW_connection_alarm ";
    }
    if(mb_ERROR1.bit3_Encoder_count_alarm){
        errstr+="bit3_Encoder_count_alarm ";
    }
    if(mb_ERROR1.bit4_Drive_high_temperature_alarm){
        errstr+="bit4_Drive_high_temperature_alarm ";
    }
    if(mb_ERROR1.bit5_Drive_high_voltage_alarm){
        errstr+="bit5_Drive_high_voltage_alarm ";
    }
   if(mb_ERROR1.bit6_Drive_low_voltage_alarm){
        errstr+="bit6_Drive_low_voltage_alarm ";
    }
    if(mb_ERROR1.bit7_Drive_overcurrent_alarm){
        errstr+="bit7_Drive_overcurrent_alarm ";
    }
    if(mb_ERROR1.bit8_Absorption_resistance_alarm){
        errstr+="bit8_Absorption_resistance_alarm ";
    }
    if(mb_ERROR1.bit9_Position_error_is_too_large_to_alarm){
        errstr+="bit9_Position_error_is_too_large_to_alarm ";
    }
    if(mb_ERROR1.bit10_Logical_low_voltage_alarm){
        errstr+="bit10_Logical_low_voltage_alarm ";
    }
    if(mb_ERROR1.bit11_Motor_or_drive_IIT_alarm){
        errstr+="bit11_Motor_or_drive_IIT_alarm ";
    }
    if(mb_ERROR1.bit12_high_pulse_frequency){
        errstr+="bit12_high_pulse_frequency ";
    }
    if(mb_ERROR1.bit13_High_temperature_alarm_for_motor){
        errstr+="bit13_High_temperature_alarm_for_motor ";
    }
    if(mb_ERROR1.bit14_Motor_excitation_alarm){
        errstr+="bit14_Motor_excitation_alarm ";
    }
   if(mb_ERROR1.bit15_Memory_alarm){
        errstr+="bit15_Memory_alarm ";
    }

   if(errstr.size()==0){
           errstr+="OK";
    }

   if(errstr != "OK")	{
   		std::cout<<"[error] "<<errstr<<"   size="<<errstr.size()<<std::endl;
   	}
   
   return errstr;
}



void kincors232Fd133Driver::recordBitstauts(UINT16 statues)
{
 	m_BitStatus.bit0_isReady_on  = statues&0x0001; 
	m_BitStatus.bit1_isSwitched_on     = (statues>>1)&0x0001; 
	m_BitStatus.bit2_isOperation_enable= (statues>>2)&0x0001; 
	m_BitStatus.bit3_isFault = (statues>>3)&0x0001; 
	m_BitStatus.bit4_isVoltage_enable= (statues>>4)&0x0001; 
	m_BitStatus.bit5_Quick_stop= (statues>>5)&0x0001; 
	m_BitStatus.bit6_Switchon_disabled= (statues>>6)&0x0001; 
	m_BitStatus.bit7_Warning= (statues>>7)&0x0001; 
	m_BitStatus.bit8_Maunufacture= (statues>>8)&0x0001; 
	m_BitStatus.bit9_Remote= (statues>>9)&0x0001; 
	m_BitStatus.bit10_Target_reached= (statues>>10)&0x0001; 
	m_BitStatus.bit11_Intlim_active= (statues>>11)&0x0001; 
	m_BitStatus.bit12_Setpoint_Ack= (statues>>12)&0x0001; 
	m_BitStatus.bit13_Fllowing_Error= (statues>>13)&0x0001; 
	m_BitStatus.bit14_Commutation_Found= (statues>>14)&0x0001; 
     //#if DEBUG_ECHO
    #if 0
    std::cout<<"bit0_isReady_on ="<<(UINT16)m_BitStatus.bit0_isReady_on<<std::endl;
    std::cout<<"bit1_isSwitched_on ="<<(UINT16)m_BitStatus.bit1_isSwitched_on<<std::endl;
    std::cout<<"bit2_isOperation_enable ="<<(UINT16)m_BitStatus.bit2_isOperation_enable<<std::endl;
    std::cout<<"bit3_isFault ="<<(UINT16)m_BitStatus.bit3_isFault<<std::endl;
    std::cout<<"bit4_isVoltage_enable ="<<(UINT16)m_BitStatus.bit4_isVoltage_enable<<std::endl;
    std::cout<<"bit5_Quick_stop ="<<(UINT16)m_BitStatus.bit5_Quick_stop<<std::endl;
    std::cout<<"bit6_Switchon_disabled ="<<(UINT16)m_BitStatus.bit6_Switchon_disabled<<std::endl;
    std::cout<<"bit7_Warning ="<<(UINT16)m_BitStatus.bit7_Warning<<std::endl;
    std::cout<<"bit8_Maunufacture ="<<(UINT16)m_BitStatus.bit8_Maunufacture<<std::endl;
    std::cout<<"bit9_Remote ="<<(UINT16)m_BitStatus.bit9_Remote<<std::endl;
    std::cout<<"bit10_Target_reached ="<<(UINT16)m_BitStatus.bit10_Target_reached<<std::endl;
    std::cout<<"bit11_Intlim_active ="<<(UINT16)m_BitStatus.bit11_Intlim_active<<std::endl;
    std::cout<<"bit12_Setpoint_Ack ="<<(UINT16)m_BitStatus.bit12_Setpoint_Ack<<std::endl;
    std::cout<<"bit13_Fllowing_Error ="<<(UINT16)m_BitStatus.bit13_Fllowing_Error<<std::endl;
    std::cout<<"bit14_Commutation_Found ="<<(UINT16)m_BitStatus.bit14_Commutation_Found<<std::endl;
  #endif   
}

void kincors232Fd133Driver::recorderror1(UINT16 error1)
{
 	mb_ERROR1.bit0_Internal_error_alarm  = error1&0x0001; 
	mb_ERROR1.bit1_Encoder_ABZ_connection_alarm= (error1>>1)&0x0001; 
	mb_ERROR1.bit2_Encoder_UVW_connection_alarm= (error1>>2)&0x0001; 
	mb_ERROR1.bit3_Encoder_count_alarm= (error1>>3)&0x0001; 
	mb_ERROR1.bit4_Drive_high_temperature_alarm= (error1>>4)&0x0001; 
	mb_ERROR1.bit5_Drive_high_voltage_alarm= (error1>>5)&0x0001; 
	mb_ERROR1.bit6_Drive_low_voltage_alarm= (error1>>6)&0x0001; 
	mb_ERROR1.bit7_Drive_overcurrent_alarm= (error1>>7)&0x0001; 
	mb_ERROR1.bit8_Absorption_resistance_alarm= (error1>>8)&0x0001; 
	mb_ERROR1.bit9_Position_error_is_too_large_to_alarm= (error1>>9)&0x0001; 
	mb_ERROR1.bit10_Logical_low_voltage_alarm= (error1>>10)&0x0001; 
	mb_ERROR1.bit11_Motor_or_drive_IIT_alarm= (error1>>11)&0x0001; 
	mb_ERROR1.bit12_high_pulse_frequency= (error1>>12)&0x0001; 
	mb_ERROR1.bit13_High_temperature_alarm_for_motor= (error1>>13)&0x0001; 
	mb_ERROR1.bit14_Motor_excitation_alarm= (error1>>14)&0x0001; 
	mb_ERROR1.bit15_Memory_alarm= (error1>>15)&0x0001; 
	
     //#if DEBUG_ECHO
    #if 0
    std::cout<<"bit0_Internal_error_alarm ="<<(UINT16)mb_ERROR1.bit0_Internal_error_alarm<<std::endl;
    std::cout<<"bit1_Encoder_ABZ_connection_alarm ="<<(UINT16)mb_ERROR1.bit1_Encoder_ABZ_connection_alarm<<std::endl;
    std::cout<<"bit2_Encoder_UVW_connection_alarm ="<<(UINT16)mb_ERROR1.bit2_Encoder_UVW_connection_alarm<<std::endl;
    std::cout<<"bit3_Encoder_count_alarm ="<<(UINT16)mb_ERROR1.bit3_Encoder_count_alarm<<std::endl;
    std::cout<<"bit4_Drive_high_temperature_alarm ="<<(UINT16)mb_ERROR1.bit4_Drive_high_temperature_alarm<<std::endl;
    std::cout<<"bit5_Drive_high_voltage_alarm ="<<(UINT16)mb_ERROR1.bit5_Drive_high_voltage_alarm<<std::endl;
	std::cout<<"bit6_Drive_low_voltage_alarm ="<<(UINT16)mb_ERROR1.bit6_Drive_low_voltage_alarm<<std::endl;
	std::cout<<"bit7_Drive_overcurrent_alarm ="<<(UINT16)mb_ERROR1.bit7_Drive_overcurrent_alarm<<std::endl;
	std::cout<<"bit8_Absorption_resistance_alarm ="<<(UINT16)mb_ERROR1.bit8_Absorption_resistance_alarm<<std::endl;
	std::cout<<"bit9_Position_error_is_too_large_to_alarm ="<<(UINT16)mb_ERROR1.bit9_Position_error_is_too_large_to_alarm<<std::endl;
	std::cout<<"bit10_Logical_low_voltage_alarm ="<<(UINT16)mb_ERROR1.bit10_Logical_low_voltage_alarm<<std::endl;
	std::cout<<"bit11_Motor_or_drive_IIT_alarm ="<<(UINT16)mb_ERROR1.bit11_Motor_or_drive_IIT_alarm<<std::endl;
	std::cout<<"bit12_high_pulse_frequency ="<<(UINT16)mb_ERROR1.bit12_high_pulse_frequency<<std::endl;
	std::cout<<"bit13_High_temperature_alarm_for_motor ="<<(UINT16)mb_ERROR1.bit13_High_temperature_alarm_for_motor<<std::endl;
  #endif   
}

void kincors232Fd133Driver::recorderror2(UINT16 statues)
{

}

void  kincors232Fd133Driver::signleRecord(message &msg)
{

    unsigned char * pd =(unsigned char *)(msg.get_fram_data());
    #if DEBUG_ECHO
       int x=(int)(unsigned char)pd[0];
        std::cout<<"kincors232Fd133Driver Get_sizetype ="<<boost::format("%02X ")%x<<std::endl;
	 msg.print_hex();	
      #endif
      
	if(msg.Get_sizetype()==0x60){    //设置成功
		m_ConfigFaild = true;
	}
	else if((msg.Get_sizetype()==RBYTE1)||(msg.Get_sizetype()==RBYTE2)||(msg.Get_sizetype()==RBYTE4)){
	
	        unsigned int y=(unsigned int)msg.Get_CMD();
			#if DEBUG_ECHO
	       std::cout<<"kincors232Fd133Driver cmd ="<<boost::format("%X ")%y<<std::endl;	
		   #endif
		   
		switch(msg.Get_CMD()){
			case GetCruPos:   //实时位置
				m_ActPos = (int)msg.get_data_m_param34();
				 #if DEBUG_ECHO
				std::cout<<"m_ActPos="<<boost::format("%d ")%m_ActPos<<std::endl;
				 #endif
				break;
			case GetVelocity:  //实时速度
				m_ActVelocity = (int)msg.get_data_m_param34();
				 #if DEBUG_ECHO
				std::cout<<"m_ActVelocity="<<boost::format("%d ")%m_ActVelocity<<std::endl;
				  #endif
				break;
			case GetCurrent: //实时电流
				m_ActCurrent = (INT16)msg.get_data_m_param3();
				#if DEBUG_ECHO
				std::cout<<"m_ActCurrent="<<boost::format("%d ")%m_ActCurrent<<std::endl;
				#endif
				break;
				
			case GetUdc:   //直流母线电压
				m_ActUdc = msg.get_data_m_param3();
				#if DEBUG_ECHO
				std::cout<<"m_ActUdc="<<m_ActUdc<<std::endl;
				#endif
				//std::cout<<"kincors232Fd133Driver m_ActUdc ="<<boost::format("%X ")%msg.Get_WDATA()<<std::endl;
				break;
			case GetError:
				//错误状态
				m_error1 = msg.get_data_m_param3();
				recorderror1(m_error1);
				#if DEBUG_ECHO
				std::cout<<"m_error1="<<boost::format("%X ")%m_error1<<std::endl;
				#endif
				break;
			case GetError2:
				//错误状态2
				m_error2 = msg.get_data_m_param3();
				#if DEBUG_ECHO
				std::cout<<"m_error2="<<boost::format("%X ")%m_error2<<std::endl;
				#endif
				break;
			case GetIniTCtrlMode:
				//工作模式
				m_ctrlmode = (char)msg.get_data_m_byte(5);
				#if DEBUG_ECHO
				std::cout<<"m_ctrlmode="<<boost::format("%d ")%m_ctrlmode<<std::endl;
				#endif
				break;
			case GetCmdVelocity:
				//读取目标速度
				m_SetVelocity = (INT16)msg.get_data_m_param3();
				#if DEBUG_ECHO
				std::cout<<"m_SetVelocity="<<boost::format("%d ")%m_SetVelocity<<std::endl;
				#endif
				break;
			case GetDevTemp:
				//驱动器温度
				m_DevTemp= (INT16)msg.get_data_m_param3();
				#if DEBUG_ECHO
				std::cout<<"m_DevTemp="<<boost::format("%d ")%m_DevTemp<<std::endl;
				#endif
				break;
			case GetDevName:
				//驱动器名称
			
				break;
			case GetDevserialN:
				//驱动器序列号
				break;
			case GetDinStatues:
				//输入状态
				m_Dins = (char)msg.get_data_m_byte(5);
				break;
			case SetAVelocity:
				//加速度
				m_AVelocity = msg.get_data_m_param34();
				break;
			case SetDVelocity:
				//减速度
				m_DVelocity = msg.get_data_m_param34();
				break;
			case GetStauts:
				//工作状态
				m_DevStautes = msg.get_data_m_param3();
				recordBitstauts(m_DevStautes);
				break;
			default :
				break;
			}
		}
	else{
			m_ConfigFaild = false;
		}
		
 
}


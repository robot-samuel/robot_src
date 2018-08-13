 
#include "sytronDriver.h"
#include <numeric>
#include <string>
#include <iostream>

 
#define  DEBUG_ECHO 0

using namespace std;

sytronDriver::sytronDriver(){

	m_add=1;   //默认为广播地址
	m_ConfigFaild = true;

	m_DevStautes=0;
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
	
       errormap.insert( pair <int, std::string>(1100 , "E AbPE ") );         
	   errormap.insert( pair <int, std::string>(1200 , "E brSL ") );         
	   errormap.insert( pair <int, std::string>(1201 , "E broL ") );         
	   errormap.insert( pair <int, std::string>(1202 , "E brEr ") );         
	   errormap.insert( pair <int, std::string>(1301 , "E coUt ") );         
	   errormap.insert( pair <int, std::string>(1310 , "E cLoS ") );         
	   errormap.insert( pair <int, std::string>(1401 , "E dnRE ") );         
	   errormap.insert( pair <int, std::string>(1500 , "E EnAb ") );         
	   errormap.insert( pair <int, std::string>(1510 , "E EncU ") );         
	   errormap.insert( pair <int, std::string>(1600 , "E FrAE ") );         
	   errormap.insert( pair <int, std::string>(1700 , "E GEAr ") );         
	   errormap.insert( pair <int, std::string>(2200 , "E LUdc ") );         
	   errormap.insert( pair <int, std::string>(2500 , "E oc-A  ") );         
	   errormap.insert( pair <int, std::string>(2501 , "E oc-b  ") );         
	   errormap.insert( pair <int, std::string>(2502 , "E oc-C  ") );         
	   errormap.insert( pair <int, std::string>(2510 , "E oLod ") );         
	   errormap.insert( pair <int, std::string>(2520 , "E oSPE ") );         
	   errormap.insert( pair <int, std::string>(2530 , "E oUdc ") );         
	   errormap.insert( pair <int, std::string>(2600 , "E PArA ") );         
	   errormap.insert( pair <int, std::string>(2610 , "E PEOU ") );         
	   errormap.insert( pair <int, std::string>(2620 , "E PHAS ") );         
	   errormap.insert( pair <int, std::string>(2630 , "E PHot ") );         
	   errormap.insert( pair <int, std::string>(2631 , "E PoEr ") );         
	   errormap.insert( pair <int, std::string>(2640 , "E PorF ") );         
	   errormap.insert( pair <int, std::string>(2645 , "E PosE ") );         
	   errormap.insert( pair <int, std::string>(2650 , "E PoUt ") );         
	   errormap.insert( pair <int, std::string>(2660 , "E PS1E ") );        
	   errormap.insert( pair <int, std::string>(2661 , "E PS2E ") );         
	   errormap.insert( pair <int, std::string>(2900 , "E SPEE ") );         
	   errormap.insert( pair <int, std::string>(3000 , "E Tcon ") );         
	   errormap.insert( pair <int, std::string>(3100 , "E Ubrt ") );         
	   errormap.insert( pair <int, std::string>(3110 , "E USPn ") );         
	   errormap.insert( pair <int, std::string>(3600 , "E 2LoS ") );         
	   errormap.insert( pair <int, std::string>(3601 , "E 2EtE ") );	

}
sytronDriver::sytronDriver(unsigned char add)
    :m_add(add)
{
	m_ConfigFaild = true;
	m_DevStautes=0;
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
	m_dir_flag =1;    //转动方向标志 顺时针1 逆时针-1

       errormap.insert( pair <int, std::string>(1100 , "E AbPE ") );         
	   errormap.insert( pair <int, std::string>(1200 , "E brSL ") );         
	   errormap.insert( pair <int, std::string>(1201 , "E broL ") );         
	   errormap.insert( pair <int, std::string>(1202 , "E brEr ") );         
	   errormap.insert( pair <int, std::string>(1301 , "E coUt ") );         
	   errormap.insert( pair <int, std::string>(1310 , "E cLoS ") );         
	   errormap.insert( pair <int, std::string>(1401 , "E dnRE ") );         
	   errormap.insert( pair <int, std::string>(1500 , "E EnAb ") );         
	   errormap.insert( pair <int, std::string>(1510 , "E EncU ") );         
	   errormap.insert( pair <int, std::string>(1600 , "E FrAE ") );         
	   errormap.insert( pair <int, std::string>(1700 , "E GEAr ") );         
	   errormap.insert( pair <int, std::string>(2200 , "E LUdc ") );         
	   errormap.insert( pair <int, std::string>(2500 , "E oc-A  ") );         
	   errormap.insert( pair <int, std::string>(2501 , "E oc-b  ") );         
	   errormap.insert( pair <int, std::string>(2502 , "E oc-C  ") );         
	   errormap.insert( pair <int, std::string>(2510 , "E oLod ") );         
	   errormap.insert( pair <int, std::string>(2520 , "E oSPE ") );         
	   errormap.insert( pair <int, std::string>(2530 , "E oUdc ") );         
	   errormap.insert( pair <int, std::string>(2600 , "E PArA ") );         
	   errormap.insert( pair <int, std::string>(2610 , "E PEOU ") );         
	   errormap.insert( pair <int, std::string>(2620 , "E PHAS ") );         
	   errormap.insert( pair <int, std::string>(2630 , "E PHot ") );         
	   errormap.insert( pair <int, std::string>(2631 , "E PoEr ") );         
	   errormap.insert( pair <int, std::string>(2640 , "E PorF ") );         
	   errormap.insert( pair <int, std::string>(2645 , "E PosE ") );         
	   errormap.insert( pair <int, std::string>(2650 , "E PoUt ") );         
	   errormap.insert( pair <int, std::string>(2660 , "E PS1E ") );        
	   errormap.insert( pair <int, std::string>(2661 , "E PS2E ") );         
	   errormap.insert( pair <int, std::string>(2900 , "E SPEE ") );         
	   errormap.insert( pair <int, std::string>(3000 , "E Tcon ") );         
	   errormap.insert( pair <int, std::string>(3100 , "E Ubrt ") );         
	   errormap.insert( pair <int, std::string>(3110 , "E USPn ") );         
	   errormap.insert( pair <int, std::string>(3600 , "E 2LoS ") );         
	   errormap.insert( pair <int, std::string>(3601 , "E 2EtE ") );
}

sytronDriver::~sytronDriver(){



}

void sytronDriver::MyMemclear(const void *src,int count)   
{   
 
    assert(src);   
    while (count--)   
    {   
       *(char *)src=0;   

        src = (char *)src + 1;   
    }   
} 

//查询实际位置
std::string sytronDriver::Cmd_GetCruPos()
{
		
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN01D_CurPos);
	sendmsg.set_data_m_length(0x0002);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}


//查询实际速度
std::string sytronDriver::Cmd_GetVelocity()
{
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN000_ACTVelocity);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//查询实际电流
std::string sytronDriver::Cmd_GetCurrent()
{
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN002_Current);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//查询直流总线电压
std::string sytronDriver::Cmd_GetUdc()
{
	//   //直流总线电压 （单位：0.1V）
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN080_CurUDC);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//查询实时报警错误状态
std::string sytronDriver::Cmd_GetError()
{
	//  //实时报警错误状态
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN036_ERROR);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//查询工作模式
std::string sytronDriver::Cmd_GetIniTCtrlMode()
{
	//40 60 60 00 00 00 00 00   //工作模式

	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(FN003_CTRLMODE);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//查询驱动器状态字
std::string sytronDriver::Cmd_GetStauts()
{
	//40 41 60 00 00 00 00 00   //驱动器状态字
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN02B_APPS1);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//驱动器目标速度
std::string sytronDriver::Cmd_GetCmdVelocity()
{
	//40 FF 60 00 00 00 00 00   //驱动器目标速度
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN007_CMDVelocity);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//驱动器温度
std::string sytronDriver::Cmd_GetDevTemp()
{
	//40 F7 60 0B 00 00 00 00   //驱动器温度
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN024_IPM_TEM);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}


//设备名称
std::string sytronDriver::Cmd_GetDevName()
{
	//40 08 10 00 00 00 00 00   //设备名称
	 std::string res;
	
	 return res;
	 
}



//硬件序列号
std::string sytronDriver::Cmd_GetDevserialN()
{
	//40 F8 2F 00 00 00 00 00   //硬件序列号
	 std::string res;
	
	 return res;
	 
}

//输入开口状态
std::string sytronDriver::Cmd_GetDinStatues()
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Read_Holding);  //读数据

	sendmsg.set_data_m_address(DN030_DIN);
	sendmsg.set_data_m_length(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
}


//使能伺服闭环
std::string sytronDriver::Cmd_SetEnable()
{
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Write_Single_Register);  //读数据

	sendmsg.set_data_m_address(FN010_ENABLE);
	sendmsg.set_data_m_write_data(0x0001);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}


//使能伺服闭环
std::string sytronDriver::Cmd_SetRealse()
{
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Write_Single_Register);  //读数据

	sendmsg.set_data_m_address(FN010_ENABLE);
	sendmsg.set_data_m_write_data(0x0000);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}



//设置速度
std::string sytronDriver::Cmd_SetVelocity(INT16 Velocity)
{
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Write_Single_Register);  //读数据

	sendmsg.set_data_m_address(CN000_CMDVelocity);
	sendmsg.set_data_m_write_data((unsigned short int)Velocity*(dir()));
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}
//停车
std::string sytronDriver::Cmd_SetStop()
{
	//40 FD 60 00 00 00 00 00   //指令停车
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Write_Single_Register);  //读数据

	sendmsg.set_data_m_address(CN000_CMDVelocity);
	sendmsg.set_data_m_write_data((unsigned short int)0);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}
//急停车
std::string sytronDriver::Cmd_QuickStop()
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Write_Single_Register);  //读数据

	sendmsg.set_data_m_address(FN012_EMERGENCY);
	sendmsg.set_data_m_write_data(1);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}

//清楚错误
std::string sytronDriver::Cmd_SetClearError()
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Write_Single_Register);  //读数据

	sendmsg.set_data_m_address(FN011_CLEARERROR);
	sendmsg.set_data_m_write_data(1);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}


std::string sytronDriver::Cmd_SetLifeGuard(INT16 TIMEOUT)
{
	//40 FD 60 00 00 00 00 00   //输入开口状态
	message sendmsg;

	sendmsg.set_fram_ID(m_add);
	sendmsg.set_fram_info(Write_Single_Register);  //读数据

	sendmsg.set_data_m_address(FN0E3_LIFEGUARD);
	sendmsg.set_data_m_write_data((unsigned short int)TIMEOUT);
	unsigned short check  = sendmsg.getCRC();
	sendmsg.set_data_m_crc_check(check);
	std::string res = sendmsg.to_string(8);
	
	print_hex(res);
	return res;
	 
}




void sytronDriver::Set_add(unsigned char  add)
{
	m_add = add;
 }

unsigned char sytronDriver::add()
{
	return m_add;
 }

bool sytronDriver::ConfigFaild()
{
	return m_ConfigFaild;
 }


void sytronDriver::print_hex(unsigned char * data)
{
	#if DEBUG_ECHO
	std::cout<< "sytronDriver:print_hex:";
	for(std::size_t index=0;index<10;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";
	#endif

 }

void sytronDriver::print_hex(std::string &data)
{
	#if 0
	int Num = data.size();
	std::cout<< "sytronDriver:print_hex:";
	for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";
	#endif

 }

 void sytronDriver::set_dir(int dir)
{
    this->m_dir_flag = dir;
}

int sytronDriver::dir()
{
    return this->m_dir_flag ;
}

unsigned  int sytronDriver::CruPos(){ return m_ActPos;}   //'电机当前位置
 short int sytronDriver::cru_speed(){ return m_ActVelocity;}   //电机当前运行速度
 short int sytronDriver::cru_speedcmd(){ return m_SetVelocity;}   //'电机速度调节指令值
 short int sytronDriver::Udc(){ return m_ActUdc;}  //母线电压值
 short int sytronDriver::effectiveTorque_I(){ return m_ActCurrent;}  //'电机有效转矩率(绝对值): 电流法
short int sytronDriver::IpmTem(){ return m_DevTemp;}  //'功率器件温度（单位：度）
unsigned short int sytronDriver::DrivStatus(){ return m_DevStautes;} 
unsigned short int  sytronDriver::Dins(){return m_Dins;}
std::string sytronDriver::GetStrError(){    
	std::string error;    
	error = errormap[m_error1];        
	if(error== ""){            
		error = "OK";        
		}    
	return error;
}


void sytronDriver::recordBitstauts(UINT16 statues)
{
 	m_BitStatus.bit0_isSRDY  = statues&0x0001; 
	m_BitStatus.bit1_isENA_SRV     = (statues>>1)&0x0001; 
	m_BitStatus.bit2_isRes1= (statues>>2)&0x0001; 
	m_BitStatus.bit3_isFault= (statues>>3)&0x0001; 
	m_BitStatus.bit4_isRes2= (statues>>4)&0x0001; 
	m_BitStatus.bit5_EMGING = (statues>>5)&0x0001; 
	m_BitStatus.bit6_isDIS_SRV = (statues>>6)&0x0001; 
	m_BitStatus.bit7_Warning = (statues>>7)&0x0001; 
	m_BitStatus.bit8_isRes3 = (statues>>8)&0x0001; 
	m_BitStatus.bit9_isRes4 = (statues>>9)&0x0001; 
	m_BitStatus.bit10_isTR = (statues>>10)&0x0001; 
	m_BitStatus.bit11_isINL= (statues>>11)&0x0001; 
	m_BitStatus.bit12_isRes5= (statues>>12)&0x0001; 
	m_BitStatus.bit13_isRes6= (statues>>13)&0x0001; 
	m_BitStatus.bit14_isRes7= (statues>>14)&0x0001; 
	m_BitStatus.bit15_isRes8 = (statues>>15)&0x0001; 
     //#if DEBUG_ECHO
    #if DEBUG_ECHO
    std::cout<<"bit0_isSRDY ="<<(UINT16)m_BitStatus.bit0_isSRDY<<std::endl;
    std::cout<<"bit1_isENA_SRV ="<<(UINT16)m_BitStatus.bit1_isENA_SRV<<std::endl;
    std::cout<<"bit2_isRes1 ="<<(UINT16)m_BitStatus.bit2_isRes1<<std::endl;
    std::cout<<"bit3_isFault ="<<(UINT16)m_BitStatus.bit3_isFault<<std::endl;
    std::cout<<"bit4_isRes2 ="<<(UINT16)m_BitStatus.bit4_isRes2<<std::endl;
    std::cout<<"bit5_EMGING ="<<(UINT16)m_BitStatus.bit5_EMGING<<std::endl;
    std::cout<<"bit6_isDIS_SRV ="<<(UINT16)m_BitStatus.bit6_isDIS_SRV<<std::endl;
    std::cout<<"bit7_Warning ="<<(UINT16)m_BitStatus.bit7_Warning<<std::endl;
    std::cout<<"bit8_isRes3 ="<<(UINT16)m_BitStatus.bit8_isRes3<<std::endl;
    std::cout<<"bit9_isRes4 ="<<(UINT16)m_BitStatus.bit9_isRes4<<std::endl;
    std::cout<<"bit10_isTR ="<<(UINT16)m_BitStatus.bit10_isTR<<std::endl;
    std::cout<<"bit11_isINL ="<<(UINT16)m_BitStatus.bit11_isINL<<std::endl;
    std::cout<<"bit12_isRes5 ="<<(UINT16)m_BitStatus.bit12_isRes5<<std::endl;
    std::cout<<"bit13_isRes6 ="<<(UINT16)m_BitStatus.bit13_isRes6<<std::endl;
    std::cout<<"bit14_isRes7 ="<<(UINT16)m_BitStatus.bit14_isRes7<<std::endl;
    std::cout<<"bit15_isRes8 ="<<(UINT16)m_BitStatus.bit15_isRes8<<std::endl;
  #endif   
}

int sytronDriver::signleRecord(message &msg,unsigned short int cmd){

	if(msg.recv_get_add() != add()){
			return -1;
		}
	
	if(msg.recv_get_cmd()==Read_Holding){
		switch(cmd){
			case DN02B_APPS1:
				m_DevStautes = msg.recv_get_data1();
				recordBitstauts(m_DevStautes);
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN02B_APPS1:"<<m_DevStautes<<std::endl;
				#endif
				break;
			case FN003_CTRLMODE:
				m_ctrlmode  = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:FN003_CTRLMODE"<<m_ctrlmode<<std::endl;
				#endif
				break;
			case DN000_ACTVelocity:
				m_ActVelocity = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN000_ACTVelocity:"<<m_ActVelocity<<std::endl;
				#endif
				break;
			case DN007_CMDVelocity:
				m_SetVelocity = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN007_CMDVelocity:"<<m_SetVelocity<<std::endl;
				#endif
				break;
			case DN036_ERROR:
				m_error1 = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN036_ERROR:"<<m_error1<<std::endl;
				#endif
				break;
			case DN024_IPM_TEM:
				m_DevTemp = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN024_IPM_TEM:"<<m_DevTemp<<std::endl;
				#endif
				break;
			case DN032_DIO_IN:
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN032_DIO_IN"<<std::endl;
				#endif
				break;
			case DN002_Current:
				m_ActCurrent = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN002_Current:"<<m_ActCurrent<<std::endl;
				#endif
				break;
			case DN01D_CurPos:
				m_ActPos = msg.recv_get_data2();
				m_ActPos <<=16;
				m_ActPos += msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN01D_CurPos:"<<m_ActPos<<std::endl;
				#endif
				break;
			case DN080_CurUDC:
				m_ActUdc = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN080_CurUDC:"<<m_ActUdc<<std::endl;
				#endif
				
				break;
			case DN030_DIN:
				//unsigned short int data= 
				m_Dins = msg.recv_get_data1();
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:DN030_DIN:"<<m_Dins<<std::endl;
				#endif
			default:
				break;
			}

		}
	else if(msg.recv_get_cmd()==Write_Single_Register){
		switch(cmd){
			case FN010_ENABLE:
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:FN010_ENABLE"<<std::endl;
				#endif
				break;
			case FN011_CLEARERROR:
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:FN011_CLEARERROR"<<std::endl;
				#endif
				break;
			case FN012_EMERGENCY:
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:FN012_EMERGENCY"<<std::endl;
				#endif
				break;
			case FN0E3_LIFEGUARD:
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:FN0E3_LIFEGUARD"<<std::endl;
				#endif
				break;
			case CN000_CMDVelocity:
				#if DEBUG_ECHO
				std::cout<< "sytronDriver:recv:CN000_CMDVelocity"<<std::endl;
				#endif
				break;
			default:
				break;
			}

		}
	return -2;

}


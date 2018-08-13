

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream> 

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>


#include "RemoterMode.hpp"
#include "mbcrc.h"

#include <geometry_msgs/Twist.h>


#define ECHO_DEBUG 0
class MAsyncSerialImpl
{
public:
    CallbackAsyncSerial serial;
    std::string receivedData;
};


RemoterMode::RemoterMode(std::string& port, unsigned int rate)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;

	onlinehandel.Set_timeout(2);
	onlinehandel.online=0;
	

	StatusMessage.Clear();
	onlineStatusM.Set_timeout(2);
	onlineStatusM.online=0;
	
	info_init();
	msg_init();

	ctrlgate = 3;  //查询状态
	gatestatues = GATE_OPEN;
	onlineGate.Set_timeout(5);
		
}

RemoterMode::~RemoterMode()
{
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
void RemoterMode::open(std::string devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname,baudrate);
    } catch(boost::system::system_error&)
    {
        //Errors during open
    }
    pimpl->serial.setCallback(bind(&RemoterMode::readCallback,this, _1, _2));
}

void RemoterMode::close()
{
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(boost::system::system_error&)
    {
        //Errors during port close
    }
    pimpl->receivedData.clear();//Clear eventual data remaining in read buffer
}

bool RemoterMode::isOpen()
{
    return pimpl->serial.isOpen();
}

bool RemoterMode::errorStatus()
{
    return pimpl->serial.errorStatus();
}

void RemoterMode::write(std::string data)
{
    pimpl->serial.writeString(data);
}

void RemoterMode::writechar(const char *data, size_t size)
{
    pimpl->serial.write(data,size);
}

void RemoterMode::readCallback(const char *data, size_t size)
{
    //pimpl->receivedData+=QString::fromAscii(data,size);
        std::string res;
        res.clear();
        res.assign(data,size);
        pimpl->receivedData+=res;
	const char* ch=pimpl->receivedData.c_str();
	 
	#if ECHO_DEBUG
	print_hex(pimpl->receivedData);
	#endif
  
}


void RemoterMode::RemoterMode_master()
{
	
             //int count=0;
		ros::Rate loop_rate(10);
		 std::string str="wait for recv";
		write(str);
             while(ros::ok())
                    {
                   	  onlinehandel.check_connect();
			  onlineStatusM.check_connect();
			 //onlineGate.check_connect();
			 record_remoter_com();
			 //记录无线模块通信状态
			   loop_rate.sleep();
			 if(onlinehandel.online==0){  
				 TC_Getonline_devHANDEL();
				 msg_init();
				 //发送数据包消息
			 	}
			 else{
				//使用长数据包
				TC_Getinfo_devHANDEL();
				
			 	}
			 
			 if(onlineStatusM.online == 1){
			         ros_RemoterInfo_SendRpc();
			 	}
			 else{
			 		msg_init();
					ros_RemoterInfo_SendRpc();
			 	}
			 //自动门相关
			 // boost::this_thread::sleep(boost::posix_time::milliseconds(50));
			// if(onlineGate.online==1){
				//	autogate_ctrl(0);		

			 //	}
			// else{
				//	ctrlgate = 3;
				//	autogate_ctrl(0);
			 //	}
                 //   boost::this_thread::sleep(boost::posix_time::milliseconds(50));

                    }
	
}


 void RemoterMode::rossub_mode_configCall(const std_msgs::String::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string Infostr = msg->data;	
	if(Infostr.size()>10){
			//保存功能位
		try{
				ParamConfig config;
				config.ParseFromString(Infostr); 
				ParamConfig_Config_Error_Mode * Config_Error_Mode= config.mutable_config_error_msg();
				if(Config_Error_Mode->obstaclestoperror_ischeck()){
					//前避障功能
					info.IO_INPUT_1 |= (0x01<<6);		
					}
				else{
					info.IO_INPUT_1 &= (~(0x01<<6));
					}

				if(Config_Error_Mode->bobstaclestoperror_ischeck()){
					//后避障功能
					info.IO_INPUT_1 |= (0x01<<7);		
					}
				else{
					info.IO_INPUT_1 &= (~(0x01<<7));
					}
				
			}
			catch(google::protobuf::FatalException e){
				
				std::cout << "======Controller_statuesCall======"<<std::endl;
				std::cout << e.what() << std::endl;
			}			
	
		

			}

 } 

 void RemoterMode::Controller_statuesCall(const std_msgs::String::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string Infostr = msg->data;	
	if(Infostr.size()>50){ 
		try{
		PackageMessage packMSg;
		packMSg.Clear();	
		packMSg.ParseFromString(Infostr); 

		if(packMSg.has_callmessage()){
				PackageMessage_Call * callmsg = packMSg.mutable_callmessage();
				if(callmsg->parameters_size()==1){
					RobotStatusMessage msg;
					msg.ParseFromString(callmsg->parameters(0)); 
					if(msg.has_controller()){
							StatusMessage.CopyFrom(msg);
							//ControllerMessage driver;
							//ControllerMessage * ctroller = StatusMessage.mutable_controller();
							//if(ctroller->has_stat()){
								//std::cout<<"--------StatusMessage.has_controller()-----stat =  "<<ctroller->stat()<<std::endl;	
								//}
				                      //if(ctroller->has_ctrl_device()){
								//std::cout<<"--------StatusMessage.has_controller()-----ctrl_device =  "<<ctroller->ctrl_device() <<std::endl;
				                      	//}
							record_statues_info_buffer();
							record_alarm_error_buffer();
							onlineStatusM.Set_connect();
							//存储到指定位置
							
						}
					
					}

			}
		
		

			}
		catch(google::protobuf::FatalException e){
				
				std::cout << "======Controller_statuesCall======"<<std::endl;
				std::cout << e.what() << std::endl;
			}			
		}
 } 

 
 void RemoterMode::extern_driver_statuesCall(const dgvmsg::EncounterV::ConstPtr& msg)
{
	//std::cout << "====RemoterMode==extern_driver_statuesCall======"<<std::endl;
	info.LF_Encoder = msg->cnt[0].encounter;
	info.RF_Encoder = msg->cnt[1].encounter;
	info.LB_Encoder = msg->cnt[2].encounter;
	info.RB_Encoder = msg->cnt[3].encounter;

 } 
 //电力上不用
 void RemoterMode::Ph3Info_ros_cmdCall(const std_msgs::String::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string Infostr = msg->data;	
	if(Infostr.size()>3){ 

		}
 } 
void RemoterMode::ExternInfo_ros_cmdCall(const std_msgs::String::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string Infostr = msg->data;	
	if(Infostr.size()>50){ 
		try{
			//std::cout << "====RemoterMode==ExternInfo_ros_cmdCall======"<<std::endl;
			ExternSensor msg;
			msg.ParseFromString(Infostr); 
			if(msg.has_online()&&(msg.temp_humi_size()==2)&&(msg.fan_msg_size()==2)&&(msg.io_collision_size()==8)){
					
					record_io_infrared_buffer(msg);
				}
			}
		catch(google::protobuf::FatalException e){
				
				std::cout << "======ExternInfo_ros_cmdCall==2===="<<std::endl;
				std::cout << e.what() << std::endl;
			}		
		}
	
 } 

 void RemoterMode::BmsInfo_ros_cmdCall(const std_msgs::String::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string Infostr = msg->data;	
	if(Infostr.size()>10){ 
		#if ECHO_DEBUG
		std::cout << "====RemoterMode==BmsInfo_ros_cmdCall======"<<std::endl;
		#endif
		try{
			BmsMessage msg;
			msg.ParseFromString(Infostr); 
				 if(msg.stat()==1){  //BMS 在线
				 
				 		info.COM_ALARM &=(~(0x01<<1));	
						
						if(msg.has_bms_soc()){
							info.BMS_SOC = (unsigned char)(msg.bms_soc());
							}
						
						if(msg.has_bms_voltage()){
							info.BMS_Voltage = (unsigned short int)(msg.bms_voltage()*100);
							}
						if(msg.has_bms_sys_current()){

							info.BMS_Current = ((unsigned short int)((msg.bms_sys_current()+10)*10));
							}
						//info.BMS_Current=5;
						if(msg.bms_sys_current()<0){
							//放电状态
							info.BMS_STATUES = info.BMS_STATUES&(~(0x01<<1));
							//std::cout<<"info.BMS_STATUES-1-== "<<(boost::format("%02X ")%(int)info.BMS_STATUES)<<std::endl;
							}
						else{
							info.BMS_STATUES = info.BMS_STATUES|((0x01<<1));
							//std::cout<<"info.BMS_STATUES-2-== "<<(boost::format("%02X ")%(int)info.BMS_STATUES)<<std::endl;
							}
					#if 0
					std::cout<<"bmsinfo->bms_soc()--== "<<msg.bms_soc()<<std::endl;
					std::cout<<"bmsinfo->bms_voltage()--== "<<msg.bms_voltage()<<std::endl;
					std::cout<<"bmsinfo->bms_sys_current()--== "<<msg.bms_sys_current()<<std::endl;
					#endif
				 	}
				 else{
					//BMS通信不在线
					
						info.COM_ALARM |=((0x01<<1));	
							
						}				

				 	}
			catch(google::protobuf::FatalException e){
					
					std::cout << "======ExternInfo_ros_cmdCall======"<<std::endl;
					std::cout << e.what() << std::endl;
				}	
			}

 } 


int RemoterMode::netrecvDelay(
    boost::shared_ptr<MAsyncSerialImpl> pa,
    int size,
    int timeout,
    std::string errprint){
    
    int count=0;
    while(pa->receivedData.size()<size){
          count++;
          boost::this_thread::sleep(boost::posix_time::milliseconds(1));
          if(count>timeout){
                          std::cout<<errprint<<":"<<pa->receivedData.size()<<'\a'<<std::endl;
			   // print_hex(pimpl->receivedData);
                         // printf("\a");
                          count=0;
                         return pa->receivedData.size();
                       }
            }
        return pa->receivedData.size();

}


void RemoterMode::print_dec(char *datBUF,int Num){

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<Num;index++){
                  int x=(unsigned  char)datBUF[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void RemoterMode::print_hex(char *datBUF,int Num){

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
 
void RemoterMode::print_hex(std::string str)
 {
	int Num = str.size();
	const char * datBUF =  str.c_str();
	
  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }



char RemoterMode::GetCheck(char * pdata){		
	char sum=0;	
	for(int index=2;index<6;index++){            
		sum += pdata[index];		
		}	
	return sum;	
}
char RemoterMode::GetCheck(char * pdata,int len){
	char sum=0;	
	for(int index=2;index<len+2;index++){            
		sum += pdata[index];		
		}	
	return sum;	
}
void RemoterMode::msg_init(){
	
	stdmsg.online = 0;
	
	stdmsg.linear.x=0;
	stdmsg.linear.y=0;
	stdmsg.linear.z=0;
	
	stdmsg.angular.x=0;
	stdmsg.angular.y=0;
	stdmsg.angular.z=0;
	
	stdmsg.IOSTATUES1.resize(8);
	stdmsg.IOSTATUES2.resize(8);
	stdmsg.buttons.resize(8);

}
void RemoterMode::info_init(){
	
	info.SYS_Controller=LASER_USER;
	info.SYS_Temperature=60;
	info.SYS_Humidity=10;
	info.BMS_SOC=50;
	info.BMS_Voltage=4950;
	info.BMS_Current=-25;
	info.BMS_STATUES=1;
	info.LF_Encoder=0;
	info.RF_Encoder=0;
	info.LB_Encoder=0;
	info.RB_Encoder=0;

			info.LF_Motor_Status=Motor_STOP;
			info.RF_Motor_Status=Motor_STOP;
			info.LB_Motor_Status=Motor_STOP;
			info.LF_Motor_Status=Motor_STOP;

			info.LF_ERROR_CODE=0;
			info.RF_ERROR_CODE=0;
			info.LB_ERROR_CODE=0;
			info.RB_ERROR_CODE=0;

			info.IO_INPUT_1=0x00;
			info.IO_INPUT_2=0x00;

			info.YAW=0;
			info.PITCH=0;
			info.ROLL=0;

			info.OtherInfo=0;

			info.ATM103_2_TEMP=60;
			info.ATM103_2_HUIM=12;

			info.COM_ALARM=0;

			info.SYS_ERROR_1=0;
			info.SYS_ERROR_2=0;
			
			 info.RES=0;

}
std::string  RemoterMode::TC_Cmd_nomal_package(char dev)
{
	
	boost::array<char,nomal_length> data;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[0] = HEAD;
	data[1] = HEAD2;
	data[2] = nomal_length-3;
	data[3]=  0x01;  //低位
	data[4]=  dev; //高位
	data[5]=  GetCheck(pd,nomal_length-3);

	#if ECHO_DEBUG
	print_hex(pd,nomal_length);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_Cmd_extend_package1()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD1;

	data[index++]  =  info.SYS_Controller;
	
	data[index++]  = info.SYS_Temperature+60;
	data[index++]  = info.SYS_Humidity;

	data[index++]  = info.BMS_SOC;
	

	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}
std::string  RemoterMode::TC_Cmd_extend_package2()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD2;

	char * p1= (char *)&info.BMS_Voltage;
	data[index++]  = *(p1+1);
	data[index++]  = *(p1);
	
	data[index++]  = info.BMS_Current;
	
	data[index++]  = info.BMS_STATUES;
	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_Cmd_extend_package3()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD3;

	char * p2= (char *)&info.LF_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);
	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_Cmd_extend_package4()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD4;

	char * p2= (char *)&info.RF_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);
	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}



std::string  RemoterMode::TC_Cmd_extend_package5()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD5;

	char * p2= (char *)&info.LB_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);
	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_Cmd_extend_package6()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD6;

	char * p2= (char *)&info.RB_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);
	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}


std::string  RemoterMode::TC_Cmd_extend_package7()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD7;

	char * p3= (char *)&info.LF_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	

	p3= (char *)&info.RF_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	
	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_Cmd_extend_package8()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD8;

	char * p3= (char *)&info.LB_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	

	p3= (char *)&info.RB_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	
	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_Cmd_extend_package9()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD9;

	data[index++]  = info.IO_INPUT_1;
	data[index++]  = info.IO_INPUT_2;
	data[index++]  = info.ATM103_2_TEMP+60;
	data[index++]  = info.ATM103_2_HUIM;

	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_Cmd_extend_package10()
{
	
	boost::array<char,9> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xDA;

	data[index++]  = info.COM_ALARM;
	data[index++]  = info.SYS_ERROR_1;
	data[index++]  = info.SYS_ERROR_2;
	data[index++]  = info.RES;

	
	data[index]=  GetCheck(pd,index-2);

	#if ECHO_DEBUG
	print_hex(pd,index+1);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}




std::string  RemoterMode::TC_Cmd_extend_package()
{
	
	boost::array<char,extend_length> data;
	int index=0;
	char* pd=data.c_array();	
	data.assign(0);
	
	data[index++] = HEAD;
	data[index++]  = HEAD2;
	data[index++]  = 0x06;
	data[index++] =  0xD1;

	data[index++]  =  info.SYS_Controller;
	
	data[index++]  = info.SYS_Temperature;
	data[index++]  = info.SYS_Humidity;

	data[index++]  = info.BMS_SOC;
	
	char * p1= (char *)&info.BMS_Voltage;
	data[index++]  = *(p1+1);
	data[index++]  = *(p1);

	data[index++]  = info.BMS_Current;

	data[index++]  = info.BMS_STATUES;

	char * p2= (char *)&info.LF_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);

	p2= (char *)&info.RF_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);

	p2= (char *)&info.LB_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);	

	p2= (char *)&info.RB_Encoder;
	
	data[index++]  = *(p2+3);
	data[index++]  = *(p2+2);
	data[index++]  = *(p2+1);
	data[index++]  = *(p2+0);

	data[index++]  = (info.LF_Motor_Status)|(info.RF_Motor_Status<<2)|(info.LB_Motor_Status<<4)|(info.LF_Motor_Status<<6);

	char * p3= (char *)&info.LF_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	

	p3= (char *)&info.RF_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	

	p3= (char *)&info.LB_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	

	p3= (char *)&info.RB_ERROR_CODE;
	data[index++]  = *(p3+1);
	data[index++]  = *(p3+0);	

	data[index++]  = info.IO_INPUT_1;
	data[index++]  = info.IO_INPUT_2;

	char * p4= (char *)&info.YAW;
	data[index++]  = *(p4+1);
	data[index++]  = *(p4+0);	

	p4= (char *)&info.PITCH;
	data[index++]  = *(p4+1);
	data[index++]  = *(p4+0);	

	p4= (char *)&info.ROLL;
	data[index++]  = *(p4+1);
	data[index++]  = *(p4+0);	

	data[index++]  = info.OtherInfo;

	data[index++]  = info.ATM103_2_TEMP;
	data[index++]  = info.ATM103_2_HUIM;
	data[index++]  = info.COM_ALARM;
	data[index++]  = info.SYS_ERROR_1;
	data[index++]  = info.SYS_ERROR_2;
	data[index++]  = info.RES;

	data[index]=  GetCheck(pd,extend_length-3);

	#if ECHO_DEBUG
	print_hex(pd,extend_length);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

void  RemoterMode::TC_write(std::string cmd){
	int size = cmd.size();
	const char* ch=cmd.c_str();
	for(int index=0;index<size;index++){
		writechar(&ch[index],1 );
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
}

std::string RemoterMode::turecmd(){
	static int num=0;
	std::string cmd;
	switch(num){
		case 0:
			cmd =TC_Cmd_extend_package1();
			break;
		case 1:
			cmd=TC_Cmd_extend_package2();
			break;
		case 2:
			cmd =TC_Cmd_extend_package3();
			break;
		case 3:
			cmd =TC_Cmd_extend_package4();
			break;
		case 4:
			cmd =TC_Cmd_extend_package5();
			break;
		case 5:
			cmd =TC_Cmd_extend_package6();
			break;
		case 6:
			cmd =TC_Cmd_extend_package7();
			break;
		case 7:
			cmd =TC_Cmd_extend_package8();
			break;
		case 8:
			cmd =TC_Cmd_extend_package9();
			break;
		case 9:
			cmd =TC_Cmd_extend_package10();
			break;
		default:
			num = 0;
			cmd=TC_Cmd_extend_package1();
			break;
			
		}
	num++;
	return cmd;

}
std::string  RemoterMode::TC_Getonline_devHANDEL()
{
	//查询在门控制器是否在线
    	std::string res=TC_Cmd_nomal_package(DEV_HANDEL);
       //发送命令
       pimpl->receivedData.clear();
	TC_write(res );
	boost::this_thread::sleep(boost::posix_time::milliseconds(30));
	int size = netrecvDelay(pimpl,nomal_length,50,"TC_Getonline_devHANDEL recv timeout");
	
	if(size>=nomal_length){
		
		if(procss_recv_online_buffer(DEV_HANDEL)){
				onlinehandel.Set_connect();
				#if ECHO_DEBUG
				print_hex(pimpl->receivedData);
				#endif
				msg_init();
				
			}
		else{
			msg_init();
			}
		
		}
    	return res;
}
std::string  RemoterMode::TC_Getinfo_devHANDEL()
{
	//查询在门控制器是否在线
    	std::string res=turecmd();
       //发送命令
       pimpl->receivedData.clear();
	TC_write(res );
	boost::this_thread::sleep(boost::posix_time::milliseconds(50));
	int size = netrecvDelay(pimpl,16,100,"TC_Getinfo_devHANDEL recv timeout");
	
	if(size>=16){
		
		if(procss_recv_extern_buffer()){
				onlinehandel.Set_connect();
				//print_hex(pimpl->receivedData);
				//发送数据包消息
				record_recv_extern_buffer();
				#if ECHO_DEBUG
				printf_recv_extern_buffer();
				#endif
				
			}
		else{

			msg_init();

			}
		
		}
    	return res;
}

bool RemoterMode::procss_recv_online_buffer(char cmdid){
	
	const char* ch=pimpl->receivedData.c_str();
	int size = pimpl->receivedData.size();
	unsigned char data1 = (unsigned char)ch[0];
	unsigned char data2 = (unsigned char)ch[1];

	if((data1 != SEND1)||(data2 != SEND2)){
		return false;
		}
	if(((unsigned char)ch[2] != 0x03)){
		return false;
		}
	if(((unsigned char)ch[3] != 0x01)){
		return false;
		}
	if(((unsigned char)ch[4] != cmdid)){
		return false;
		}
	
	return true;
}

bool RemoterMode::procss_recv_extern_buffer(){
	
	const char* ch=pimpl->receivedData.c_str();
	int size = pimpl->receivedData.size();
	unsigned char data1 = (unsigned char)ch[0];
	unsigned char data2 = (unsigned char)ch[1];

	if((data1 != SEND1)||(data2 != SEND2)){
		return false;
		}
	if(((unsigned char)ch[2] != 0x0c)){
		return false;
		}
	
	if(((unsigned char)ch[3] != 0x02)){
		return false;
		}
		
	
	return true;
}
void RemoterMode::record_recv_extern_buffer(){
	
	const char* ch=pimpl->receivedData.c_str();
	int size = pimpl->receivedData.size();

	stdmsg.online = onlinehandel.online;

	unsigned char data1 = (unsigned char)ch[4];
	stdmsg.IOSTATUES1[0] = (data1&0x01);     //通信连接使能
	stdmsg.IOSTATUES1[1] = ((data1>>1)&0x01);  //充电控制 
	stdmsg.IOSTATUES1[2] = ((data1>>2)&0x01); //前避障控制
	stdmsg.IOSTATUES1[3] = ((data1>>3)&0x01); //后避障控制
	stdmsg.IOSTATUES1[4] = ((data1>>4)&0x01); //防跌落控制
	stdmsg.IOSTATUES1[5] = ((data1>>5)&0x01); //速度模式切换
	stdmsg.IOSTATUES1[6] = ((data1>>6)&0x01); //云镜控制切换
	stdmsg.IOSTATUES1[7] = ((data1>>7)&0x01); //保留
	
	unsigned char data2 = (unsigned char)ch[5];  
	stdmsg.IOSTATUES2[0] = (data2&0x01);       //状态拨码通道1
	stdmsg.IOSTATUES2[1] = ((data2>>1)&0x01);   //状态拨码通道2
	stdmsg.IOSTATUES2[2] = ((data2>>2)&0x01);//状态拨码通道3
	stdmsg.IOSTATUES2[3] = ((data2>>3)&0x01);   //状态拨码通道4
	stdmsg.IOSTATUES2[4] = ((data2>>4)&0x01); //ZOOM 放大
	stdmsg.IOSTATUES2[5] = ((data2>>5)&0x01); //ZOOM 缩小
	stdmsg.IOSTATUES2[6] = ((data2>>6)&0x01); //焦距拉远
	stdmsg.IOSTATUES2[7] = ((data2>>7)&0x01);  //焦距拉近

	unsigned char data3 = (unsigned char)ch[6]; 
	stdmsg.buttons[0] = (data3&0x01);     //云台复位触发(自动复位)
	stdmsg.buttons[1] = ((data3>>1)&0x01); //云台回HOME位(自动复位)
	stdmsg.buttons[2] = ((data3>>2)&0x01); //红外自动调焦(自动复位)
	stdmsg.buttons[3] = ((data3>>3)&0x01);
	stdmsg.buttons[4] = ((data3>>4)&0x01);
	stdmsg.buttons[5] = ((data3>>5)&0x01);
	stdmsg.buttons[6] = ((data3>>6)&0x01);
	stdmsg.buttons[7] = ((data3>>7)&0x01);
	
	
	stdmsg.linear.x=(ch[7]*256+ch[8]);
	stdmsg.linear.y=(ch[9]*256+ch[10]);
	stdmsg.linear.z=0;
	
	stdmsg.angular.x=(ch[11]*256+ch[12]);
	stdmsg.angular.y=(ch[13]*256+ch[14]);
	stdmsg.angular.z=0;


	if(fabs(stdmsg.linear.x)<10){
			stdmsg.linear.x=0;
		}
	if(fabs(stdmsg.linear.y)<10){
			stdmsg.linear.y=0;
		}
	if(fabs(stdmsg.angular.x)<10){
			stdmsg.angular.x=0;
		}
	if(fabs(stdmsg.angular.y)<10){
			stdmsg.angular.y=0;
		}
	
	#if 0
	cout<<"stdmsg.linear.x = "<<stdmsg.linear.x<<std::endl;
	cout<<"stdmsg.linear.y = "<<stdmsg.linear.y<<std::endl;
	cout<<"stdmsg.angular.x = "<<stdmsg.angular.x<<std::endl;
	cout<<"stdmsg.angular.y = "<<stdmsg.angular.y<<std::endl;
	#endif

}
void RemoterMode::printf_recv_extern_buffer(){
	
	
	std::cout<<"stdmsg.IOSTATUES1[0]= "<< (int)stdmsg.IOSTATUES1[0]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[1]= "<< (int)stdmsg.IOSTATUES1[1]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[2]= "<< (int)stdmsg.IOSTATUES1[2]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[3]= "<< (int)stdmsg.IOSTATUES1[3]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[4]= "<< (int)stdmsg.IOSTATUES1[4]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[5]= "<< (int)stdmsg.IOSTATUES1[5]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[6]= "<< (int)stdmsg.IOSTATUES1[6]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[7]= "<< (int)stdmsg.IOSTATUES1[7]<<std::endl;

	std::cout<<"stdmsg.IOSTATUES2[0]= "<< (int)stdmsg.IOSTATUES2[0]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[1]= "<< (int)stdmsg.IOSTATUES2[1]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[2]= "<< (int)stdmsg.IOSTATUES2[2]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[3]= "<< (int)stdmsg.IOSTATUES2[3]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[4]= "<< (int)stdmsg.IOSTATUES2[4]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[5]= "<< (int)stdmsg.IOSTATUES2[5]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[6]= "<< (int)stdmsg.IOSTATUES2[6]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[7]= "<< (int)stdmsg.IOSTATUES2[7]<<std::endl;	
	
	std::cout<<"stdmsg.buttons[0]= "<< (int)stdmsg.buttons[0]<<std::endl;
	std::cout<<"stdmsg.buttons[1]= "<< (int)stdmsg.buttons[1]<<std::endl;
	std::cout<<"stdmsg.buttons[2]= "<< (int)stdmsg.buttons[2]<<std::endl;
	std::cout<<"stdmsg.buttons[3]= "<< (int)stdmsg.buttons[3]<<std::endl;
	std::cout<<"stdmsg.buttons[4]= "<< (int)stdmsg.buttons[4]<<std::endl;
	std::cout<<"stdmsg.buttons[5]= "<< (int)stdmsg.buttons[5]<<std::endl;
	std::cout<<"stdmsg.buttons[6]= "<< (int)stdmsg.buttons[6]<<std::endl;
	std::cout<<"stdmsg.buttons[7]= "<< (int)stdmsg.buttons[7]<<std::endl;

	std::cout<<"stdmsg.linear.x= "<< stdmsg.linear.x<<std::endl;
	std::cout<<"stdmsg.linear.y= "<< stdmsg.linear.y<<std::endl;
	std::cout<<"stdmsg.linear.z= "<< stdmsg.linear.z<<std::endl;
	
	std::cout<<"stdmsg.angular.x= "<< stdmsg.angular.x<<std::endl;
	std::cout<<"stdmsg.angular.y= "<< stdmsg.angular.y<<std::endl;
	std::cout<<"stdmsg.angular.z= "<< stdmsg.angular.z<<std::endl;	

}

void RemoterMode::ros_RemoterInfo_SendRpc()
{   
	//std::cout << "====RemoterMode==ros_RemoterInfo_SendRpc======"<<std::endl;
     //dgvmsg::remoter stdmsg;     
     Remoterpub.publish(stdmsg);
     ros::spinOnce();
}


void RemoterMode::record_statues_info_buffer(){

	if(StatusMessage.has_controller()){
		
				ControllerMessage * ctroller = StatusMessage.mutable_controller();
				info.SYS_Controller = (unsigned char )(ctroller->ctrl_device());
				
				//std::cout<<"--------StatusMessage.has_controller()-----ctrl_device =  "<<ctroller->ctrl_device() <<std::endl;
				
				if(ctroller->dirvermessage_size() == 4){
						
						ControllerMessage_Dirver *dirverLF = ctroller->mutable_dirvermessage(0);
						if(dirverLF->actvelocity()>0){
								info.LF_Motor_Status = Motor_FORWARD;
							}
						else if(dirverLF->actvelocity()<0){
								info.LF_Motor_Status = Motor_BACKWARD;
							}
						else if(dirverLF->actvelocity()==0){
								info.LF_Motor_Status = Motor_STOP;
							}
						ControllerMessage_Dirver *dirverRF = ctroller->mutable_dirvermessage(1);
						if(dirverRF->actvelocity()>0){
								info.RF_Motor_Status = Motor_FORWARD;
							}
						else if(dirverRF->actvelocity()<0){
								info.RF_Motor_Status = Motor_BACKWARD;
							}
						else if(dirverRF->actvelocity()==0){
								info.RF_Motor_Status = Motor_STOP;
							}
						
						ControllerMessage_Dirver *dirverLB = ctroller->mutable_dirvermessage(2);
						if(dirverLB->actvelocity()>0){
								info.LB_Motor_Status = Motor_FORWARD;
							}
						else if(dirverLB->actvelocity()<0){
								info.LB_Motor_Status = Motor_BACKWARD;
							}
						else if(dirverLB->actvelocity()==0){
								info.LB_Motor_Status = Motor_STOP;
							}
						ControllerMessage_Dirver *dirverRB = ctroller->mutable_dirvermessage(3);
						if(dirverRB->actvelocity()>0){
								info.RB_Motor_Status = Motor_FORWARD;
							}
						else if(dirverRB->actvelocity()<0){
								info.RB_Motor_Status = Motor_BACKWARD;
							}
						else if(dirverRB->actvelocity()==0){
								info.RB_Motor_Status = Motor_STOP;
							}

						//急停开关状态
						if((dirverLF->emergencyinputvalue()==true)||(dirverRF->emergencyinputvalue()==true)
							||(dirverLB->emergencyinputvalue()==true)||(dirverRB->emergencyinputvalue()==true)){

							info.BMS_STATUES = info.BMS_STATUES|((0x01<<2));
								
							}
						else{
								info.BMS_STATUES = info.BMS_STATUES&(~(0x01<<2));
							}
						//电源开关状体
						info.BMS_STATUES = info.BMS_STATUES|((0x01F<<3));

						//报警和错误状态
						
					}
				/*
			if(ctroller->has_bmsmsg()){	
				std::cout<<"--------ctroller->has_bmsmsg()----- "<<std::endl;
				 BmsMessage * bmsinfo = ctroller->mutable_bmsmsg();
				 if(bmsinfo->stat()==1){  //BMS 在线
				 		
						if(bmsinfo->has_bms_soc()){
							info.BMS_SOC = (unsigned char)(bmsinfo->bms_soc());
							}
						
						if(bmsinfo->has_bms_voltage()){
							info.BMS_Voltage = (unsigned short int)(bmsinfo->bms_voltage()*100);
							}
						if(bmsinfo->has_bms_sys_current()){

							info.BMS_Current = ((unsigned short int)((bmsinfo->bms_sys_current()+10)*10));
							}
					#if ECHO_DEBUG
					std::cout<<"bmsinfo->bms_soc()--== "<<bmsinfo->bms_soc()<<std::endl;
					std::cout<<"bmsinfo->bms_voltage()--== "<<bmsinfo->bms_voltage()<<std::endl;
					std::cout<<"bmsinfo->bms_sys_current()--== "<<bmsinfo->bms_sys_current()<<std::endl;
					#endif
				 	}

				}
				*/
			
			//外协处理器
			if(ctroller->has_sensormsg()){
				//获取温湿度
				//std::cout<<"ctroller->has_sensormsg() "<<std::endl;
				ExternSensor * Sensor= ctroller->mutable_sensormsg();
				//std::cout<<"Sensor->temp_humi_size() = "<<Sensor->temp_humi_size()<<std::endl;
				if(Sensor->temp_humi_size()==2){
					ExternSensor_Mode_Temp_humi * Mode_Temp_humi = Sensor->mutable_temp_humi(1);
					if(Mode_Temp_humi->has_interior_temp()){
						info.ATM103_2_TEMP = (unsigned char)Mode_Temp_humi->interior_temp();
						int x=(unsigned  char)info.ATM103_2_TEMP;
                   				//std::cout<<boost::format("%d ")%x;
						//std::cout<<"info.ATM103_2_TEMP = "<<(boost::format("%d ")%x)<<std::endl;
						}
					if(Mode_Temp_humi->has_interior_humi()){
						info.ATM103_2_HUIM =  (unsigned char)Mode_Temp_humi->interior_humi();
						int x=(unsigned  char)info.ATM103_2_HUIM;
						//std::cout<<"info.ATM103_2_HUIM = "<<(boost::format("%d ")%x)<<std::endl;

						}
					Mode_Temp_humi = Sensor->mutable_temp_humi(0);
					if(Mode_Temp_humi->has_interior_temp()){
						
						info.SYS_Temperature = (unsigned char)Mode_Temp_humi->interior_temp();
						//std::cout<<"info.SYS_Temperature = "<<Mode_Temp_humi->interior_temp()<<std::endl;
						}
					if(Mode_Temp_humi->has_interior_humi()){
						info.SYS_Humidity =  (unsigned char)Mode_Temp_humi->interior_humi();
						//std::cout<<"info.SYS_Humidity = "<<Mode_Temp_humi->interior_humi()<<std::endl;

						}					
					}
					


				}
		}
	


}

std::string  RemoterMode::TC_Cmd_Gate_package(char cmd,unsigned short int add,unsigned short int size)
{
	
	boost::array<unsigned char,8> data;
	unsigned char* pd=data.c_array();	
	data.assign(0);
	
	data[0] = GATE_ADD;
	data[1] = cmd;

	unsigned char * p2= (unsigned char *)&add;
	
	data[2] = *(p2+1);
	data[3] = *(p2);

	unsigned char * p1= (unsigned char *)&size;
	data[4] = *(p1+1);
	data[5] = *(p1);

	unsigned short int check = crc16(pd,6);
	unsigned char * p3= (unsigned char *)&check;
	data[6] = *(p3+1);
	data[7] = *(p3);	

	#if ECHO_DEBUG
	print_hex((char *)pd,8);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

std::string  RemoterMode::TC_GATEDEV_Getstatues(){
	
	std::string scmd=TC_Cmd_Gate_package(GATE_RECV,0x00,0x02);
       //发送命令
       pimpl->receivedData.clear();
	   
	write(scmd );
	//boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	int size = netrecvDelay(pimpl,nomal_length,200,"TC_GATEDEV_statues recv timeout");
	
	if(size>=nomal_length){
		      if(procss_recv_gatestatues_buffer()){
				onlineGate.Set_connect();
		      	}
		}
    	return scmd;
}
void RemoterMode::autogate_ctrl(int cmd){
	if((ctrlgate == 1)&&(gatestatues != GATE_OPEN)){
			//开门
			
			TC_GATEDEV_Open(0x0000);
			TC_GATEDEV_Close(0x0000);
			TC_GATEDEV_Open(0xff00);
			TC_GATEDEV_Getstatues();
			if(gatestatues == GATE_OPEN){
					ctrlgate = 3;
				}
		}
	else if ((ctrlgate == 0)&&(gatestatues != GATE_CLOSE)){
			//关门
			
			TC_GATEDEV_Close(0x0000);
			TC_GATEDEV_Open(0x0000);
			TC_GATEDEV_Close(0xff00);
			TC_GATEDEV_Getstatues();
			if(gatestatues == GATE_CLOSE){
					ctrlgate = 3;
				}
		}
	else{
			TC_GATEDEV_Close(0x0000);
			TC_GATEDEV_Open(0x0000);
			//查询门状态
			TC_GATEDEV_Getstatues();
		}

	
}
std::string  RemoterMode::TC_GATEDEV_Close(unsigned short int sw){
	
	std::string scmd=TC_Cmd_Gate_package(GATE_WRITE,0x0000,sw);
       //发送命令
       pimpl->receivedData.clear();
	write(scmd );
	//boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	int size = netrecvDelay(pimpl,8,200,"TC_GATEDEV_Close recv timeout");
	
	if(size>=8){
			onlineGate.Set_connect();
		}
    	return scmd;
}
std::string  RemoterMode::TC_GATEDEV_Open(unsigned short int sw){
	
	std::string scmd=TC_Cmd_Gate_package(GATE_WRITE,0x0001,sw);
       //发送命令
       pimpl->receivedData.clear();
	write(scmd );
	//boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	int size = netrecvDelay(pimpl,8,200,"TC_GATEDEV_Open recv timeout");
	
	if(size>=8){
		onlineGate.Set_connect();	
		}
    	return scmd;
}

bool RemoterMode::procss_recv_gatestatues_buffer(){
	
	const char* ch=pimpl->receivedData.c_str();
	int size = pimpl->receivedData.size();
	unsigned char data1 = (unsigned char)ch[0];
	unsigned char data2 = (unsigned char)ch[1];

	if((data1 != 0x01)||(data2 != 0x02)){
		return false;
		}
	if(((unsigned char)ch[2] != 0x01)){
		return false;
		}
	
	unsigned short int check = crc16((unsigned char *)ch, 4);
	unsigned short int recvcrc = ((unsigned char)ch[4])*256+ ((unsigned char)ch[5]);
	//std::cout<<"check ="<<check;
	//std::cout<<"recvcrc ="<<recvcrc<<std::endl;
	
	if(check != recvcrc){
			return false;
		}

	gatestatues = ch[3];
	std::cout<<"gatestatues ="<<gatestatues<<std::endl;
	
	return true;
}


void RemoterMode::record_io_infrared_buffer(ExternSensor &msg){

	if(msg.io_infrared(4)==0){
			info.IO_INPUT_1 |= 0x01;
		}
	else{
			info.IO_INPUT_1 &= (~0x01);
		}

	if(msg.io_infrared(5)==0){
			info.IO_INPUT_1 |= (0x01<<1);
		}
	else{
			info.IO_INPUT_1 &= (~(0x01<<1));
		}

	if(msg.io_infrared(6)==0){
			info.IO_INPUT_1 |= (0x01<<2);
		}
	else{
			info.IO_INPUT_1 &= (~(0x01<<2));
		}

	if(msg.io_infrared(7)==0){
			info.IO_INPUT_1 |= (0x01<<3);
		}
	else{
			info.IO_INPUT_1 &= (~(0x01<<3));
		}

	
	ExternSensor_Mode_PX24ES * Mode_PX24ES = msg.mutable_px24es();
	if(Mode_PX24ES->out1() == 0){
			info.IO_INPUT_1 |= (0x01<<4);
		}
	else{
			info.IO_INPUT_1 &= (~(0x01<<4));
		}
	if(Mode_PX24ES->out2() == 0){
			info.IO_INPUT_1 |= (0x01<<5);
		}
	else{
			info.IO_INPUT_1 &= (~(0x01<<5));
		}

	//防跌落
	if(msg.io_infrared(2)==1){
			info.IO_INPUT_2 |= 0x01;
		}
	else{
			info.IO_INPUT_2 &= (~0x01);
		}
	if(msg.io_infrared(3)==1){
			info.IO_INPUT_2 |= (0x01<<1);
		}
	else{
			info.IO_INPUT_2 &= (~(0x01<<1));
		}
	
}

void RemoterMode::record_alarm_error_buffer()
{
	if(StatusMessage.has_controller()){
			//
			ControllerMessage *controller = StatusMessage.mutable_controller();
			if(controller->has_errormsg())
				{
					ErrorMessage * error =	controller->mutable_errormsg();
					//协处理器 通信状态
					if(error->has_externcomdeverror()){
							if(error->externcomdeverror()){
									info.COM_ALARM |=((0x01<<7));	
								}
							else{
									info.COM_ALARM &=(~(0x01<<7));	
								}
						}
					
				  	//左前驱动器 通信状态
					if(error->has_leftfdrivercomerror()){
							if(error->leftfdrivercomerror()){
									info.COM_ALARM |=((0x01<<2));	
								}
							else{
									info.COM_ALARM &=(~(0x01<<2));	
								}
						}
					if(error->has_rightfdrivercomerror()){
							if(error->rightfdrivercomerror()){
									info.COM_ALARM |=((0x01<<3));	
								}
							else{
									info.COM_ALARM &=(~(0x01<<3));	
								}
						}
					if(error->has_leftbdrivercomerror()){
							if(error->leftbdrivercomerror()){
									info.COM_ALARM |=((0x01<<4));	
								}
							else{
									info.COM_ALARM &=(~(0x01<<4));	
								}
						}
					if(error->has_rightbdrivercomerror()){
							if(error->rightbdrivercomerror()){
									info.COM_ALARM |=((0x01<<5));	
								}
							else{
									info.COM_ALARM &=(~(0x01<<5));	
								}
						}

				//系统故障
					if(error->has_bmssyserror()){
							if(error->bmssyserror()){
									
									info.SYS_ERROR_1 |=((0x01<<0));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<0));	
								}
						}
					//左前驱动器故障
					if(error->has_carleftfdrivererror()){
							if(error->carleftfdrivererror()){
									
									info.SYS_ERROR_1 |=((0x01<<1));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<1));	
								}
						}
					//右前驱动器故障
					if(error->has_carrightfdrivererror()){
							if(error->carrightfdrivererror()){
									
									info.SYS_ERROR_1 |=((0x01<<2));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<2));	
								}
						}
					//左后驱动器故障
					if(error->has_carleftbdrivererror()){
							if(error->carleftbdrivererror()){
									
									info.SYS_ERROR_1 |=((0x01<<3));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<3));	
								}
						}
					//右后驱动器故障
					if(error->has_carrightbdrivererror()){
							if(error->carrightbdrivererror()){
									
									info.SYS_ERROR_1 |=((0x01<<4));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<4));	
								}
						}
					
					//前障碍物告警
					if(error->has_obstaclestoperror()){
							if(error->obstaclestoperror()){
									
									info.SYS_ERROR_1 |=((0x01<<5));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<5));	
								}
						}
					if(error->has_bobstaclestoperror()){
							if(error->bobstaclestoperror()){
									
									info.SYS_ERROR_1 |=((0x01<<6));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<6));	
								}
						}
					if(error->has_dropstoperror()){
							if(error->dropstoperror()){
									
									info.SYS_ERROR_1 |=((0x01<<7));	
								}
							else{
									info.SYS_ERROR_1 &=(~(0x01<<7));	
								}
						}

				}
			if(controller->has_alarmmsg())
				{


				}
		}



}

void RemoterMode::record_remoter_com()
{
	if(onlinehandel.online == 1){
			info.COM_ALARM &=(~0x01);

		}
	else{
			//有问题，置1
			info.COM_ALARM |=(0x01);
		}


}
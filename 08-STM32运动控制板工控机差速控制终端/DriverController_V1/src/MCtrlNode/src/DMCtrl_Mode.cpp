

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


#include "std_msgs/String.h"
#include "dgvmsg/ctrl_ModeMessage.h"
#include "dgvmsg/Encounter.h"
#include "dgvmsg/EncounterV.h"

#include "mbcrc.h"
#include "DMCtrl_Mode.hpp"


#define DEBUGECHO 0

class MAsyncSerialImpl
{
public:
    CallbackAsyncSerial serial;
    std::string receivedData;
};


DMCtrl_Mode::DMCtrl_Mode(std::string& port, unsigned int rate)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;
	pimpl->receivedData.clear();

	uart_onlineMsg.online=0;
	uart_onlineMsg.Set_timeout(2);
	
	param_init();
	
}

DMCtrl_Mode::~DMCtrl_Mode()
{
	//TC_Cmd_package(0,0);
	
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
void DMCtrl_Mode::open(std::string devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname,baudrate);
    } catch(boost::system::system_error&)
    {
        //Errors during open
    }
    pimpl->serial.setCallback(bind(&DMCtrl_Mode::readCallback,this, _1, _2));
}

void DMCtrl_Mode::close()
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

bool DMCtrl_Mode::isOpen()
{
    return pimpl->serial.isOpen();
}

bool DMCtrl_Mode::errorStatus()
{
    return pimpl->serial.errorStatus();
}

void DMCtrl_Mode::write(std::string data)
{
    pimpl->serial.writeString(data);
}

void DMCtrl_Mode::readCallback(const char *data, size_t size)
{
    //pimpl->receivedData+=QString::fromAscii(data,size);
        std::string res;
        res.clear();
        res.assign(data,size);
        pimpl->receivedData+=res;
	#if DEBUGECHO
	print_hex(pimpl->receivedData);
	#endif
	procss_recv_buffer();

}

void DMCtrl_Mode::DMCtrl_Mode_master()
{
	
             int count=0;
	      ros::Rate loop_rate(10);
		 //std::string str="wait for recv";
		//write(str);
             while(ros::ok())
                    {
                   	uart_onlineMsg.check_connect();
			loop_rate.sleep();
			
			test_set();
	//std::cout<<"uart_onlineMsg.online "<<uart_onlineMsg.online<<std::endl;
   	//std::cout<<"LFMotor_error="<<LFMotor_error<<std::endl;	
	//std::cout<<"RFMotor_error="<<RFMotor_error<<std::endl;	
	
			if(uart_onlineMsg.online){
				if((LFMotor_error ==0)&&(RFMotor_error== 0)){
						write( TC_Cmd_package(leftrpm,rightrpm));   
					}
				else{
						write( TC_Cmd_package(0,0));   
					}
				}
			else{
					write( TC_Cmd_package(0,0));   
				}

			//填写反馈数据
			ros_sendRpc_Driver();
                     // boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			boost::this_thread::interruption_point(); 
			dirv_pub_encounter();
			
                    }
	
}


void DMCtrl_Mode::param_init()
{

	speed = 0;
	angel  = 0;

	leftrpm = 0;
	rightrpm = 0;
	
	leftstatues = CLOSE_STOP;
	rightstatues= CLOSE_STOP;
	
	en_obt_front = false;
	en_obt_back = false;
	
	en_Crash_front = true;
	en_Crash_back = true;

	set_vau_out1 = false;
	set_vau_out2 = false;
	set_vau_out3 = false;
	set_vau_out4 = false;

	set_vau_real1 = false;
	set_vau_real2 = false;

	F_ObtDis = 50;
	B_ObtDis = 50;

	recv_ptr = 0;
	recv_count = 0;
	Readarray.assign(0);

	 LF_counter=0;
	 RF_counter=0;
	 LB_counter=0;
	 RB_counter=0;



	LFMotor_Status=0;
	RFMotor_Status=0;
	LBMotor_Status=0;
	RBMotor_Status=0;

	LFMotor_error=0;
	RFMotor_error=0;
	LBMotor_error=0;
	RBMotor_error=0;

	Obt1_dis=0;
	Obt2_dis=0;
	Obt3_dis=0;
	Obt4_dis=0;

	
	Obt_Status=0;
	Com_Status=0;
	IO_Status=0;

}
char DMCtrl_Mode::TC_Cmd_Ctrl( )
{
	char ctrl=0;
	 if(en_Crash_front){
	      ctrl |= (0X01<<0);
	  }
	  else{
	      ctrl &= (~0X01);
	  }
	  
	 if(en_Crash_back){
	      ctrl |= (0X01<<1);
	  }
	  else{
	       ctrl &= (~(0X01<<1));
	  }
	  
	 if(en_obt_front){
	      ctrl |= (0X01<<2);
	  }
	  else{
	      ctrl &= (~(0X01<<2));
	  }

	 if(en_obt_back){
	      ctrl |= (0X01<<3);
	  }
	  else{
	      ctrl &= (~(0X01<<3));
	  }

	 if(set_vau_out1){
	      ctrl |= (0X01<<4);
	  }
	  else{
	      ctrl &= (~(0X01<<4));
	  }

	 if(set_vau_out2){
	      ctrl |= (0X01<<5);
	  }
	  else{
	      ctrl &= (~(0X01<<5));
	  }

	 if(set_vau_out3){
	      ctrl |= (0X01<<6);
	  }
	  else{
	      ctrl &= (~(0X01<<6));
	  }

	 if(set_vau_out4){
	      ctrl |= (0X01<<7);
	  }
	  else{
	      ctrl &= (~(0X01<<7));
	  }

	return   ctrl;
}

std::string  DMCtrl_Mode::TC_Cmd_package( short int leftrpm,short int rightrpm)
{
	
	boost::array<char,nomal_length> data;
	short int Data1=leftrpm;
	short int Data2=rightrpm;
	
	data.assign(0);
	
	data[0] = HEAD_H;
	data[1] = HEAD_L;

	
	data[2] = TC_Cmd_Ctrl();

	 char MStautes = 0;
	if(leftrpm!=0){
		 MStautes |= (0X01<<0);
		}
	else{
		 MStautes &= (~(0X03<<0));
		}
	if(rightrpm!=0){
		 MStautes |= (0X01<<2);
		}
	else{
		 MStautes &= (~(0X03<<2));
		}
	
	
	data[3] = MStautes;

	  short int LFspeed = leftrpm;
	  if(LFspeed>3500){
	      LFspeed = 3500;
	  }
	  else if (LFspeed<-3500){
	       LFspeed = -3500;
	  }
	  
	  char * ps1  = (char *)(&LFspeed);
	  data[4] = * (ps1+1);
	  data[5] = * (ps1+0);


	  short int RFspeed = rightrpm;
	  if(RFspeed>3500){
	      RFspeed = 3500;
	  }
	  else if (RFspeed<-3500){
	       RFspeed = -3500;
	  }
	  
	  char * ps2  = (char *)(&RFspeed);
	  data[6] = * (ps2+1);
	  data[7] = * (ps2+0);	

	  data[8] = 0;

	//继电器控制
	  char ReCtrl = 0;
	  if(set_vau_real1){
	      ReCtrl |= (0X01<<0);
	  }
	  else{
	      ReCtrl &= (~(0X01<<0));
	  }

	  if(set_vau_real2){
	      ReCtrl |= (0X01<<1);
	  }
	  else{
	      ReCtrl &= (~(0X01<<1));
	  }

	  data[9] = ReCtrl;	  

	  data[10] = F_ObtDis;
	  data[11] = B_ObtDis;

	  data[12] = nomal_length-3;
	  data[13] = TAIL_H;
	  data[14] = TAIL_L;
	  
    	std::string res(data.begin(),data.end());
	#if DEBUGECHO
	print_hex(res);
	#endif
	
    	return res;
	
}

 void DMCtrl_Mode::print_hex( char *datBUF,int Num)
 {

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
  void DMCtrl_Mode::print_hex(std::string str)
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

int  DMCtrl_Mode::Get_check()
{
	unsigned char * ptr=(unsigned char *)Readarray.c_array();
	
	unsigned char Data1 = ptr[nomal_recvlength-4];

	unsigned char crc = 0;
		
	crc +=(unsigned char )RECV_HEAD1;
	crc +=( unsigned char )RECV_HEAD2;
	crc +=( unsigned char )(nomal_recvlength-1);
	
	for(int i =0;i<nomal_recvlength-4;i++){

			crc +=ptr[i];
			//std::cout<< "Get_check:ptr = "<<boost::format("%02X ")%(int)ptr[i]<<std::endl;
		}
	#if DEBUGECHO
	std::cout<< "Get_check:Data1 = "<<boost::format("%02X ")%(int)Data1<<std::endl;
	std::cout<< "Get_check:crc = "<<boost::format("%02X ")%(int)crc<<std::endl;
	#endif

	if(Data1!=crc){
		return 1;
		}
	
	return 1;
}

void  DMCtrl_Mode::Get_Sdata()
{
	unsigned char * ptr=(unsigned char *)Readarray.c_array();

	
	unsigned char * ps1  = (unsigned char *)(&LF_counter);
	
	* (ps1+3) = ptr[0];
	* (ps1+2) = ptr[1];	
	* (ps1+1) = ptr[2];	
	* (ps1+0) = ptr[3];
	
	//std::cout<<" LF_counter = " <<LF_counter<<std::endl;	

	unsigned char * ps2  = (unsigned char *)(&RF_counter);
	
	* (ps2+3) = ptr[4];
	* (ps2+2) = ptr[5];	
	* (ps2+1) = ptr[6];	
	* (ps2+0) = ptr[7];
	
	//std::cout<<" RF_counter = " <<RF_counter<<std::endl;	

	LFMotor_Status = ptr[8];
	RFMotor_Status = ptr[9];

	unsigned char * ps3  = (unsigned char *)(&LFMotor_error);
	
	* (ps3+3) = ptr[10];
	* (ps3+2) = ptr[11];	

	unsigned char * ps4  = (unsigned char *)(&RFMotor_error);
	
	* (ps4+3) = ptr[12];
	* (ps4+2) = ptr[13];	

	Obt1_dis = ptr[14];
	Obt2_dis = ptr[15];
	Obt3_dis = ptr[16];
	Obt4_dis = ptr[17];


	Obt_Status = ptr[18];
	Com_Status = ptr[19];
	IO_Status = ptr[20];

	
}

void DMCtrl_Mode::test_set()
{
	
	leftstatues = CLOSE_STOP;
	rightstatues= CLOSE_STOP;
	
	en_obt_front = true;
	en_obt_back = true;
	
	en_Crash_front = true;
	en_Crash_back = true;

	set_vau_out1 = false;
	set_vau_out2 = false;
	set_vau_out3 = false;
	set_vau_out4 = false;

	set_vau_real1 = false;
	set_vau_real2 = false;

	F_ObtDis = 20;
	B_ObtDis = 20;
	
}

bool DMCtrl_Mode::procss_recv_buffer()
{

	std::string recvstr = pimpl->receivedData;
	pimpl->receivedData.clear();
	const char* ch=recvstr.c_str();
	int size = recvstr.size();	

	
	for(int index=0;index<size;index++){
		unsigned char data = (unsigned char)ch[index];
	 	switch(recv_ptr){
			case 0:
				if(data==RECV_HEAD1){
						recv_ptr=1;
						recv_count=0;
						Readarray.assign(0);
					}
				break;
			case 1:
				if(data==RECV_HEAD2){
						recv_ptr=2;
						recv_count=0;
						Readarray.assign(0);
					}
				else{
						recv_ptr=0;
						recv_count=0;
						Readarray.assign(0);
					}
				break;
			case 2:
				if(data==nomal_recvlength-1){
						recv_ptr=3;
						recv_count=0;
						Readarray.assign(0);
					}
				else{
						recv_ptr=0;
						recv_count=0;
						Readarray.assign(0);
					}
				break;
			case 3:
				Readarray[recv_count]=data;
				recv_count++;
				if(recv_count>=(nomal_recvlength-3)){
						recv_ptr=0;
						recv_count=0;
						
						if(1==Get_check()){  //接收到完整的数据
							uart_onlineMsg.Set_connect();
							
							Get_Sdata();

							Readarray.assign(0);
							return true;
							}
						else{
								std::cout<<ttyport<<" recv worong !"<<std::endl;
							}
						Readarray.assign(0);
						return false;
					}
				break;				
			default:
				recv_ptr=0;
				recv_count=0;
				Readarray.assign(0);
				pimpl->receivedData.clear();
				break;

	 		}
		}

	return false;

}
  

//SUB 接收命令 
void DMCtrl_Mode::Net2Ctrl_driverCall(const dgvmsg::DriverVelocity::ConstPtr& cmdmsg)
{


        //前进速度
        int size = cmdmsg->driver.size();
	 #if 0
	 std::cout<<"Net2Ctrl_driverCall="<<size<<std::endl;
	 #endif
	for(int index=0;index<size;index++){
			//std::cout<<"cmdmsg->driver[index].add ="<<cmdmsg->driver[index].add<<" VRPM="<<cmdmsg->driver[index].VRPM<<std::endl;
			if(cmdmsg->driver[index].add ==DEV_DIR_ADD_LF){
					leftrpm = cmdmsg->driver[index].VRPM;
					
				}
			if(cmdmsg->driver[index].add ==DEV_DIR_ADD_RF){
					rightrpm = cmdmsg->driver[index].VRPM;
				}
		}

      //  ClearJoyFlag();  
}

void DMCtrl_Mode::dirv_pub_encounter()
{   
	dgvmsg::EncounterV Ent;
        Ent.cnt.resize(2);
		
	Ent.cnt[0].devadd=(int)DEV_DIR_ADD_LF;
	Ent.cnt[0].now=  ros::Time::now();
	Ent.cnt[0].encounter = LF_counter;
		
	Ent.cnt[1].devadd=(int)DEV_DIR_ADD_RF;
	Ent.cnt[1].now=  ros::Time::now();
	Ent.cnt[1].encounter = RF_counter;

	
	
   #if 0
   	std::cout<<"********************ros_SendEncounterRpc**********************"<<std::endl;
	std::cout<<"LF_counter="<<LF_counter<<std::endl;
   	std::cout<<"RF_counter="<<RF_counter<<std::endl;
   #endif
     LCEncounterpub.publish(Ent);
     ros::spinOnce();
	
}


void DMCtrl_Mode::ros_sendRpc_Driver()
{
	 //dgvmsg::DriverStatues Statues;
	 
	 
	Statues.device_status = uart_onlineMsg.online;
	
	Statues.encounter.resize(2);
	Statues.encounter[0] = LF_counter;
	Statues.encounter[1] = RF_counter;
	
	Statues.Motor_Status.resize(2);
	Statues.Motor_Status[0] = LFMotor_Status;
	Statues.Motor_Status[1] = RFMotor_Status;

	Statues.errorcode.resize(2);
	Statues.errorcode[0] = LFMotor_error;
	Statues.errorcode[1] = RFMotor_error;

	Statues.Obt_dis.resize(4);
	Statues.Obt_dis[0] = Obt1_dis;
	Statues.Obt_dis[1] = Obt2_dis;
	Statues.Obt_dis[2] = Obt3_dis;
	Statues.Obt_dis[3] = Obt4_dis;
	
	Statues.fcrash_io = ((Obt_Status>>0)&0x01);
	Statues.bcrash_io = ((Obt_Status>>1)&0x01);
	
	Statues.Obt_en_Status.resize(4);
	Statues.Obt_en_Status[0] = ((Obt_Status>>2)&0x01);
	Statues.Obt_en_Status[1] = ((Obt_Status>>3)&0x01);
	Statues.Obt_en_Status[2] = ((Obt_Status>>4)&0x01);
	Statues.Obt_en_Status[3] = ((Obt_Status>>5)&0x01);
	Statues.fObt_en_Status   = ((Obt_Status>>6)&0x01);
	Statues.bObt_en_Status  = ((Obt_Status>>7)&0x01);

	
	
	Statues.LMotor_com_Status  = ((Com_Status>>0)&0x01);
	Statues.RMotor_com_Status  = ((Com_Status>>1)&0x01);
	Statues.Obt_com_Status.resize(4);
	Statues.Obt_com_Status[0]  = ((Com_Status>>4)&0x01);
	Statues.Obt_com_Status[1]  = ((Com_Status>>5)&0x01);
	Statues.Obt_com_Status[2]  = ((Com_Status>>6)&0x01);
	Statues.Obt_com_Status[3]  = ((Com_Status>>7)&0x01);



	Statues.input_IO_Status.resize(4);
	Statues.input_IO_Status[0]  = ((IO_Status>>0)&0x01);
	Statues.input_IO_Status[1]  = ((IO_Status>>1)&0x01);
	Statues.input_IO_Status[2]  = ((IO_Status>>2)&0x01);
	Statues.input_IO_Status[3]  = ((IO_Status>>3)&0x01);

	Statues.out_IO_Status.resize(4);
	Statues.out_IO_Status[0]  = ((IO_Status>>4)&0x01);
	Statues.out_IO_Status[1]  = ((IO_Status>>5)&0x01);
	Statues.out_IO_Status[2]  = ((IO_Status>>6)&0x01);
	Statues.out_IO_Status[3]  = ((IO_Status>>7)&0x01);



	
     LCCompub.publish(Statues);
     ros::spinOnce();
}




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
#include "uart_ctrl_Mode.hpp"

class MAsyncSerialImpl
{
public:
    CallbackAsyncSerial serial;
    std::string receivedData;
};


uart_ctrl_Mode::uart_ctrl_Mode(std::string& port, unsigned int rate)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;
	onlineMsg.online=0;
	pimpl->receivedData.clear();

	 recv_ptr = 0;
	 comRecvFlag = false;
	 recv_count=0;
	 

	 vspeed =0;  //���ٶ�
	 aspeed =0;  //���ٶ�


	 LF_counter=0;
	 RF_counter=0;
	 LB_counter=0;
	 RB_counter=0;

	 LF_counter_recv_flag=0;
	 RF_counter_recv_flag=0;
	 LB_counter_recv_flag=0;
	 RB_counter_recv_flag=0;
	
}

uart_ctrl_Mode::~uart_ctrl_Mode()
{
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
void uart_ctrl_Mode::open(std::string devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname,baudrate);
    } catch(boost::system::system_error&)
    {
        //Errors during open
    }
    pimpl->serial.setCallback(bind(&uart_ctrl_Mode::readCallback,this, _1, _2));
}

void uart_ctrl_Mode::close()
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

bool uart_ctrl_Mode::isOpen()
{
    return pimpl->serial.isOpen();
}

bool uart_ctrl_Mode::errorStatus()
{
    return pimpl->serial.errorStatus();
}

void uart_ctrl_Mode::write(std::string data)
{
    pimpl->serial.writeString(data);
}

void uart_ctrl_Mode::readCallback(const char *data, size_t size)
{
    //pimpl->receivedData+=QString::fromAscii(data,size);
        std::string res;
        res.clear();
        res.assign(data,size);
        pimpl->receivedData+=res;
	#if 0
	print_hex(pimpl->receivedData);
	#endif
	
	comRecvFlag = procss_recv_buffer();
	if(comRecvFlag){
				LF_counter_recv_flag=0;
				LB_counter_recv_flag=0;
				RB_counter_recv_flag=0;
				RF_counter_recv_flag=0;
		onlineMsg.Set_connect();
		
		}
	pimpl->receivedData.clear();

 	/* 
 for(std::size_t  index=0;index<size;index++)
        {
             int x=ch[index];
             std::cout<<boost::format("%02X ")%x;
           
        }
 	std::cout<<std::endl;
 	std::cout<<"recv size="<<size<<std::endl;
 	*/ 
}

void uart_ctrl_Mode::ros_SendEncounterRpc()
{   
	dgvmsg::EncounterV Ent;
        Ent.cnt.resize(4);
		
	Ent.cnt[0].devadd=(int)DEV_DIR_ADD_LF;
	Ent.cnt[0].now=  ros::Time::now();
	Ent.cnt[0].encounter = LF_counter;
		
	Ent.cnt[1].devadd=(int)DEV_DIR_ADD_RF;
	Ent.cnt[1].now=  ros::Time::now();
	Ent.cnt[1].encounter = RF_counter;

	Ent.cnt[2].devadd=(int)DEV_DIR_ADD_LB;
	Ent.cnt[2].now=  ros::Time::now();
	Ent.cnt[2].encounter = LB_counter;

	Ent.cnt[3].devadd =(int)DEV_DIR_ADD_RB;
	Ent.cnt[3].now =  ros::Time::now();
	Ent.cnt[3].encounter = RB_counter;
	
	
   #if 0

   #endif
     LCEncounterpub.publish(Ent);
     ros::spinOnce();
}

void uart_ctrl_Mode::ros_SendRpc()
{   
	dgvmsg::ctrl_ModeMessage ctrMsg;
	
	ctrMsg.online = onlineMsg.online;
	ctrMsg.twist.linear.x = vspeed;  //��������ٶ�
	ctrMsg.twist.linear.y = aspeed;   //�Ҳ������ٶ�
	ctrMsg.twist.linear.z=  0.0;
	// heading driven by left joysticj left and right
	ctrMsg.twist.angular.x = aspeed;
	ctrMsg.twist.angular.y =  0.0;
	ctrMsg.twist.angular.z =  0.0;
	
   #if 0

   #endif
     LCCompub.publish(ctrMsg);
     ros::spinOnce();
}

void uart_ctrl_Mode::uart_ctrl_Mode_master()
{
	
             int count=0;
	      ros::Rate loop_rate(20);
		 //std::string str="wait for recv";
		//write(str);
             while(ros::ok())
                    {
                   
			loop_rate.sleep();
			onlineMsg.check_connect();
			if(comRecvFlag){
			      //if(waitencounter()){
					ros_SendRpc();
					count=0;
					write(TC_return_package());
					boost::this_thread::sleep(boost::posix_time::milliseconds(1));
					comRecvFlag=false;	
					//}
				//boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				}
			if(onlineMsg.online == 0){
				count++;
				if(count>4){
					vspeed=0;
					aspeed=0;
					ros_SendRpc();
					count=0;
					}
				}
			    
                    //boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
                    //����ENCOUNTER��Ϣ
			//ros_SendEncounterRpc();
                    }
	
}

void uart_ctrl_Mode::Drivers_ros_cmdCall(const dgvmsg::Encounter::ConstPtr& msg)
{
		
	if(msg->devadd== DEV_DIR_ADD_LF){
		LF_counter = msg->encounter;
		//std::cout<<ttyport<<"uart_ctrl_Mode ->DEV_DIR_ADD_LF = "<<LF_counter<<std::endl;
	}
	else if(msg->devadd == DEV_DIR_ADD_RF){
		RF_counter = msg->encounter;
		//std::cout<<ttyport<<"uart_ctrl_Mode ->DEV_DIR_ADD_RF = "<<RF_counter<<std::endl;
	}
	else if(msg->devadd== DEV_DIR_ADD_LB){
		LB_counter = msg->encounter;
		//std::cout<<ttyport<<"uart_ctrl_Mode ->DEV_DIR_ADD_LB = "<<LB_counter<<std::endl;
	}
	else if(msg->devadd == DEV_DIR_ADD_RB){
		RB_counter = msg->encounter;
		//std::cout<<ttyport<<"uart_ctrl_Mode ->DEV_DIR_ADD_RB = "<<RB_counter<<std::endl;
	}			                   
 } 


bool uart_ctrl_Mode::procss_recv_buffer()
{
	const char* ch=pimpl->receivedData.c_str();
	int size = pimpl->receivedData.size();
	
	//std::cout<<"recv a string size="<<size<<std::endl;
	
	for(int index=0;index<size;index++){
		unsigned char data = (unsigned char)ch[index];
	 	switch(recv_ptr){
			case 0:
				if(data==HEAD){
						recv_ptr=1;
					}
				break;
			case 1:
				if(data==SEND){
						recv_ptr=2;
						recv_count=0;
						Readarray.assign(0);
					}
				else{
						recv_ptr=0;
						recv_count=0;
					}
				break;
			case 2:
				Readarray[recv_count]=data;
				recv_count++;
				if(recv_count>=10){
						recv_ptr=0;
						recv_count=0;
						
						pimpl->receivedData.clear();
						//std::cout<<"recv a string"<<std::endl;
						if(1==Get_check()){
							vspeed = Get_Sdata1();
							aspeed = Get_Sdata2();
							//std::cout<<"speed="<<vspeed<<"angel speed = "<<aspeed<<std::endl;
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
/*
unsigned short int uart_ctrl_Mode::CRC16(unsigned char *p,unsigned int len)
{
	unsigned char i;
	int j;
	unsigned short int uiCRC = 0xffff;

	for(j=0;j<len;j++)
	{
		uiCRC ^= (*p);
		p++;
		for( i=8; i != 0; i--)
		{
			if( uiCRC & 1 ){
				uiCRC >>= 1;
				uiCRC ^= 0xa001;
			}
			else{
				uiCRC>>=1;
			}
		}
	}
	return(uiCRC);
}
*/
float  uart_ctrl_Mode::Get_Sdata2()
{
	float Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[4];
	* (ps1+2) = Readarray[5];	
	* (ps1+1) = Readarray[6];	
	* (ps1+0) = Readarray[7];
	
	return Data1;
}

float  uart_ctrl_Mode::Get_Sdata1()
{
	float Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[0];
	* (ps1+2) = Readarray[1];	
	* (ps1+1) = Readarray[2];	
	* (ps1+0) = Readarray[3];	
	
	return Data1;
}
int  uart_ctrl_Mode::Get_check()
{
	unsigned short int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
    * (ps1+1) = Readarray[9];
    * (ps1+0) = Readarray[8];	
	//int x=(unsigned  char)Readarray[8];
	
	//std::cout<<"Readarray[8] =" <<boost::format("%02X ")%x<<std::endl;
	//x=(unsigned  char)Readarray[9];
	//std::cout<<"Readarray[9] =" <<boost::format("%02X ")%x<<std::endl;
	
	unsigned char * ptr=(unsigned char *)Readarray.c_array();
	unsigned short int crc=crc16(ptr,8);
	//std::cout<<"crc check = "<<crc<<" recv="<<Data1<<std::endl;
	if(Data1!=crc){
		return -1;
		}
	
	return 1;
}

std::string  uart_ctrl_Mode::TC_return_package()
{

	
	boost::array<char,20> data;
	 //char * pd=data.c_array();
	
	data.assign(0);
	
	data[0] = 0x88;
	data[1] = 0xAA;
	
	unsigned char * ps1  = (unsigned char *)(&LF_counter);
	data[5]=  *(ps1);ps1++;  //��λ
	data[4]=  *(ps1);ps1++; //��λ
	data[3]=  *(ps1);ps1++; //��λ
	data[2]=  *(ps1);ps1++; //��λ
	
	ps1  = (unsigned char *)(&RF_counter);
	data[9]=  *(ps1);ps1++;  //��λ
	data[8]=  *(ps1);ps1++; //��λ
	data[7]=  *(ps1);ps1++; //��λ
	data[6]=  *(ps1);ps1++; //��λ

	ps1  = (unsigned char *)(&LB_counter);
	data[13]=  *(ps1);ps1++;  //��λ
	data[12]=  *(ps1);ps1++; //��λ
	data[11]=  *(ps1);ps1++; //��λ
	data[10]=  *(ps1);ps1++; //��λ

	ps1  = (unsigned char *)(&RB_counter);
	data[17]=  *(ps1);ps1++;  //��λ
	data[16]=  *(ps1);ps1++; //��λ
	data[15]=  *(ps1);ps1++; //��λ
	data[14]=  *(ps1);ps1++; //��λ
	//����У��
	 unsigned char* pd=(unsigned char *)data.c_array();	
        unsigned short int check= crc16(&pd[2],16);
		
	unsigned char * p  = (unsigned char *)(&check);
	data[18]=  *(p);p++;  //��λ
	data[19]=  *(p);p++; //��λ
	/*
	std::cout<<"LF_counter= "<<LF_counter<<std::endl;
	std::cout<<"RF_counter= "<<RF_counter<<std::endl;
	std::cout<<"LB_counter= "<<LB_counter<<std::endl;
	std::cout<<"RB_counter= "<<RB_counter<<std::endl;

	std::cout<<"DLF_counter= "<<(LF_counter -preLF_counter) <<std::endl;
	std::cout<<"DRF_counter= "<<(RF_counter -preRF_counter) <<std::endl;
	std::cout<<"DLB_counter= "<<(LB_counter -preLB_counter) <<std::endl;
	std::cout<<"DRB_counter= "<<(RB_counter -preRB_counter) <<std::endl;
      */

	preLF_counter=LF_counter;
	preRF_counter=RF_counter;
	preLB_counter=LB_counter;
	preRB_counter=RB_counter;
	
	
	#if 0
	print_hex(pd,20);
	#endif
	
    	std::string res(data.begin(),data.end());
    	return res;
	
}

void uart_ctrl_Mode::print_dec(unsigned char *datBUF,int Num)
{

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<Num;index++){
                  int x=(unsigned  char)datBUF[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void uart_ctrl_Mode::print_hex(unsigned char *datBUF,int Num)
 {

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
  void uart_ctrl_Mode::print_hex(std::string str)
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


std::string uart_ctrl_Mode::Igetstring(int num)
{
	
  std::ostringstream s1;
  s1 <<num;
  std::string s2 = s1.str();
	return s2;
}
int uart_ctrl_Mode::waitencounter(){

	 std::cout<<"LF_counter_recv_flag="<<LF_counter_recv_flag<<std::endl;
	 std::cout<<"RF_counter_recv_flag="<<RF_counter_recv_flag<<std::endl;
	 std::cout<<"LB_counter_recv_flag="<<LB_counter_recv_flag<<std::endl;
	 std::cout<<"RB_counter_recv_flag="<<RB_counter_recv_flag<<std::endl;
	 
	if((LF_counter_recv_flag>=1 )
		&&(RF_counter_recv_flag>=1)
		&&(LB_counter_recv_flag>=1)
		&&(RB_counter_recv_flag>=1)){
		if((LF_counter_recv_flag==RF_counter_recv_flag)&&
			(LF_counter_recv_flag==LB_counter_recv_flag)&&
			(LF_counter_recv_flag==RB_counter_recv_flag)){
				LF_counter_recv_flag=0;
				LB_counter_recv_flag=0;
				RB_counter_recv_flag=0;
				RF_counter_recv_flag=0;
				return 1;
			}
		}
	return 0;

}

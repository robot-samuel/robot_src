#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>



#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "dgvmsg/remoter.h"
#include "dgvmsg/Encounter.h"
#include "dgvmsg/EncounterV.h"

#include "dgvmsg/remoterctrl.h"

#include "AsyncSerial.h"

#include "zmqclient.h"

using namespace std;
using namespace boost;


class MAsyncSerialImpl;

class OnlineLogMessage
{
public:
       int online;
	   int timeout;
       boost::posix_time::ptime time_recvnow;  
       boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse;
       OnlineLogMessage()
       	{
		online = 0; 
		timeout=2;
		time_recvnow = boost::posix_time::microsec_clock::universal_time();  
       	}
	int check_connect()
	{   

	    if(online == 1)
	        {
	            //
	             boost::posix_time::ptime now;
	            // 这里为微秒为单位;这里可以将microsec_clock替换成second_clock以秒为单位;  
	            now = boost::posix_time::microsec_clock::universal_time();
	            time_elapse = now - time_recvnow;
	            // 类似GetTickCount，只是这边得到的是2个时间的ticket值的差，以微秒为单位;  
	            int ticks = time_elapse.ticks();  
	            // 得到两个时间间隔的秒数;   
	            int sec = time_elapse.total_seconds();
	            if(sec>=timeout){
	                    online = 0;
	                }
	            
	        
		}
        return online;
	        
	} 

	int Set_connect()
	{   
              online=1;
                    // 这里为微秒为单位;这里可以将microsec_clock替换成second_clock以秒为单位;  
               time_recvnow = boost::posix_time::microsec_clock::universal_time();  
	        
	} 
	int Set_timeout(int time)
	{   
		timeout =  time;
	} 	
       
};


class RemoterMode
{
    

private:
    RemoterMode(const RemoterMode& rhs);
    RemoterMode& operator=(const RemoterMode& rhs);
    
  


public:
	
	ros::Publisher Remoterpub;

    RemoterMode(std::string& port, unsigned int rate);
    
    ~RemoterMode();

	void RemoterMode_master();
	void open(std::string devname, unsigned int baudrate);
	void close();
	bool isOpen();
	bool errorStatus();
	void write(std::string data);
	void writechar(const char *data, size_t size);
	void readCallback(const char *data, size_t size);
	void  TC_write(std::string cmd);


	int netrecvDelay(
    		boost::shared_ptr<MAsyncSerialImpl> pa,
    		int size,
   	 	int timeout,
    		std::string errprint
    		);
	void print_dec(char *datBUF,int Num);
	 void print_hex(char *datBUF,int Num);
	 void print_hex(std::string str);

   enum { HEAD = 0x66,
   		HEAD2 = 0xAA,
   		SEND1=0x88,
   		SEND2=0xBB};

   enum { nomal_length = 6 };
   enum { extend_length = 54 };
   
   enum { DEV_GATE = 0x02,
   		DEV_HANDEL = 0x01};	 

    enum CtrlID{
                    NONE = 0,    //空闲
                    NET_USER=1,  //网络用户控制
                    RS232_USER=2,  //网络用户控制
                    LASER_USER, 
                    REMOTE_JOY,
                    USBREMOTE_JOY   //遥控JOY手柄控制                
                           };
    enum MotorS{
                    Motor_STOP = 0,    //空闲
                    Motor_FORWARD=1,  //网络用户控制
                    Motor_BACKWARD=2,  //网络用户控制
                    Motor_FREE,              
                           };
	
   enum dev_GATE_CMD{
            GATE_RECV = 0X02,
	     GATE_WRITE = 0X05,		
    };	
   enum dev_GATE_STATUES{
            GATE_CLOSE= 0X01,
	     //GATE_ING1 = 0X01,	
	     //GATE_ING2 = 0X02,
	     GATE_OPEN = 0X00,	
    };	   
   enum dev_add{
            MYADD  =100,
            DEV_DIR_ADD_LF =100,
            DEV_DIR_ADD_LB =102,
            DEV_DIR_ADD_RF =101,
            DEV_DIR_ADD_RB =103,
            GATE_ADD = 0x01,
    };
	struct Controller_Info{

			unsigned char SYS_Controller;
			unsigned char SYS_Temperature;
			unsigned char SYS_Humidity;
			unsigned char BMS_SOC;
			unsigned short int BMS_Voltage;
			unsigned char BMS_Current;
			unsigned char BMS_STATUES;
			unsigned int LF_Encoder;
			unsigned int RF_Encoder;
			unsigned int LB_Encoder;
			unsigned int RB_Encoder;

			unsigned char LF_Motor_Status;
			unsigned char RF_Motor_Status;
			unsigned char LB_Motor_Status;
			unsigned char RB_Motor_Status;

			unsigned short int LF_ERROR_CODE;
			unsigned short int RF_ERROR_CODE;
			unsigned short int LB_ERROR_CODE;
			unsigned short int RB_ERROR_CODE;

			unsigned char IO_INPUT_1;
			unsigned char IO_INPUT_2;

			unsigned short int YAW;
			unsigned short int PITCH;
			unsigned short int ROLL;

			unsigned char OtherInfo;

			unsigned char ATM103_2_TEMP;
			unsigned char ATM103_2_HUIM;

			unsigned char COM_ALARM;

			unsigned char SYS_ERROR_1;
			unsigned char SYS_ERROR_2;
			
			 unsigned char RES;

		};


	 std::string  TC_Cmd_nomal_package(char dev); 
	 
	 std::string  TC_Cmd_extend_package1();
	 std::string  TC_Cmd_extend_package2();
	 std::string  TC_Cmd_extend_package3();
	 std::string  TC_Cmd_extend_package4();
	 std::string  TC_Cmd_extend_package5();
	 std::string  TC_Cmd_extend_package6();
	 std::string  TC_Cmd_extend_package7();
	 std::string  TC_Cmd_extend_package8();
	 std::string  TC_Cmd_extend_package9();
	 std::string  TC_Cmd_extend_package10();

	  
	  std::string  TC_Cmd_extend_package();
	 
	

	 char GetCheck(char * pdata);
	 char GetCheck(char * pdata,int len);

	 bool procss_recv_online_buffer(char cmdid);
	 std::string  TC_Getonline_devHANDEL();

	void info_init();
	 bool procss_recv_extern_buffer();
	 std::string  TC_Getinfo_devHANDEL();
	 
	 


	 void Controller_statuesCall(const std_msgs::String::ConstPtr& msg);
	 void extern_driver_statuesCall(const dgvmsg::EncounterV::ConstPtr& msg);
	 void Ph3Info_ros_cmdCall(const std_msgs::String::ConstPtr& msg);
	 void ExternInfo_ros_cmdCall(const std_msgs::String::ConstPtr& msg);
	  void BmsInfo_ros_cmdCall(const std_msgs::String::ConstPtr& msg);
	   void rossub_mode_configCall(const std_msgs::String::ConstPtr& msg);


	  void ros_RemoterInfo_SendRpc();
	  void msg_init();
	  void record_recv_extern_buffer();
	  void record_statues_info_buffer();
	  void printf_recv_extern_buffer();

	  std::string  TC_Cmd_Gate_package(char cmd,unsigned short int add,unsigned short int size);
	  
	  std::string  TC_GATEDEV_Getstatues();
	  std::string  TC_GATEDEV_Close(unsigned short int sw);
	  std::string  TC_GATEDEV_Open(unsigned short int sw);
	  void autogate_ctrl(int cmd);
	  bool procss_recv_gatestatues_buffer();
	  
	  std::string turecmd();

	  void record_io_infrared_buffer(ExternSensor &msg);
	  void record_remoter_com();
	  void record_alarm_error_buffer();
	
private:
	std::string ttyport;
	int  bandrate;

	boost::shared_ptr<MAsyncSerialImpl> pimpl; ///< Pimpl idiom


	int online_recv_ptr;
	int online_recv_count;

	OnlineLogMessage onlineGate;	
	OnlineLogMessage onlinehandel;
	OnlineLogMessage onlineStatusM;

	RobotStatusMessage StatusMessage;
	dgvmsg::remoter stdmsg;
	struct Controller_Info info;


	int ctrlgate;
	int gatestatues;
	
		//extern
	OnlineLogMessage ExternSnsorNode;	
	ExternSensor  ExternModeInfo;
	
};






#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>



#include "ros/ros.h"
#include "ros/console.h"

#include "AsyncSerial.h"
#include "zmqclient.h"


using namespace std;
using namespace boost;

class MAsyncSerialImpl;

class OnlineLogMessage
{
public:
       int online;
       boost::posix_time::ptime time_recvnow;  
       boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse;
       OnlineLogMessage()
       	{
		online = 0; 
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
	            if(sec>=2){
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
	
       
};

class ExternMode
{
    

private:
    ExternMode(const ExternMode& rhs);
    ExternMode& operator=(const ExternMode& rhs);
    

   enum { nomal_length = 10 };
   enum { extend_length = 28 };
   enum { ADD = 0x01 };
   enum { HEAD = 0xA0,
   		SEND = 0xB0};
   
  enum CMD{
 	 ID_TC_GetBox1_temHuim  = 0x01,
         ID_TC_GetBox2_temHuim   = 0x02,
         ID_TC_GetFan1_openvalu   = 0x03,
         ID_TC_GetFan1_closevalu   = 0x04,
         ID_TC_GetFan2_openvalu  = 0x05,
         ID_TC_GetFan2_closevalu   = 0x06,
         ID_TC_Fan2_enable   = 0x07,
         ID_TC_Fan2_release   = 0x08,
         ID_TC_Fan1_enable   = 0x09,
         ID_TC_Fan1_release   = 0x10,
         ID_TC_SetFan1_valu   = 0x11,
         ID_TC_SetFan2_valu   = 0x12,
         ID_TC_GetFan1_valu   = 0x13,
         ID_TC_GetFan2_valu   = 0x14,
         ID_TC_GetPt100ChannelAB   = 0x15,
         ID_TC_GetPt100ChannelCD   = 0x16,
         ID_SAFETY_GetLEDStates   = 0x17,
         ID_SAFETY_SetLEDColor   = 0x18,
         ID_SAFETY_GetIOInput   = 0x19,
         ID_TC_LED_LEFT   = 0x20,
         ID_TC_LED_RIGHT   = 0x21,
         ID_Ctrl_CentrControl   = 0xfe,
         
        };

   enum Fan_Mode{
		Mode_auto = 0,
		Mode_close = 1,
		Mode_open = 2,
		Mode_Null = 3,
        };
   enum Color_Mode{
		Mode_Lclose = 0,
		Mode_red = 1,
		Mode_green = 2,
		Mode_blue = 3,
		Mode_red_fli = 4,  //闪烁
		Mode_green_fli = 5,
		Mode_blue_fli = 6,
        };
   enum TC_LED_Mode{
	LED_close = 1,
	LED_open = 0,
   	};
   
public:
	
	ros::Publisher Externpub;

    ExternMode(std::string& port, unsigned int rate);
    
    ~ExternMode();

	void ExternMode_master();
	void open(std::string devname, unsigned int baudrate);
	void close();
	bool isOpen();
	bool errorStatus();
	void write(std::string data);
	void readCallback(const char *data, size_t size);


	void  process_recv_buffer();
	int getcheck( char * p);
	void  RecordData(char cmd);
	//void ros_SendRpc(std::string msg);
	void packdata();
	void ConfigMode(std::string data);
	void cmdCallback(const std_msgs::String::ConstPtr& msg);

	int netrecvDelay(
	    boost::shared_ptr<MAsyncSerialImpl> pa,
	    int size,
	    int timeout,
	    std::string errprint);
    
	std::string  TC_Cmd_package(char cmdID, short int sData1,short int sData2);
	unsigned short int  CheckSUM( char *datBUF,int Num);
	unsigned short int  GetCheckSUM(  unsigned char *datBUF,int Num);
	void print_dec(char *datBUF,int Num);
	void print_hex(char *datBUF,int Num);
	void print_hex(std::string str);
	
	
	std::string  TC_GetBox1_temHuim();
	std::string  TC_GetBox2_temHuim();
	std::string  TC_GetFan1_openvalu();
	std::string  TC_GetFan1_closevalu();
	std::string  TC_GetFan2_openvalu();
	std::string  TC_GetFan2_closevalu();
	std::string  TC_Fan2_enable();
	std::string  TC_Fan2_release();
	std::string  TC_Fan1_enable();
	std::string  TC_Fan1_release();
	std::string  TC_SetFan1_valu( short int open, short int close);
	std::string  TC_SetFan2_valu(short int open, short int close);
	std::string  TC_GetFan1_valu();
	std::string  TC_GetFan2_valu();
	std::string  TC_GetPt100ChannelAB();
	std::string  TC_GetPt100ChannelCD();
	std::string  TC_SAFETY_GetLEDStates();
	std::string  TC_SAFETY_SetLEDColor(short int coloe);
	std::string  TC_SAFETY_GetIOInput();
	std::string  TC_LED_LEFT();
	std::string  TC_LED_RIGHT();
	
	//std::string  TC_Ctrl_CentrControl(char fan1,char fan2,char color);
	std::string  TC_Ctrl_CentrControl(char fan1,char fan2,char color,char lightL,char lightR);
	int  check_nomal_recv_buffer(char cmd);
	int  check_extend_recv_buffer(char cmd);
	short int  Get_Sdata1();
	short int  Get_Sdata2();
	std::string  Get_Ledstr(char led);	
	void Set_infrared(char IO);
	void Set_collision(char IO);
	void Record_Extend_buffer();
	void Printf_Extend_buffer();
private:
	std::string ttyport;
	int  bandrate;
	OnlineLogMessage Devonline;	
	boost::shared_ptr<MAsyncSerialImpl> pimpl; ///< Pimpl idiom

	int ptr_resv;
	int recv_count;	
	boost::array<char,extend_length> Readarray;
	ExternSensor SensorMsg;

	char fan1_mode;
	short int  fan1_open;
	short int  fan1_close;
	char fan1_statue;
	
	char fan2_mode;
	short int  fan2_open;
	short int  fan2_close;
	char fan2_statue;
	
	char Ledcolor;

	bool config_flag;

	char light_left;
	char light_right;

	int recvsize;
	
};






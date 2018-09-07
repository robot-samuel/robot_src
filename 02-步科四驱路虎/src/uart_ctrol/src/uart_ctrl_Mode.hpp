#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>



#include "ros/ros.h"
#include "ros/console.h"

#include "AsyncSerial.h"
#include "zmqclient.h"


#include <robot2015_common/cpp/ControllerMessage.pb.h>

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
	            // ����Ϊ΢��Ϊ��λ;������Խ�microsec_clock�滻��second_clock����Ϊ��λ;  
	            now = boost::posix_time::microsec_clock::universal_time();
	            time_elapse = now - time_recvnow;
	            // ����GetTickCount��ֻ����ߵõ�����2��ʱ���ticketֵ�Ĳ��΢��Ϊ��λ;  
	            int ticks = time_elapse.ticks();  
	            // �õ�����ʱ���������;   
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
                    // ����Ϊ΢��Ϊ��λ;������Խ�microsec_clock�滻��second_clock����Ϊ��λ;  
               time_recvnow = boost::posix_time::microsec_clock::universal_time();  
	        
	} 
	
       
};

class uart_ctrl_Mode
{
    

private:
    uart_ctrl_Mode(const uart_ctrl_Mode& rhs);
    uart_ctrl_Mode& operator=(const uart_ctrl_Mode& rhs);
    
  enum { nomal_length = 12 };
   enum { HEAD = 0xBB,
   		SEND = 0x66};
   

public:
   enum dev_add{
            MYADD  =100,
            DEV_DIR_ADD_LF =101,
            DEV_DIR_ADD_LB =102,
            DEV_DIR_ADD_RF =103,
            DEV_DIR_ADD_RB =104,
    };
	
	enum CtrlID{
		NONE = 0,    //����
		NET_USER=1,  //�����û�����
		RS232_USER=2,  //�����û�����
		LASER_USER, 
		REMOTE_JOY,
		USBREMOTE_JOY   //ң��JOY�ֱ�����                
			   };

	ros::Publisher LCCompub;
	ros::Publisher LCEncounterpub;

    uart_ctrl_Mode(std::string& port, unsigned int rate);
    
    ~uart_ctrl_Mode();

	void uart_ctrl_Mode_master();
	void open(std::string devname, unsigned int baudrate);
	void close();
	bool isOpen();
	bool errorStatus();
	void write(std::string data);
	void readCallback(const char *data, size_t size);
	void ros_SendRpc();
	void ros_SendEncounterRpc();
	
	
	//void extern_driver_statuesCall(const dgvmsg::Encounter::ConstPtr& msg);
	void Drivers_ros_cmdCall(const dgvmsg::Encounter::ConstPtr& msg);
	
	bool procss_recv_buffer();
	//unsigned short int CRC16(unsigned char *p,unsigned int len);
	float  Get_Sdata2();
	float  Get_Sdata1();
	int  Get_check();
	std::string  TC_return_package();
	void print_dec(unsigned char *datBUF,int Num);
	void print_hex(unsigned char *datBUF,int Num);
	void print_hex(std::string str);
	std::string Igetstring(int num);
	int waitencounter();
	
	
private:
	std::string ttyport;
	int  bandrate;

	boost::shared_ptr<MAsyncSerialImpl> pimpl; ///< Pimpl idiom
	OnlineLogMessage onlineMsg;
	
	int recv_ptr;
	int recv_count;
	bool comRecvFlag;
	boost::array<unsigned char,12> Readarray;
	
	float vspeed;  //���ٶ�
	float aspeed;  //���ٶ�

	int LF_counter;
	int RF_counter;
	int LB_counter;
	int RB_counter;

	int preLF_counter;
	int preRF_counter;
	int preLB_counter;
	int preRB_counter;

	int LF_counter_recv_flag;
	int RF_counter_recv_flag;
	int LB_counter_recv_flag;
	int RB_counter_recv_flag;
	
	

	
};






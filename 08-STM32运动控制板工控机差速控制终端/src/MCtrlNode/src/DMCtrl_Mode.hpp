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

class DMCtrl_Mode
{
    

private:
    DMCtrl_Mode(const DMCtrl_Mode& rhs);
    DMCtrl_Mode& operator=(const DMCtrl_Mode& rhs);
    
  enum { nomal_length = 15 };
  
  enum {
  	HEAD_H = 0X66,
	HEAD_L = 0XAA,
	
	TAIL_H = 0X77,
	TAIL_L = 0XCC
  	};
   

public:

	
	enum CtrlID{
		NONE = 0,    //����
		NET_USER=1,  //�����û�����
		RS232_USER=2,  //�����û�����
		LASER_USER, 
		REMOTE_JOY,
		USBREMOTE_JOY   //ң��JOY�ֱ�����                
			   };

	enum MOTERS{
		CLOSE_STOP = 0,    //�ջ�ͣ��
		MOTER_RUN1 = 1,    //����
		MOTER_RUN = 2,    //����
		MOTER_REALSE = 3    //�ͷ�
		};

	ros::Publisher LCCompub;

    DMCtrl_Mode(std::string& port, unsigned int rate);
    
    ~DMCtrl_Mode();

	void DMCtrl_Mode_master();
	void open(std::string devname, unsigned int baudrate);
	void close();
	bool isOpen();
	bool errorStatus();
	void write(std::string data);
	void readCallback(const char *data, size_t size);

	void param_init();
	char TC_Cmd_Ctrl( );
	
	


	
private:
	std::string ttyport;
	int  bandrate;

	boost::shared_ptr<MAsyncSerialImpl> pimpl; ///< Pimpl idiom
	OnlineLogMessage onlineMsg;
	

	
	int speed;
	int angel;

	int leftrpm;
	int rightrpm;
	int leftstatues;
	int rightstatues;
	
	bool en_obt_front;
	bool en_obt_back;
	
	bool en_Crash_front;
	bool en_Crash_back;

	bool set_vau_out1;
	bool set_vau_out2;
	bool set_vau_out3;
	bool set_vau_out4;

	bool set_vau_real1;
	bool set_vau_real2;

	unsigned char F_ObtDis;
	unsigned char B_ObtDis;
};






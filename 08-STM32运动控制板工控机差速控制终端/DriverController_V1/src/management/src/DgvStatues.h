#pragma once

#include "dgvmsg/ctrl_ModeMessage.h"
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include "dgvmsg/remoter.h"
#include "dgvmsg/DriverStatues.h"
#include "dgvmsg/Bms.h"
#include "dgvmsg/ExternSensor.h"

#include <boost/date_time/posix_time/posix_time.hpp>  
#include "zmqclient.h"
#include "zlog.h"


#define LEFTF_OBT 7
#define RIGHTF_OBT 6
#define LEFTB_OBT 5
#define RIGHTB_OBT 4

#define LEFTF_DROP 3
#define RIGHTF_DROP 2

class ErrorLogMessage
{
public:
	int time_start;
	int time_end;
	std::string Strerror;
	int errorcode;
};
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
	            // ����Ϊ΢��Ϊ��λ;������Խ�microsec_clock�滻��second_clock����Ϊ��λ;  
	            now = boost::posix_time::microsec_clock::universal_time();
	            time_elapse = now - time_recvnow;
	            // ����GetTickCount��ֻ����ߵõ�����2��ʱ���ticketֵ�Ĳ��΢��Ϊ��λ;  
	            int ticks = time_elapse.ticks();  
	            // �õ�����ʱ����������;   
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
                    // ����Ϊ΢��Ϊ��λ;������Խ�microsec_clock�滻��second_clock����Ϊ��λ;  
               time_recvnow = boost::posix_time::microsec_clock::universal_time();  
	        
	} 
	int Set_timeout(int time)
	{   
		timeout =  time;
	} 	
       
};

class Dgv_statues
{
public:

	enum dev_add{
            DEV_DIR_ADD_LF =100,
            DEV_DIR_ADD_RF =101,

		};
	
	enum CtrlID{
                    NONE = 0,    //����
                    NET_USER=1,  //�����û�����
                    RS232_USER=2,  //�����û�����
                    LASER_USER, 
                    REMOTE_JOY,
                    USBREMOTE_JOY   //ң��JOY�ֱ�����                
		};
	enum DEV_FANS{
    			ID_FAN_1 = 1,
    			ID_FAN_2 = 2
		};  

	enum DEV_BCTRL{
    			ID_ENABLE = 1,
    			ID_DISENABLE = 0
		};  
	enum DEV_NUM{
    			DRIVER_AMOUNT = 2
		};  
public:

	Dgv_statues();
	~Dgv_statues();
	void MyMemclear(const void *src,int count);
	
	//driver
	
	unsigned char driveradd[DRIVER_AMOUNT];
	dgvmsg::DriverStatues Driverstatues;

	void DriverStatuesInit();

	//USBREMOTE_JOY
	OnlineLogMessage USBJostickNode;
	geometry_msgs::Twist Joycmd_velocity; 
	
	int joyconnect;
	int joystick_count;
	 void print_joymsg(const sensor_msgs::Joy::ConstPtr& joy);

	void JoycmdClear();

	void OnlineGroupInit();
	void CheckError();
	void CheckExternError();
	void CheckAlarm();
	void CheckOnline();
	
	//NET_USER
	OnlineLogMessage CenterNode;	
	int center_isconnect ;
	int center_count ;
	


	int speedVelocity; //����ʵ���ٶ�
	int angelVelocity; //�������ٶ�

	int cmd_speedVelocity; //�����ٶ�����
	int cmd_angelVelocity; //�������ٶ�����
	
	void msg_init();
		
//private:

	
	
};

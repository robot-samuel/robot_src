#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "zmqclient.h"
#include "dgvmsg/remoter.h"


#define RECV_WAITE_TIME 500
#define RECV_NO_COUNTER 3

using namespace std;
using namespace boost;


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
		timeout = 2;
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

class pantiltctrlMode
{
    

private:
    pantiltctrlMode(const pantiltctrlMode& rhs);
    pantiltctrlMode& operator=(const pantiltctrlMode& rhs);
	
public:    
	enum CONNECT{YES  =1,NO=0};
public:

    pantiltctrlMode();
	
    ~pantiltctrlMode();

	void pantiltctrlMode_master();

	void ros_SendRpc(std::string &msg);
	std::string packdata();
	 void RemoterInfo_ros_cmdCall(const dgvmsg::remoter::ConstPtr& msg);
	void msg_init();
	int check_connect();
	
	 std::string get_session(void);
	 std::string packdata_right(int speed);
	 std::string packdata_stop();
	 std::string packdata_Left(int speed);
	 std::string packdata_UP(int speed);
	 std::string packdata_DOWN(int speed);
	 std::string packdata_home();
	 std::string packdata_reset();
	 std::string packdata_infread_autofocus();

	 std::string packdata_zoom_big(int speed);
	 std::string packdata_zoom_small();
	 
	 void printf_recv_extern_buffer();
	 
	
	int DriverecvDelay(  
	 	int size,    
	 	int timeout,    
	 	std::string errprint);	
	//ros
	ros::Publisher pantiltctrlInfopub;		
private:


	OnlineLogMessage onlineMsg;
	
	OnlineLogMessage RemoteNode;
	int Remoteconnect;
	int Old_Remoteconnect;
	int Remote_count;
	dgvmsg::remoter Remotestdmsg;	
	  int stopUD_counter;
	 int stopRL_counter;
};






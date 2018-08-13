#pragma once


#include "cppzmq-master/zmq.hpp"
#include "zmqclient.h"

#include <cstdlib>

#include <string>
#include<map>
//#include <zmq.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>

#include <robot2015_common/cpp/PackageMessage.pb.h>


#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include <ros/time.h>  

#define SUB_TIMEOUT 2500  //  msecs, (> 1000!)
#define ERROR_context_t_INIT "zmqcpp 001"
#define ERROR_connect_PUB "zmqcpp 002"
#define ERROR_connect_SUB "zmqcpp 003"

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
	            if(sec>=10){
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

class zmqclient
{  

		
public:  

        zmqclient() ;
        zmqclient(std::string &ipc_pub,std::string &ipc_sub);
        ~zmqclient() ;
        
        void insert_topic(std::string topic);

        void connect(void);
		void connect_pub(void);
		void connect_sub(void);
		
		void destory_sub(void);
		void destory_pub(void);
		void destory(void);
		
		bool isconnect();
		
	        void zmqrecvLoop();
		void 	rospubtask();
		void 	zmqpubtask();

		void print_zmq_version();
		bool s_send(const std::string  &key);
		bool s_sendmore (const std::string  &string);
		bool msg_send(zmq::message_t& msg);
		bool msg_sendmore ( zmq::message_t& msg);
		std::string s_recv ();
		
		std::string get_msg_string (zmq::message_t& message);

    void dirv_update_Callback(const std_msgs::String::ConstPtr& msg);
    void dirv_zmq_Callback(const std_msgs::String::ConstPtr& msg);
    std::string Get_to(std::string fun);
    void zmq_SendRpc(std::string msg,std::string topic);
    void dirv_ros_SendRpc(std::string msg);

    int  show_mainmsg(PackageMessage & msg);
    void unpackage_main(std::string fun);
    
     std::string zmqheart_SendRpc(void);
      std::string get_session(void);
      int proc_zmq_recvheart(std::string pack);
    ros::Publisher rospub;
private: 
    std::map<std::string,int > zmqcmdlist;
     //operator handle
     zmq::context_t  * zmqcontext;
     //boost::shared_ptr< zmq::context_t > zmqcontext;
     bool connectflag;
     std::string zmqadd_pub_;
     std::string zmqadd_sub_;
     // Construct a map of strings
     std::map<std::string,std::string> zmqsubtopic_s;
     int topic_size;
     zmq::socket_t  * zmqpub_;
     zmq::socket_t  * zmqsub_;

    OnlineLogMessage center_online;
     //boost::shared_ptr< zmq::socket_t > zmqpub_;
    // boost::shared_ptr< zmq::socket_t > zmqsub_;

};  


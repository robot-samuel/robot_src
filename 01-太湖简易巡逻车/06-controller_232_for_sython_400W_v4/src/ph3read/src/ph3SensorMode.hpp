#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include "ros/ros.h"
#include "ros/console.h"

#include "std_msgs/String.h"

#include "AsyncSerial.h"
#include "ph3Sensor.h"
#include "zmqclient.h"



#define RECV_WAITE_TIME 100
#define RECV_NO_COUNTER 3

using namespace std;
using namespace boost;


class sytronDriver;
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
	            // �õ�����ʱ����������;   
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

class ph3SensorMode
{
    

private:
    ph3SensorMode(const ph3SensorMode& rhs);
    ph3SensorMode& operator=(const ph3SensorMode& rhs);
    

public:

    ph3SensorMode(std::string& port, unsigned int rate);
	
    ~ph3SensorMode();

	void ph3SensorMode_master();
	int  open(std::string devname, unsigned int baudrate);
	void close();
	bool isOpen();
	bool errorStatus();
	void write(std::string data);
	void readCallback(const char *data, size_t size);
	
	
	void print_dec(unsigned char *datBUF,int Num);
	void print_hex(unsigned char *datBUF,int Num);
	void print_hex(std::string str);


	void DevCmd_GetPH3();

	
	
	int DriverecvDelay(  
	 	int size,    
	 	int timeout,    
	 	std::string errprint);	

	std::string packdata();
	void ros_SendRpc(std::string msg);
	

	ros::Publisher Ph3Info;
private:
	std::string ttyport;
	int  bandrate;

	boost::shared_ptr<MAsyncSerialImpl> pimpl; ///< Pimpl idiom
	OnlineLogMessage onlineMsg;
	
	ph3Sensor ph3sensor;

};






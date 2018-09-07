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
	            if(sec>=5){
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

class sensor_weather_Mode
{
    

private:
    sensor_weather_Mode(const sensor_weather_Mode& rhs);
    sensor_weather_Mode& operator=(const sensor_weather_Mode& rhs);
    
  enum { nomal_length = 12 };
   enum { HEAD = 0xBB,
   		SEND = 0x66};
   

public:

	ros::Publisher LCCompub;
	//ros::Publisher LCEncounterpub;

    sensor_weather_Mode(std::string& port, unsigned int rate);
    
    ~sensor_weather_Mode();


	void open(std::string devname, unsigned int baudrate);
	void close();
	bool isOpen();
	bool errorStatus();
	void write(std::string data);
	void readCallback(const char *data, size_t size);

	
	void sensor_weather_Mode_master();

	unsigned short int CRC16(unsigned char *p,unsigned int len);

	void print_dec(unsigned char *datBUF,int Num);
	void print_hex( char *datBUF,int Num);
	void print_hex(std::string str);
	int Clear_recv();
	
	int DriverecvDelay(  
	 	int size,    
	 	int timeout,    
	 	std::string errprint);


	std::string GetweatherData();
	std::string Precipitation_Data_zero();
	void  RecordData(std::string recv);
	 std::string get_session(void);
	 int ros_rospub_sensordate_SendRpc();
	 std::string PackWeather(void);
	
private:
	std::string ttyport;
	int  bandrate;

	boost::shared_ptr<MAsyncSerialImpl> pimpl; ///< Pimpl idiom
	OnlineLogMessage onlineMsg;


	

	std::string SDN ;  //��С����
	std::string SDM ;  //ƽ������
	std::string SDX ; //������

	std::string SSN ;   //��С����
	std::string SSM ;  //ƽ������
	std::string SSX ;    //������

	std::string STA ;  //�¶�
	std::string SUA ;  //ʪ��

	std::string SPA ;  //��ѹ
	std::string SRC ;  //����	


	float fDN ;  //��С����
	float fDM ;  //ƽ������
	float fDX ; //������

	float fSN ;   //��С����
	float fSM ;  //ƽ������
	float fSX ;    //������

	float fTA ;  //�¶�
	float fUA ;  //����ʪ��

	float fPA ;  //��ѹ
	float fRC ;  //����	
	
};






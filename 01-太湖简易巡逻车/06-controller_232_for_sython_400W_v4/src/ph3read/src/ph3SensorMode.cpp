

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



#include "ph3SensorMode.hpp"

#define  DEBUG_ECHO 1

class MAsyncSerialImpl
{
public:
    CallbackAsyncSerial serial;
    std::string receivedData;
};


ph3SensorMode::ph3SensorMode(std::string& port, unsigned int rate)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;
	ph3sensor.Set_add(1);

	onlineMsg.online=0;

	
	
}

ph3SensorMode::~ph3SensorMode()
{
  
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
int ph3SensorMode::open(std::string devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname,baudrate);
    } catch(boost::system::system_error&)
    {
        //Errors during open
        return 0;
    }
    pimpl->serial.setCallback(bind(&ph3SensorMode::readCallback,this, _1, _2));
	return 1;
}

void ph3SensorMode::close()
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

bool ph3SensorMode::isOpen()
{
    return pimpl->serial.isOpen();
}

bool ph3SensorMode::errorStatus()
{
    return pimpl->serial.errorStatus();
}

void ph3SensorMode::write(std::string data)
{
    pimpl->serial.writeString(data);
}

void ph3SensorMode::readCallback(const char *data, size_t size)
{
    //pimpl->receivedData+=QString::fromAscii(data,size);
        std::string res;
        res.clear();
        res.assign(data,size);
        pimpl->receivedData+=res;
	//const char* ch=pimpl->receivedData.c_str();
	#if 0
	print_hex(pimpl->receivedData);
	#endif
	/*
	comRecvFlag = procss_recv_buffer();
	if(comRecvFlag){
		onlineMsg.Set_connect();
		
		}
		*/

}

void ph3SensorMode::ph3SensorMode_master()
{
	
       std::cout<< "hello ph3SensorMode_master "<<std::endl;
	ros::Rate loop_rate(0.5);		
			
         try{
                    while(ros::ok()){
						
			loop_rate.sleep();
			DevCmd_GetPH3();
			onlineMsg.check_connect();
			ros_SendRpc(packdata());
			//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			
			
                     }
             }
        catch(boost::thread_interrupted&){
                 std::cout << "thread interrupt" << std:: endl;
             }
    
	
}


void ph3SensorMode::print_dec(unsigned char *datBUF,int Num)
{

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<Num;index++){
                  int x=(unsigned  char)datBUF[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void ph3SensorMode::print_hex(unsigned char *datBUF,int Num)
 {

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
  void ph3SensorMode::print_hex(std::string str)
 {
	int Num = str.size();
	const char * datBUF =  str.c_str();
	
  std::cout<< "ph3SensorMode print_recv hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }

int ph3SensorMode::DriverecvDelay(  
 	int size,    
 	int timeout,    
 	std::string errprint){       
 	int count=0;    
	while(pimpl->receivedData.size()<size){          
		count++;          
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));         
		if(count>timeout){                        
			#if DEBUG_ECHO                          
			std::cout<<errprint<<":"<<pimpl->receivedData.size()<<'\a'<<std::endl;	
			print_hex(pimpl->receivedData);
			#endif                         // printf("\a");                          
			count=0;                         
			return pimpl->receivedData.size();                       
			}            
		}        
	return pimpl->receivedData.size();
}

void ph3SensorMode::DevCmd_GetPH3(){

    //发送
        static int recv_timeout=0;
	pimpl->receivedData.clear();
        write(ph3sensor.Cmd_GetPH3());
	boost::this_thread::sleep(boost::posix_time::milliseconds(50));	
	
	std::string errprint = ttyport+" DevCmd_GetPH3 recv  error";  
	int recvsize=DriverecvDelay(18,RECV_WAITE_TIME,errprint);
        if(recvsize>=18){
			//存储数据
			ph3sensor.recorddata(pimpl->receivedData);
			onlineMsg.Set_connect();
                   }
         else{
			 if(recvsize==18){
			ph3sensor.recorddata(pimpl->receivedData);
			onlineMsg.Set_connect();				 
			 }
			 else{
			 recv_timeout++;
			 if(recv_timeout>=RECV_NO_COUNTER){
                     			onlineMsg.online=0;
					recv_timeout=0;
					//print_test( vecpa[0]);
			 	}
			 }
                   }
}

std::string ph3SensorMode::packdata()
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
	 
     GOOGLE_PROTOBUF_VERIFY_VERSION;

        ph3airmsg msg;
	msg.Clear();

	msg.set_ph3_ppm(ph3sensor.ph3Vau);
	msg.set_online(onlineMsg.online);
          //序列化到内存
          std::ostringstream stream;
		  
          msg.SerializeToOstream(&stream);
		  
         std::string text = stream.str();
		 
	return text;
	
	
}

void ph3SensorMode::ros_SendRpc(std::string msg)
{   
    //ros::NodeHandle npub;
    // ros::Publisher zmq_pub = npub.advertise<std_msgs::String>("dirv_recvcmd", 1);
   
    std_msgs::String stdmsg;
     stdmsg.data = msg;
   // ROS_INFO("ros pub: to=%s,size= [%d]", "dirv_recvcmd",stdmsg.data.size());
   #if 0
    std::cout<<"ros pub: to="
        <<"dirv_recvcmd"<<",size="
        <<stdmsg.data.size()<<std::endl;
   #endif
     Ph3Info.publish(stdmsg);
     ros::spinOnce();
}
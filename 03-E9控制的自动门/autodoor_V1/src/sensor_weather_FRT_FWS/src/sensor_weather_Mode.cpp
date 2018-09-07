

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream> 

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/date_time/gregorian/gregorian.hpp>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>


#include "std_msgs/String.h"


#include "sensor_weather_Mode.hpp"

using  namespace boost;
using namespace boost::uuids;

class MAsyncSerialImpl
{
public:
    CallbackAsyncSerial serial;
    std::string receivedData;
};


sensor_weather_Mode::sensor_weather_Mode(std::string& port, unsigned int rate)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;
	onlineMsg.online=0;
	pimpl->receivedData.clear();
	

	fDN = 0.0;
	fDM = 0.0;
	fDX  = 0.0;

	fSN = 0.0;
	fSM  = 0.0;
	fSX = 0.0;

	fTA  = 0.0;
	fUA  = 0.0;

	fPA  = 0.0;
	fRC = 0.0;
	
}

sensor_weather_Mode::~sensor_weather_Mode()
{
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
void sensor_weather_Mode::open(std::string devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname,baudrate);
    } catch(boost::system::system_error&)
    {
        //Errors during open
    }
    pimpl->serial.setCallback(bind(&sensor_weather_Mode::readCallback,this, _1, _2));
}

void sensor_weather_Mode::close()
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

bool sensor_weather_Mode::isOpen()
{
    return pimpl->serial.isOpen();
}

bool sensor_weather_Mode::errorStatus()
{
    return pimpl->serial.errorStatus();
}

void sensor_weather_Mode::write(std::string data)
{
    pimpl->serial.writeString(data);
}

void sensor_weather_Mode::readCallback(const char *data, size_t size)
{
    //pimpl->receivedData+=QString::fromAscii(data,size);
        std::string res;
        res.clear();
        res.assign(data,size);
        pimpl->receivedData+=res;
	#if 0
	print_hex(pimpl->receivedData);
	#endif
	
	

}
int sensor_weather_Mode::DriverecvDelay(  
 	int size,    
 	int timeout,    
 	std::string errprint){       
 	int count=0;    
	while(pimpl->receivedData.size()<size){          
		count++;          
		boost::this_thread::sleep(boost::posix_time::milliseconds(2));         
		if(count>timeout){                        
			#if 1                          
			std::cout<<errprint<<":"<<pimpl->receivedData.size()<<'\a'<<std::endl;	
			//print_hex(pimpl->receivedData);
			#endif                         // printf("\a");                          
			count=0;  
			int size = pimpl->receivedData.size();
			return size;                       
			}            
		}        
	return pimpl->receivedData.size();
}

int sensor_weather_Mode::Clear_recv(){       
	pimpl->receivedData.clear();
}

int sensor_weather_Mode::ros_rospub_sensordate_SendRpc()
{   
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	   
	std_msgs::String stdmsg;
      

	//数据封装
	PackageMessage MSGSnd;
        MSGSnd.Clear();
	MSGSnd.set_sessionid(get_session());
        MSGSnd.set_from( ZMQ_NAME_WEATHERSERVICE);
        MSGSnd.set_to(ZMQ_NAME_SERVER);
        
        MSGSnd.set_time( time(NULL));
	//
	PackageMessage_Call * pcallv=MSGSnd.mutable_callmessage();
	pcallv ->set_function( "UploadWeatherInfo");
	std::string swether = PackWeather();
	if(swether.size()>10){
			pcallv->add_parameters(swether);
		}
	else{
			return -1;  
		}
	

	//PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
	//Result->clear_resultdata();
	
      //序列化到内存
    std::ostringstream stream;
        try{
   
    MSGSnd.SerializeToOstream(&stream);
      	}
      catch(...){
		       std::cout <<"["<<__FUNCTION__ << "]"<< "SerializeToOstream a error"<<std::endl;  
			return -1;  
		    } 
    
    std::string text = stream.str();

	stdmsg.data = text;
	LCCompub.publish(stdmsg);
	ros::spinOnce();
	return 1; 
}


void sensor_weather_Mode::sensor_weather_Mode_master(){

       std::cout<< "hello sensor_weather_Mode_master "<<std::endl;

         
         ros::Rate loop_rate(1);
	 int count =0;
		 
         try{
                    while(ros::ok()){
				count++;
				GetweatherData();
				loop_rate.sleep();
				onlineMsg.check_connect();
				if(count>=10){
					count = 0;
					ros_rospub_sensordate_SendRpc();
					}
                     }
             }
        catch(boost::thread_interrupted&){
                 std::cout << "sensor_weather_Mode_master  interrupt" << std:: endl;
             }
    

}

std::string sensor_weather_Mode::GetweatherData(){

	boost::array< char,5> data;
	data.assign(0);
	
	data[0] = '0';
	data[1] = 'R';
	data[2] = '0';
	data[3] = 0x0d;
	data[4] = 0x0a;
	
	std::string res(data.begin(),data.end());
	
	#if 1
	 //char* pd=data.c_array();
	//print_hex(pd,5);
	//std::cout<<"GetweatherData=" << res << std::endl;
	#endif
	
    	
	//return res;
	
	pimpl->receivedData.clear();
	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
	write(res);
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	int size = DriverecvDelay(103,50,"GetweatherData recv timeout");
	if(size>=103){
			onlineMsg.Set_connect();
			std::string recv = pimpl->receivedData;
			//std::cout<<"GetweatherData=" << recv << std::endl;
			RecordData(recv);
		}

	
}

void  sensor_weather_Mode::RecordData(std::string recv)
{
	std::string head = "0R0";
	std::string DN = ",Dn=";  //最小风向
	std::string DM = ",Dm=";  //平均风向
	std::string DX = ",Dx="; //最大风向

	std::string SN = ",Sn=";   //最小风速
	std::string SM = ",Sm=";  //平均风速
	std::string SX = ",Sx=";    //最大风速

	std::string TA = ",Ta=";  //温度
	std::string UA = ",Ua=";  //湿度

	std::string PA = ",Pa=";  //气压
	std::string RC = ",Rc=";  //雨量

	boost::array< char,2> data;
	data[0] = 0x0d;
	data[1] = 0x0a;
	std::string SEnd(data.begin(),data.end());  //结束符

	string::iterator ite;
	
	if(recv.find(head)<recv.length()){
			//找到子串
			std::size_t  dnpos=recv.find(DN);
			std::size_t  DMpos=recv.find(DM);
			std::size_t  DXpos=recv.find(DX);

			std::size_t  SNpos=recv.find(SN);
			std::size_t  SMpos=recv.find(SM);
			std::size_t  SXpos=recv.find(SX);

			std::size_t  TApos=recv.find(TA);
			std::size_t  UApos=recv.find(UA);

			std::size_t  PApos=recv.find(PA);
			std::size_t  RCpos=recv.find(RC);
			std::size_t  SENDpos=recv.find(SEnd);
			
			if(dnpos!=std::string::npos){
					//找到合适位置
					std::string dndate;
					ite = recv.begin();
					for(std::size_t i=(dnpos+4); i < DMpos-1; i++){
							//
							dndate+=ite[i];
						}
					  //std::cout<< "dndate:" << dndate <<std::endl;
					   SDN = dndate;
						try{  
							  
							    fDN=boost::lexical_cast<float>(SDN); // 将字符串转化为整数  
							     //std::cout<< "fDN:" << fDN <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "dndate:" << dndate <<std::endl;
								std::cout<< "fDN:" << "bad_lexical_cast" <<std::endl;
							        fDN=0;
							} 
				}
			if(DMpos!=std::string::npos){
					//找到合适位置
					std::string dmdate;
					ite = recv.begin();
					for(std::size_t i=(DMpos+4); i < DXpos-1; i++){
							//
							dmdate+=ite[i];
						}
					  // std::cout<< "dmdate:" << dmdate<<std::endl;	
					   SDM = dmdate;
						try{  
							  
							    fDM=boost::lexical_cast<float>(SDM); // 将字符串转化为整数  
							    // std::cout<< "fDM:" << fDM <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "dmdate:" << dmdate <<std::endl;
								std::cout<< "fDM:" << "bad_lexical_cast" <<std::endl;
							        fDM=0;
							} 
				}
			if(DXpos!=std::string::npos){
					//找到合适位置
					std::string dxdate;
					ite = recv.begin();
					for(std::size_t i=(DXpos+4); i < SNpos-1; i++){
							//
							dxdate+=ite[i];
						}
					  // std::cout<< "dxdate:" << dxdate<<std::endl;	
					   SDX = dxdate;
						try{  
							  
							    fDX=boost::lexical_cast<float>(SDX); // 将字符串转化为整数  
							     //std::cout<< "fDX:" << fDX <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "dxdate:" << dxdate <<std::endl;
								std::cout<< "fDX:" << "bad_lexical_cast" <<std::endl;
							        fDX=0;
							} 
				}

			if(SNpos!=std::string::npos){
					//找到合适位置
					std::string sndate;
					ite = recv.begin();
					for(std::size_t i=(SNpos+4); i < SMpos-1; i++){
							//
							sndate+=ite[i];
						}
					  // std::cout<< "sndate:" << sndate<<std::endl;	
					   SSN = sndate;
						try{  
							  
							    fSN=boost::lexical_cast<float>(SSN); // 将字符串转化为整数  
							     //std::cout<< "fSN:" << fSN <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "sndate:" << sndate <<std::endl;
								std::cout<< "fSN:" << "bad_lexical_cast" <<std::endl;
							        fSN=0;
							} 
				}
			if(SMpos!=std::string::npos){
					//找到合适位置
					std::string smdate;
					ite = recv.begin();
					for(std::size_t i=(SMpos+4); i < SXpos-1; i++){
							//
							smdate+=ite[i];
						}
					  // std::cout<< "smdate:" << smdate<<std::endl;	
					   SSM = smdate;
						try{  
							  
							    fSM=boost::lexical_cast<float>(SSM); // 将字符串转化为整数  
							    // std::cout<< "fSM:" << fSM <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "smdate:" << smdate <<std::endl;
								std::cout<< "fSM:" << "bad_lexical_cast" <<std::endl;
							        fSM=0;
							} 
				}
			if(SXpos!=std::string::npos){
					//找到合适位置
					std::string sxdate;
					ite = recv.begin();
					for(std::size_t i=(SXpos+4); i < TApos-1; i++){
							//
							sxdate+=ite[i];
						}
					 //  std::cout<< "sxdate:" << sxdate<<std::endl;		
					   SSX = sxdate;
						try{  
							  
							    fSX=boost::lexical_cast<float>(SSX); // 将字符串转化为整数  
							 //    std::cout<< "fSX:" << fSX <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "sxdate:" << sxdate <<std::endl;
								std::cout<< "fSX:" << "bad_lexical_cast" <<std::endl;
							        fSX=0;
							} 
				}

			if(TApos!=std::string::npos){
					//找到合适位置
					std::string tadate;
					ite = recv.begin();
					for(std::size_t i=(TApos+4); i < UApos-1; i++){
							//
							tadate+=ite[i];
						}
					 //  std::cout<< "tadate:" << tadate<<std::endl;	
					   STA = tadate;
						try{  
							  
							    fTA=boost::lexical_cast<float>(STA); // 将字符串转化为整数  
							   //  std::cout<< "fTA:" << fTA <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "tadate:" << tadate <<std::endl;
								std::cout<< "fTA:" << "bad_lexical_cast" <<std::endl;
							        fTA=0;
							} 
				}
			if(UApos!=std::string::npos){
					//找到合适位置
					std::string uadate;
					ite = recv.begin();
					for(std::size_t i=(UApos+4); i < PApos-1; i++){
							//
							uadate+=ite[i];
						}
					 //  std::cout<< "uadate:" << uadate<<std::endl;	
					   SUA = uadate;
						try{  
							  
							    fUA=boost::lexical_cast<float>(SUA); // 将字符串转化为整数  
							  //   std::cout<< "fUA:" << fUA <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "uadate:" << uadate <<std::endl;
								std::cout<< "fUA:" << "bad_lexical_cast" <<std::endl;
							        fUA=0;
							} 
				}
			if(PApos!=std::string::npos){
					//找到合适位置
					std::string padate;
					ite = recv.begin();
					for(std::size_t i=(PApos+4); i < RCpos-1; i++){
							//
							padate+=ite[i];
						}
					   //std::cout<< "padate:" << padate<<std::endl;	
					   SPA = padate;
						try{  
							  
							    fPA =boost::lexical_cast<float>(SPA ); // 将字符串转化为整数  
							    // std::cout<< "fPA:" << fPA <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "padate:" << padate <<std::endl;
								std::cout<< "fPA:" << "bad_lexical_cast" <<std::endl;
							        fPA=0;
							} 
				}
			if(RCpos!=std::string::npos){
					//找到合适位置
					std::string RCdate;
					ite = recv.begin();
					for(std::size_t i=(RCpos+4); i <SENDpos-1 ; i++){
							//
							RCdate+=ite[i];
						}
					   //std::cout<< "RCdate:" << RCdate<<std::endl;	
					   SRC = RCdate;	
						try{  
							  
							    fRC =boost::lexical_cast<float>(SRC ); // 将字符串转化为整数  
							   //  std::cout<< "fRC:" << fRC <<std::endl;
							}  
						catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
							{  
								std::cout<< "RCdate:" << RCdate <<std::endl;
								std::cout<< "fRC:" << "bad_lexical_cast" <<std::endl;
							        fRC=0;
							}
				}
		}
	
}

std::string sensor_weather_Mode::Precipitation_Data_zero(){

	boost::array<char,7> data;
	data.assign(0);
	
	data[0] = 'a';
	data[1] = 'X';
	data[2] = 'Z';
	data[3] = 'R';
	data[4] = 'U';
	data[5] = 0x0d;
	data[6] = 0x0a;

	#if 0
	char* pd=data.c_array();
	print_hex(pd,7);
	#endif
	
    	std::string res(data.begin(),data.end());
	return res;
}

//设置平均风速
/*
std::string sensor_weather_Mode::Set_Tasow(int period){

	boost::array<char,5> data;
	data.assign(0);
	
	data[0] = 'a';
	data[1] = 'X';
	data[2] = 'Z';
	data[1] = 'R';
	data[2] = 'U';
	data[3] = 0x0d;
	data[4] = 0x0a;

	#if 0
	char* pd=data.c_array();
	print_hex(pd,5);
	#endif
	
    	std::string res(data.begin(),data.end());
	return res;
}

*/


unsigned short int sensor_weather_Mode::CRC16(unsigned char *p,unsigned int len)
{
	unsigned char i;
	int j;
	unsigned short int uiCRC = 0xffff;

	for(j=0;j<len;j++)
	{
		uiCRC ^= (*p);
		p++;
		for( i=8; i != 0; i--)
		{
			if( uiCRC & 1 ){
				uiCRC >>= 1;
				uiCRC ^= 0xa001;
			}
			else{
				uiCRC>>=1;
			}
		}
	}
	return(uiCRC);
}



void sensor_weather_Mode::print_dec(unsigned char *datBUF,int Num)
{

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<Num;index++){
                  int x=(unsigned  char)datBUF[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void sensor_weather_Mode::print_hex( char *datBUF,int Num)
 {

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
  void sensor_weather_Mode::print_hex(std::string str)
 {
	int Num = str.size();
	const char * datBUF =  str.c_str();
	
  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }


 std::string sensor_weather_Mode::get_session(void)
     {
         
         boost::uuids::random_generator rgen;
         boost::uuids::uuid u= rgen();
         std::string str;
         assert(uuid::static_size() == 16);
         assert(u.size() == 16);
         std::stringstream ss;
         ss << u; //uuid输出到字符串流
         ss>>str;//字符串流输出到字符串对象
         // std::cout<<"uuid="<<str<<std::endl;
         return str;
     }



std::string sensor_weather_Mode::PackWeather(void){

       GOOGLE_PROTOBUF_VERIFY_VERSION;	 

	WeatherMessage Weather;
	

	Weather.set_online(onlineMsg.online);
	Weather.set_humi(fUA);
	Weather.set_temp(fTA);

	Weather.set_rain(fRC);
	Weather.set_pressure(fPA);

	Weather.set_speed(fSM);    //平均风速
	Weather.set_maxspeed(fSX);    //最大风速
	Weather.set_minispeed(fSN);     //最小风速


	Weather.set_direction(fDM);
	Weather.set_maxdir(fDX);
	Weather.set_minidir(fDN);



      //序列化到内存
    std::ostringstream stream;
    //Weather.SerializeToOstream(&stream);
          try{
   
    Weather.SerializeToOstream(&stream);
      	}
      catch(...){
		       std::cout <<"["<<__FUNCTION__ << "]"<< "SerializeToOstream a error"<<std::endl;  
			return NULL;  
		    }   
    std::string text = stream.str();
	
	  std::cout<< "online = " << onlineMsg.online<<std::endl;
	  std::cout<< "fUA = " << fUA<<std::endl;
	  std::cout<< "fTA = " << fTA<<std::endl;
	  std::cout<< "fRC = " << fRC<<std::endl;
	  std::cout<< "fPA = " << fPA<<std::endl;
	  std::cout<< "fSM = " << fSM<<std::endl;
	  std::cout<< "fSX = " << fSX<<std::endl;
	  std::cout<< "fSN = " << fSN<<std::endl;
	  std::cout<< "fDM = " << fDM<<std::endl;
	  std::cout<< "fDX = " << fDX<<std::endl;
	  std::cout<< "fDN = " << fDN<<std::endl;
	  
	  
	return text;
	
	 }


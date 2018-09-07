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

#include <boost/date_time/posix_time/posix_time.hpp>

#include "ros/ros.h"
#include "ros/console.h"

#include "std_msgs/String.h"


#include "AsyncSerial.h"
#include "sensor_weather_Mode.hpp"
#include "zmqclient.h"



int main(int argc, char **argv)
{
	ros::init(argc, argv, "SensorWeatherNode");

	std::string   COM=DCOM3;
	int badrate=19200;
//参数打印
	std::cout<<"argc= "<<argc<<std::endl;
	for(int index=0;index<argc;index++){
			std::cout<<"argv"<<"["<<index<<"]= "<<argv[index]<<std::endl;
		}
	std::string argv_com=argv[1];
	std::string argv_bandrate=argv[2];

	
	COM=argv_com;

	try{  
		    badrate=boost::lexical_cast<int>(argv_bandrate); // 将字符串转化为整数  
		}  
	catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		{  
		        badrate=19200;
		} 

	
	sensor_weather_Mode LCCom(COM,badrate);
		
	boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

	// pub 
	LCCom.LCCompub = handel->advertise<std_msgs::String>("sensor_weather_updataall", 10);
	
	//LCCom.LCEncounterpub = handel->advertise<dgvmsg::EncounterV>("DriverEncounterS", 1);
	
	//驱动模块刷新的状态数据
	//std::string topic_lf="Synthondriverencounter"+LCCom.Igetstring((int)laser_ctrl_Mode::DEV_DIR_ADD_LF);
	//ros::Subscriber rossub_mode_driverLF =
	//handel->subscribe(topic_lf, 1,  &laser_ctrl_Mode::extern_driver_statuesCallLF,&LCCom);
	//ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok",topic_lf.c_str());

	
	
	boost::thread t1(boost::bind(&sensor_weather_Mode::sensor_weather_Mode_master,&LCCom));  
	
	ros::spin();
	t1.join();

	return 0;


}






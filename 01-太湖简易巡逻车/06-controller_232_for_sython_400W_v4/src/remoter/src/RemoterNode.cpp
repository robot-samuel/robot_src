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
#include "dgvmsg/remoter.h"
#include "dgvmsg/EncounterV.h"
#include "AsyncSerial.h"
#include "zmqclient.h"
#include "RemoterMode.hpp"




int main(int argc, char **argv)
{
   

              ros::init(argc, argv, "RemoterNode");

	       std::string   COM=DCOM7;
		 int badrate=9600;

	//参数打印
	std::cout<<"argc= "<<argc<<std::endl;
	for(int index=0;index<argc;index++){
			std::cout<<"argv"<<"["<<index<<"]= "<<argv[index]<<std::endl;
		}
	std::string argv_com=argv[1];
	std::string argv_bandrate=argv[2];


	COM=argv_com;
	badrate=9600;
	try{  
		    badrate=boost::lexical_cast<int>(argv_bandrate); // 将字符串转化为整数  
		}  
	catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		{  
		        badrate=9600;
		} 
             std::cout<<"RemoterNode:"<<"COM = "<<COM;
		std::cout<<"badrate = "<<badrate<<std::endl;	 
		
		RemoterMode Remoter(COM,badrate);
		
               boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

		// pub 
               Remoter.Remoterpub = handel->advertise<dgvmsg::remoter>("RemoterInfo", 10);

	//SUB
	//编码器信息
	ros::Subscriber rossub_mode_drivers =
	handel->subscribe("dirv_Dcupdataall",1,  &RemoterMode::Controller_statuesCall,&Remoter);
	ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok","dirv_Dcupdataall");
	
	//编码器信息
	ros::Subscriber rossub_mode_Encounter =
	handel->subscribe("DriverEncounterS",1,  &RemoterMode::extern_driver_statuesCall,&Remoter);
	ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok","DriverEncounterS");

	//来自外部传感器模块
	ros::Subscriber rossub_mode_Extern =
	handel->subscribe("ExternInfo", 1, &RemoterMode::ExternInfo_ros_cmdCall,&Remoter);
	ROS_INFO("  Initialization rossub_mode_Extern[topic = %s] port  is ok","ExternInfo");

	//来自BMS模块
	ros::Subscriber rossub_mode_BMS =
	handel->subscribe("BmsInfo", 1, &RemoterMode::BmsInfo_ros_cmdCall,&Remoter);
	ROS_INFO("  Initialization rossub_mode_BMS[topic = %s] port  is ok","BmsInfo");

	//来自磷化氢模块
	ros::Subscriber rossub_mode_PH3 =
	handel->subscribe("Ph3Info", 1, &RemoterMode::Ph3Info_ros_cmdCall,&Remoter);
	ROS_INFO("  Initialization rossub_mode_PH3[topic = %s] port  is ok","Ph3Info");
	

	//配置信息
	ros::Subscriber rossub_mode_recvconfig =
	handel->subscribe("updateconfig",1,  &RemoterMode::rossub_mode_configCall,&Remoter);
	ROS_INFO("  RemoterNode Initialization rossub_mode_recvconfig[topic = %s] port  is ok","updateconfig");
	
	boost::thread t1(boost::bind(&RemoterMode::RemoterMode_master,&Remoter));  
	
	      
	ros::spin();
	t1.join();

              return 0;


}






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


#include "AsyncSerial.h"
#include "sytronDriverMode.hpp"
#include "zmqclient.h"

#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "dgvmsg/ctrl_ModeMessage.h"
#include "dgvmsg/DriverVelocity.h"
#include "dgvmsg/Encounter.h"
#include "dgvmsg/DriverStatues.h"

#include "sytronDriver.h"

int main(int argc, char **argv)
{
   	ros::init(argc, argv, "SynthondriverNode");
	
	std::string   COM1="/dev/ttyS2";
		 //int badrate=115200;
		
//参数打印
	std::cout<<"argc= "<<argc<<std::endl;
	for(int index=0;index<argc;index++){
			std::cout<<"argv"<<"["<<index<<"]= "<<argv[index]<<std::endl;
		}

	std::string argv_add=argv[1];
	std::string argv_com=argv[2];
	std::string argv_bandrate=argv[3];
	std::string argv_dir=argv[4];
	//串口地址
	std::string   COM=argv_com;
	if((argv_com==DCOM3)||(argv_com==DCOM4)||(argv_com==DCOM5)||(argv_com==DCOM6)){
		
		COM=argv_com;
		}
	else{
		std::cout<<"argv_add  wrong ! "<<std::endl;
		return 0;
		}
	//串口波特率
	int badrate=115200;
	try{  
		    badrate=boost::lexical_cast<int>(argv_bandrate); // 将字符串转化为整数  
		}  
	catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		{  
		        badrate=38400;
		} 
	//驱动器地址
	int add = 1;
	try{  
		    add=boost::lexical_cast<int>(argv_add); // 将字符串转化为整数
		    if((add != sytronDriverMode::DEV_DIR_ADD_LF )&&
			(add != sytronDriverMode::DEV_DIR_ADD_LB )&&
			(add != sytronDriverMode::DEV_DIR_ADD_RF )&&
			(add != sytronDriverMode::DEV_DIR_ADD_RB )){
				std::cout<<"add wrong ! "<<(int)add<<std::endl;
				return 0;
		    	}
		}  
	catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		{  
		        add=0;
				std::cout<<"add wrong ! "<<std::endl;
			return 0;
		}
	//旋转方向
	int dir=0;
	try{  
		    dir=boost::lexical_cast<int>(argv_dir); // 将字符串转化为整数
		    if((dir != 1 )&&
			(dir != -1 )){
			std::cout<<"dir wrong ! "<<std::endl;
				return 0;
		    	}
		}  
	catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		{  
		        dir=0;
			std::cout<<"dir wrong ! "<<std::endl;
			return 0;
		}
	
		sytronDriverMode driver(COM,badrate,add,dir);
		
		boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

	// pub 
	driver.rospub_mode_driverstatues = handel->advertise<dgvmsg::DriverStatues>("Synthonfeedback", 10);
	std::string topic="Synthondriverencounter"+argv_add;	
	driver.rospub_mode_driverencounter = handel->advertise<dgvmsg::Encounter>(topic, 1);
	
	
	//驱动模块刷新的状态数据
	ros::Subscriber rossub_mode_driver =
	handel->subscribe("Net2Ctrl", 10,  &sytronDriverMode::Net2Ctrl_driverCall,&driver);
	ROS_INFO("  Initialization rossub_mode_driver Net2Ctrl port  is ok");

	ros::Subscriber rossub_mode_Sdriver = 
		handel->subscribe("Net2SCtrl", 10, &sytronDriverMode::Net2SCtrl_driverCall,&driver);
	ROS_INFO("  Initialization rossub_mode_Sdriver Net2SCtrl  port  is ok");
		
        boost::thread t1(boost::bind(&sytronDriverMode::sytronDriverMode_master,&driver));  


	ros::spin();
	t1.join();

              return 0;


}






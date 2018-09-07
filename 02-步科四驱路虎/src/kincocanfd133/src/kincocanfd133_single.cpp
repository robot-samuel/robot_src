

#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "dgvmsg/Encounter.h"
#include "dgvmsg/EncounterV.h"

#include <string>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/asio.hpp>


#include "zmqclient.h"
#include "socketcan.hpp"
#include "kincocanfd133Mode.hpp"


/*
#include "robot2015_common/cpp/PackageMessage.pb.h"
#include "robot2015_common/cpp/ControllerMessage.pb.h"
#include "robot2015_common/cpp/ParamConfig.pb.h"
*/
#define BROADCAST  0x0000
#define P2P  0x0100

//D9-D10
#define CHANGSTATUS  0x0480
#define CHANGCMD         0x0000
#define SUPERSTATUS   0x0000
#define SUPERCMD         0x0000

#define VER "QTsingtooRobot_v1-2016-06-22-1922"
#define NODENAME "Kincocanfd133Node"



int main(int argc, char **argv)
{
	ros::init(argc, argv,NODENAME);
	boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle);
	//设置接收过滤
         int fid[4]={kincocanfd133Mode::DEV_DIR_ADD_LF+CHANGSTATUS+P2P,               
            kincocanfd133Mode::DEV_DIR_ADD_LB+CHANGSTATUS+P2P,              
            kincocanfd133Mode::DEV_DIR_ADD_RF+CHANGSTATUS+P2P,               
            kincocanfd133Mode::DEV_DIR_ADD_RB+CHANGSTATUS+P2P}; 

	//参数打印	
	std::cout<<"argc= "<<argc<<std::endl;	
	for(int index=0;index<argc;index++){			
	std::cout<<"argv"<<"["<<index<<"]= "<<argv[index]<<std::endl;		
	}	
	
	std::string argv_add=argv[1];	
	std::string argv_com=argv[2];	
	std::string argv_bandrate=argv[3];	
	std::string argv_dir=argv[4];

	std::string  canport_id=CAN1;
	if((argv_com==CAN1)||(argv_com==CAN2)){				
		canport_id=argv_com;		
		}	
	else{		
		std::cout<<"argv_add  wrong ! "<<std::endl;		
		return 0;		
		}


	int add = 127;
	try{  
		    add=boost::lexical_cast<int>(argv_add); // 将字符串转化为整数
		    if((add <=0)&&(add >127)){
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
	
	int canrecv_driver_id = (int)(CHANGSTATUS+P2P+kincocanfd133Mode::MYADD);
	 
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

	 boost::shared_ptr<socketcan> drivcan1(new socketcan(canport_id,fid,4));
	 
         std::vector < boost::shared_ptr<socketcan> > veccanpa;            
          veccanpa.reserve(2);    
            for(int size=0;size <2;size++){                
                        veccanpa.push_back(drivcan1);                
                  }
			
	kincocanfd133Mode Kdriver(canport_id,add,dir);



	
	// pub 
	Kdriver.rospub_mode_driverstatues = handel->advertise<std_msgs::String>("Kincodriverfeedback", 10);
	ROS_INFO("  Initialization rossub_mode_driver Kincodriverfeedback port  is ok");
	Kdriver.rospub_mode_encounter = handel->advertise<dgvmsg::Encounter>("driverencounter", 10);
	
	//驱动模块刷新的状态数据	
	//驱动模块刷新的状态数据
	
	ros::Subscriber rossub_mode_driver =
	handel->subscribe("Net2Ctrl", 10,  &kincocanfd133Mode::Net2Ctrl_driverCall,&Kdriver);
	ROS_INFO("  Initialization rossub_mode_driver Net2Ctrl port  is ok");

	ros::Subscriber rossub_mode_Sdriver = 
		handel->subscribe("Net2SCtrl", 10, &kincocanfd133Mode::Net2SCtrl_driverCall,&Kdriver);
	ROS_INFO("  Initialization rossub_mode_Sdriver Net2SCtrl  port  is ok");

	
	boost::thread t1(boost::bind(&kincocanfd133Mode::kincocanfd133Mode_master,&Kdriver,veccanpa));  
	
	ros::spin();
	t1.join();

	return 0;

}




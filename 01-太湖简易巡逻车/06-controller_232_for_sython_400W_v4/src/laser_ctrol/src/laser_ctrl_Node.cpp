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
#include <geometry_msgs/Twist.h>
#include "dgvmsg/Encounter.h"
#include "dgvmsg/ctrl_ModeMessage.h"

#include "AsyncSerial.h"
#include "laser_ctrl_Mode.hpp"
#include "zmqclient.h"
#include "dgvmsg/EncounterV.h"


int main(int argc, char **argv)
{
	ros::init(argc, argv, "LaserCtrlNode");

	std::string   COM=DCOM8;
	int badrate=115200;
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
		        badrate=115200;
		} 

	
	laser_ctrl_Mode LCCom(COM,badrate);
		
	boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

	// pub 
	LCCom.LCCompub = handel->advertise<dgvmsg::ctrl_ModeMessage>("LaserCtrlCmd", 1);
	
	LCCom.LCEncounterpub = handel->advertise<dgvmsg::EncounterV>("DriverEncounterS", 1);
	
	//驱动模块刷新的状态数据
	std::string topic_lf="Synthondriverencounter"+LCCom.Igetstring((int)laser_ctrl_Mode::DEV_DIR_ADD_LF);
	ros::Subscriber rossub_mode_driverLF =
	handel->subscribe(topic_lf, 1,  &laser_ctrl_Mode::extern_driver_statuesCallLF,&LCCom);
	ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok",topic_lf.c_str());

	std::string topic_lb="Synthondriverencounter"+LCCom.Igetstring((int)laser_ctrl_Mode::DEV_DIR_ADD_LB);
	ros::Subscriber rossub_mode_driverLB =
	handel->subscribe(topic_lb, 1,  &laser_ctrl_Mode::extern_driver_statuesCallLB,&LCCom);
	ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok",topic_lb.c_str());

	std::string topic_rf="Synthondriverencounter"+LCCom.Igetstring((int)laser_ctrl_Mode::DEV_DIR_ADD_RF);
	ros::Subscriber rossub_mode_driverRF =
	handel->subscribe(topic_rf, 1,  &laser_ctrl_Mode::extern_driver_statuesCallRF,&LCCom);
	ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok",topic_rf.c_str());

	std::string topic_rb="Synthondriverencounter"+LCCom.Igetstring((int)laser_ctrl_Mode::DEV_DIR_ADD_RB);
	ros::Subscriber rossub_mode_driverRB =
	handel->subscribe(topic_rb, 1,  &laser_ctrl_Mode::extern_driver_statuesCallRB,&LCCom);
	ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok",topic_rb.c_str());

	
	boost::thread t1(boost::bind(&laser_ctrl_Mode::laser_ctrl_Mode_master,&LCCom));  
	
	ros::spin();
	t1.join();

	return 0;


}






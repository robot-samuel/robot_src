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
#include "uart_ctrl_Mode.hpp"
#include "zmqclient.h"
#include "dgvmsg/EncounterV.h"


int main(int argc, char **argv)
{
	ros::init(argc, argv, "uart_ctrlNode");

	std::string   COM1=DCOM4;
	int badrate=115200;
	
	uart_ctrl_Mode LCCom(COM1,badrate);
		
	boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

	// pub 
	LCCom.LCCompub = handel->advertise<dgvmsg::ctrl_ModeMessage>("UartCtrlCmd", 1);
	
	//LCCom.LCEncounterpub = handel->advertise<dgvmsg::EncounterV>("DriverEncounterS", 1);
	
	//creat sub node
	ros::Subscriber rossub_mode_Driver =
	handel->subscribe("driverencounter", 10, &uart_ctrl_Mode::Drivers_ros_cmdCall,&LCCom);
	ROS_INFO("  Initialization rossub_mode_Driver[topic = %s] port  is ok","driverencounter");


	//crate main thread
	boost::thread t1(boost::bind(&uart_ctrl_Mode::uart_ctrl_Mode_master,&LCCom));  
	
	ros::spin();
	t1.join();

	return 0;


}






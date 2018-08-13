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
#include "pantiltctrlMode.hpp"
#include "zmqclient.h"



int main(int argc, char **argv)
{
   	ros::init(argc, argv, "pantiltctrlNode");
	boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 
	
	
	pantiltctrlMode pantiltctrl;
	// pub 
	pantiltctrl.pantiltctrlInfopub = handel->advertise<std_msgs::String>("zmqreturn", 20);

	//来自无线操控箱的消息
	ros::Subscriber rossub_mode_remote =
	handel->subscribe("RemoterInfo", 10, &pantiltctrlMode::RemoterInfo_ros_cmdCall,&pantiltctrl);
	ROS_INFO("  Initialization rossub_mode_remote[topic = %s] port  is ok","RemoterInfo");

	
        boost::thread t1(boost::bind(&pantiltctrlMode::pantiltctrlMode_master,&pantiltctrl));  
			   
	ros::spin();		   
       t1.join();

        return 0;


}






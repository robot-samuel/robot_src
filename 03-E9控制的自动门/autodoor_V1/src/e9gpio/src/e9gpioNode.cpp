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
#include "dgvmsg/E9gpioCtrl.h"
#include "dgvmsg/E9gpioS.h"


#include "e9gpioMode.hpp"
#include "zmqclient.h"



int main(int argc, char **argv)
{
   	ros::init(argc, argv, "e9gpioNode");
	  
        //ros::NodeHandle n;    
        boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle);
	E9GpioMode GpioMode;

	GpioMode.rospub_obt =  handel->advertise<dgvmsg::E9gpioS>("OBTsingle", 10);


	ros::Subscriber rossub_mode_gpioin =
		handel->subscribe("ctrl_autodoor", 10,  &E9GpioMode::gpio_cmdCall,&GpioMode);
	ROS_INFO("  Initialization rossub_mode_gpioin[topic = %s] port  is ok","ctrl_autodoor");

	
        //crate main thread
        boost::thread th_main(boost::bind(&E9GpioMode::E9GpioMode_master,&GpioMode));

        ros::spin();
        th_main.join();

}






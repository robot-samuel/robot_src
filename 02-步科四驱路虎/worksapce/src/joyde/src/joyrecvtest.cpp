
#include <string>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>


#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include "joystick.hpp"




int main(int argc, char **argv)
{

    ros::init(argc, argv, "JoyTestNode");
    Joystick joyhandle;
    
    joyhandle.init();
    joyhandle.procReadPub();
    return 0;


}


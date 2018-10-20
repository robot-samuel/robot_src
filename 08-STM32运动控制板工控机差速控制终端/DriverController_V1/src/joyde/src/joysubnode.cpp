
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




// global cmd vel twist
static geometry_msgs::Twist command_velocity;

void print_joymsg(const sensor_msgs::Joy::ConstPtr& joy){
    
    std::string buttonsstr;
    for(int index=0;index< joy->buttons.size();index++){
            boost::format fmt("buttons[%d]=%f");
            fmt %index;
            fmt % joy->buttons[index];
            buttonsstr+=fmt.str()+",";
        }
    //std::cout<<str<<std::endl;str.clear();
     std::string axesstr;
    for(int index=0;index< joy->axes.size();index++){
            boost::format fmt("axes[%d]=%f");
            fmt %index;
            fmt %joy->axes[index];
            axesstr+=fmt.str()+",";
        } 
    
      boost::format fmt("[%1%]");
      fmt %joy->header.stamp;
      
    //std::cout<<str<<std::endl;
     ROS_INFO("%s %s\n%s",fmt.str().c_str(),buttonsstr.c_str(),axesstr.c_str());
    
}

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  // X vel driven by left joystick for and aft
  //command_velocity.linear.x  = 2.0*joy->axes[1];
  //command_velocity.linear.y  = 2.0*joy->axes[0];
  //command_velocity.linear.z  = 2.0*joy->axes[7];
  // heading driven by left joysticj left and right
  //command_velocity.angular.x  = -1.0*joy->axes[3];
  //command_velocity.angular.y  = -1.0*joy->axes[4];
  //command_velocity.angular.z  = -1.0*(joy->axes[2] - joy->axes[5]);
  
  print_joymsg(joy);

}



int main(int argc, char **argv)
{

    ros::init(argc, argv, "JoyctrlNode");
    ros::NodeHandle n;
    
   //topic
   ros::Subscriber sub =n.subscribe("Joyctrl", 10, joyCallback);
    ros::spin();
    return 0;


}



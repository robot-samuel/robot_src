

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




#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>

#include "DgvManage.h"



#define VER "dgv_manageNode-2016-06-27-1922"
#define NODENAME "dgv_manageNode"

int main(int argc, char **argv)
{
    try{

        ros::init(argc, argv, NODENAME);
        //ros::NodeHandle n;    
        boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle);
        Dgv_Manage Manager;
        
        ROS_INFO("%s is start now,version=%s",NODENAME,VER);
        
        std::string canport_id;
        int canrecv_sensor_id;
          
          {
              ROS_INFO("%s:prepare for loading param",NODENAME);
              //�������
              ros::NodeHandle nparam;  
          
          }
           
        
        ROS_INFO("  Initialization parameters of %s is ok",NODENAME);

        //初始化ROS_pub线程
        Manager.rospub_mode_zmqreturn =  handel->advertise<std_msgs::String>("zmqreturn", 10);
        Manager.rospub_mode_uodateall =  handel->advertise<std_msgs::String>("dirv_Dcupdataall", 10);
		
	    //(1)网络命令中转到驱动模块
	    Manager.rospub_mode_net2ctrl =  handel->advertise<dgvmsg::DriverVelocity>("Net2Ctrl", 10);
	    Manager.rospub_mode_net2Sctrl =  handel->advertise<std_msgs::String>("Net2SCtrl", 10);        

	    //来自USB遥控手柄
	    ros::Subscriber rossub_mode_joyctrl =
	    handel->subscribe("Joyctrl", 10, &Dgv_Manage::joyCallback,&Manager);
	    ROS_INFO("  Initialization rossub_mode_joyctrl[topic = %s] port  is ok","Joyctrl");

	     //来自驱动器的状态信息反馈
	    ros::Subscriber rossub_mode_Driver =
	    handel->subscribe("Kincodriverfeedback", 10, &Dgv_Manage::Drivers_ros_cmdCall,&Manager);
	    ROS_INFO("  Initialization rossub_mode_Driver[topic = %s] port  is ok","Kincodriverfeedback");


        //来自UART通信口的命令
	    ros::Subscriber rossub_mode_uart =
	    handel->subscribe("UartCtrlCmd", 10, &Dgv_Manage::uartCtrl_ros_cmdCall,&Manager);
	    ROS_INFO("  Initialization rossub_mode_uart[topic = %s] port  is ok","UartCtrlCmd");

	//�����ⲿ����ͨ�ż�������            
	    ros::Subscriber rossub_mode_zmqrecv =
	    handel->subscribe("zmqcmd", 10,  &Dgv_Manage::extern_zmq_cmdCall,&Manager);
	    ROS_INFO("  Initialization rossub_mode_zmqrecv[topic = %s] port  is ok","zmqcmd");
	
        //crate main thread
        boost::thread th_main(boost::bind(&Dgv_Manage::th_mainloop,&Manager));

        ros::spin();
        th_main.join();

        }
    catch(std::exception& e)
        {
            
          std::cerr << e.what() << std::endl;
        }   
  return 0;
}




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
#include "dgvmsg/DriverVelocity.h"
#include "dgvmsg/remoter.h"
#include "dgvmsg/remoterctrl.h"
#include "dgvmsg/Bms.h"
#include "DgvManage.h"
#include "dgvmsg/ExternSensor.h"


#define VER "dgv_manageNode-2016-06-27-1922"
#define NODENAME "dgv_manageNode"

int main(int argc, char **argv)
{
    try{

          ros::init(argc, argv, NODENAME);
          ros::NodeHandle handel;    
        // boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle);
        Dgv_Manage Manager;
        
        ROS_INFO("%s is start now,version=%s",NODENAME,VER);
        
          std::string canport_id;
          int canrecv_sensor_id;
          
           {
               ROS_INFO("%s:prepare for loading param",NODENAME);
               //读入参数
                 ros::NodeHandle nparam;  
          
               }
           
        
           ROS_INFO("  Initialization parameters of %s is ok",NODENAME);

           //初始化ROS_pub线程
           //外围传感参数配置
            Manager.rospub_mode_zmqreturn =  handel.advertise<std_msgs::String>("zmqreturn", 1000);
            Manager.rospub_mode_uodateall =  handel.advertise<std_msgs::String>("dirv_Dcupdataall", 1000);
	   Manager.rospub_mode_updateconfig=  handel.advertise<std_msgs::String>("updateconfig", 10);
	//(1)网络命令中转到驱动模块
	Manager.rospub_mode_net2ctrl =  handel.advertise<dgvmsg::DriverVelocity>("Net2Ctrl", 10);
	Manager.rospub_mode_net2Sctrl =  handel.advertise<std_msgs::String>("Net2SCtrl", 10);

	//发送给协处理器的控制命令
	Manager.rospub_mode_ExternCmd =  handel.advertise<std_msgs::String>("extern_sensor/cmd", 10);
	Manager.rospub_mode_RemoterCmd =  handel.advertise<dgvmsg::remoterctrl>("RemoterNode/cmd", 10);
	//发送给BMS模块的配置命令
	Manager.rospub_mode_BMSModeCmd =  handel.advertise<std_msgs::String>("Bms_Mode/cmd", 10);            

	//来自外部网络通信集的命令            
	ros::Subscriber rossub_mode_zmqrecv =
	handel.subscribe("zmqcmd", 10,  &Dgv_Manage::extern_zmq_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_zmqrecv[topic = %s] port  is ok","zmqcmd");

	//来自遥控手柄的消息
	ros::Subscriber rossub_mode_joyctrl =
	handel.subscribe("Joyctrl", 10, &Dgv_Manage::joyCallback,&Manager);
	ROS_INFO("  Initialization rossub_mode_joyctrl[topic = %s] port  is ok","Joyctrl");

	//来自驱动器组
	ros::Subscriber rossub_mode_Driver =
	handel.subscribe("Synthonfeedback", 10, &Dgv_Manage::Drivers_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_Driver[topic = %s] port  is ok","Synthonfeedback");

	//来自激光导航模块
	ros::Subscriber rossub_mode_Laser =
	handel.subscribe("LaserCtrlCmd", 10, &Dgv_Manage::LaserCtrl_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_Laser[topic = %s] port  is ok","LaserCtrlCmd");

	//来自外部传感器模块
	ros::Subscriber rossub_mode_Extern =
	handel.subscribe("ExternInfo", 10, &Dgv_Manage::ExternInfo_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_Extern[topic = %s] port  is ok","ExternInfo");

	//来自BMS模块
	ros::Subscriber rossub_mode_BMS =
	handel.subscribe("BmsInfo", 10, &Dgv_Manage::BmsInfo_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_BMS[topic = %s] port  is ok","BmsInfo");



	//来自无线操控箱的消息
	ros::Subscriber rossub_mode_remote =
	handel.subscribe("RemoterInfo", 10, &Dgv_Manage::RemoterInfo_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_remote[topic = %s] port  is ok","RemoterInfo");	
            //建立主循环线程
             boost::thread th_main(boost::bind(&Dgv_Manage::th_mainloop,&Manager));

           ros::spin();
            th_main.join();
            }
        catch(std::exception& e){
            
                std::cerr << e.what() << std::endl;
            }   
  return 0;
}


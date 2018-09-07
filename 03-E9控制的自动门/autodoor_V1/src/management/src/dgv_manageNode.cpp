

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
#include "DgvManage.h"
#include "dgvmsg/E9gpioCtrl.h"
#include "dgvmsg/E9gpioS.h"


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

           //初始化ROS_pub线程
           //外围传感参数配置
	Manager.rospub_mode_zmqreturn =  handel.advertise<std_msgs::String>("zmqreturn", 10);
	Manager.rospub_mode_uodateall =  handel.advertise<std_msgs::String>("dirv_Dcupdataall", 10);
	Manager.rospub_ctrl_autodoor =  handel.advertise<dgvmsg::E9gpioCtrl>("ctrl_autodoor", 10);

        

	//来自外部网络通信集的命令            
	ros::Subscriber rossub_mode_zmqrecv =
	handel.subscribe("zmqcmd", 10,  &Dgv_Manage::extern_zmq_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_zmqrecv[topic = %s] port  is ok","zmqcmd");

	ros::Subscriber rossub_mode_gpioin =
	handel.subscribe("OBTsingle", 10,  &Dgv_Manage::gpio_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_gpioin [topic = %s] port  is ok","OBTsingle");
	

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


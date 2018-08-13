

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
               //�������
                 ros::NodeHandle nparam;  
          
               }
           
        
           ROS_INFO("  Initialization parameters of %s is ok",NODENAME);

           //��ʼ��ROS_pub�߳�
           //��Χ���в�������
            Manager.rospub_mode_zmqreturn =  handel.advertise<std_msgs::String>("zmqreturn", 1000);
            Manager.rospub_mode_uodateall =  handel.advertise<std_msgs::String>("dirv_Dcupdataall", 1000);
	   Manager.rospub_mode_updateconfig=  handel.advertise<std_msgs::String>("updateconfig", 10);
	//(1)����������ת������ģ��
	Manager.rospub_mode_net2ctrl =  handel.advertise<dgvmsg::DriverVelocity>("Net2Ctrl", 10);
	Manager.rospub_mode_net2Sctrl =  handel.advertise<std_msgs::String>("Net2SCtrl", 10);

	//���͸�Э�������Ŀ�������
	Manager.rospub_mode_ExternCmd =  handel.advertise<std_msgs::String>("extern_sensor/cmd", 10);
	Manager.rospub_mode_RemoterCmd =  handel.advertise<dgvmsg::remoterctrl>("RemoterNode/cmd", 10);
	//���͸�BMSģ�����������
	Manager.rospub_mode_BMSModeCmd =  handel.advertise<std_msgs::String>("Bms_Mode/cmd", 10);            

	//�����ⲿ����ͨ�ż�������            
	ros::Subscriber rossub_mode_zmqrecv =
	handel.subscribe("zmqcmd", 10,  &Dgv_Manage::extern_zmq_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_zmqrecv[topic = %s] port  is ok","zmqcmd");

	//����ң���ֱ�����Ϣ
	ros::Subscriber rossub_mode_joyctrl =
	handel.subscribe("Joyctrl", 10, &Dgv_Manage::joyCallback,&Manager);
	ROS_INFO("  Initialization rossub_mode_joyctrl[topic = %s] port  is ok","Joyctrl");

	//������������
	ros::Subscriber rossub_mode_Driver =
	handel.subscribe("Synthonfeedback", 10, &Dgv_Manage::Drivers_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_Driver[topic = %s] port  is ok","Synthonfeedback");

	//���Լ��⵼��ģ��
	ros::Subscriber rossub_mode_Laser =
	handel.subscribe("LaserCtrlCmd", 10, &Dgv_Manage::LaserCtrl_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_Laser[topic = %s] port  is ok","LaserCtrlCmd");

	//�����ⲿ������ģ��
	ros::Subscriber rossub_mode_Extern =
	handel.subscribe("ExternInfo", 10, &Dgv_Manage::ExternInfo_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_Extern[topic = %s] port  is ok","ExternInfo");

	//����BMSģ��
	ros::Subscriber rossub_mode_BMS =
	handel.subscribe("BmsInfo", 10, &Dgv_Manage::BmsInfo_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_BMS[topic = %s] port  is ok","BmsInfo");



	//�������߲ٿ������Ϣ
	ros::Subscriber rossub_mode_remote =
	handel.subscribe("RemoterInfo", 10, &Dgv_Manage::RemoterInfo_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_remote[topic = %s] port  is ok","RemoterInfo");	
            //������ѭ���߳�
             boost::thread th_main(boost::bind(&Dgv_Manage::th_mainloop,&Manager));

           ros::spin();
            th_main.join();
            }
        catch(std::exception& e){
            
                std::cerr << e.what() << std::endl;
            }   
  return 0;
}


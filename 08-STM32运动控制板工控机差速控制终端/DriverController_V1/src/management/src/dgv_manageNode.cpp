

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



#include "DgvManage.h"






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

	//(1)����������ת������ģ��
	Manager.rospub_mode_net2ctrl =  handel.advertise<dgvmsg::DriverVelocity>("Net2Ctrl", 10);
	Manager.rospub_mode_net2Sctrl =  handel.advertise<std_msgs::String>("Net2SCtrl", 10);

     

	//�����ⲿ����ͨ�ż�������            
	ros::Subscriber rossub_mode_zmqrecv =
	handel.subscribe("zmqcmd", 10,  &Dgv_Manage::extern_zmq_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_zmqrecv[topic = %s] port  is ok","zmqcmd");

	//����ң���ֱ�����Ϣ
	ros::Subscriber rossub_mode_joyctrl =
	handel.subscribe("Joyctrl", 10, &Dgv_Manage::joyCallback,&Manager);
	ROS_INFO("  Initialization rossub_mode_joyctrl[topic = %s] port  is ok","Joyctrl");


	//����ģ�鷴����״̬
	ros::Subscriber rossub_mode_driver =
	handel.subscribe("FDriverStatues", 10, &Dgv_Manage::Drivers_ros_cmdCall,&Manager);
	ROS_INFO("  Initialization rossub_mode_driver[topic = %s] port  is ok","FDriverStatues");

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


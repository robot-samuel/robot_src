#pragma once

#include <map>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include "dgvmsg/E9gpioCtrl.h"
#include "dgvmsg/E9gpioS.h"



#include "zmqclient.h"
#include "zlogClass.h"




class Dgv_Manage
{
    public:
  	Dgv_Manage();
	~Dgv_Manage();
	
 	void extern_zmq_cmdCall(const std_msgs::String::ConstPtr& msg);
	int printPackageMessage(std::string strmsg);
	
	void th_mainloop();
	int proc_zmq_recvcmd(std::string pack);

	


	int  Zmq_Recv_ctrlmode_autudoor(PackageMessage &MsgRECV);
	int  Zmq_Recv_getstautes_autodoor(PackageMessage &MsgRECV);
	void  check_obt(const dgvmsg::E9gpioS::ConstPtr& msg);
	void ros_zmqreturn_SendRpc(std::string msg);

	void gpio_cmdCall(const dgvmsg::E9gpioS::ConstPtr& msg);

	void ros_pub_ctrl_autodoor_SendRpc( int  cmd);
	
	

	public: //JOY
	   	enum ButtonNum{START  =7};	
		enum CONNECT{YES  =1,NO=0};	
		enum DOORSTAUYES{DOORS_OPEN=1,DOORS_CLOSE=0};	


	
  enum CtrlID{
                    NONE = 0,    //空闲
                    NET_USER=1,  //网络用户控制
                    RS232_USER=2,  //网络用户控制
                    LASER_USER, 
                    REMOTE_JOY,
                    USBREMOTE_JOY   //遥控JOY手柄控制                
                           };
  

  
  enum ZMQCMDID{
                    FUN_HEART =1,    /*心跳*/
                    FUN_CTRL_AUTODOOR,     	/*自动门控制*/ 
                    FUN_GETSTAUTES_AUTODOOR,     	/*自动门状态*/ 

};

   	ros::Publisher rospub_mode_zmqreturn;
	ros::Publisher rospub_mode_uodateall;
	ros::Publisher rospub_ctrl_autodoor;




    private:
		
        int ctrl_ID; 
	int  cmd_IRQ; 
	std::string  AutoDoor_statues;
	std::map<std::string,int > zmqcmdlist;
	int cmdnew;
	int cmdold;
	int AutoDoor_Itatues;
	int AutoDoor_oldtatues;
	
	zlogClass logfile;
	

};


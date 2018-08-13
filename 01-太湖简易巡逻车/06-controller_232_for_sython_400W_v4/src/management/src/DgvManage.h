#pragma once

#include <map>
#include <ros/ros.h>
#include <std_msgs/String.h>

#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include "dgvmsg/ctrl_ModeMessage.h"
#include "dgvmsg/Twist.h"
#include "dgvmsg/DriverVelocity.h"
#include "dgvmsg/DriverNode.h"
#include <sensor_msgs/Joy.h>
#include "dgvmsg/remoter.h"
#include "dgvmsg/remoterctrl.h"
#include "dgvmsg/DriverStatues.h"
#include "dgvmsg/ExternSensor.h"

#include "zmqclient.h"
#include "DgvStatues.h"





#define MAXSPEED 1200
#define MAXSPEEDTH 600
//#define DRIVER_AMOUNT 4

class Dgv_Manage
{
    public:
  	Dgv_Manage();
	~Dgv_Manage();
	
 	void extern_zmq_cmdCall(const std_msgs::String::ConstPtr& msg);
	void printPackageMessage(std::string strmsg);
	
	void th_mainloop();
	int proc_zmq_recvcmd(std::string pack);
	int proc_ros_recvcmd(std::string pack);
	
	std::string get_session(void);
	void ros_zmqreturn_SendRpc(std::string msg);
	void ros_zmqupdate_SendRpc(std::string msg);
	void ros_Net2Ctrl_SendRpc(std::string scmd,int speed,int angel);
	void ros_pub_EXterncmd_SendRpc(std::string msg);
	std::string PackScmd(std::string cmd,std::string par);
	std::string Pack_EXterncmd_CtrlLED(std::string cmd);
	
	
	int  Zmq_Recv_GET_TOKEN(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_FORWARD(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_BACKWARD(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_TURNRIGHT(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_TURNLEFT(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_STOP(PackageMessage &MsgRECV);
	int  Zmq_Recv_GET_SPEED(PackageMessage &MsgRECV);
	int  Zmq_Recv_SPEED_ANGLE(PackageMessage &MsgRECV);
	int  Zmq_Recv_LEFT_RIGHT(PackageMessage &MsgRECV);
	int  Zmq_Recv_SET_TOKEN(PackageMessage &MsgRECV);
	int  Zmq_Recv_CONFIGCAR(PackageMessage &MsgRECV);
	int  Zmq_Recv_GetCONFIGCAR(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_EmergencyStop(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_Realse_Loop(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_Enable_Loop(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_ClearError(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_STOPFREE(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_ctrlmode_light(PackageMessage &MsgRECV);
	int  Zmq_Recv_ctrlmode_fobstac(PackageMessage &MsgRECV);
	int  Zmq_Recv_ctrlmode_bobstac(PackageMessage &MsgRECV);
	int  Zmq_Recv_ctrlmode_autudoor(PackageMessage &MsgRECV);
	int  Zmq_Recv_CAR_ctrl_LED(PackageMessage &MsgRECV);
	
	

	public: //JOY
	   	enum ButtonNum{START  =7};	
		enum CONNECT{YES  =1,NO=0};	
		// geometry_msgs::Twist Joycmd_velocity; 
		//int joyconnect;
		//int joystick_count;
		void print_joymsg(const sensor_msgs::Joy::ConstPtr& joy);
		void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
		void ros_pub_joy_autorun();
		void record_joy_cmd();
		float check_jody_speed(int id,float cmd);

		void ros_pub_LaserCtrl_autorun();
		void record_LaserCtrl_cmd();
		float check_LaserCtrl_speed(int id,float cmd);

		
		
	public:
		int check_connect();

		int is_carcanbectrl();
		int car_run_ptr;
		//net ctrl
		int cmd_speedVelocity;
		int cmd_angelVelocity;

		int net_left;
		int net_right;
		void trans_4_velocity( float speed, float angel);
		int calculate_rpm_speed(char dev_add, float speed);
		// void Drivers_ros_cmdCall(const std_msgs::String::ConstPtr& msg);
		  void Drivers_ros_cmdCall(const dgvmsg::DriverStatues::ConstPtr& msg);
		
		
		 void LaserCtrl_ros_cmdCall(const dgvmsg::ctrl_ModeMessage::ConstPtr& msg);
		 
		 void ExternInfo_ros_cmdCall(const dgvmsg::ExternSensor::ConstPtr& msg);
		 
		   void BmsInfo_ros_cmdCall(const dgvmsg::Bms::ConstPtr& msg);
		   void printfBmsInfo(const dgvmsg::Bms::ConstPtr&  msg);
		   
		 void Ultrasound_ros_cmdCall(const std_msgs::String::ConstPtr& msg);
		  void RemoterInfo_ros_cmdCall(const dgvmsg::remoter::ConstPtr& msg);
		  
		 
		 std::string  pack_dirv_Dcupdataal();
		 void caulate_speed();
		 void ros_LaserNet2Ctrl_SendRpc(std::string scmd,int Lspeed,int Rspeed);
		 void trans_4_Laservelocity( float leftspeed, float rightspeed);

		 void record_remoter_cmd();
		float check_remoter_speed(int id,unsigned int cmd);
		void ros_pub_remoter_autorun();

		std::string Pack_EXterncmd_CtrlLight(int cmd);
		void ros_pub_RemoterCmd_SendRpc(int cmd);
		void ros_updateconfig_SendRpc(std::string msg);
		void CtrlIDturn();
		
    public:


   enum dev_add{
            MYADD  =100,
            DEV_DIR_ADD_LF =100,
            DEV_DIR_ADD_LB =102,
            DEV_DIR_ADD_RF =101,
            DEV_DIR_ADD_RB =103,
    };
	
  enum CtrlID{
                    NONE = 0,    //����
                    NET_USER=1,  //�����û�����
                    RS232_USER=2,  //�����û�����
                    LASER_USER, 
                    REMOTE_JOY,
                    USBREMOTE_JOY   //ң��JOY�ֱ�����                
                           };
  
   enum Color_Mode{
		Mode_Lclose = 0,
		Mode_red = 1,
		Mode_green = 2,
		Mode_blue = 3,
		Mode_red_fli = 4,  //��˸
		Mode_green_fli = 5,
		Mode_blue_fli = 6,
        };
  
  enum ZMQCMDID{
                    FUN_HEART =1,    /*����*/
                    FUN_GET_TOKEN,	/*��������ֵ*/
                    FUN_GET_ALLSTATUS,	/*�������״̬*/
                    FUN_GET_SPEED,	/*��������ٶ�����*/
                    FUN_CTRL_FORWARD,	/*�Զ�ǰ��*/
                    FUN_CTRL_BACKWARD,	/*�Զ�����*/
                    FUN_CTRL_TURNLEFT,	/*�Զ���ת*/
                    FUN_CTRL_TURNRIGHT,	/*�Զ���ת*/
                    FUN_CTRL_STOP,	/*ʹ��ͣ������ͣ��*/
                    FUN_CTRL_STOPFREE,	/*stop run=0*/
                    FUN_CTRL_RUN_SPEED_ANGLE,	/*���ո����ٶ�ֵ�˶�*/
                    FUN_CTRL_RUN_LEFT_RIGHT,	/*���ո����������ٶ�ֵ�˶�*/
                    FUN_DISSOLUTION_ALARM,	/*�Ӵ����б���*/
                    FUN_SET_TOKEN,	/*��������ֵ*/
                    FUN_CONFIGCAR,	/*���ó���*/
                    FUN_GETCONFIGCAR,	/*��õ�ǰ������������*/
                    FUN_UPDATA_ALL,
                    FUN_Realse_Loop,     	/*�������ͷ�*/ 
                    FUN_Enable_Loop,     	/*������ʹ��*/   
                    FUN_ClearError,     	/*�������������*/   
                    FUN_EmergencyStop,     	/*ʹ�ü�ͣ����ͣ��*/          
                    FUN_CTRL_LIGHT,     	/*ǰ�ƿ���*/
                    FUN_CTRL_BOBSTAC,     	/*�����*/
                    FUN_CTRL_FOBSTAC,     	/*ǰ����*/ 
                    FUN_CTRL_AUTODOOR,     	/*�Զ��ſ���*/ 
                    FUN_CTRL_LED,     	/*��ɫ�ƿ���*/ 
};

  enum CARCMDID{
                    CAR_CTRL_FORWARD=0x20,	/*�Զ�ǰ��*/
                    CAR_CTRL_BACKWARD,	/*�Զ�����*/
                    CAR_CTRL_TURNLEFT,	/*�Զ���ת*/
                    CAR_CTRL_TURNRIGHT,	/*�Զ���ת*/
                    CAR_CTRL_STOP,	/*ͣ��*/
                    CAR_CTRL_RUN_SPEED_ANGLE,	/*���ո����ٶ�ֵ�˶�*/
                    CAR_CTRL_RUN_LEFT_RIGHT,	/*���ո����ٶ�ֵ�˶�*/
                    CAR_CTRL_STOPFREE,
                    CAR_CTRL_EmergencyStop,	
                    CAR_CTRL_Realse_Loop,
                    CAR_CTRL_Enable_Loop,
                    CAR_CTRL_ClearError,
                    CAR_CTRL_DEFAUL,
                    
                           };
   	ros::Publisher rospub_mode_zmqreturn;
	ros::Publisher rospub_mode_uodateall;
	ros::Publisher rospub_mode_net2ctrl;
	ros::Publisher rospub_mode_net2Sctrl;
	ros::Publisher rospub_mode_ExternCmd;
	ros::Publisher rospub_mode_BMSModeCmd;
	ros::Publisher rospub_mode_updateconfig;
	ros::Publisher rospub_mode_RemoterCmd;

	const float de_ReductionRatiol = 32;
	const float de_WheelSsize = 3.1415*0.3200;  //�����ܳ� 12������
	
	const float deLF_WheelSsize = 3.1415*0.3200;  //�����ܳ� 12������LF
	const float deLB_WheelSsize = 3.1415*0.3200;  //�����ܳ� 12������LB
	
	const float deRF_WheelSsize = 3.1415*0.3200;  //�����ܳ� 12������RF
	const float deRB_WheelSsize = 3.1415*0.3200;  //�����ܳ� 12������RB
	
	
	const int de_MAXRPM = 3500;  //���ת�� 0.1RPM
	const int de_JODYMAXSPEED = 1200;  //�ɱ�ң������������ٶ� ��mm/s
	const int de_JODYMAXANGEL = 600;  //�ɱ�ң������������ٶ� ��mm/s
	const int de_CTRLMAXSPEED = 1700;   //�ɱ�ָ���������ٶ� ��mm/s
	const int de_CTRLMAXANGLE = 1200;   //�ɱ�ָ���������ٶ� ��mm/s
	const int de_CTRLRATE = 25;   //ָ���������
	
	const int de_CTRLMAXRPM = ((int)((de_CTRLMAXSPEED/(1000*de_WheelSsize))*(de_ReductionRatiol*60*1)));  //���ת�� 0.1RPM	
	const int de_LCTRLMAXRPM = ((int)((de_CTRLMAXSPEED/(1000*deLF_WheelSsize))*(de_ReductionRatiol*60*1)));  //���ת�� 0.1RPM
	const int de_RCTRLMAXRPM = ((int)((de_CTRLMAXSPEED/(1000*deRF_WheelSsize))*(de_ReductionRatiol*60*1)));  //���ת�� 0.1RPM
	
	const int de_JODYMAXRPM = ((int)((de_JODYMAXSPEED/(1000*de_WheelSsize))*(de_ReductionRatiol*60*1)));  //ң�����ɿ������ת�� 0.1RPM		
	const int de_LJODYMAXRPM = ((int)((de_JODYMAXSPEED/(1000*deLF_WheelSsize))*(de_ReductionRatiol*60*1)));  //ң�����ɿ������ת�� 0.1RPM	
	const int de_RJODYMAXRPM = ((int)((de_JODYMAXSPEED/(1000*deRF_WheelSsize))*(de_ReductionRatiol*60*1)));  //ң�����ɿ������ת�� 0.1RPM	

	const int de_MAXSPEED =( (int)((de_MAXRPM/(de_ReductionRatiol*60*1))*de_WheelSsize*1000)); //�������ת��

    private:
		
        int ctrl_ID; 
	int  old_ctrl_ID; 
	std::map<std::string,int > zmqcmdlist;
	 int   velocity[Dgv_statues::DRIVER_AMOUNT];  //�ٶ�ָ��
	Dgv_statues  statues;
	

	 int remtorMAXSPEED;  //�ɱ�ң������������ٶ� ��mm/s
	 int remtorMAXANGEL ;  //�ɱ�ң������������ٶ� ��mm/s	 

};


#pragma once



#include "zmqclient.h"
#include "DgvStatues.h"


#include "zlogClass.h"




#define MAXSPEED 1200
#define MAXSPEEDTH 600
//#define DRIVER_AMOUNT 4

class Dgv_Manage
{
    public:
  	Dgv_Manage();
	~Dgv_Manage();
	
 	void extern_zmq_cmdCall(const std_msgs::String::ConstPtr& msg);

 	void Drivers_ros_cmdCall(const dgvmsg::DriverStatues::ConstPtr& msg);
	
	void th_mainloop();
	int proc_zmq_recvcmd(std::string pack);

	
	std::string get_session(void);
	void ros_zmqreturn_SendRpc(std::string msg);
	void ros_Net2Ctrl_SendRpc(std::string scmd,int speed,int angel);
	std::string PackScmd(std::string cmd,std::string par);

	
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
		
		
		  
		 
		 void caulate_speed();


		void CtrlIDturn();
		
    public:


   enum dev_add{
            MYADD  =100,
            DEV_DIR_ADD_LF =100,
            DEV_DIR_ADD_RF =101
    };
	
  enum CtrlID{
                    NONE = 0,    //空闲
                    NET_USER=1,  //网络用户控制
                    RS232_USER=2,  //网络用户控制
                    LASER_USER, 
                    REMOTE_JOY,
                    USBREMOTE_JOY   //遥控JOY手柄控制                
                           };
  
   enum Color_Mode{
		Mode_Lclose = 0,
		Mode_red = 1,
		Mode_green = 2,
		Mode_blue = 3,
		Mode_red_fli = 4,  //闪烁
		Mode_green_fli = 5,
		Mode_blue_fli = 6,
        };
  
  enum ZMQCMDID{
                    FUN_HEART =1,    /*心跳*/
                    FUN_GET_TOKEN,	/*设置令牌值*/
                    FUN_GET_ALLSTATUS,	/*获得所有状态*/
                    FUN_GET_SPEED,	/*获得所有速度数据*/
                    FUN_CTRL_FORWARD,	/*自动前进*/
                    FUN_CTRL_BACKWARD,	/*自动后退*/
                    FUN_CTRL_TURNLEFT,	/*自动左转*/
                    FUN_CTRL_TURNRIGHT,	/*自动右转*/
                    FUN_CTRL_STOP,	/*使用停车命令停车*/
                    FUN_CTRL_STOPFREE,	/*stop run=0*/
                    FUN_CTRL_RUN_SPEED_ANGLE,	/*按照给定速度值运动*/
                    FUN_CTRL_RUN_LEFT_RIGHT,	/*按照给定所有轮速度值运动*/
                    FUN_DISSOLUTION_ALARM,	/*接触所有报警*/
                    FUN_SET_TOKEN,	/*设置令牌值*/
                    FUN_CONFIGCAR,	/*配置车体*/
                    FUN_GETCONFIGCAR,	/*获得当前所有配置数据*/
                    FUN_UPDATA_ALL,
                    FUN_Realse_Loop,     	/*驱动器释放*/ 
                    FUN_Enable_Loop,     	/*驱动器使能*/   
                    FUN_ClearError,     	/*清除驱动器故障*/   
                    FUN_EmergencyStop,     	/*使用急停命令停车*/          
                    FUN_CTRL_LIGHT,     	/*前灯控制*/
                    FUN_CTRL_BOBSTAC,     	/*后避障*/
                    FUN_CTRL_FOBSTAC,     	/*前避障*/ 
                    FUN_CTRL_AUTODOOR,     	/*自动门控制*/ 
                    FUN_CTRL_LED,     	/*三色灯控制*/ 
                   FUN_CTRL_Carsh,
};

  enum CARCMDID{
                    CAR_CTRL_FORWARD=0x20,	/*自动前进*/
                    CAR_CTRL_BACKWARD,	/*自动后退*/
                    CAR_CTRL_TURNLEFT,	/*自动左转*/
                    CAR_CTRL_TURNRIGHT,	/*自动右转*/
                    CAR_CTRL_STOP,	/*停车*/
                    CAR_CTRL_RUN_SPEED_ANGLE,	/*按照给定速度值运动*/
                    CAR_CTRL_RUN_LEFT_RIGHT,	/*按照给定速度值运动*/
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


	const float de_ReductionRatiol = 32;
	const float de_WheelD= 0.2000;
	const float PI= 3.1415;
	const int de_MAXRPM = 3500;  //
	const int de_CTRLRATE = 25;   //指令跟新周期
	
	const float de_WheelSsize = PI*de_WheelD;  //轮子周长 12寸轮子
	
	const float deLF_WheelSsize = PI*de_WheelD;  //轮子周长 12寸轮子LF
	const float deLB_WheelSsize = PI*de_WheelD;  //轮子周长 12寸轮子LB
	
	const float deRF_WheelSsize = PI*de_WheelD;  //轮子周长 12寸轮子RF
	const float deRB_WheelSsize = PI*de_WheelD;  //轮子周长 12寸轮子RB
	
	
	
	const int de_JODYMAXSPEED = 1000;  //可被遥控器控制最大速度 寸mm/s
	const int de_JODYMAXANGEL = 600;  //可被遥控器控制最大速度 寸mm/s
	const int de_CTRLMAXSPEED = 1000;   //可被指令控制最大速度 寸mm/s
	const int de_CTRLMAXANGLE = 600;   //可被指令控制最大速度 寸mm/s
	
	
	const int de_CTRLMAXRPM = ((int)((de_CTRLMAXSPEED/(1000*de_WheelSsize))*(de_ReductionRatiol*60*1)));  //最大转序 0.1RPM	
	const int de_LCTRLMAXRPM = ((int)((de_CTRLMAXSPEED/(1000*deLF_WheelSsize))*(de_ReductionRatiol*60*1)));  //最大转序 0.1RPM
	const int de_RCTRLMAXRPM = ((int)((de_CTRLMAXSPEED/(1000*deRF_WheelSsize))*(de_ReductionRatiol*60*1)));  //最大转序 0.1RPM
	
	const int de_JODYMAXRPM = ((int)((de_JODYMAXSPEED/(1000*de_WheelSsize))*(de_ReductionRatiol*60*1)));  //遥控器可控制最大转序 0.1RPM		
	const int de_LJODYMAXRPM = ((int)((de_JODYMAXSPEED/(1000*deLF_WheelSsize))*(de_ReductionRatiol*60*1)));  //遥控器可控制最大转序 0.1RPM	
	const int de_RJODYMAXRPM = ((int)((de_JODYMAXSPEED/(1000*deRF_WheelSsize))*(de_ReductionRatiol*60*1)));  //遥控器可控制最大转序 0.1RPM	

	const int de_MAXSPEED =( (int)((de_MAXRPM/(de_ReductionRatiol*60*1))*de_WheelSsize*1000)); //理论最大转速

    private:
		
        int ctrl_ID; 
	int  old_ctrl_ID; 
	std::map<std::string,int > zmqcmdlist;
	 int   velocity[Dgv_statues::DRIVER_AMOUNT];  //速度指令
	Dgv_statues  statues;
	

	 int remtorMAXSPEED;  //可被遥控器控制最大速度 寸mm/s
	 int remtorMAXANGEL ;  //可被遥控器控制最大速度 寸mm/s	 


	 
	zlogClass logfile;
	

};


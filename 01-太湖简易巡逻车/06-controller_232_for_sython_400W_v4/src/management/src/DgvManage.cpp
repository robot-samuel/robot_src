#include <string>
#include <iostream>


#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>



#include "DgvManage.h"


#define DUBUG_ECHO 0

Dgv_Manage::Dgv_Manage()
{

            int index=1;
            zmqcmdlist.insert(std::make_pair("HeartBeat",index++ ));
            zmqcmdlist.insert(std::make_pair("gettoken",index++));
            zmqcmdlist.insert(std::make_pair( "getallstatues",index++));  
            zmqcmdlist.insert(std::make_pair( "getspeed",index++));  
            zmqcmdlist.insert(std::make_pair("ctrlforward",index++));    
            zmqcmdlist.insert(std::make_pair("ctrlbackward",index++));   
            zmqcmdlist.insert(std::make_pair("ctrlturnleft",index++));   
            zmqcmdlist.insert(std::make_pair( "ctrlturnright",index++));   
            zmqcmdlist.insert(std::make_pair("ctrlstop",index++));     //cmdstop
            zmqcmdlist.insert(std::make_pair("ctrlstopfree",index++)); //stop run=0
            zmqcmdlist.insert(std::make_pair("ctrlrun-speed-angle",index++));   
            zmqcmdlist.insert(std::make_pair("ctrlrun-left-right",index++));    
            zmqcmdlist.insert(std::make_pair("dissolutionalarm",index++));   
            zmqcmdlist.insert(std::make_pair("settoken",index++));   
            zmqcmdlist.insert(std::make_pair("configcar",index++));   
            zmqcmdlist.insert(std::make_pair("getcarconfig",index++));  
            zmqcmdlist.insert(std::make_pair("Dcupdataall",index++)); 
            
           zmqcmdlist.insert(std::make_pair("ctrlRealse",index++));
           zmqcmdlist.insert(std::make_pair("ctrlEnable",index++));
           zmqcmdlist.insert(std::make_pair("ClearError",index++));
           zmqcmdlist.insert(std::make_pair("EmergencyStop",index++));
		   	
	     zmqcmdlist.insert(std::make_pair("ctrlmode_light",index++));
	     zmqcmdlist.insert(std::make_pair("ctrlmode_bobstac",index++));
	     zmqcmdlist.insert(std::make_pair("ctrlmode_fobstac",index++));
	     zmqcmdlist.insert(std::make_pair("ctrlmode_autodoor",index++));
	   //  
	    zmqcmdlist.insert(std::make_pair("ctrlLed",index++));
	   


	cmd_speedVelocity=500; //单位mm
	cmd_angelVelocity=100;  //单位mm
	net_left=500;  //单位mm
	net_right=500;  //单位mm

	ctrl_ID = NONE;
	old_ctrl_ID = NONE;

	std::cout << "======Dgv_Manage init======"<<std::endl;

	 remtorMAXSPEED = 1200 ;  //可被遥控器控制最大速度 寸mm/s
	 remtorMAXANGEL = 600;  //可被遥控器控制最大速度 寸mm/s	
           
}

Dgv_Manage::~Dgv_Manage()
 {


}
 void Dgv_Manage::RemoterInfo_ros_cmdCall(const dgvmsg::remoter::ConstPtr& msg)
{
		statues.RemoteNode.Set_connect();
		statues.Remote_count = 0;
		if(msg->online == 1){
		if((msg->IOSTATUES1[0] == YES)&&(msg->IOSTATUES1[6]==NO)){
                   
                    statues.Remoteconnect = YES;
                }
		else{
			 statues.Remoteconnect = NO;
			 statues.Remote_count = 0;
			}


		//存储关键值
	
		statues.Remotestdmsg.linear.x = msg->linear.x;   //LX
		statues.Remotestdmsg.linear.y =  msg->linear.y;  //LY  前后
		statues.Remotestdmsg.linear.z = msg->linear.z;

		statues.Remotestdmsg.angular.x = msg->angular.x;  //RX  左右
		statues.Remotestdmsg.angular.y =  msg->angular.y;  //RY
		//statues.Remotestdmsg.angular.x = msg->angular.x;
		//statues.Remotestdmsg.angular.y = msg->angular.y;
		statues.Remotestdmsg.angular.z = msg->angular.z;


		statues.Remotestdmsg.IOSTATUES1[0] = msg->IOSTATUES1[0];
		statues.Remotestdmsg.IOSTATUES1[1] = msg->IOSTATUES1[1];
		statues.Remotestdmsg.IOSTATUES1[2] = msg->IOSTATUES1[2];
		statues.Remotestdmsg.IOSTATUES1[3] = msg->IOSTATUES1[3];
		statues.Remotestdmsg.IOSTATUES1[4] = msg->IOSTATUES1[4];
		statues.Remotestdmsg.IOSTATUES1[5] = msg->IOSTATUES1[5];
		statues.Remotestdmsg.IOSTATUES1[6] = msg->IOSTATUES1[6];
		statues.Remotestdmsg.IOSTATUES1[7] = msg->IOSTATUES1[7];

		statues.Remotestdmsg.IOSTATUES2[0] = msg->IOSTATUES2[0];
		statues.Remotestdmsg.IOSTATUES2[1] = msg->IOSTATUES2[1];
		statues.Remotestdmsg.IOSTATUES2[2] = msg->IOSTATUES2[2];
		statues.Remotestdmsg.IOSTATUES2[3] = msg->IOSTATUES2[3];
		statues.Remotestdmsg.IOSTATUES2[4] = msg->IOSTATUES2[4];
		statues.Remotestdmsg.IOSTATUES2[5] = msg->IOSTATUES2[5];
		statues.Remotestdmsg.IOSTATUES2[6] = msg->IOSTATUES2[6];
		statues.Remotestdmsg.IOSTATUES2[7] = msg->IOSTATUES2[7];	

		statues.Remotestdmsg.buttons[0] = msg->buttons[0];
		statues.Remotestdmsg.buttons[1] = msg->buttons[1];
		statues.Remotestdmsg.buttons[2] = msg->buttons[2];
		statues.Remotestdmsg.buttons[3] = msg->buttons[3];
		statues.Remotestdmsg.buttons[4] = msg->buttons[4];
		statues.Remotestdmsg.buttons[5] = msg->buttons[5];
		statues.Remotestdmsg.buttons[6] = msg->buttons[6];
		statues.Remotestdmsg.buttons[7] = msg->buttons[7];	

              if( statues.Remoteconnect == YES){
			  	//控制连接的时候生效
		//前避障开关
		if(statues.Remotestdmsg.IOSTATUES1[2] == YES){
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_obstaclestoperror_ischeck(true);
		}
		else{
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_obstaclestoperror_ischeck(false);
			}
		//后避障
		if(statues.Remotestdmsg.IOSTATUES1[3] == YES){
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_bobstaclestoperror_ischeck(true);
		}
		else{
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_bobstaclestoperror_ischeck(false);
			}

		//防跌落控制
		if(statues.Remotestdmsg.IOSTATUES1[4] == YES){
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_dropstoperror_ischeck(true);
		}
		else{
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_dropstoperror_ischeck(false);
			}

		//低速高速切换
		//在主循环里面判断
		
              	}
		#if 0
			//消息打印
		std::cout<<"Remotestdmsg.linear.x = "<< statues.Remotestdmsg.linear.x<<std::endl;
		std::cout<<"Remotestdmsg.linear.y = "<< statues.Remotestdmsg.linear.y<<std::endl;
		std::cout<<"Remotestdmsg.angular.x = "<< statues.Remotestdmsg.angular.x<<std::endl;
		std::cout<<"Remotestdmsg.angular.y = "<< statues.Remotestdmsg.angular.y<<std::endl;
		#endif
			}
		else{
				statues.msg_init();
			}
	
 } 

void Dgv_Manage::printfBmsInfo(const dgvmsg::Bms::ConstPtr& msg)
{
	
	  std::cout<< "bmsSensorrMode printfinfo:"<<std::endl;
	  std::cout<< "stat:"<<msg->Stat<<std::endl;
	  std::cout<< "RSOC:"<<msg->BMS_SOC<<std::endl;
	  std::cout<< "Soc_full:"<<msg->BMS_FULL_SOC<<std::endl;
	  std::cout<< "Voltage:"<<msg->BMS_Voltage<<std::endl;
	  std::cout<< "Current:"<<msg->BMS_sys_Current<<std::endl;
	  std::cout<< "Temp1:"<<msg->BMS_battery_tempA<<std::endl;
	  std::cout<< "Temp2:"<<msg->BMS_battery_tempB<<std::endl;

}
 void Dgv_Manage::BmsInfo_ros_cmdCall(const dgvmsg::Bms::ConstPtr& msg)
{
	#if 0
	printfBmsInfo(msg);
	#endif 
	statues.BmsModeInfo.Stat = msg->Stat;
	statues.BmsModeInfo.BMS_SOC = msg->BMS_SOC;
	statues.BmsModeInfo.BMS_FULL_SOC = msg->BMS_FULL_SOC;
	statues.BmsModeInfo.BMS_Voltage = msg->BMS_Voltage;
	statues.BmsModeInfo.BMS_sys_Current = msg->BMS_sys_Current;
	statues.BmsModeInfo.BMS_battery_tempA = msg->BMS_battery_tempA;
	statues.BmsModeInfo.BMS_battery_tempB =msg->BMS_battery_tempB;
	
	statues.BmsSysNode.Set_connect();

 } 
/*
 void Dgv_Manage::ExternInfo_ros_cmdCall(const dgvmsg::ExternSensor::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string Infostr = msg->data;	
	if(Infostr.size()>50){ 
		//std::cout << "======ExternInfo_ros_cmdCall===1==="<<std::endl;
		try{
		ExternSensor msg;
		msg.ParseFromString(Infostr); 
		if(msg.has_online()&&(msg.temp_humi_size()==2)&&(msg.fan_msg_size()==2)&&(msg.io_collision_size()==8)){
			
			statues.ExternModeInfo.CopyFrom(msg);
			
			statues.ExternSnsorNode.Set_connect();
			statues.CheckExternError();
			}
			}
		catch(google::protobuf::FatalException e){
				
				std::cout << "======ExternInfo_ros_cmdCall==2===="<<std::endl;
				std::cout << e.what() << std::endl;
			}
		//std::cout << "======ExternModeInfo======online="<<statues.ExternModeInfo.online()<<std::endl;
		}
	
 } 
*/
  void Dgv_Manage::ExternInfo_ros_cmdCall(const dgvmsg::ExternSensor::ConstPtr& msg)
{
	statues.ExternModeInfo.online = msg->online;
	
	statues.ExternModeInfo.LED_status = msg->LED_status;

	
	statues.ExternModeInfo.IO_Infrared[0] = msg->IO_Infrared[0];
	statues.ExternModeInfo.IO_Infrared[1] = msg->IO_Infrared[1];
	statues.ExternModeInfo.IO_Infrared[2] = msg->IO_Infrared[2];
	statues.ExternModeInfo.IO_Infrared[3] = msg->IO_Infrared[3];
	statues.ExternModeInfo.IO_Infrared[4] = msg->IO_Infrared[4];
	statues.ExternModeInfo.IO_Infrared[5] = msg->IO_Infrared[5];
	statues.ExternModeInfo.IO_Infrared[6] = msg->IO_Infrared[6];
	statues.ExternModeInfo.IO_Infrared[7] = msg->IO_Infrared[7];

	
	statues.ExternModeInfo.IO_collision[0] = msg->IO_collision[0];
	statues.ExternModeInfo.IO_collision[1] = msg->IO_collision[1];
	statues.ExternModeInfo.IO_collision[2] = msg->IO_collision[2];
	statues.ExternModeInfo.IO_collision[3] = msg->IO_collision[3];
	statues.ExternModeInfo.IO_collision[4] = msg->IO_collision[4];
	statues.ExternModeInfo.IO_collision[5] = msg->IO_collision[5];
	statues.ExternModeInfo.IO_collision[6] = msg->IO_collision[6];
	statues.ExternModeInfo.IO_collision[7] = msg->IO_collision[7];

	
	statues.ExternModeInfo.Temp_Pt100[0] = msg->Temp_Pt100[0];
	statues.ExternModeInfo.Temp_Pt100[1] = msg->Temp_Pt100[1];
	statues.ExternModeInfo.Temp_Pt100[2] = msg->Temp_Pt100[2];
	statues.ExternModeInfo.Temp_Pt100[3] = msg->Temp_Pt100[3];

	statues.ExternModeInfo.interior_temp[0] = msg->interior_temp[0];
	statues.ExternModeInfo.interior_humi[0] = msg->interior_humi[0];
	statues.ExternModeInfo.interior_temp[1] = msg->interior_temp[1];
	statues.ExternModeInfo.interior_humi[1] = msg->interior_humi[1];


	statues.ExternModeInfo.PX24ES_out1 = msg->PX24ES_out1;
	statues.ExternModeInfo.PX24ES_out2 = msg->PX24ES_out2;
	statues.ExternModeInfo.PX24ES_Extraneouslight_out = msg->PX24ES_Extraneouslight_out;
	
	

	
 } 
 void Dgv_Manage::LaserCtrl_ros_cmdCall(const dgvmsg::ctrl_ModeMessage::ConstPtr& msg)
{
       statues.LaserCrelNode.Set_connect();
	statues.laserCmd.online = msg->online;
	statues.laserCmd.twist.linear.x = msg->twist.linear.x;
	statues.laserCmd.twist.linear.y =msg->twist.linear.y;
	statues.laserCmd.twist.linear.z=  0.0;
	// heading driven by left joysticj left and right
	statues.laserCmd.twist.angular.x = msg->twist.angular.x;
	statues.laserCmd.twist.angular.y =  0.0;
	statues.laserCmd.twist.angular.z =  0.0;
	#if 0
	statues.print_laserCmdMsg(msg);
	#endif
	
	if((ctrl_ID <= LASER_USER)&&(statues.laserCmd.online == YES)){
		statues.Laser_isconnect = YES;
		statues.Laser_count=0; 
		}
	else if((statues.laserCmd.online == NO)){
		statues.Laser_isconnect = NO;
		statues.Laser_count=0; 
		}
 } 
 
  void Dgv_Manage::Drivers_ros_cmdCall(const dgvmsg::DriverStatues::ConstPtr& msg)
{
	

		if(msg->device_add == DEV_DIR_ADD_LF){
			//statues.Dirver[0].CopyFrom(DriverInforecv);
			
			statues.feedback[0].device_status = msg->device_status;
			statues.feedback[0].device_add = msg->device_add;
			statues.feedback[0].actvelocityRPM = msg->actvelocityRPM;
			statues.feedback[0].setvelocityRPM = msg->setvelocityRPM;
			statues.feedback[0].dc_voltage = msg->dc_voltage;
			statues.feedback[0].ipm_temperature = msg->ipm_temperature;
			statues.feedback[0].actcurrent =  msg->actcurrent;
			statues.feedback[0].actualpos = msg->actualpos;
			statues.feedback[0].emergencyinputvalue = msg->emergencyinputvalue;
			statues.feedback[0].errorcode	= msg->errorcode;
			statues.driverNode[0].Set_connect();
		}
		else if(msg->device_add == DEV_DIR_ADD_RF){
			statues.feedback[1].device_status = msg->device_status;
			statues.feedback[1].device_add = msg->device_add;
			statues.feedback[1].actvelocityRPM = msg->actvelocityRPM;
			statues.feedback[1].setvelocityRPM = msg->setvelocityRPM;
			statues.feedback[1].dc_voltage = msg->dc_voltage;
			statues.feedback[1].ipm_temperature = msg->ipm_temperature;
			statues.feedback[1].actcurrent =  msg->actcurrent;
			statues.feedback[1].actualpos = msg->actualpos;
			statues.feedback[1].emergencyinputvalue = msg->emergencyinputvalue;
			statues.feedback[1].errorcode	= msg->errorcode;
			statues.driverNode[1].Set_connect();
		}
		else if(msg->device_add == DEV_DIR_ADD_LB){
			statues.feedback[2].device_status = msg->device_status;
			statues.feedback[2].device_add = msg->device_add;
			statues.feedback[2].actvelocityRPM = msg->actvelocityRPM;
			statues.feedback[2].setvelocityRPM = msg->setvelocityRPM;
			statues.feedback[2].dc_voltage = msg->dc_voltage;
			statues.feedback[2].ipm_temperature = msg->ipm_temperature;
			statues.feedback[2].actcurrent =  msg->actcurrent;
			statues.feedback[2].actualpos = msg->actualpos;
			statues.feedback[2].emergencyinputvalue = msg->emergencyinputvalue;
			statues.feedback[2].errorcode	= msg->errorcode;
			statues.driverNode[2].Set_connect();
		}
		else if(msg->device_add== DEV_DIR_ADD_RB){
			statues.feedback[3].device_status = msg->device_status;
			statues.feedback[3].device_add = msg->device_add;
			statues.feedback[3].actvelocityRPM = msg->actvelocityRPM;
			statues.feedback[3].setvelocityRPM = msg->setvelocityRPM;
			statues.feedback[3].dc_voltage = msg->dc_voltage;
			statues.feedback[3].ipm_temperature = msg->ipm_temperature;
			statues.feedback[3].actcurrent =  msg->actcurrent;
			statues.feedback[3].actualpos = msg->actualpos;
			statues.feedback[3].emergencyinputvalue = msg->emergencyinputvalue;
			statues.feedback[3].errorcode	= msg->errorcode;
			statues.driverNode[3].Set_connect();
		}

 } 
  /*
 void Dgv_Manage::Drivers_ros_cmdCall(const std_msgs::String::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string cmdstr = msg->data;
	if(cmdstr.size()>10){
		//std::cout << "======Drivers_rcos_mdCall======"<<std::endl;
		try{
    		ControllerMessage_Dirver DriverInforecv;
    		DriverInforecv.ParseFromString(cmdstr);
		if(DriverInforecv.device_add() == DEV_DIR_ADD_LF){
			statues.Dirver[0].CopyFrom(DriverInforecv);
			statues.driverNode[0].Set_connect();
		}
		else if(DriverInforecv.device_add() == DEV_DIR_ADD_RF){
			statues.Dirver[1].CopyFrom(DriverInforecv);
			statues.driverNode[1].Set_connect();
		}
		else if(DriverInforecv.device_add() == DEV_DIR_ADD_LB){
			statues.Dirver[2].CopyFrom(DriverInforecv);
			statues.driverNode[2].Set_connect();
		}
		else if(DriverInforecv.device_add() == DEV_DIR_ADD_RB){
			statues.Dirver[3].CopyFrom(DriverInforecv);
			statues.driverNode[3].Set_connect();
		}
			}
		catch(google::protobuf::FatalException e){
				
				std::cout << "======Drivers_rcos_mdCall===2==="<<std::endl;
				std::cout << e.what() << std::endl;
			}
		
	}
	                   
 } 
*/
 void Dgv_Manage::extern_zmq_cmdCall(const std_msgs::String::ConstPtr& msg)
{
                // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
               // GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string cmdstr = msg->data;
	if(cmdstr.size()>30){
			//std::cout << "======extern_zmq_cmdCall======"<<std::endl;
			proc_zmq_recvcmd(cmdstr);
		}
                      
 } 
 void Dgv_Manage::print_joymsg(const sensor_msgs::Joy::ConstPtr& joy)
    {
            
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
 
void Dgv_Manage:: joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
           double scale = 1. / (1. - 0.05) / 32767.;
            //print_joymsg(joy);
            if(ctrl_ID <=USBREMOTE_JOY){
          // X vel driven by left joystick for and aft
          statues.Joycmd_velocity.linear.x = joy->axes[1]*-1;
          statues.Joycmd_velocity.linear.y =  0.0;
          statues.Joycmd_velocity.linear.z=  0.0;
          // heading driven by left joysticj left and right
          statues.Joycmd_velocity.angular.x = joy->axes[3]*-1;
          statues.Joycmd_velocity.angular.y =  0.0;
          statues.Joycmd_velocity.angular.z = 0;
          //Joycmd_velocity.angular.z =  scale*joy->axes[6];
                }
            else{
                statues.Joycmd_velocity.linear.x = 0.0;
                statues.Joycmd_velocity.linear.y = 0.0;
                statues.Joycmd_velocity.linear.z=  0.0;
                // heading driven by left joysticj left and right
                statues.Joycmd_velocity.angular.x = 0.0;
                statues.Joycmd_velocity.angular.y =  0.0;
                statues.Joycmd_velocity.angular.z =  0.0;
                }
			
            if(joy->buttons[START] == YES){
                    //ctrl_ID = REMOTE_JOY;
                    statues.joyconnect = YES;
                }

	     if((joy->buttons[0]==YES)&&(joy->buttons[5]==YES)){
		 	//关门
			ros_pub_RemoterCmd_SendRpc(0);
	     	}
		 
	     if((joy->buttons[3]==YES)&&(joy->buttons[5]==YES)){
		 	//开门
			ros_pub_RemoterCmd_SendRpc(1);
	     	}
		 
            if(fabs(statues.Joycmd_velocity.linear.x)<0.1 ){
			statues.Joycmd_velocity.linear.x= 0.0;
                }
            if(fabs(statues.Joycmd_velocity.angular.x)<0.1 ){
			statues.Joycmd_velocity.angular.x = 0.0;
                }

          //print_joymsg(joy);
          statues.joystick_count =0;
		  
	statues.USBJostickNode.Set_connect();
	
     cmd_speedVelocity=(int)check_jody_speed(0,statues.Joycmd_velocity.linear.x);
     cmd_angelVelocity=(int)check_jody_speed(1,statues.Joycmd_velocity.angular.x);
          #if 0
          ROS_INFO("I heard SPEED =%f   ANGEL=%f", statues.Joycmd_velocity.linear.x , 
          statues.Joycmd_velocity.angular.x );
          #endif 

}
float Dgv_Manage::check_jody_speed(int id,float cmd)
{

	float velocity=0;
	if(id == 0){
		//speed
		velocity = cmd  ;
		velocity*=((de_JODYMAXSPEED*1.0)/(32767*1.0));	
		if(fabs(velocity)>de_JODYMAXSPEED){
			if(velocity>0){
				velocity = de_JODYMAXSPEED;
				}
			else if(velocity<0){
				velocity = 0-de_JODYMAXSPEED;
				}
			return velocity;
			}
	}
	else{
		//angle
		velocity = cmd  ;
		velocity*=((de_JODYMAXANGEL*1.0)/(32767*1.0));	
		if(fabs(velocity)>de_JODYMAXANGEL){
			if(velocity>0){
				velocity = de_JODYMAXANGEL;
				}
			else if(velocity<0){
				velocity = 0-de_JODYMAXANGEL;
				}
			return velocity;
			}
		}
	
	return velocity;
}

float Dgv_Manage::check_remoter_speed(int id,unsigned int cmd)
{

	int velocity=0;
	if(id == 0){
		//speed
		velocity = cmd ;
		velocity*=((remtorMAXSPEED*1.0)/(115*1.0));	
		if(fabs(velocity)>remtorMAXSPEED){
			if(velocity>0){
				velocity = remtorMAXSPEED;
				}
			else if(velocity<0){
				velocity = 0-remtorMAXSPEED;
				}
			return velocity;
			}
	}
	else{
		//angle
		velocity = cmd  ;
		velocity*=((remtorMAXANGEL*1.0)/(115*1.0));	
		if(fabs(velocity)>remtorMAXANGEL){
			if(velocity>0){
				velocity = remtorMAXANGEL;
				}
			else if(velocity<0){
				velocity = 0-remtorMAXANGEL;
				}
			return velocity;
			}
		}
	
	return velocity;
}

void Dgv_Manage::record_joy_cmd(){
	
    geometry_msgs::Twist dirvcmd_velocity; 

     dirvcmd_velocity = statues.Joycmd_velocity;
	 
     float  speed =check_jody_speed(0,dirvcmd_velocity.linear.x);
     float angel  =check_jody_speed(1,dirvcmd_velocity.angular.x);
      //记录速度指令 
     statues.cmd_angelVelocity = angel;
     statues.cmd_speedVelocity = speed;	
}


void Dgv_Manage::ros_pub_joy_autorun()
{
    geometry_msgs::Twist dirvcmd_velocity; 

     dirvcmd_velocity = statues.Joycmd_velocity;

     float  speed =check_jody_speed(0,dirvcmd_velocity.linear.x);
     float angel  =check_jody_speed(1,dirvcmd_velocity.angular.x);

	 
     ros_Net2Ctrl_SendRpc("",(int)speed,(int)angel);
}
void Dgv_Manage::record_remoter_cmd(){

     float  speed=check_remoter_speed(0,(int)(statues.Remotestdmsg.linear.y*-1));
     float angel   =check_remoter_speed(1,(int)(statues.Remotestdmsg.angular.x*-1));
     if(fabs(speed)<=10){
		speed=0;	
     	}
     if(fabs(angel)<=10){
		angel=0;	

     	}
      //记录速度指令 
     statues.cmd_angelVelocity = (int)(speed);
     statues.cmd_speedVelocity = (int)(angel);
}
void Dgv_Manage::ros_pub_remoter_autorun()
{
    dgvmsg::ctrl_ModeMessage dirvcmd_velocity; 

     dirvcmd_velocity = statues.laserCmd;
	
     float  speed=check_remoter_speed(0,(int)(statues.Remotestdmsg.linear.y*-1));
     float angel   =check_remoter_speed(1,(int)(statues.Remotestdmsg.angular.x*-1));
     if(fabs(speed)<=10){
		speed=0;	
     	}
     if(fabs(angel)<=10){
		angel=0;	

     	}
    ros_Net2Ctrl_SendRpc("",(int)speed,(int)angel);

}

void Dgv_Manage::record_LaserCtrl_cmd()
{
    dgvmsg::ctrl_ModeMessage dirvcmd_velocity; 

     dirvcmd_velocity = statues.laserCmd;

     float  speedL =check_LaserCtrl_speed(0,dirvcmd_velocity.twist.linear.x);
     float speedR  =check_LaserCtrl_speed(0,dirvcmd_velocity.twist.linear.y);
      //记录速度指令 
     statues.cmd_angelVelocity = (int)(speedR-speedL);
     statues.cmd_speedVelocity = ((int)(speedL+speedR)>>1);

}

void Dgv_Manage::ros_pub_LaserCtrl_autorun()
{
    dgvmsg::ctrl_ModeMessage dirvcmd_velocity; 

     dirvcmd_velocity = statues.laserCmd;

     float  speedL =check_LaserCtrl_speed(0,dirvcmd_velocity.twist.linear.x);
     float speedR  =check_LaserCtrl_speed(0,dirvcmd_velocity.twist.linear.y);

	 
     ros_LaserNet2Ctrl_SendRpc("",(int)speedL,(int)speedR);
}
float Dgv_Manage::check_LaserCtrl_speed(int id,float cmd)
{

	float velocity=0;
	if(id == 0){
		//speed 
		velocity = cmd  ;
		//velocity*=1000.0;	
		if(fabs(velocity)>de_CTRLMAXSPEED){
			if(velocity>0){
				velocity = de_CTRLMAXSPEED;
				}
			else if(velocity<0){
				velocity = 0-de_CTRLMAXSPEED;
				}
			return velocity;
			}
	}
	else{ 
		//angle 
		velocity = cmd  ;
		//velocity*=1000.0;	
		if(fabs(velocity)>de_CTRLMAXANGLE){
			if(velocity>0){
				velocity = de_CTRLMAXANGLE;
				}
			else if(velocity<0){
				velocity = 0-de_CTRLMAXANGLE;
				}
			return velocity;
			}
		}
	
	return velocity;
}
int Dgv_Manage::check_connect()
{
               if(statues.joyconnect == YES){
                  statues.joystick_count++;
                  if(statues.joystick_count >=90){             
                         //触发一次超时接收
                        #if DUBUG_ECHO
                         ROS_INFO("joystick is losted **********************************");
                        #endif
                         statues.joystick_count =0;
                         statues.joyconnect = NO;
     
                       statues.Joycmd_velocity.linear.x=0;
                       statues.Joycmd_velocity.angular.x=0;
			ros_Net2Ctrl_SendRpc("",0,0);
                         //ctrl_ID = NONE;
                         
                     }
                }
              else{
                    statues.joystick_count =0;
                }

               if(statues.Remoteconnect== YES){
                  statues.Remote_count++;
                  if(statues.Remote_count >=90){             
                         //触发一次超时接收
                        #if 1
                         ROS_INFO("Remoteconnect is losted **********************************");
                        #endif
                         statues.Remote_count =0;
                         statues.Remoteconnect = NO;
     			    statues.msg_init();
			  ros_Net2Ctrl_SendRpc("",0,0);
                         //ctrl_ID = NONE;
                         
                     }
                }
              else{
                    statues.Remote_count =0;
		     statues.msg_init();
                }
			  

	if(statues.Laser_isconnect== YES){
		statues.Laser_count++;
		if(statues.Laser_count>90){
			statues.Laser_count=0;
			statues.Laser_isconnect = NO;
                        #if DUBUG_ECHO
                         ROS_INFO("Laser Ctrl is losted **********************************");
                        #endif
			}
		}
	else{
		statues.Laser_count=0;
		}
               
              if(statues.center_isconnect == YES){
                    statues.center_count++;
                  if(statues.center_count >=150){             
                         //触发一次超时接收
                         #if DUBUG_ECHO
                         ROS_INFO("NET USER is losted **********************************");
                         #endif
                         statues.center_count =0;
                         statues.center_isconnect = NO;
			  
                     }
                }
              else{
                    statues.center_count =0;
                }
              
	if(statues.joyconnect == YES){
		if(ctrl_ID<=USBREMOTE_JOY){
			ctrl_ID = USBREMOTE_JOY;
			return ctrl_ID;
			}
		}
	if(statues.Remoteconnect == YES){
		if(ctrl_ID<=REMOTE_JOY){
			ctrl_ID = REMOTE_JOY;
			return ctrl_ID;
			}
		}
	if(statues.Laser_isconnect== YES){
		if(ctrl_ID<=LASER_USER){
			ctrl_ID = LASER_USER;
			return ctrl_ID;
			}
		}
	if(statues.center_isconnect == YES){
		if(ctrl_ID<=NET_USER){
			ctrl_ID = NET_USER;
			return ctrl_ID;
			}
		}
	ctrl_ID = NONE;
	return ctrl_ID;
	
}
int Dgv_Manage::is_carcanbectrl()
{

	if(statues.checkCarCasherror()&&(statues.cmd_speedVelocity>0)){
			//std::cout << "======IsCarCash======"<<std::endl;
			return 0;
		}
	if(statues.checkCarObstacleerror()&&(statues.cmd_speedVelocity>0)){
			//std::cout << "======IsCarObstacle======"<<std::endl;
			return 0;
		}
	if(statues.checkCarBObstacleerror()&&(statues.cmd_speedVelocity<0)){
			//std::cout << "======IsCarBObstacle======"<<std::endl;
			return 0;
		}
	if(statues.checkCarDroperror()){
			//std::cout << "======checkCarDroperror======"<<std::endl;
			return 0;
		}
    return 1;
      
}

void Dgv_Manage::th_mainloop()
{
        
    ros::Rate loop_rate(de_CTRLRATE);
    int satas_count = 0;
    int configup_count = 0;
	std::cout << "======th_mainloop======"<<std::endl;
    while (ros::ok()){
        loop_rate.sleep();
        check_connect();
        satas_count++;
        configup_count++;
        
        //update all satues per 1s
        CtrlIDturn();
        if(ctrl_ID == NET_USER){
			
		//std::cout<<"ctrl_ID == NET_USER"<<std::endl;

                switch(car_run_ptr){
                        case CAR_CTRL_FORWARD:
			      //记录速度指令 
			     statues.cmd_angelVelocity = cmd_angelVelocity;
			     statues.cmd_speedVelocity = cmd_speedVelocity;
                          if(is_carcanbectrl()){
				ros_Net2Ctrl_SendRpc("",cmd_speedVelocity,0);
                            }
                          else{
                            // cmd_speedVelocity = 0;
                             //cmd_angelVelocity = 0;                         
                            // ros_pub_net_autorun();
                                car_run_ptr =CAR_CTRL_DEFAUL; 
                            }
                            break;
                        case CAR_CTRL_BACKWARD:
			      //记录速度指令 
			     statues.cmd_angelVelocity = cmd_angelVelocity;
			     statues.cmd_speedVelocity = cmd_speedVelocity;
                            if(is_carcanbectrl()){
				ros_Net2Ctrl_SendRpc("",cmd_speedVelocity,0);
                                }
                            else{
                                car_run_ptr =CAR_CTRL_DEFAUL; 


                              }

                            break;
                        case CAR_CTRL_TURNLEFT:
			      //记录速度指令 
			     statues.cmd_angelVelocity = cmd_angelVelocity;
			     statues.cmd_speedVelocity = cmd_speedVelocity;
                              if(is_carcanbectrl()){
					ros_Net2Ctrl_SendRpc("",0,cmd_angelVelocity);
                                }
                          else{
                              car_run_ptr =CAR_CTRL_DEFAUL; 


                            }                            
                            break;
                        case CAR_CTRL_TURNRIGHT:
			      //记录速度指令 
			     statues.cmd_angelVelocity = cmd_angelVelocity;
			     statues.cmd_speedVelocity = cmd_speedVelocity;
                             if(is_carcanbectrl()){
					ros_Net2Ctrl_SendRpc("",0,cmd_angelVelocity);
                                }
                           else{
                               car_run_ptr =CAR_CTRL_DEFAUL; 


                            }                           
                            break;
                        case CAR_CTRL_STOPFREE:
			      //记录速度指令 
			     statues.cmd_angelVelocity = 0;
			     statues.cmd_speedVelocity = 0;
				ros_Net2Ctrl_SendRpc("",0,0);
                            break;
                        case CAR_CTRL_RUN_SPEED_ANGLE:
			      //记录速度指令 
			     statues.cmd_angelVelocity = 0;
			     statues.cmd_speedVelocity = 0;
                              if(is_carcanbectrl()){
                                   ros_Net2Ctrl_SendRpc("",cmd_speedVelocity,cmd_angelVelocity);
                                }
                            else{
                               car_run_ptr =CAR_CTRL_DEFAUL; 


                            }                                
                            break;
                        case CAR_CTRL_RUN_LEFT_RIGHT:
			      //记录速度指令 
			     statues.cmd_angelVelocity = net_left-net_right;
			     statues.cmd_speedVelocity = (net_left+net_right)>>1;
                            if(is_carcanbectrl()){
                                     ros_Net2Ctrl_SendRpc("",net_left,net_right);
                                }
                            else{
                                car_run_ptr =CAR_CTRL_DEFAUL; 
                            
                            
                             }   

                            break;
                         case CAR_CTRL_STOP:
			      //记录速度指令 
			     statues.cmd_angelVelocity = 0;
			     statues.cmd_speedVelocity = 0;
                              // if(is_carcanbectrl()){
                                {
                                    std::string cmd="CAR_CTRL_STOP";
					ros_Net2Ctrl_SendRpc(cmd,0,0);
                                   
                                }
                               // }
                              break;
                          case CAR_CTRL_EmergencyStop:
			      //记录速度指令 
			     statues.cmd_angelVelocity = 0;
			     statues.cmd_speedVelocity = 0;
                             //if(is_carcanbectrl()){
                                {
                                std::string cmd="CAR_CTRL_EmergencyStop";
				  ros_Net2Ctrl_SendRpc(cmd,0,0);
                                //if(statues.check_isstop()){
                                       // car_run_ptr =CAR_CTRL_DEFAUL; 
                                    //}
                                }
                               // }
                             break;
                             case CAR_CTRL_Realse_Loop:
			      //记录速度指令 
			     statues.cmd_angelVelocity = 0;
			     statues.cmd_speedVelocity = 0;
                                //if(is_carcanbectrl()){
                                    {
                                   std::string cmd="CAR_CTRL_Realse_Loop";
					ros_Net2Ctrl_SendRpc(cmd,0,0);
					
                                  // if(statues.check_isrelease()){
                                          // car_run_ptr =CAR_CTRL_DEFAUL; 
                                      // }
                                    }

                                 break;
                               case CAR_CTRL_Enable_Loop:
			      //记录速度指令 
			     statues.cmd_angelVelocity = 0;
			     statues.cmd_speedVelocity = 0;
                                   // if(is_carcanbectrl())
                                   {
                                        
                                       std::string cmd="CAR_CTRL_Enable_Loop";
                                     ros_Net2Ctrl_SendRpc(cmd,0,0);
                                        //ros_pub_net_Scmd(PackScmd(cmd,par));
                                       //if(statues.check_isenable()){
                                               //car_run_ptr =CAR_CTRL_DEFAUL; 
                                          // }
                                       }
                                 
                                     break;
                                     case CAR_CTRL_ClearError:
			      //记录速度指令 
			     statues.cmd_angelVelocity = 0;
			     statues.cmd_speedVelocity = 0;
                                         // if(is_carcanbectrl()){
                                            {
                                            std::string cmd="CAR_CTRL_ClearError";
                                            ros_Net2Ctrl_SendRpc(cmd,0,0);

                                            }
                                       break;
                         default :
                            cmd_angelVelocity = 0;
                            cmd_speedVelocity = 0;
			      //记录速度指令 
			     statues.cmd_angelVelocity = cmd_angelVelocity;
			     statues.cmd_speedVelocity = cmd_speedVelocity;
                            ros_Net2Ctrl_SendRpc("",cmd_angelVelocity,cmd_speedVelocity);
                  //  car_run_ptr =CAR_CTRL_DEFAUL;                           
                            break;
                    }

            }
            else if(ctrl_ID == USBREMOTE_JOY){
				#if DUBUG_ECHO
		         std::cout<<"ctrl_ID == USBREMOTE_JOY"<<std::endl;
				#endif
                        car_run_ptr =CAR_CTRL_DEFAUL; 
			  record_joy_cmd();
                        if(is_carcanbectrl()){
	                       
				 ros_pub_joy_autorun();
	                        cmd_angelVelocity = 0;
	                        cmd_speedVelocity = 0;
				 //statues.JoycmdClear();
                            }
                        else{
	                             cmd_angelVelocity = 0;
	                            	cmd_speedVelocity = 0;
					statues.JoycmdClear();
	                             	ros_pub_joy_autorun();
                            #if DUBUG_ECHO
                                ROS_INFO("can not ctrl ros pub: to=%s,linear.x = [%f] linear.y=[%f]", "driver/joycmd",Joycmd_velocity.linear.x,Joycmd_velocity.linear.y);
                            #endif
                            }
				
                    }
		else if(ctrl_ID == LASER_USER){
                       #if DUBUG_ECHO
                        std::cout<<"ctrl_ID == LASER_USER"<<std::endl;
                        #endif
			record_LaserCtrl_cmd();
			car_run_ptr =CAR_CTRL_DEFAUL; 
                        if(is_carcanbectrl()){
                        	 ros_pub_LaserCtrl_autorun();
                        	cmd_angelVelocity = 0;
                       	 cmd_speedVelocity = 0;
                            }
			else{
                        	cmd_angelVelocity = 0;
                       	 cmd_speedVelocity = 0;
				statues.JoycmdClear();
				statues.laserCmdClear();
                            	}
			}
            else if(ctrl_ID == REMOTE_JOY){
				#if 1
		         std::cout<<"ctrl_ID == REMOTE_JOY"<<std::endl;
				#endif
                        car_run_ptr =CAR_CTRL_DEFAUL; 
			if(statues.Remotestdmsg.IOSTATUES1[5] == YES){
					//高速
					 remtorMAXSPEED = 1200 ;  //可被遥控器控制最大速度 寸mm/s
					 remtorMAXANGEL = 600;  //可被遥控器控制最大速度 寸mm/s	
				 
				}
			else{
					//低速
					 remtorMAXSPEED = 600 ;  //可被遥控器控制最大速度 寸mm/s
					 remtorMAXANGEL = 400;  //可被遥控器控制最大速度 寸mm/s	
					

				}
			  record_remoter_cmd();
                        if(is_carcanbectrl()){
	                       
				ros_pub_remoter_autorun();
	                        cmd_angelVelocity = 0;
	                       cmd_speedVelocity = 0;
				 //statues.JoycmdClear();
                            }
                        else{
	                             cmd_angelVelocity = 0;
	                            	cmd_speedVelocity = 0;
					statues.msg_init();
					ros_pub_remoter_autorun();
	                           
                            }
				
                    }
            else{
                        #if DUBUG_ECHO
                        std::cout<<"ctrl_ID == NONE"<<std::endl;
                        #endif
                        car_run_ptr =CAR_CTRL_DEFAUL; 
                        cmd_angelVelocity = 0;
                        cmd_speedVelocity = 0; 
						
			statues.JoycmdClear();

                }
            if(satas_count>=12){
                   ros_zmqupdate_SendRpc(pack_dirv_Dcupdataal());
                    satas_count=0;
                }
            //if((configup_count%10)==0){
                   // upcarconfig();
                   //上传一次本地的配置信息
                 //   configup_count=0;
		     //ros_updateconfig_SendRpc(statues.pack_all_configinfo());
		  
             //   }
            
		statues.ExternSnsorNode.check_connect();
		for(int index=0;index<Dgv_statues::DRIVER_AMOUNT;index++){
            		statues.driverNode[index].check_connect();
	    	}
		//statues.ZmqTransNode.check_connect();
		//statues.LaserCrelNode.check_connect();
		//statues.BmsSysNode.check_connect();
		//statues.Ph3SysNode.check_connect();
		statues.CheckOnline();
		statues.CheckAlarm();
		caulate_speed();
		statues.CheckError();
		
		
            }		

}   

void Dgv_Manage::printPackageMessage(std::string strmsg)
    {
       std::cout << "==================This is PackageMessage"
        <<"message.================"<<std::endl;
       
        PackageMessage pmsg;
        
        pmsg.ParseFromString(strmsg);
         std::cout << "PackageMessage  message{" << std::endl;
        if(pmsg.has_token())
            {
                std::cout << "token: " << pmsg.token()<<std::endl;
            }
        if(pmsg.has_sessionid())
            {
                std::cout << "sessionid: " << pmsg.sessionid()<<std::endl;
            }
        if(pmsg.has_time())
            {
                std::cout << "time: " << pmsg.time()<<std::endl;
            }
        if(pmsg.has_from())
            {
                std::cout << "from: " << pmsg.from()<<std::endl;
            }
        if(pmsg.has_to())
            {
                std::cout << "to: " << pmsg.to()<<std::endl;
            }
        if(pmsg.has_callmessage())
            {
                std::cout << "  call message{" << std::endl;
                if(pmsg.callmessage().has_service())
                   {
                       std::cout << "  service: " << pmsg.callmessage().
service()<<std::endl;
                   }
                  if(pmsg.callmessage().has_function())
                   {
                       std::cout << "  function: " << pmsg.callmessage().
function()<<std::endl;
                   }
                  if(pmsg.callmessage().parameters_size()!=0)
                   {
                       int size= pmsg.callmessage().parameters_size();
                       for(int index=0;  index<size;index++)
                           {
                                std::cout << "  parameters[" <<index<<"]= "<< 
pmsg.callmessage().parameters(index)<<std::endl;
                           }  
                   } 

                std::cout << "  }" << std::endl;
            }
        if(pmsg.has_resultmessage())
            {
                std::cout << "  resultmessage{" << std::endl;
                   PackageMessage_Result *presult =
                        pmsg.mutable_resultmessage();
                   if(presult->has_errorcode()){
                        std::cout << "  errorcode=" <<presult->errorcode()<< std::endl;
                    }
                   if(presult->has_resultdata()){
                        std::cout << "  resultdata size=" 
                        <<presult->resultdata().size()<< std::endl;
                    }
             
                std::cout << "  }" << std::endl;
            }


        std::cout << "}" << std::endl;
    }


int Dgv_Manage::proc_zmq_recvcmd(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    PackageMessage msgrecv;
    msgrecv.ParseFromString(pack);
      int ret=0;
    std::string strto=msgrecv.to();
    
    if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //转发给ZMQ_NAME_DRIVERCTRL
            PackageMessage_Call* callmsg = msgrecv.mutable_callmessage();
            std::string fun=callmsg->function();
            std::map<std::string,int >::iterator it = zmqcmdlist.find(fun); 
            if( it == zmqcmdlist.end() ) {
                    std::cout<<"we can not find function name:"<<fun<<std::endl;
                    return -1;
                } 
         
            int funid= (int)(it->second);
            #if 1
	     if(funid!=1){
            std::cout<<"recv a zmqcmd:"<<fun<<"["<<funid<<"]"<<std::endl;
	     	}
            #endif
            switch(funid){
                    case FUN_HEART:
                        if( ctrl_ID <= NET_USER){
                            statues.center_isconnect = YES;
                            statues.center_count=0; 
                            }
                        statues.ZmqTransNode.Set_connect();
                        break;
		    case FUN_CTRL_FORWARD:
			ret=Zmq_Recv_CAR_FORWARD(msgrecv);
                        if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_FORWARD;
                            }
			break;
		    case FUN_CTRL_BACKWARD:
				ret=Zmq_Recv_CAR_BACKWARD(msgrecv);
                            if(ret==0){
                            //
                                car_run_ptr = CAR_CTRL_BACKWARD;
                            }
			break;
		    case FUN_CTRL_TURNLEFT:
				Zmq_Recv_CAR_TURNLEFT(msgrecv);
                        if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_TURNLEFT;
                            }
			break;
		    case FUN_CTRL_TURNRIGHT:
				Zmq_Recv_CAR_TURNRIGHT(msgrecv);
                   if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_TURNRIGHT;
                            }  
			break;
		    case FUN_CTRL_STOP:
				Zmq_Recv_CAR_STOP(msgrecv);
                            car_run_ptr = CAR_CTRL_STOP;
			break;
		    case FUN_CTRL_STOPFREE:
				Zmq_Recv_CAR_STOPFREE(msgrecv);
				car_run_ptr = CAR_CTRL_STOPFREE;  
			break;
		    case FUN_CTRL_RUN_SPEED_ANGLE:
				Zmq_Recv_SPEED_ANGLE(msgrecv);
                         if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_RUN_SPEED_ANGLE;
                            }   
			break;
		    case FUN_CTRL_RUN_LEFT_RIGHT:
				Zmq_Recv_LEFT_RIGHT(msgrecv);
                     if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_RUN_LEFT_RIGHT;
                            }  
			break;
		    case FUN_Realse_Loop:
				Zmq_Recv_CAR_Realse_Loop(msgrecv);
				car_run_ptr = CAR_CTRL_Realse_Loop;  
			break;
		    case FUN_Enable_Loop:
				Zmq_Recv_CAR_Enable_Loop(msgrecv);
				car_run_ptr = CAR_CTRL_Enable_Loop;  
			break;
		    case FUN_ClearError:
				Zmq_Recv_CAR_ClearError(msgrecv);
				car_run_ptr = CAR_CTRL_ClearError;  
			break;
		    case FUN_EmergencyStop:
				Zmq_Recv_CAR_EmergencyStop(msgrecv);
				car_run_ptr = CAR_CTRL_EmergencyStop;  
			break;
		    case FUN_CTRL_LIGHT:
				
				Zmq_Recv_CAR_ctrlmode_light(msgrecv);
			break;
		    case FUN_CTRL_BOBSTAC:
				Zmq_Recv_ctrlmode_bobstac(msgrecv);
			break;
		    case FUN_CTRL_FOBSTAC:
				Zmq_Recv_ctrlmode_fobstac(msgrecv);
			break;
		case FUN_CTRL_AUTODOOR:
			Zmq_Recv_ctrlmode_autudoor(msgrecv);
			break;
		case FUN_CTRL_LED:
			Zmq_Recv_CAR_ctrl_LED(msgrecv);
			break;
			
			
            	}
            return funid;
      }                
          return -1;      
}   
int Dgv_Manage::proc_ros_recvcmd(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    PackageMessage msgrecv;
    msgrecv.ParseFromString(pack);
      int ret=0;
      #if DUBUG_ECHO
   ROS_INFO("proc_ros_recvcmd");
      #endif
    std::string strto=msgrecv.to();
    
    if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //转发给ZMQ_NAME_DRIVERCTRL
            PackageMessage_Call* callmsg = msgrecv.mutable_callmessage();
            std::string fun=callmsg->function();
            std::map<std::string,int >::iterator it = zmqcmdlist.find(fun); 
            if( it == zmqcmdlist.end() ) {
                    std::cout<<"we can not find function name:"<<fun<<std::endl;
                    return -1;
                } 
         
            int funid= (int)(it->second);
            #if 0
            std::cout<<"recv a roscmd:"<<fun<<"["<<funid<<"]"<<std::endl;
            #endif
    	}
          return -1;      
}   


std::string Dgv_Manage::get_session(void)
{
        
        boost::uuids::random_generator rgen;
        boost::uuids::uuid u= rgen();
        std::string str;
        assert(boost::uuids::uuid::static_size() == 16);
        assert(u.size() == 16);
        std::stringstream ss;
        ss << u; //uuid输出到字符串流
        ss>>str;//字符串流输出到字符串对象
        // std::cout<<"uuid="<<str<<std::endl;
        return str;
}


std::string Dgv_Manage::Pack_EXterncmd_CtrlLight(int cmd){

	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION; 
	ExternSensorConfig SensorConfig;

	SensorConfig.set_led_flight(cmd);

	
       std::string res;
	if (!SensorConfig.SerializeToString(&res)  ) {
	           std::cerr << "Failed to write msg Controller ExternSensorConfig." << std::endl; 
	           return ""; 
	       }	

    return res;    
}
std::string Dgv_Manage::Pack_EXterncmd_CtrlLED(std::string cmd){

	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION; 
	ExternSensorConfig SensorConfig;
	SensorConfig.Clear();
	
	if(cmd == "LED_RED"){
		SensorConfig.set_led_color(Mode_red_fli);
		}
	else if(cmd == "LED_GREEN"){

		SensorConfig.set_led_color(Mode_green_fli);
		}
	else if(cmd == "LED_BLUE"){

		SensorConfig.set_led_color(Mode_blue_fli);
		}	

	
       std::string res;
	if (!SensorConfig.SerializeToString(&res)  ) {
	           std::cerr << "Failed to write msg Controller ExternSensorConfig." << std::endl; 
	           return ""; 
	       }	

    return res;    
}
void Dgv_Manage::ros_pub_RemoterCmd_SendRpc(int cmd)
{   

	dgvmsg::remoterctrl ctrlmsg;
	ctrlmsg.ctrl_autodoor = cmd;
     rospub_mode_RemoterCmd.publish(ctrlmsg);
     ros::spinOnce();
}
void Dgv_Manage::ros_pub_EXterncmd_SendRpc(std::string msg)
{   

    std_msgs::String stdmsg;
     stdmsg.data = msg;
     #if 0
    std::cout<<"ros pub: extern_sensor/cmd"<<"size= "<<stdmsg.data.size()<<std::endl;
     #endif
     rospub_mode_ExternCmd.publish(stdmsg);
     ros::spinOnce();
}
void Dgv_Manage::ros_zmqreturn_SendRpc(std::string msg)
{   
    //ros::NodeHandle npub;
    // ros::Publisher zmq_pub = npub.advertise<std_msgs::String>("dirv_recvcmd", 1);
   
    std_msgs::String stdmsg;
     stdmsg.data = msg;
     #if DUBUG_ECHO
    ROS_INFO("ros pub: to=%s,size= [%d]", "zmqreturn",stdmsg.data.size());
     #endif
     rospub_mode_zmqreturn.publish(stdmsg);
     ros::spinOnce();
}

void Dgv_Manage::ros_zmqupdate_SendRpc(std::string msg)
{   
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION; 
	PackageMessage packMSg;
	packMSg.Clear();
	packMSg.set_sessionid(get_session());
	packMSg.set_from(ZMQ_NAME_DRIVERCTRL);
	packMSg.set_to(ZMQ_NAME_ALLNODE);

	PackageMessage_Call * call = packMSg.mutable_callmessage();
	call->set_function( UPDATE_FUN_NAME);
	call->add_parameters(msg);

    std::string res;
       if (!packMSg.SerializeToString(&res)  ) {
           std::cerr << "Failed to write msg ControllerMessage." << std::endl; 
          // return ""; 
       }  	
	
    std_msgs::String stdmsg;
     stdmsg.data = res;
     #if DUBUG_ECHO
    ROS_INFO("ros pub: to=%s,size= [%d]", "dirv_Dcupdataall",stdmsg.data.size());
     #endif
     rospub_mode_uodateall.publish(stdmsg);
     ros::spinOnce();
}

void Dgv_Manage::ros_Net2Ctrl_SendRpc(std::string scmd,int speed,int angel)
{   
	//ros::NodeHandle npub;
	#if 0
	std::cout<<"scmd="<<scmd<<std::endl;
	std::cout<<"speed="<<speed<<std::endl;
	std::cout<<"angel="<<angel<<std::endl;
	#endif
	
	if(scmd == ""){
		trans_4_velocity(speed,angel);
		dgvmsg::DriverVelocity Motec;
		Motec.driver.resize(4);
	#if 0
	std::cout<<"velocity[0]="<<velocity[0]<<std::endl;
	std::cout<<"velocity[1]="<<velocity[1]<<std::endl;
	std::cout<<"velocity[2]="<<velocity[2]<<std::endl;
	std::cout<<"velocity[3]="<<velocity[3]<<std::endl;
	#endif
            
		Motec.driver[0].name="DEV_DIR_ADD_LF";
		Motec.driver[0].add = DEV_DIR_ADD_LF;
		Motec.driver[0].VRPM = velocity[0];

		Motec.driver[1].name="DEV_DIR_ADD_LB";
		Motec.driver[1].add = DEV_DIR_ADD_LB;
		Motec.driver[1].VRPM = velocity[1];

		Motec.driver[2].name="DEV_DIR_ADD_RF";
		Motec.driver[2].add = DEV_DIR_ADD_RF;
		Motec.driver[2].VRPM = velocity[2];

		Motec.driver[3].name="DEV_DIR_ADD_RB";
		Motec.driver[3].add = DEV_DIR_ADD_RB;
		Motec.driver[3].VRPM = velocity[3];

		rospub_mode_net2ctrl.publish(Motec);
		}
	else{
			std_msgs::String stdmsg;
			
			std::string Sdrvcmd=PackScmd(scmd,"");
			stdmsg.data = Sdrvcmd;
			 rospub_mode_net2Sctrl.publish(stdmsg);
		}
     
     ros::spinOnce();
}

void Dgv_Manage::ros_LaserNet2Ctrl_SendRpc(std::string scmd,int Lspeed,int Rspeed)
{   
	//ros::NodeHandle npub;
	#if 0
	std::cout<<"scmd="<<scmd<<std::endl;
	std::cout<<"speed="<<speed<<std::endl;
	std::cout<<"angel="<<angel<<std::endl;
	#endif
	
	if(scmd == ""){
		trans_4_Laservelocity(Lspeed,Rspeed);
		dgvmsg::DriverVelocity Motec;
		Motec.driver.resize(4);
	#if 0
	std::cout<<"velocity[0]="<<velocity[0]<<std::endl;
	std::cout<<"velocity[1]="<<velocity[1]<<std::endl;
	std::cout<<"velocity[2]="<<velocity[2]<<std::endl;
	std::cout<<"velocity[3]="<<velocity[3]<<std::endl;
	#endif
            
		Motec.driver[0].name="DEV_DIR_ADD_LF";
		Motec.driver[0].add = DEV_DIR_ADD_LF;
		Motec.driver[0].VRPM = velocity[0];

		Motec.driver[1].name="DEV_DIR_ADD_LB";
		Motec.driver[1].add = DEV_DIR_ADD_LB;
		Motec.driver[1].VRPM = velocity[1];

		Motec.driver[2].name="DEV_DIR_ADD_RF";
		Motec.driver[2].add = DEV_DIR_ADD_RF;
		Motec.driver[2].VRPM = velocity[2];

		Motec.driver[3].name="DEV_DIR_ADD_RB";
		Motec.driver[3].add = DEV_DIR_ADD_RB;
		Motec.driver[3].VRPM = velocity[3];

		rospub_mode_net2ctrl.publish(Motec);
		}
	else{
			std_msgs::String stdmsg;
			
			std::string Sdrvcmd=PackScmd(scmd,"");
			stdmsg.data = Sdrvcmd;
			 rospub_mode_net2Sctrl.publish(stdmsg);
		}
     
     ros::spinOnce();
}
//速度分配
void Dgv_Manage::trans_4_velocity( float speed, float angel)
{
	float leftspeed;
	float rightspeed;
	//LEFT
	leftspeed = (speed-angel/2);
       //LF
       
       velocity[0]=calculate_rpm_speed(DEV_DIR_ADD_LF,leftspeed);
         if(abs(velocity[0])>de_MAXRPM){
                 if((velocity[0])>0){
                        velocity[0]=de_MAXRPM;
                    }
                 else{
                        velocity[0]=0-de_MAXRPM;
                    }
            }
	
       velocity[1]=calculate_rpm_speed(DEV_DIR_ADD_LB,leftspeed);
         if(abs(velocity[1])>de_MAXRPM){
                 if((velocity[1])>0){
                        velocity[1]=de_MAXRPM;
                    }
                 else{
                        velocity[1]=0-de_MAXRPM;
                    }
            }	

	//RIGHT
	
	rightspeed = (speed+angel/2);
       velocity[2]=calculate_rpm_speed(DEV_DIR_ADD_RF,rightspeed);
         if(abs(velocity[2])>de_MAXRPM){
                 if((velocity[2])>0){
                        velocity[2]=de_MAXRPM;
                    }
                 else{
                        velocity[2]=0-de_MAXRPM;
                    }
            }
	
       velocity[3]=calculate_rpm_speed(DEV_DIR_ADD_RB,rightspeed);
         if(abs(velocity[3])>de_MAXRPM){
                 if((velocity[3])>0){
                        velocity[3]=de_MAXRPM;
                    }
                 else{
                        velocity[3]=0-de_MAXRPM;
                    }
            }	
          

}
void Dgv_Manage::trans_4_Laservelocity( float leftspeed, float rightspeed)
{
	//float leftspeed;
	//float rightspeed;
	//LEFT
	//leftspeed = (speed-angel/2);
       //LF
       
       //velocity[0]=calculate_rpm_speed(DEV_DIR_ADD_LF,leftspeed);
        velocity[0] = leftspeed;
	   
         if(abs(velocity[0])>de_MAXRPM){
                 if((velocity[0])>0){
                        velocity[0]=de_MAXRPM;
                    }
                 else{
                        velocity[0]=0-de_MAXRPM;
                    }
            }
	
       //velocity[1]=calculate_rpm_speed(DEV_DIR_ADD_LB,leftspeed);
       velocity[1] = leftspeed;
         if(abs(velocity[1])>de_MAXRPM){
                 if((velocity[1])>0){
                        velocity[1]=de_MAXRPM;
                    }
                 else{
                        velocity[1]=0-de_MAXRPM;
                    }
            }	

	//RIGHT
	
	//rightspeed = (speed+angel/2);
       //velocity[2]=calculate_rpm_speed(DEV_DIR_ADD_RF,rightspeed);
        velocity[2] = rightspeed;
         if(abs(velocity[2])>de_MAXRPM){
                 if((velocity[2])>0){
                        velocity[2]=de_MAXRPM;
                    }
                 else{
                        velocity[2]=0-de_MAXRPM;
                    }
            }
	
       //velocity[3]=calculate_rpm_speed(DEV_DIR_ADD_RB,rightspeed);
	   velocity[3] = rightspeed;
         if(abs(velocity[3])>de_MAXRPM){
                 if((velocity[3])>0){
                        velocity[3]=de_MAXRPM;
                    }
                 else{
                        velocity[3]=0-de_MAXRPM;
                    }
            }	
          

}

int Dgv_Manage::calculate_rpm_speed(char dev_add, float speed)
{
       float rpm=0;
	 if(dev_add == DEV_DIR_ADD_LF){
       //LF
    	 rpm = (float)( 
        (speed/(1000.0*deLF_WheelSsize))*(de_ReductionRatiol*60*1));
        }
      else if(dev_add ==DEV_DIR_ADD_LB){
	  	//LB
          	rpm = (float)( 
              (speed/(1000.0*deLB_WheelSsize))*(de_ReductionRatiol*60*1));

        }
      else if(dev_add ==DEV_DIR_ADD_RF){
	  	//RF
          rpm = (float)( 
              (speed/(1000.0*deRF_WheelSsize))*(de_ReductionRatiol*60*1));

        }
      else if(dev_add ==DEV_DIR_ADD_RB){
	  	//RB
          rpm = (float)( 
              (speed/(1000.0*deRB_WheelSsize))*(de_ReductionRatiol*60*1));

        }

    if(fabs(rpm)<5){rpm=0;}
    return (int)rpm;
}

std::string Dgv_Manage::PackScmd(std::string cmd,std::string par)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION; 
    
    PackageMessage msg;
     msg.set_sessionid(get_session());
     msg.set_from("manager");
     msg.set_to("driver");
     msg.set_time( time(NULL));
	 
     PackageMessage_Call* callmsg = msg.mutable_callmessage();
     callmsg->set_function( cmd);
     callmsg->add_parameters(par);

   
   //序列化到内存
   std::ostringstream stream;
   msg.SerializeToOstream(&stream);
   
   std::string ctext = stream.str();
   
  return ctext;
}


int  Dgv_Manage::Zmq_Recv_GET_SPEED(PackageMessage &MsgRECV)
 {
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
	
	//装配
	Result->set_errorcode(0);

         
	MSGSnd.clear_callmessage();

        MSGSnd.set_from(ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());

	//MSGSnd.set_sessionid(get_session());
	MSGSnd.set_sessionid(MsgRECV.sessionid());
	MSGSnd.set_time( time(NULL));
    
          //序列化到内存
        std::ostringstream stream;
        MSGSnd.SerializeToOstream(&stream);

        std::string text = stream.str();
        
        //printPackageMessage(text);
        //发送出去
        ros_zmqreturn_SendRpc(text);
           
	return 0;	
	
}



int  Dgv_Manage::Zmq_Recv_CAR_FORWARD(PackageMessage &MsgRECV)
{
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));
    
	//保存状态值
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	//pcallv->function()
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_speedVelocity = recvspeed>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvspeed<0?0:recvspeed);
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	
	
}

int  Dgv_Manage::Zmq_Recv_CAR_BACKWARD(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//保存状态值
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_speedVelocity= recvspeed>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvspeed<0?0:recvspeed);
		   cmd_speedVelocity = 0-cmd_speedVelocity;
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}	
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
    //printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

}

int  Dgv_Manage::Zmq_Recv_CAR_TURNRIGHT(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//保存状态值
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_angelVelocity = recvspeed>de_CTRLMAXANGLE?de_CTRLMAXANGLE:(recvspeed<0?0:recvspeed);
		    cmd_angelVelocity = 0-cmd_angelVelocity;
			
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_TURNLEFT(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_angelVelocity = recvspeed>de_CTRLMAXANGLE?de_CTRLMAXANGLE:(recvspeed<0?0:recvspeed);
		   
		   
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_STOP(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_STOPFREE(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//保存状态值
	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_EmergencyStop(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//保存状态值
	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}


int  Dgv_Manage::Zmq_Recv_ctrlmode_bobstac(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//配置灯光开关
	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	if(size >= 1){
		 int cmd=0;
		    try{  
		        	cmd=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        cmd=0;  
		    } 
		if(cmd){
			
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_bobstaclestoperror_ischeck(true);
			}
		else{
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_bobstaclestoperror_ischeck(false);
			}
			
		}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}


int  Dgv_Manage::Zmq_Recv_ctrlmode_fobstac(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//配置灯光开关
	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	if(size >= 1){
		 int cmd=0;
		    try{  
		        	cmd=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        cmd=0;  
		    } 
		if(cmd){
			
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_obstaclestoperror_ischeck(true);
			}
		else{
			ParamConfig_Config_Error_Mode * Config_Error_Mode = statues.config.mutable_config_error_msg();
			Config_Error_Mode->set_obstaclestoperror_ischeck(false);
			}
			
		}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_ctrlmode_light(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//配置灯光开关
	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	if(size >= 1){
		 int cmd=0;
		    try{  
		        	cmd=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        cmd=0;  
		    } 
			ros_pub_EXterncmd_SendRpc(Pack_EXterncmd_CtrlLight(cmd));
		}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_ctrl_LED(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//配置LED 灯的闪烁
	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	if(size == 1){
			std::string cmd = pcallv->parameters(0);
			ros_pub_EXterncmd_SendRpc(Pack_EXterncmd_CtrlLED(cmd));
		}
	else{

			Result->set_errorcode(1234);
		}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_ctrlmode_autudoor(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//配置灯光开关
	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	if(size >= 1){
		 int cmd=0;
		    try{  
		        	cmd=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        cmd=0;  
		    } 
		  ros_pub_RemoterCmd_SendRpc(cmd);
		}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_Realse_Loop(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//保存状态值
	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_Enable_Loop(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));
		
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}

      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_ClearError(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//保存状态值
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}



int  Dgv_Manage::Zmq_Recv_SPEED_ANGLE(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//保存状态值
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	//pcallv->function()
	int size = pcallv->parameters_size();

	if(size == 2){
		int recvspeed=0;
	  	int recvth=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_speedVelocity = recvspeed>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvspeed<(0-de_CTRLMAXSPEED)?(0-de_CTRLMAXSPEED):recvspeed);

		    try{  
		        	recvth=boost::lexical_cast<int>(pcallv->parameters(1)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvth=0;  
		    } 
		   cmd_angelVelocity = recvth>de_CTRLMAXANGLE?de_CTRLMAXANGLE:(recvth<(0-de_CTRLMAXANGLE)?(0-de_CTRLMAXANGLE):recvth);
		   
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}	
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_LEFT_RIGHT(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//保存状态值
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	//pcallv->function()
	int size = pcallv->parameters_size();
	if(size == 2){
		int recvspeed=0;
	  	int recvth=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvspeed=0;  
		    } 
		   net_left= recvspeed>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvspeed<(0-de_CTRLMAXSPEED)?(0-de_CTRLMAXSPEED):recvspeed);

		    try{  
		        	recvth=boost::lexical_cast<int>(pcallv->parameters(1)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        recvth=0;  
		    } 
		   net_right= recvth>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvth<(0-de_CTRLMAXSPEED)?(0-de_CTRLMAXSPEED):recvth);
	Result->set_errorcode(0);
	}
	
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //序列化到内存
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
    //printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}



int  Dgv_Manage::Zmq_Recv_CONFIGCAR(PackageMessage &MsgRECV)
 {
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
	//ROS_INFO("proc_zmq_recvcmd->Zmq_Recv_CONFIGCAR");
	//装配
	Result->set_errorcode(0);
         //Result->set_resultdata(param.token());
         
	MSGSnd.clear_callmessage();
        MSGSnd.set_from(ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());

	//MSGSnd.set_sessionid(get_session());
	MSGSnd.set_sessionid(MsgRECV.sessionid());
	MSGSnd.set_time( time(NULL));

        //保存数据
        PackageMessage_Call* callmsg = MsgRECV.mutable_callmessage(); 
         if(callmsg->parameters_size()==1){
              
             
            }
         else{
                Result->set_errorcode(0xD001);
            }
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
          //序列化到内存
        std::ostringstream stream;
        MSGSnd.SerializeToOstream(&stream);

        std::string text = stream.str();
        
        //printPackageMessage(text);
        //发送出去
        ros_zmqreturn_SendRpc(text);
           
	 return (int)(Result->errorcode());
	
}

int  Dgv_Manage::Zmq_Recv_GetCONFIGCAR(PackageMessage &MsgRECV)
 {
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
	
	//装配
	Result->set_errorcode(0);
        // Result->set_resultdata(param.token());
         
	MSGSnd.clear_callmessage();
        MSGSnd.set_from(ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());

	//MSGSnd.set_sessionid(get_session());
	MSGSnd.set_sessionid(MsgRECV.sessionid());
	MSGSnd.set_time( time(NULL));

        //保存数据
     
          //序列化到内存
        std::ostringstream stream;
        MSGSnd.SerializeToOstream(&stream);

        std::string text = stream.str();
        
         //printPackageMessage(text);
        //发送出去
        ros_zmqreturn_SendRpc(text);
           
	 return (int)(Result->errorcode());
	
}

std::string  Dgv_Manage::pack_dirv_Dcupdataal()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION; 
	static int count=0;
	count++;	
	ControllerMessage ConTrolerstate;
	ConTrolerstate.Clear();
	try{

	
	ConTrolerstate.set_stat(count);
	ConTrolerstate.set_ctrl_device(ctrl_ID);
	//std::cout<<"pack_dirv_Dcupdataal == ctrl_ID = "<<ctrl_ID<<std::endl;
	
	ConTrolerstate.set_linevelocity((statues.speedVelocity*0.0001));
	ConTrolerstate.set_thvelocity((statues.angelVelocity*0.0001));

try{
	ControllerMessage_Dirver * drivermsg = ConTrolerstate.add_dirvermessage();
		drivermsg->set_device_status(statues.feedback[0].device_status);
		drivermsg->set_device_add(statues.feedback[0].device_add);
		drivermsg->set_actvelocity(statues.feedback[0].actvelocityRPM);
		drivermsg->set_setvelocity(statues.feedback[0].setvelocityRPM);
		drivermsg->set_ctrl_dc_voltage(statues.feedback[0].dc_voltage);
		drivermsg->set_ctrl_ipm_temperature(statues.feedback[0].ipm_temperature);
		drivermsg->set_actcurrent(statues.feedback[0].actcurrent);
		drivermsg->set_actualpos(statues.feedback[0].actualpos);
		drivermsg->set_emergencyinputvalue(statues.feedback[0].emergencyinputvalue);
		drivermsg->set_error(statues.feedback[0].errorcode);
		//drivermsg->.clear_motec();
		
	//RF
	 drivermsg = ConTrolerstate.add_dirvermessage();
		drivermsg->set_device_status(statues.feedback[1].device_status);
		drivermsg->set_device_add(statues.feedback[1].device_add);
		drivermsg->set_actvelocity(statues.feedback[1].actvelocityRPM);
		drivermsg->set_setvelocity(statues.feedback[1].setvelocityRPM);
		drivermsg->set_ctrl_dc_voltage(statues.feedback[1].dc_voltage);
		drivermsg->set_ctrl_ipm_temperature(statues.feedback[1].ipm_temperature);
		drivermsg->set_actcurrent(statues.feedback[1].actcurrent);
		drivermsg->set_actualpos(statues.feedback[1].actualpos);
		drivermsg->set_emergencyinputvalue(statues.feedback[1].emergencyinputvalue);
		drivermsg->set_error(statues.feedback[1].errorcode);
		//drivermsg->.clear_motec();
	//LB
	drivermsg = ConTrolerstate.add_dirvermessage();
		drivermsg->set_device_status(statues.feedback[2].device_status);
		drivermsg->set_device_add(statues.feedback[2].device_add);
		drivermsg->set_actvelocity(statues.feedback[2].actvelocityRPM);
		drivermsg->set_setvelocity(statues.feedback[2].setvelocityRPM);
		drivermsg->set_ctrl_dc_voltage(statues.feedback[2].dc_voltage);
		drivermsg->set_ctrl_ipm_temperature(statues.feedback[2].ipm_temperature);
		drivermsg->set_actcurrent(statues.feedback[2].actcurrent);
		drivermsg->set_actualpos(statues.feedback[2].actualpos);
		drivermsg->set_emergencyinputvalue(statues.feedback[2].emergencyinputvalue);
		drivermsg->set_error(statues.feedback[2].errorcode);
		//drivermsg->.clear_motec();
	//RB
	drivermsg = ConTrolerstate.add_dirvermessage();
		drivermsg->set_device_status(statues.feedback[3].device_status);
		drivermsg->set_device_add(statues.feedback[3].device_add);
		drivermsg->set_actvelocity(statues.feedback[3].actvelocityRPM);
		drivermsg->set_setvelocity(statues.feedback[3].setvelocityRPM);
		drivermsg->set_ctrl_dc_voltage(statues.feedback[3].dc_voltage);
		drivermsg->set_ctrl_ipm_temperature(statues.feedback[3].ipm_temperature);
		drivermsg->set_actcurrent(statues.feedback[3].actcurrent);
		drivermsg->set_actualpos(statues.feedback[3].actualpos);
		drivermsg->set_emergencyinputvalue(statues.feedback[3].emergencyinputvalue);
		drivermsg->set_error(statues.feedback[3].errorcode);
		//drivermsg->.clear_motec();
		}
	catch(...){
			std::cout << "======pack_dirv_Dcupdataal==ConTrolerstate===="<<std::endl;
			ConTrolerstate.Clear();
			
		}


	//UltrasoundMessage * Ultrasoundmsg = ConTrolerstate.mutable_ultrasound();
	//Ultrasoundmsg->CopyFrom(statues.UltrasoundInfo);

	BmsMessage * BmsMsg = ConTrolerstate.mutable_bmsmsg();
	//BmsMsg->CopyFrom(statues.BmsModeInfo);
	BmsMsg->set_stat(statues.BmsSysNode.online);
	
	BmsMsg->set_bms_battery_tempa(statues.BmsModeInfo.BMS_battery_tempA);
	BmsMsg->set_bms_battery_tempb(statues.BmsModeInfo.BMS_battery_tempB);
	BmsMsg->set_bms_full_soc(statues.BmsModeInfo.BMS_FULL_SOC);
	BmsMsg->set_bms_soc(statues.BmsModeInfo.BMS_SOC);
	BmsMsg->set_bms_voltage(statues.BmsModeInfo.BMS_Voltage);
	BmsMsg->set_bms_sys_current(statues.BmsModeInfo.BMS_sys_Current);

	
	ExternSensor * Pextern = ConTrolerstate.mutable_sensormsg();
	Pextern->set_online(statues.ExternSnsorNode.online);
	Pextern->set_led_status(statues.ExternModeInfo.LED_status);
	Pextern->add_temp_pt100(statues.ExternModeInfo.Temp_Pt100[0]);
	Pextern->add_temp_pt100(statues.ExternModeInfo.Temp_Pt100[1]);
	Pextern->add_temp_pt100(statues.ExternModeInfo.Temp_Pt100[2]);
	Pextern->add_temp_pt100(statues.ExternModeInfo.Temp_Pt100[3]);

	
	// 2路 温湿度
	ExternSensor_Mode_Temp_humi  * pMode_Temp_humi = 
		Pextern->add_temp_humi();
	pMode_Temp_humi->Clear();
	pMode_Temp_humi->set_interior_temp(statues.ExternModeInfo.interior_temp[0]);
	pMode_Temp_humi->set_interior_humi(statues.ExternModeInfo.interior_humi[0]);

	pMode_Temp_humi = Pextern->add_temp_humi();
	pMode_Temp_humi->Clear();
	pMode_Temp_humi->set_interior_temp(statues.ExternModeInfo.interior_temp[1]);
	pMode_Temp_humi->set_interior_humi(statues.ExternModeInfo.interior_humi[1]);	


	//预留8路防跌落，实际使用前4路
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[0]);
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[1]);
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[2]);
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[3]);
	
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[4]);
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[5]);
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[6]);
	Pextern->add_io_infrared(statues.ExternModeInfo.IO_Infrared[7]);

	//预留8路防碰撞，实际不使用
	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[0]);
	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[1]);
	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[2]);
	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[3]);

	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[4]);
	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[5]);
	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[6]);
	Pextern->add_io_collision(statues.ExternModeInfo.IO_collision[7]);

	ExternSensor_Mode_PX24ES * pMode_PX24ES =Pextern->mutable_px24es();
	pMode_PX24ES->set_out1(statues.ExternModeInfo.PX24ES_out1);
	pMode_PX24ES->set_out2(statues.ExternModeInfo.PX24ES_out2);
	pMode_PX24ES->set_extraneouslight_out(statues.ExternModeInfo.PX24ES_Extraneouslight_out);

	ErrorMessage  * Perror =  ConTrolerstate.mutable_errormsg();
	Perror->CopyFrom(statues.ErrorMsg);

	AlarmMessage * Palarm = ConTrolerstate.mutable_alarmmsg();
	Palarm->CopyFrom(statues.AlarmMsg);



		   
		}
	
	catch(google::protobuf::FatalException e){
				
				std::cout << "======pack_dirv_Dcupdataal======"<<std::endl;
				std::cout << e.what() << std::endl;
	}
	
	RobotStatusMessage StatusMessage;
	StatusMessage.Clear();
	ControllerMessage *PConTrolerstate  = StatusMessage.mutable_controller();
	PConTrolerstate->CopyFrom(ConTrolerstate);
	
	    std::string res;
	       if (!StatusMessage.SerializeToString(&res)  ) {
	           std::cerr << "Failed to write msg Controller StatusMessage Message." << std::endl; 
	           return ""; 
	       }	

    return res;    
    
}


void Dgv_Manage::caulate_speed(){
	
	int speed_lf=statues.feedback[0].actvelocityRPM;
	int speed_lb=statues.feedback[2].actvelocityRPM;
	int speed_rf=statues.feedback[1].actvelocityRPM;
	int speed_rb=statues.feedback[3].actvelocityRPM;

	statues.speedVelocity=(((speed_lf+speed_lb)>>1)+((speed_rf+speed_rb)>>1))>>1;
	statues.angelVelocity = (((speed_lf+speed_lb)>>1)-((speed_rf+speed_rb)>>1));

	
}

void Dgv_Manage::ros_updateconfig_SendRpc(std::string msg)
{
	int size = msg.size();
	if(size>10){
	        std_msgs::String stdmsg;
	        stdmsg.data = msg;
     		rospub_mode_updateconfig.publish(stdmsg);
     		ros::spinOnce();
		}
}

void Dgv_Manage::CtrlIDturn()
{

	if(ctrl_ID !=old_ctrl_ID){
			//发送停车
                     std::string cmd="CAR_CTRL_STOP";
			ros_Net2Ctrl_SendRpc(cmd,0,0);
		}
	old_ctrl_ID = ctrl_ID;

}

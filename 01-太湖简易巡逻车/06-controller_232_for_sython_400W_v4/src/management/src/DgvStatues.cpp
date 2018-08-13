

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <iostream>  
#include <sstream> 
#include <string> 

#include "DgvStatues.h"

Dgv_statues::Dgv_statues()
{
	for(int index=0;index<DRIVER_AMOUNT;index++){
			driveradd[index] = DEV_DIR_ADD_LF+index;
			driverNode[index].online = 0;
			//Dirver[index].Clear();
		}
	

	Laser_isconnect=0;
	Laser_count=0;
	laserCmdClear();
	
	//ExternModeInfo.Clear();
	//Init_ExternModeInfo();
	ExternModeInfo.IO_Infrared.resize(8);
	ExternModeInfo.IO_collision.resize(8);
	ExternModeInfo.Temp_Pt100.resize(4);
	ExternModeInfo.interior_temp.resize(2);
	ExternModeInfo.interior_humi.resize(2);

	joystick_count = 0;
	joyconnect = 0;
	JoycmdClear();

	center_isconnect = 0;
	center_count = 0;

	User232connect = 0;
	User232_count = 0;

	ErrorMsg.Clear();
	AlarmMsg.Clear();

	ParamConfig_Config_Mode_Driver *Config_Mode_Driver =config.mutable_config_driver_msg();
	Config_Mode_Driver->set_autorun_angel(700);
	Config_Mode_Driver->set_autorun_speed(900);
	Config_Mode_Driver->set_remote_angel(700);
	Config_Mode_Driver->set_remote_speed(1500);
	
	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();
	//������������
	Config_Error_Mode->set_carleftbdrivererror_ischeck(true);
	Config_Error_Mode->set_carleftfdrivererror_ischeck(true);
	Config_Error_Mode->set_carrightbdrivererror_ischeck(true);
	Config_Error_Mode->set_carrightfdrivererror_ischeck(true);

	//������ͨ�Žڵ���
	Config_Error_Mode->set_rightbdrivercomerror_ischeck(true);
	Config_Error_Mode->set_rightfdrivercomerror_ischeck(true);
	Config_Error_Mode->set_leftbdrivercomerror_ischeck(true);
	Config_Error_Mode->set_leftfdrivercomerror_ischeck(true);

	//��ײͣ��
	Config_Error_Mode->set_cashswitchstoperror_ischeck(false);
	//����ͣ��
	Config_Error_Mode->set_obstaclestoperror_ischeck(false);
	//�����
	Config_Error_Mode->set_bobstaclestoperror_ischeck(false);
	//������
	Config_Error_Mode->set_dropstoperror_ischeck(true);
	//���ش���
	Config_Error_Mode->set_currentoverloaderror_ischeck(true);



	ParamConfig_Config_Alarm_Mode * Config_Alarm_Mode = config.mutable_config_alarm_msg();
	//BMSͨ��Ԥ��
	Config_Alarm_Mode->set_bmsmodcomlostalarm_ischeck(true);
	//Э������ģ��Ԥ��
	Config_Alarm_Mode->set_ecanexternsnsorndeoff_ischeck(true);
	//��ͣԤ�����ʹ��
	Config_Alarm_Mode->set_emergency_status_stop_ischeck(true);
	//�������Ԥ��
	Config_Alarm_Mode->set_ctrolboxhightemalarm_ischeck(true);
	//PH3ģ����
	Config_Alarm_Mode->set_ph3readnodeoff_ischeck(true);
	//PH3ģ�鴮��ͨ�ż���
	Config_Alarm_Mode->set_ph3readcomlost_ischeck(true);
	//USBң��ģ��
	Config_Alarm_Mode->set_joytestnodeoff_ischeck(true);
	



	OnlineGroupInit();
	//std::cout << "======Dgv_statues init======"<<std::endl;
	msg_init();
	
	
}

Dgv_statues::~Dgv_statues()
{


}



void Dgv_statues::MyMemclear(const void *src,int count)   
{   
 
    assert(src);   
    while (count--)   
    {   
       *(char *)src=0;   

        src = (char *)src + 1;   
    }   
} 

void Dgv_statues::JoycmdClear(){

	Joycmd_velocity.linear.x = 0.0;
	Joycmd_velocity.linear.y = 0.0;
	Joycmd_velocity.linear.z=  0.0;
	
	// heading driven by left joysticj left and right
	Joycmd_velocity.angular.x = 0.0;
	Joycmd_velocity.angular.y =  0.0;
	Joycmd_velocity.angular.z =  0.0;

}

 void Dgv_statues::print_joymsg(const sensor_msgs::Joy::ConstPtr& joy){
            
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
              
             std::cout<<fmt.str()<<buttonsstr<<axesstr<<std::endl;
             //ROS_INFO("%s %s\n%s",fmt.str().c_str(),buttonsstr.c_str(),axesstr.c_str());
}
void Dgv_statues::laserCmdClear(){

	laserCmd.online = 0;
	laserCmd.twist.linear.x = 0;
	laserCmd.twist.linear.y =0.0;
	laserCmd.twist.linear.z=  0.0;
	// heading driven by left joysticj left and right
	laserCmd.twist.angular.x = 0;
	laserCmd.twist.angular.y =  0.0;
	laserCmd.twist.angular.z =  0.0;
}

 void Dgv_statues::print_laserCmdMsg(const dgvmsg::ctrl_ModeMessage::ConstPtr& msg)
{
	std::cout<<"print_laserCmdMsg:"<<std::endl;
	std::cout<<"laserCmd.online:"<<msg->online<<std::endl;
	std::cout<<"laserCmd.twist.linear.x:"<<msg->twist.linear.x<<std::endl;
	std::cout<<"laserCmd.twist.linear.y:"<<msg->twist.linear.y<<std::endl;
	std::cout<<"laserCmd.twist.angular.x:"<<msg->twist.angular.x<<std::endl;
	
}


int Dgv_statues::IsDriverComError(){
	

            	if(feedback[0].device_status == 0){
					ErrorMsg.set_leftfdrivercomerror(true);
					return 0;
            		}
		else{
			ErrorMsg.set_leftfdrivercomerror(false);
		}
            	if(feedback[2].device_status == 0){
					ErrorMsg.set_leftbdrivercomerror(true);
					return 0;
            		}
		else{
			ErrorMsg.set_leftbdrivercomerror(false);
		}
            	if(feedback[1].device_status == 0){
					ErrorMsg.set_rightfdrivercomerror(true);
					return 0;
            		}
		else{
			ErrorMsg.set_rightfdrivercomerror(false);
		}
            	if(feedback[3].device_status == 0){
					ErrorMsg.set_rightbdrivercomerror(true);
					return 0;
            		}
		else{
			ErrorMsg.set_rightbdrivercomerror(false);
		}

	return 1;
}



int Dgv_statues::IsDriverError(){
	
            	if(feedback[0].errorcode != "OK"){
					ErrorMsg.set_carleftfdrivererror(true);
					return 1;
            		}
		else{
			ErrorMsg.set_carleftfdrivererror(false);
		}
            	if(feedback[2].errorcode != "OK"){
					ErrorMsg.set_carrightfdrivererror(true);
					return 1;
            		}
		else{
			ErrorMsg.set_carrightfdrivererror(false);
		}
            	if(feedback[1].errorcode != "OK"){
					ErrorMsg.set_carleftbdrivererror(true);
					return 1;
            		}
		else{
			ErrorMsg.set_carleftbdrivererror(false);
		}
            	if(feedback[3].errorcode != "OK"){
					ErrorMsg.set_carrightbdrivererror(true);
					return 1;
            		}
		else{
			ErrorMsg.set_carrightbdrivererror(false);
		}
	return 0;
}


int Dgv_statues::IsDriverNodeConnect(){

            	if(driverNode[0].online != 1){
					ErrorMsg.set_driverleftfnodeoff(true);
					return 1;
            		}
		else{
			ErrorMsg.set_driverleftfnodeoff(false);
		}
            	if(driverNode[2].online != 1){
					ErrorMsg.set_driverleftbnodeoff(true);
					return 1;
            		}
		else{
			ErrorMsg.set_driverleftbnodeoff(false);
		}
            	if(driverNode[1].online != 1){
					ErrorMsg.set_driverrightfnodeoff(true);
					return 1;
            		}
		else{
			ErrorMsg.set_driverrightfnodeoff(false);
		}
            	if(driverNode[3].online != 1){
					ErrorMsg.set_driverrightbnodeoff(true);
					return 1;
            		}
		else{
			ErrorMsg.set_driverrightbnodeoff(false);
		}
	return 0;


}


int Dgv_statues::IsCarCash(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();

	if(Config_Error_Mode->cashswitchstoperror_ischeck()==true){
	if(ExternModeInfo.IO_collision[2]==0){
			//������ײ
			ErrorMsg.set_cashswitchstoperror(true);
			return 1;
		}
	else{
			ErrorMsg.set_cashswitchstoperror(false);
		}
		}
	else{
			ErrorMsg.set_cashswitchstoperror(false);
		}
	
	
     return 0;

}
int Dgv_statues::checkCarCasherror(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();

	if(Config_Error_Mode->cashswitchstoperror_ischeck()==true){
		if(ErrorMsg.cashswitchstoperror() == true){
				//������ײ
				
				return 1;
			}
	
		}
     return 0;

}
int Dgv_statues::checkCarObstacleerror(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();
     
	if(Config_Error_Mode->obstaclestoperror_ischeck()==true){
	if(ErrorMsg.obstaclestoperror()==true){
			//����ǰ����
			return 1;
		}
		}
		
     return 0;

}
//�����
int Dgv_statues::checkCarBObstacleerror(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();
     
	if(Config_Error_Mode->bobstaclestoperror_ischeck()==true){
	if(ErrorMsg.bobstaclestoperror() == true){
			//���������
			return 1;
		}
		}
	
     return 0;

}
//��������
int Dgv_statues::checkCarDroperror(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();
     
	if(Config_Error_Mode->dropstoperror_ischeck()==true){
	if(ErrorMsg.dropstoperror()== true){
			//���������
			return 1;
		}
		}
	
     return 0;

}



int Dgv_statues::IsCarObstacle(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();
     
	if(Config_Error_Mode->obstaclestoperror_ischeck()==true){
		//ExternSensor_Mode_PX24ES * Mode_PX24ES = ExternModeInfo.mutable_px24es();
	if((ExternModeInfo.IO_Infrared[LEFTF_OBT]==0)||(ExternModeInfo.IO_Infrared[RIGHTF_OBT]==0)||(ExternModeInfo.PX24ES_out1==0)
		||(ExternModeInfo.PX24ES_out2==0)){
			//����ǰ����
			ErrorMsg.set_obstaclestoperror(true);
			return 1;
		}
	else{
			ErrorMsg.set_obstaclestoperror(false);
		}
		}
	else{
			ErrorMsg.set_obstaclestoperror(false);
		}
	
	
     return 0;

}

//�����
int Dgv_statues::IsCarBObstacle(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();

	 
	if(Config_Error_Mode->bobstaclestoperror_ischeck()==true){
	if((ExternModeInfo.IO_Infrared[LEFTB_OBT]==0)||(ExternModeInfo.IO_Infrared[RIGHTB_OBT]==0)){
			//���������
		
			ErrorMsg.set_bobstaclestoperror(true);
			return 1;
		}
	else{
			ErrorMsg.set_bobstaclestoperror(false);
		}
		}
	else{
			ErrorMsg.set_bobstaclestoperror(false);
		}
	
	
     return 0;

}

//����������
int Dgv_statues::IsCarDropStop(){

	ParamConfig_Config_Error_Mode * Config_Error_Mode = config.mutable_config_error_msg();

	 
	if(Config_Error_Mode->dropstoperror_ischeck() == true){
	if((ExternModeInfo.IO_Infrared[LEFTF_DROP]==1)||(ExternModeInfo.IO_Infrared[RIGHTF_DROP]==1)){
			//����������
		
			ErrorMsg.set_dropstoperror(true);
			return 1;
		}
	else{
			ErrorMsg.set_dropstoperror(false);
		}
		}
	else{
			ErrorMsg.set_dropstoperror(false);
		}
	
	
     return 0;

}

int Dgv_statues::IsCarEmergencyStop(){
	ParamConfig_Config_Alarm_Mode * Config_Alarm_Mode = config.mutable_config_alarm_msg();
	
     
	if(Config_Alarm_Mode->emergency_status_stop_ischeck()==true){
		
            	if(feedback[0].emergencyinputvalue != false){
					AlarmMsg.set_emergency_status_stop(true);
					return 1;
            		}

            	if(feedback[1].emergencyinputvalue != false){
					AlarmMsg.set_emergency_status_stop(true);
					return 1;
            		}

            	if(feedback[2].emergencyinputvalue != false){
					AlarmMsg.set_emergency_status_stop(true);
					return 1;
            		}
	
            	if(feedback[3].emergencyinputvalue != false){
					AlarmMsg.set_emergency_status_stop(true);
					return 1;
            		}
		AlarmMsg.set_emergency_status_stop(false);
		}
	else{
			AlarmMsg.set_emergency_status_stop(false);
		}
	
	
     return 0;

}

int Dgv_statues::CheckAlarm_JoyTestNodeOFF(){
	ParamConfig_Config_Alarm_Mode * Config_Alarm_Mode = config.mutable_config_alarm_msg();
	if(Config_Alarm_Mode->joytestnodeoff_ischeck() == true){
		if(USBJostickNode.online==0){
				AlarmMsg.set_joytestnodeoff(true);
			}
		else{
				AlarmMsg.set_joytestnodeoff(false);
			}

		}

}
int Dgv_statues::CheckAlarm_EcanExternSnsorNdeOFF(){
	ParamConfig_Config_Alarm_Mode * Config_Alarm_Mode = config.mutable_config_alarm_msg();
	if(Config_Alarm_Mode->ecanexternsnsorndeoff_ischeck()== true){

		if(ExternSnsorNode.online == 0){
				AlarmMsg.set_ecanexternsnsorndeoff(true);
			}
		else{
				AlarmMsg.set_ecanexternsnsorndeoff(false);
			}

		}
}
int Dgv_statues::CheckAlarm_CtrolBoxHighTem(){
	ParamConfig_Config_Alarm_Mode * Config_Alarm_Mode = config.mutable_config_alarm_msg();
	if(Config_Alarm_Mode->ctrolboxhightemalarm_ischeck()== true){
		try{
			//if(ExternModeInfo.temp_humi_size()>=2){
				//ExternSensor_Mode_Temp_humi * 	Mode_Temp_humi = ExternModeInfo.mutable_temp_humi(0);
				if(ExternModeInfo.interior_temp[0]>70){
						AlarmMsg.set_ctrolboxhightemalarm(true);
					}
				else{
						AlarmMsg.set_ctrolboxhightemalarm(false);
					}
				//}
					}
		catch(std::exception& e){
				std::cerr << e.what() << std::endl;
				std::cout << "======CheckAlarm_CtrolBoxHighTem======"<<std::endl;
			}
		}

}
int Dgv_statues::CheckAlarm_BMSModComLostAlarm(){
	ParamConfig_Config_Alarm_Mode * Config_Alarm_Mode = config.mutable_config_alarm_msg();
	if(Config_Alarm_Mode->bmsmodcomlostalarm_ischeck()== true){
			if(BmsModeInfo.Stat == 0){
					AlarmMsg.set_bmsmodcomlostalarm(true);
				}
			else{
					AlarmMsg.set_bmsmodcomlostalarm(false);
				}


		}

}

void Dgv_statues::CheckAlarm_LaserComLost(){

			if(laserCmd.online==0){
					AlarmMsg.set_lasercomlost(true);
				}
			else{
					AlarmMsg.set_lasercomlost(false);

				}
}
void Dgv_statues::CheckAlarm_LaserNodeOFF(){

			if(LaserCrelNode.online==0){
					AlarmMsg.set_lasernodeoff(true);
				}
			else{
					AlarmMsg.set_lasernodeoff(false);

				}
}

void Dgv_statues::CheckError_BmsCom(){
	if(BmsModeInfo.Stat == 0){
			ErrorMsg.set_bmssyserror(true);
		}
	else{

			ErrorMsg.set_bmssyserror(false);
		}

}

void Dgv_statues::CheckError_ExternCom(){
	
	if(ExternModeInfo.online == 0){
			ErrorMsg.set_externcomdeverror(true);
		}
	else{

			ErrorMsg.set_externcomdeverror(false);
		}

}
void Dgv_statues::CheckError_CurrentOverload(){
	
	if(BmsModeInfo.BMS_sys_Current<=-50){
			ErrorMsg.set_currentoverloaderror(true);
		}
	else{

			ErrorMsg.set_currentoverloaderror(false);
		}

}

void Dgv_statues::CheckError(){
	
	IsDriverComError();
	IsDriverError();
	//IsCarCash();
	//IsCarBObstacle();
	//IsCarObstacle();
	IsDriverNodeConnect();
	CheckError_BmsCom();
	//CheckError_ExternCom();
	//CheckError_CurrentOverload();
	

}
void Dgv_statues::CheckExternError(){
	
	//IsCarCash();
	IsCarBObstacle();
	IsCarObstacle();
	IsCarDropStop();
	
	CheckError_ExternCom();
	CheckError_CurrentOverload();
	
	CheckAlarm_CtrolBoxHighTem();
}
void Dgv_statues::CheckAlarm(){

	CheckAlarm_BMSModComLostAlarm();

	 CheckAlarm_JoyTestNodeOFF();
	 
	 CheckAlarm_EcanExternSnsorNdeOFF();
	 IsCarEmergencyStop();

	 CheckAlarm_LaserComLost();
	 CheckAlarm_LaserNodeOFF();
	 
	 
	
}
void Dgv_statues::CheckOnline(){
	LaserCrelNode.check_connect();
	ExternSnsorNode.check_connect();
	BmsSysNode.check_connect();
	

	ZmqTransNode.check_connect();

	RemoteNode.check_connect();
	
	USBJostickNode.check_connect();
	CenterNode.check_connect();
	
	
}

void Dgv_statues::OnlineGroupInit(){
	LaserCrelNode.online=0;
	LaserCrelNode.Set_timeout(2);

	ExternSnsorNode.online = 0;
	ExternSnsorNode.Set_timeout(2);

	BmsSysNode.online=0;
	BmsSysNode.Set_timeout(5);
	


	ZmqTransNode.online=0;
	ZmqTransNode.Set_timeout(2);

	RemoteNode.online=0;
	RemoteNode.Set_timeout(2);

	USBJostickNode.online=0;
	USBJostickNode.Set_timeout(3);

	CenterNode.online;
	CenterNode.Set_timeout(2);
	
	
}
/*
void Dgv_statues::Init_ExternModeInfo(){
	
	// 4·PT100
	ExternModeInfo.add_temp_pt100(255);
	ExternModeInfo.add_temp_pt100(255);
	ExternModeInfo.add_temp_pt100(255);
	ExternModeInfo.add_temp_pt100(255);

	// 2· ��ʪ��
	ExternSensor_Mode_Temp_humi  * pMode_Temp_humi = 
		ExternModeInfo.add_temp_humi();
	pMode_Temp_humi->Clear();
	pMode_Temp_humi->set_interior_temp(255);
	pMode_Temp_humi->set_interior_humi(101);

	pMode_Temp_humi = ExternModeInfo.add_temp_humi();
	pMode_Temp_humi->Clear();
	pMode_Temp_humi->set_interior_temp(255);
	pMode_Temp_humi->set_interior_humi(101);
	
	// 2 ·����״̬
	ExternSensor_Mode_fan * pMode_fan = 
		ExternModeInfo.add_fan_msg();
	pMode_fan->set_fan_id(1);
	pMode_fan->set_fan_mode(1); //	Ĭ�Ϲ������Զ�ģʽ��
	pMode_fan->set_fan_status(0);  // Ĭ�� close
	pMode_fan->set_vaule_close(20);
	pMode_fan->set_vaule_open(35);

	pMode_fan = ExternModeInfo.add_fan_msg();
	pMode_fan->set_fan_id(1);
	pMode_fan->set_fan_mode(1); //	Ĭ�Ϲ������Զ�ģʽ��
	pMode_fan->set_fan_status(0);  // Ĭ�� close
	pMode_fan->set_vaule_close(20);
	pMode_fan->set_vaule_open(35);	

	//Ԥ��8·�����䣬ʵ��ʹ��ǰ4·
	ExternModeInfo.add_io_infrared(0);
	ExternModeInfo.add_io_infrared(0);
	ExternModeInfo.add_io_infrared(0);
	ExternModeInfo.add_io_infrared(0);
	
	ExternModeInfo.add_io_infrared(0);
	ExternModeInfo.add_io_infrared(0);
	ExternModeInfo.add_io_infrared(0);
	ExternModeInfo.add_io_infrared(0);

	//Ԥ��8·����ײ��ʵ�ʲ�ʹ��
	ExternModeInfo.add_io_collision(0);
	ExternModeInfo.add_io_collision(0);
	ExternModeInfo.add_io_collision(0);
	ExternModeInfo.add_io_collision(0);

	ExternModeInfo.add_io_collision(0);
	ExternModeInfo.add_io_collision(0);
	ExternModeInfo.add_io_collision(0);
	ExternModeInfo.add_io_collision(0);

	// 4·������״̬ ʵ��ȫ��ʹ��
	//SensorMsg.add_distance_ultrasonic(1200);
	//SensorMsg.add_distance_ultrasonic(1200);
	//SensorMsg.add_distance_ultrasonic(1200);
	//SensorMsg.add_distance_ultrasonic(1200);
	
	//��ɫָʾ��״̬
	//"LED_CLOSE" "LED_GREEN" "LED_RED" "LED_BLUE" "LED_GREEN_FLI" "LED_RED_FLI" "LED_BLUE_FLI"
	ExternModeInfo.set_led_status("LED_CLOSE");

	// 1·���±��ϴ�����
	ExternSensor_Mode_PX24ES * pMode_PX24ES =ExternModeInfo.mutable_px24es();
	pMode_PX24ES->set_out1(0);
	pMode_PX24ES->set_out2(0);
	pMode_PX24ES->set_extraneouslight_out(0);

}
*/


void Dgv_statues::msg_init(){

		//statues.Remotestdmsg.linear.x = (msg->linear.x-1704)/10;
		//statues.Remotestdmsg.linear.y =  (msg->linear.y-1897)/10;
		//statues.Remotestdmsg.linear.x = msg->linear.x;   //���ҷ���
		//statues.Remotestdmsg.linear.y = msg->linear.y;   //���·���
		//statues.Remotestdmsg.linear.z = msg->linear.z;

		//statues.Remotestdmsg.angular.x = (msg->angular.x-1724)/10;
		//statues.Remotestdmsg.angular.y =  (msg->angular.y-1903)/10;
	Remotestdmsg.online = 0;
	
	Remotestdmsg.linear.x=0;
	Remotestdmsg.linear.y=0;
	Remotestdmsg.linear.z=0;
	
	Remotestdmsg.angular.x=0;
	Remotestdmsg.angular.y=0;
	Remotestdmsg.angular.z=0;
	
	Remotestdmsg.IOSTATUES1.resize(8);
	Remotestdmsg.IOSTATUES2.resize(8);
	Remotestdmsg.buttons.resize(8);

}

std::string Dgv_statues::pack_all_configinfo(){


	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION; 
	
       std::string res;
	if (!config.SerializeToString(&res)  ) {
	           std::cerr << "Failed to write msg  ParamConfig. pack_all_configinfo" << std::endl; 
	           return ""; 
	       }	

    return res;   	

}
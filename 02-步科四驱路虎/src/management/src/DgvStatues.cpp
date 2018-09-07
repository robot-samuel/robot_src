

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
			Dirver[index].Clear();
		          //���л����ڴ�
		        std::ostringstream stream;
		        Dirver[index].SerializeToOstream(&stream);

		        std::string text = stream.str();
			strDirver[index]=text;
		}
	

	//laserCmdClear();
	

	ZmqTransNode.online = 0;

	joystick_count = 0;
	joyconnect = 0;
	JoycmdClear();

	center_isconnect = 0;
	center_count = 0;

	User232connect = 0;
	User232_count = 0;
	UartCmdClear();
	
	Remoteconnect = 0;
	Remote_count = 0;

	OnlineGroupInit();
	
	
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

void Dgv_statues::OnlineGroupInit(){

	//zmq通信节点
	ZmqTransNode.online=0;
	ZmqTransNode.Set_timeout(2);

	//操控手柄
	RemoteNode.online=0;
	RemoteNode.Set_timeout(2);

	//USB遥控器节点
	USBJostickNode.online=0;
	USBJostickNode.Set_timeout(3);

	//网络用户节点
	CenterNode.online;
	CenterNode.Set_timeout(2);
	
	//232用户节点
	User232Node.online;
	User232Node.Set_timeout(2);	
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

void Dgv_statues::UartCmdClear()
{
  
	UartCmdMsg.online = 0;
	UartCmdMsg.twist.linear.x = 0;
	UartCmdMsg.twist.linear.y =0.0;
	UartCmdMsg.twist.linear.z=  0.0;
	// heading driven by left joysticj left and right
	UartCmdMsg.twist.angular.x = 0;
	UartCmdMsg.twist.angular.y =  0.0;
	UartCmdMsg.twist.angular.z =  0.0;
	
}

 void Dgv_statues::print_laserCmdMsg(const dgvmsg::ctrl_ModeMessage::ConstPtr& msg)
{
	std::cout<<"print_laserCmdMsg:"<<std::endl;
	std::cout<<"laserCmd.online:"<<msg->online<<std::endl;
	std::cout<<"laserCmd.twist.linear.x:"<<msg->twist.linear.x<<std::endl;
	std::cout<<"laserCmd.twist.linear.y:"<<msg->twist.linear.y<<std::endl;
	std::cout<<"laserCmd.twist.angular.x:"<<msg->twist.angular.x<<std::endl;
	
}
void Dgv_statues::print_UartCmdMsg(const dgvmsg::ctrl_ModeMessage::ConstPtr& msg)
{
	std::cout<<"print_UartCmdMsg:"<<std::endl;
	std::cout<<"UartCmd.online:"<<msg->online<<std::endl;
	std::cout<<"UartCmd.twist.linear.x:"<<msg->twist.linear.x<<std::endl;
	std::cout<<"UartCmd.twist.linear.y:"<<msg->twist.linear.y<<std::endl;
	std::cout<<"UartCmd.twist.angular.x:"<<msg->twist.angular.x<<std::endl;

}

int Dgv_statues::is_CanContenct(){
	/*
	for(int index=0;index<DRIVER_AMOUNT;index++){
	if(driverNode[index].online == 0){
			std::cout<<"is_CanContenct ="<<index<<" driverNode =" <<driverNode[index].online<<std::endl;
			return 0;
		}
		}
		*/
	return 1;
}
int Dgv_statues::is_DriverOK(){
	for(int index=0;index<DRIVER_AMOUNT;index++){
	if(Dirver[index].error()!="OK"){
			std::cout<<"is_DriverOK ="<<index<<" Dirver =" <<Dirver[index].error()<<std::endl;
			return 0;
		}
		}
	return 1;
}


 


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




	joystick_count = 0;
	joyconnect = 0;
	JoycmdClear();

	center_isconnect = 0;
	center_count = 0;


	DriverStatuesInit();
	
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
void Dgv_statues::DriverStatuesInit()
{

	 
	Driverstatues.device_status = 0;
	
	Driverstatues.encounter.resize(2);
	Driverstatues.encounter[0] = 0;
	Driverstatues.encounter[1] = 0;
	
	Driverstatues.Motor_Status.resize(2);
	Driverstatues.Motor_Status[0] = 0;
	Driverstatues.Motor_Status[1] = 0;

	Driverstatues.errorcode.resize(2);
	Driverstatues.errorcode[0] = 0;
	Driverstatues.errorcode[1] = 0;

	Driverstatues.Obt_dis.resize(4);
	Driverstatues.Obt_dis[0] = 0;
	Driverstatues.Obt_dis[1] = 0;
	Driverstatues.Obt_dis[2] = 0;
	Driverstatues.Obt_dis[3] = 0;
	
	Driverstatues.fcrash_io = ((0>>0)&0x01);
	Driverstatues.bcrash_io = ((0>>1)&0x01);
	
	Driverstatues.Obt_en_Status.resize(4);
	Driverstatues.Obt_en_Status[0] = ((0>>2)&0x01);
	Driverstatues.Obt_en_Status[1] = ((0>>3)&0x01);
	Driverstatues.Obt_en_Status[2] = ((0>>4)&0x01);
	Driverstatues.Obt_en_Status[3] = ((0>>5)&0x01);
	Driverstatues.fObt_en_Status   = ((0>>6)&0x01);
	Driverstatues.bObt_en_Status  = ((0>>7)&0x01);

	
	
	Driverstatues.LMotor_com_Status  = ((0>>0)&0x01);
	Driverstatues.RMotor_com_Status  = ((0>>1)&0x01);
	Driverstatues.Obt_com_Status.resize(4);
	Driverstatues.Obt_com_Status[0]  = ((0>>4)&0x01);
	Driverstatues.Obt_com_Status[1]  = ((0>>5)&0x01);
	Driverstatues.Obt_com_Status[2]  = ((0>>6)&0x01);
	Driverstatues.Obt_com_Status[3]  = ((0>>7)&0x01);



	Driverstatues.input_IO_Status.resize(4);
	Driverstatues.input_IO_Status[0]  = ((0>>0)&0x01);
	Driverstatues.input_IO_Status[1]  = ((0>>1)&0x01);
	Driverstatues.input_IO_Status[2]  = ((0>>2)&0x01);
	Driverstatues.input_IO_Status[3]  = ((0>>3)&0x01);

	Driverstatues.out_IO_Status.resize(4);
	Driverstatues.out_IO_Status[0]  = ((0>>4)&0x01);
	Driverstatues.out_IO_Status[1]  = ((0>>5)&0x01);
	Driverstatues.out_IO_Status[2]  = ((0>>6)&0x01);
	Driverstatues.out_IO_Status[3]  = ((0>>7)&0x01);
	
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

void Dgv_statues::CheckError(){

	

}
void Dgv_statues::CheckExternError(){
	

}
void Dgv_statues::CheckAlarm(){


	 
	 
	
}
void Dgv_statues::CheckOnline(){

	
	USBJostickNode.check_connect();
	CenterNode.check_connect();
	
	
}

void Dgv_statues::OnlineGroupInit(){


	USBJostickNode.online=0;
	USBJostickNode.Set_timeout(3);

	CenterNode.online;
	CenterNode.Set_timeout(2);
	
	
}


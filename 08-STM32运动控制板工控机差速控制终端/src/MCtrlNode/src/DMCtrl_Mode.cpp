

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream> 

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>


#include "std_msgs/String.h"
#include "dgvmsg/ctrl_ModeMessage.h"
#include "dgvmsg/Encounter.h"
#include "dgvmsg/EncounterV.h"

#include "mbcrc.h"
#include "DMCtrl_Mode.hpp"

class MAsyncSerialImpl
{
public:
    CallbackAsyncSerial serial;
    std::string receivedData;
};


DMCtrl_Mode::DMCtrl_Mode(std::string& port, unsigned int rate)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;
	onlineMsg.online=0;
	pimpl->receivedData.clear();


	param_init();
	

	
}

DMCtrl_Mode::~DMCtrl_Mode()
{
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
void DMCtrl_Mode::open(std::string devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname,baudrate);
    } catch(boost::system::system_error&)
    {
        //Errors during open
    }
    pimpl->serial.setCallback(bind(&DMCtrl_Mode::readCallback,this, _1, _2));
}

void DMCtrl_Mode::close()
{
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(boost::system::system_error&)
    {
        //Errors during port close
    }
    pimpl->receivedData.clear();//Clear eventual data remaining in read buffer
}

bool DMCtrl_Mode::isOpen()
{
    return pimpl->serial.isOpen();
}

bool DMCtrl_Mode::errorStatus()
{
    return pimpl->serial.errorStatus();
}

void DMCtrl_Mode::write(std::string data)
{
    pimpl->serial.writeString(data);
}

void DMCtrl_Mode::readCallback(const char *data, size_t size)
{
    //pimpl->receivedData+=QString::fromAscii(data,size);
        std::string res;
        res.clear();
        res.assign(data,size);
        pimpl->receivedData+=res;
	#if 0
	print_hex(pimpl->receivedData);
	#endif
	

}

void DMCtrl_Mode::DMCtrl_Mode_master()
{
	
             int count=0;
	      ros::Rate loop_rate(10);
		 //std::string str="wait for recv";
		//write(str);
             while(ros::ok())
                    {
                   
			loop_rate.sleep();
			    
                    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

                    }
	
}


void DMCtrl_Mode::param_init()
{

	speed = 0;
	angel  = 0;

	leftrpm = 0;
	rightrpm = 0;
	
	leftstatues = CLOSE_STOP;
	rightstatues= CLOSE_STOP;
	
	en_obt_front = false;
	en_obt_back = false;
	
	en_Crash_front = true;
	en_Crash_back = true;

	set_vau_out1 = false;
	set_vau_out2 = false;
	set_vau_out3 = false;
	set_vau_out4 = false;

	set_vau_real1 = false;
	set_vau_real2 = false;

	F_ObtDis = 50;
	B_ObtDis = 50;;

}
char DMCtrl_Mode::TC_Cmd_Ctrl( )
{
	char ctrl=0;
	 if(en_Crash_front){
	      ctrl |= (0X01<<0);
	  }
	  else{
	      ctrl &= (~0X01);
	  }
	  
	 if(en_Crash_back){
	      ctrl |= (0X01<<1);
	  }
	  else{
	       ctrl &= (~(0X01<<1));
	  }
	  
	 if(en_obt_front){
	      ctrl |= (0X01<<2);
	  }
	  else{
	      ctrl &= (~(0X01<<2));
	  }

	 if(en_obt_back){
	      ctrl |= (0X01<<3);
	  }
	  else{
	      ctrl &= (~(0X01<<3));
	  }

	 if(set_vau_out1){
	      ctrl |= (0X01<<4);
	  }
	  else{
	      ctrl &= (~(0X01<<4));
	  }

	 if(set_vau_out2){
	      ctrl |= (0X01<<5);
	  }
	  else{
	      ctrl &= (~(0X01<<5));
	  }

	 if(set_vau_out3){
	      ctrl |= (0X01<<6);
	  }
	  else{
	      ctrl &= (~(0X01<<6));
	  }

	 if(set_vau_out4){
	      ctrl |= (0X01<<7);
	  }
	  else{
	      ctrl &= (~(0X01<<7));
	  }

	return   ctrl;
}

std::string  DMCtrl_Mode::TC_Cmd_package( short int leftrpm,short int rightrpm)
{
	
	boost::array<char,nomal_length> data;
	short int Data1=leftrpm;
	short int Data2=rightrpm;
	
	data.assign(0);
	
	data[0] = HEAD_H;
	data[1] = HEAD_L;

	
	data[2] = TC_Cmd_Ctrl();

	 char MStautes = 0;
	if(leftrpm!=0){
		 MStautes |= (0X01<<0);
		}
	else{
		 MStautes &= (~(0X03<<0));
		}
	if(rightrpm!=0){
		 MStautes |= (0X01<<2);
		}
	else{
		 MStautes &= (~(0X03<<2));
		}
	
	
	data[3] = MStautes;

	  short int LFspeed = leftrpm;
	  if(LFspeed>3500){
	      LFspeed = 3500;
	  }
	  else if (LFspeed<-3500){
	       LFspeed = -3500;
	  }
	  
	  char * ps1  = (char *)(&LFspeed);
	  data[4] = * (ps1+1);
	  data[5] = * (ps1+0);


	  short int RFspeed = rightrpm;
	  if(RFspeed>3500){
	      RFspeed = 3500;
	  }
	  else if (RFspeed<-3500){
	       RFspeed = -3500;
	  }
	  
	  char * ps1  = (char *)(&RFspeed);
	  data[6] = * (ps1+1);
	  data[7] = * (ps1+0);	  
	  
    	std::string res(data.begin(),data.end());
    	return res;
	
}
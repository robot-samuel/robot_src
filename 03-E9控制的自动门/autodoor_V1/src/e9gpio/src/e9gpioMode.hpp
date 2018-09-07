#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>



#include "e9gpio.h"


#include "zmqclient.h"

#include "zmqclient.h"
//#include "message.hpp"
#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "dgvmsg/E9gpioCtrl.h"
#include "dgvmsg/E9gpioS.h"


using namespace std;
using namespace boost;


#define ROSIN 1
#define MAXSPEEDRPM 32768


class E9GpioMode
{
    

private:
    E9GpioMode(const E9GpioMode& rhs);
    E9GpioMode & operator=(const E9GpioMode& rhs);
public:
	enum SWITCHB
	{
	    FLIGHT=0,
	    BLIGHT=1,
	};
public:

    E9GpioMode();	
    ~E9GpioMode();

	void E9GpioMode_master();

	
	void print_dec(unsigned char *datBUF,int Num);
	void print_hex(unsigned char *datBUF,int Num);
	void print_hex(std::string str);	

	bool E9GpioMode_init_dir();
	bool E9GpioMode_init_outvau();
	bool E9GpioMode_test_outvau(int vau);
	void  getinput();
	void  check_obt();



	
	
	ros::Publisher rospub_obt;
	void ros_rospub_obt_SendRpc();
	void gpio_cmdCall(const dgvmsg::E9gpioCtrl::ConstPtr& msg);
	bool E9GpioMode_Setoutvau(int cmd);
	
private:
	
	E9Gpio Gpio20;
	E9Gpio Gpio86;
	E9Gpio Gpio30;
	E9Gpio Gpio85;

	E9Gpio Gpio94;
	E9Gpio Gpio27;
	E9Gpio Gpio28;
	E9Gpio Gpio80;

	int intput1;
	int intput2;
	int intput3;
	int intput4;

	int recvcmd;
	int doorstatues;
	int deley;



	
	
};






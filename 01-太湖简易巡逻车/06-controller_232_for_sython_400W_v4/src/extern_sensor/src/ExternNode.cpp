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

#include <boost/date_time/posix_time/posix_time.hpp>

#include "ros/ros.h"
#include "ros/console.h"

#include "std_msgs/String.h"
#include "dgvmsg/ExternSensor.h"

#include "AsyncSerial.h"
#include "ExternMode.hpp"
#include "zmqclient.h"



int main(int argc, char **argv)
{
   

	ros::init(argc, argv, "ExternNode");
	//参数打印
	std::cout<<"argc= "<<argc<<std::endl;
	for(int index=0;index<argc;index++){
			std::cout<<"argv"<<"["<<index<<"]= "<<argv[index]<<std::endl;
		}
	std::string argv_add=argv[1];
	std::string argv_com=argv[2];
	std::string argv_bandrate=argv[3];
	std::string argv_dir=argv[4];

	std::string   COM=argv_com;
	int badrate=115200;
	try{  
		    badrate=boost::lexical_cast<int>(argv_bandrate); // 将字符串转化为整数  
		}  
	catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		{  
		        badrate=115200;
		} 
	
	ExternMode ExternDev(COM,badrate);
		
	boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

	// pub 
	ExternDev.Externpub = handel->advertise<dgvmsg::ExternSensor>("ExternInfo", 10);

	//sub
	ros::Subscriber rossub_Extern_sensor =
	handel->subscribe("extern_sensor/cmd", 10,  &ExternMode::cmdCallback,&ExternDev);
		
	boost::thread t1(boost::bind(&ExternMode::ExternMode_master,&ExternDev));  

	      
	ros::spin();
	t1.join();

	return 0;


}






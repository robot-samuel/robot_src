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


#include "AsyncSerial.h"
#include "DMCtrl_Mode.hpp"
#include "zmqclient.h"



int main(int argc, char **argv)
{
	ros::init(argc, argv, "MCtrlNode");



	std::cout<<"argc= "<<argc<<std::endl;
	for(int index=0;index<argc;index++){
			std::cout<<"argv"<<"["<<index<<"]= "<<argv[index]<<std::endl;
		}

	std::string argv_com=argv[1];
	std::string argv_band=argv[2];

	if(argc != 3){
		std::cout<<"argc  wrong ! "<<std::endl;
		return 0;
		}

	std::string  COM=argv_com;
	if((COM==DCOM1)||(COM==DCOM2)||(COM==DCOM3)||(COM==DCOM4)){
			
		}
	else{
		std::cout<<"argv_com  wrong ! "<<std::endl;
		return 0;
		}
	int badrate=115200;
	try{  
		    badrate=boost::lexical_cast<int>(argv_band); // 将字符串转化为整数
		    if((badrate != 115200 )&&(badrate != 38400 )&&(badrate != 9600 )){
			std::cout<<"badrate wrong ,only can be 115200 38400  9600! "<<std::endl;
				return 0;
		    	}
		}  
	catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		{  
		        badrate=115200;
			std::cout<<"badrate  bad_lexical_cast wrong ! "<<std::endl;
			return 0;
		}
	
	DMCtrl_Mode LCCom(COM,badrate);
		
	boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

	// pub 
	//LCCom.LCCompub = handel->advertise<dgvmsg::ctrl_ModeMessage>("CtrlCmd", 1);
	
	LCCom.LCEncounterpub = handel->advertise<dgvmsg::EncounterV>("DriverEncounterS", 10);
	LCCom.LCCompub = handel->advertise<dgvmsg::DriverStatues>("FDriverStatues", 10);

	
	//控制命令
	ros::Subscriber rossub_mode_driver =
	handel->subscribe("Net2Ctrl", 10,  &DMCtrl_Mode::Net2Ctrl_driverCall,&LCCom);
	ROS_INFO("  Initialization rossub_mode_driver Net2Ctrl port  is ok");


	//crate main thread
	boost::thread t1(boost::bind(&DMCtrl_Mode::DMCtrl_Mode_master,&LCCom));  
	
	ros::spin();
	t1.join();

	return 0;


}






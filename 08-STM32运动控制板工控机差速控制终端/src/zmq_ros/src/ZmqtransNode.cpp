
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
#include "cppzmq-master/zmq.hpp"





//#include <zmq.hpp>

#include "ros/ros.h"
#include "ros/console.h"

#include "std_msgs/String.h"


#include "zmqclient.h"
#include "zmqclient_class.hpp"





int main(int argc, char **argv)
{
   

              ros::init(argc, argv, "ZmqTransNode");
              GOOGLE_PROTOBUF_VERIFY_VERSION;

              //std::string pubadd="tcp://192.168.1.90:1234";
              //std::string subadd="tcp://192.168.1.90:5678";

              //std::string pubadd="tcp://192.168.3.210:1234";
             // std::string subadd="tcp://192.168.3.210:5678";

              std::string pubadd="tcp://192.168.3.80:1234";
              std::string subadd="tcp://192.168.3.80:5678";

  
                ROS_INFO("ros subadd =%s", subadd.c_str());
                ROS_INFO("ros pubadd =%s", pubadd.c_str());
                zmqclient roszmq( pubadd,subadd);
                roszmq.print_zmq_version();
                roszmq.insert_topic(ZMQ_NAME_DRIVERCTRL);
                roszmq.insert_topic(MYGEARTRpcTopic);
                roszmq.connect();

               boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

		// pub cmd that is from PC
               roszmq.rospub = handel->advertise<std_msgs::String>("zmqcmd", 30);

	        //sub cmd return or uodate cmd from driverctrol
               ros::Subscriber sub1 = handel->subscribe("zmqreturn" ,10,&zmqclient::dirv_zmq_Callback,&roszmq);
               ros::Subscriber sub2 = handel->subscribe("dirv_Dcupdataall", 50,  &zmqclient::dirv_update_Callback,&roszmq);

               boost::thread t1(boost::bind(&zmqclient::zmqrecvLoop,&roszmq));   
		boost::thread t2(boost::bind(&zmqclient::zmqpubtask,&roszmq));  
             
              ros::spin();
              t1.join();
	       t2.join();
             // roszmq.destory();
           // google::protobuf::ShutdownProtobufLibrary();
              return 0;


}









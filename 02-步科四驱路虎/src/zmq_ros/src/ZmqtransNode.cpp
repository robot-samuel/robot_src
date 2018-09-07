
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
              // GOOGLE_PROTOBUF_VERIFY_VERSION;
              //std::string pubadd="tcp://localhost:6665";
              //std::string subadd="tcp://localhost:6666";
              //std::string pubadd="tcp://192.168.1.60:1234";
              //std::string subadd="tcp://192.168.1.60:5678";
               std::string pubadd;
               std::string subadd;

                //获取外外部参数
                {
                   //get param
                  ros::NodeHandle nparam;  
                  if(nparam.hasParam("/ZMQADD/SUB")){
                        nparam.getParam("/ZMQADD/SUB", subadd);
                    }
                  else{
                            //subadd = "tcp://192.168.1.60:5678";
							subadd = "tcp://localhost:2016";
                            nparam.setParam("/ZMQADD/SUB", subadd);
                    }
                  if(nparam.hasParam("/ZMQADD/PUB")){
                        nparam.getParam("/ZMQADD/PUB", pubadd);
                    }
                  else{
                           //pubadd = "tcp://*:2015";
							pubadd = "tcp://localhost:2015";
                            nparam.setParam("/ZMQADD/PUB", pubadd);
                    }
			//强制链接本地的转发中心
                  // subadd = "tcp://localhost:6666";
                  // pubadd = "tcp://localhost:6665";
                   
                   ROS_INFO("ros subadd =%s", subadd.c_str());
                   ROS_INFO("ros pubadd =%s", pubadd.c_str());

                
                }
                
                zmqclient roszmq( pubadd,subadd);
                roszmq.print_zmq_version();
                roszmq.insert_topic(ZMQ_NAME_DRIVERCTRL);
                roszmq.insert_topic(MYGEARTRpcTopic);
                roszmq.connect();

               boost::shared_ptr< ros::NodeHandle> handel(new ros::NodeHandle); 

		// pub cmd that is from PC
               roszmq.rospub = handel->advertise<std_msgs::String>("zmqcmd", 10);

	        //sub cmd return or uodate cmd from driverctrol
               ros::Subscriber sub1 = handel->subscribe("zmqreturn" ,10,&zmqclient::dirv_zmq_Callback,&roszmq);
               ros::Subscriber sub2 = handel->subscribe("dirv_Dcupdataall", 10,  &zmqclient::dirv_update_Callback,&roszmq);

               boost::thread t1(boost::bind(&zmqclient::zmqrecvLoop,&roszmq));   
		boost::thread t2(boost::bind(&zmqclient::zmqpubtask,&roszmq));  
             
              ros::spin();
              t1.join();
	       t2.join();
              roszmq.destory();
           // google::protobuf::ShutdownProtobufLibrary();
              return 0;


}









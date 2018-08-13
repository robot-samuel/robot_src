


#include <algorithm>
#include <fstream>
#include <iostream>  
#include <sstream> 
#include <string> 

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


#include "pantiltctrlMode.hpp"

#define  DEBUG_ECHO 0
using  namespace boost;
using namespace boost::uuids;

pantiltctrlMode::pantiltctrlMode()
{

	onlineMsg.online=0;
	onlineMsg.Set_timeout(5);	
	Remoteconnect = NO;
	Remote_count = 0;
	Old_Remoteconnect = NO;
	   stopUD_counter = 0;
	  stopRL_counter = 0;
	
}

pantiltctrlMode::~pantiltctrlMode()
{
  
}

void pantiltctrlMode::pantiltctrlMode_master()
{
	
       std::cout<< "hello pantiltctrlMode_master "<<std::endl;
			
			
         try{
		    ros::Rate loop_rate(3);
                    while(ros::ok()){
			onlineMsg.check_connect();
			check_connect();
			if(Remoteconnect== YES){

				//左右调整
				
				if(((fabs(Remotestdmsg.angular.x)>10 )||(fabs(Remotestdmsg.linear.y)>10))&&(Remotestdmsg.IOSTATUES2[4] == 0)){

					if(fabs(Remotestdmsg.angular.x)>10){
						#if DEBUG_ECHO
						std::cout<< "hello pantiltctrlMode_master : -> packdata_LEFT right"<<std::endl;
						#endif
						if(Remotestdmsg.angular.x>0){
								std::string cmd = packdata_right((((int)fabs(Remotestdmsg.angular.x))));
								ros_SendRpc(cmd);
								//stopUD_counter=0;
							}
						else{
								std::string cmd = packdata_Left((((int)fabs(Remotestdmsg.angular.x))));
								ros_SendRpc(cmd);
								// stopUD_counter=0;
							}
						}
					if(fabs(Remotestdmsg.linear.y)>10){
						#if DEBUG_ECHO
						std::cout<< "hello pantiltctrlMode_master : -> packdata_UP DOWN"<<std::endl;
						#endif
						if(Remotestdmsg.linear.y>0){
							std::string cmd = packdata_DOWN((((int)fabs(Remotestdmsg.linear.y))));
							ros_SendRpc(cmd);
						// stopRL_counter=0;
							}
						else{
							std::string cmd = packdata_UP((((int)fabs(Remotestdmsg.linear.y))));
							ros_SendRpc(cmd);
							//stopRL_counter=0;
							}
						}
					}
					else if(Remotestdmsg.IOSTATUES2[4] == 0){
						std::string cmd = packdata_stop();	
						//std::string cmd = "HELLO";
						ros_SendRpc(cmd);
					}

					//云台zomm变大
					if(Remotestdmsg.IOSTATUES2[4] == 1){
						std::string cmd = packdata_zoom_big((int)Remotestdmsg.linear.x);
						ros_SendRpc(cmd);
						}
					}

				//俯仰调整
				
			if((Remoteconnect== NO)&&(Old_Remoteconnect == YES)){
						std::string cmd = packdata_stop();	
						ros_SendRpc(cmd);
				}
			Old_Remoteconnect = Remoteconnect;
			//ros_SendRpc(packdata());
			loop_rate.sleep();
			
			
                     }
             }
        catch(boost::thread_interrupted&){
                 std::cout << "thread interrupt" << std:: endl;
             }
    
	
}



void pantiltctrlMode::ros_SendRpc(std::string &msg)
{   
    //ros::NodeHandle npub;
    // ros::Publisher zmq_pub = npub.advertise<std_msgs::String>("dirv_recvcmd", 1);
      std_msgs::String stdmsg;
     stdmsg.data = msg;
   
     pantiltctrlInfopub.publish(stdmsg);
     ros::spinOnce();
}

std::string pantiltctrlMode::packdata()
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
	 
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;
   
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
          
         std::string text = stream.str();
	   #if 0
	   printfinfo(msg);
	   #endif
	return text;
	
	
}

 void pantiltctrlMode::RemoterInfo_ros_cmdCall(const dgvmsg::remoter::ConstPtr& msg)
{
		RemoteNode.Set_connect();
		if(msg->online == 1){
		if((msg->IOSTATUES1[0] == YES)&&(msg->IOSTATUES1[6]==YES)){
                   
                    Remoteconnect = YES;
                }
		else{
			Remoteconnect = NO;
			Remote_count = 0;
			}
		Remote_count = 0;

		//存储关键值
	
		Remotestdmsg.linear.x = msg->linear.x;   //LX
		Remotestdmsg.linear.y =  msg->linear.y;  //LY  前后
		Remotestdmsg.linear.z = msg->linear.z;

		Remotestdmsg.angular.x = msg->angular.x;  //RX  左右
		Remotestdmsg.angular.y =  msg->angular.y;  //RY
		//statues.Remotestdmsg.angular.x = msg->angular.x;
		//statues.Remotestdmsg.angular.y = msg->angular.y;
		Remotestdmsg.angular.z = msg->angular.z;


		Remotestdmsg.IOSTATUES1[0] = msg->IOSTATUES1[0];
		Remotestdmsg.IOSTATUES1[1] = msg->IOSTATUES1[1];
		Remotestdmsg.IOSTATUES1[2] = msg->IOSTATUES1[2];
		Remotestdmsg.IOSTATUES1[3] = msg->IOSTATUES1[3];
		Remotestdmsg.IOSTATUES1[4] = msg->IOSTATUES1[4];
		Remotestdmsg.IOSTATUES1[5] = msg->IOSTATUES1[5];
		Remotestdmsg.IOSTATUES1[6] = msg->IOSTATUES1[6];
		Remotestdmsg.IOSTATUES1[7] = msg->IOSTATUES1[7];

		Remotestdmsg.IOSTATUES2[0] = msg->IOSTATUES2[0];
		Remotestdmsg.IOSTATUES2[1] = msg->IOSTATUES2[1];
		Remotestdmsg.IOSTATUES2[2] = msg->IOSTATUES2[2];
		Remotestdmsg.IOSTATUES2[3] = msg->IOSTATUES2[3];
		Remotestdmsg.IOSTATUES2[4] = msg->IOSTATUES2[4];
		Remotestdmsg.IOSTATUES2[5] = msg->IOSTATUES2[5];
		Remotestdmsg.IOSTATUES2[6] = msg->IOSTATUES2[6];
		Remotestdmsg.IOSTATUES2[7] = msg->IOSTATUES2[7];	

		Remotestdmsg.buttons[0] = msg->buttons[0];
		Remotestdmsg.buttons[1] = msg->buttons[1];
		Remotestdmsg.buttons[2] = msg->buttons[2];
		Remotestdmsg.buttons[3] = msg->buttons[3];
		Remotestdmsg.buttons[4] = msg->buttons[4];
		Remotestdmsg.buttons[5] = msg->buttons[5];
		Remotestdmsg.buttons[6] = msg->buttons[6];
		Remotestdmsg.buttons[7] = msg->buttons[7];	

              if( Remoteconnect == YES){
			  	/*
			//云台复位
			if(msg->buttons[0]==1){
				ros_SendRpc(packdata_reset());
				}

			//云台回HOME位
			if(msg->buttons[1]==1){
				ros_SendRpc(packdata_home());
				}

			//红外自动调焦一次
			if(msg->buttons[2]==1){
				ros_SendRpc(packdata_infread_autofocus());
				}
				*/

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
				msg_init();
			}
		#if DEBUG_ECHO
		printf_recv_extern_buffer();
		#endif
	
 } 

void pantiltctrlMode::msg_init(){

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

int pantiltctrlMode::check_connect()
{

               if(Remoteconnect== YES){
                  Remote_count++;
                  if(Remote_count >=60){             
                         //触发一次超时接收
                        #if 0
                         ROS_INFO("pantiltctrlMode Remoteconnect is losted **********************************");
                        #endif
                         Remote_count =0;
                         Remoteconnect = NO;
     			    msg_init();

                     }
                }
              else{
                   Remote_count =0;
		     msg_init();
                }
			  
	return 1;
	
}
 std::string pantiltctrlMode::get_session(void)
     {
         
         boost::uuids::random_generator rgen;
         boost::uuids::uuid u= rgen();
         std::string str;
         assert(uuid::static_size() == 16);
         assert(u.size() == 16);
         std::stringstream ss;
         ss << u; //uuid输出到字符串流
         ss>>str;//字符串流输出到字符串对象
         // std::cout<<"uuid="<<str<<std::endl;
         return str;
     }
 
std::string pantiltctrlMode::packdata_right(int speed)
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
	 
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;

	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "Right");
        int vau = (int)(speed*(64.0/100.0));
	std::string strspeed = boost::lexical_cast<std::string>(vau);

	Call->add_parameters(strspeed);

	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	return text;
	
	
}

std::string pantiltctrlMode::packdata_Left(int speed)
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
	 
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;

	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "Left");
	  int vau = (int)(speed*(64.0/100.0));
	std::string strspeed = boost::lexical_cast<std::string>(vau);

	Call->add_parameters(strspeed);

	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	return text;
	
	
}


std::string pantiltctrlMode::packdata_UP(int speed)
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
	 
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;
	msg.Clear();
	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "Up");
	 int vau = (int)(speed*(64.0/100.0));
	std::string strspeed = boost::lexical_cast<std::string>(vau);

	Call->add_parameters(strspeed);

	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	return text;
	
	
}



std::string pantiltctrlMode::packdata_DOWN(int speed)
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
	 
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;
	msg.Clear();
	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "Down");
	int vau = (int)(speed*(64.0/100.0));
	std::string strspeed = boost::lexical_cast<std::string>(vau);

	Call->add_parameters(strspeed);

	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	return text;
	
	
}


std::string pantiltctrlMode::packdata_stop()
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
    
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;
	msg.Clear();
	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);
	msg.set_time( time(NULL));
	
        PackageMessage_Call* callmsg = msg.mutable_callmessage();
        callmsg->set_function("Stop",4);	
		
          //序列化到内存
	    std::string res="";
	       if (!msg.SerializeToString(&res)  ) {
	           std::cout << "Failed to write msg packdata_stop Message." << std::endl; 
	           return ""; 
	       }
	return res;
		
}


std::string pantiltctrlMode::packdata_home()
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
    
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;

	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "GoHome");

	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	//stop_counter++;
	return text;
	
	
}

std::string pantiltctrlMode::packdata_reset()
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
    
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;

	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "RemoteReset");

	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	//stop_counter++;
	return text;
	
	
}

std::string pantiltctrlMode::packdata_zoom_big(int speed)
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
    
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;

	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "ZoomChange");
	
	int vau = (int)(speed*(5.0/100.0));
	std::string strspeed = boost::lexical_cast<std::string>(vau);	
	//std::string zoom = "1";
	std::cout<<"packdata_zoom_big ---->strspeed = "<<strspeed<<std::endl;
	Call->add_parameters(strspeed);
	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	//stop_counter++;
	return text;
	
	
}

std::string pantiltctrlMode::packdata_zoom_small()
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
    
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;

	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_PANTILT);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "ZoomChange");
	
	std::string zoom = "-1";

	Call->add_parameters(zoom);
	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	//stop_counter++;
	return text;
	
	
}


std::string pantiltctrlMode::packdata_infread_autofocus()
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
    
     GOOGLE_PROTOBUF_VERIFY_VERSION;

	PackageMessage msg;

	msg.set_sessionid(get_session());
   	msg.set_from(ZMQ_NAME_DRIVERCTRL);
	msg.set_to(ZMQ_NAME_CAMSERV);

 	PackageMessage_Call *Call = 	msg.mutable_callmessage();

	Call->set_function( "AutoFocus");
	//int vau = (int)(speed*(30.0/100.0));
	//std::string strspeed = boost::lexical_cast<std::string>(vau);

	//Call->add_parameters(strspeed);

	
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
         std::string text = stream.str();
	//stop_counter++;
	return text;
	
	
}



void pantiltctrlMode::printf_recv_extern_buffer(){
	
	
	std::cout<<"stdmsg.IOSTATUES1[0]= "<< (int)Remotestdmsg.IOSTATUES1[0]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[1]= "<< (int)Remotestdmsg.IOSTATUES1[1]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[2]= "<< (int)Remotestdmsg.IOSTATUES1[2]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[3]= "<< (int)Remotestdmsg.IOSTATUES1[3]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[4]= "<< (int)Remotestdmsg.IOSTATUES1[4]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[5]= "<< (int)Remotestdmsg.IOSTATUES1[5]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[6]= "<< (int)Remotestdmsg.IOSTATUES1[6]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES1[7]= "<< (int)Remotestdmsg.IOSTATUES1[7]<<std::endl;

	std::cout<<"stdmsg.IOSTATUES2[0]= "<< (int)Remotestdmsg.IOSTATUES2[0]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[1]= "<< (int)Remotestdmsg.IOSTATUES2[1]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[2]= "<< (int)Remotestdmsg.IOSTATUES2[2]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[3]= "<< (int)Remotestdmsg.IOSTATUES2[3]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[4]= "<< (int)Remotestdmsg.IOSTATUES2[4]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[5]= "<< (int)Remotestdmsg.IOSTATUES2[5]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[6]= "<< (int)Remotestdmsg.IOSTATUES2[6]<<std::endl;
	std::cout<<"stdmsg.IOSTATUES2[7]= "<< (int)Remotestdmsg.IOSTATUES2[7]<<std::endl;	
	
	std::cout<<"stdmsg.buttons[0]= "<< (int)Remotestdmsg.buttons[0]<<std::endl;
	std::cout<<"stdmsg.buttons[1]= "<< (int)Remotestdmsg.buttons[1]<<std::endl;
	std::cout<<"stdmsg.buttons[2]= "<< (int)Remotestdmsg.buttons[2]<<std::endl;
	std::cout<<"stdmsg.buttons[3]= "<< (int)Remotestdmsg.buttons[3]<<std::endl;
	std::cout<<"stdmsg.buttons[4]= "<< (int)Remotestdmsg.buttons[4]<<std::endl;
	std::cout<<"stdmsg.buttons[5]= "<< (int)Remotestdmsg.buttons[5]<<std::endl;
	std::cout<<"stdmsg.buttons[6]= "<< (int)Remotestdmsg.buttons[6]<<std::endl;
	std::cout<<"stdmsg.buttons[7]= "<< (int)Remotestdmsg.buttons[7]<<std::endl;

	std::cout<<"stdmsg.linear.x= "<< Remotestdmsg.linear.x<<std::endl;
	std::cout<<"stdmsg.linear.y= "<< Remotestdmsg.linear.y<<std::endl;
	std::cout<<"stdmsg.linear.z= "<< Remotestdmsg.linear.z<<std::endl;
	
	std::cout<<"stdmsg.angular.x= "<< Remotestdmsg.angular.x<<std::endl;
	std::cout<<"stdmsg.angular.y= "<< Remotestdmsg.angular.y<<std::endl;
	std::cout<<"stdmsg.angular.z= "<< Remotestdmsg.angular.z<<std::endl;	

}



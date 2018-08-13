
#include "zmqclient_class.hpp"

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


#define DUBUG_ECHO 0

using  namespace boost;
using namespace boost::uuids;

zmqclient::zmqclient(){
    //define by urser  
        zmqadd_pub_ = "tcp://localhost:6665" ;
        zmqadd_sub_ =  "tcp://localhost:6666";
            int index=1;
            zmqcmdlist.insert(std::make_pair("zmqheart",index++ ));   
            
        topic_size=0;
        
        //if u use ROS ,it can be write like this
    
        //define socket
        connectflag = false;
        try{
            //boost::shared_ptr< zmq::context_t > m_zmqcontext(new zmq::context_t(1,1024));
            zmqcontext = new zmq::context_t(1,1024);
       
            }
        catch(...){
                        std::cout << "zmqclient: an error_t was thrown->" << ERROR_context_t_INIT <<std::endl;  
            }
        //boost::shared_ptr< zmq::socket_t > m_spub(new zmq::socket_t(*zmqcontext,ZMQ_PUB));
      // boost::shared_ptr< zmq::socket_t > m_ssub(new zmq::socket_t(*zmqcontext,ZMQ_SUB));
        zmqpub_=new zmq::socket_t(*zmqcontext,ZMQ_PUB);
        zmqsub_ =new zmq::socket_t(*zmqcontext,ZMQ_SUB);
}
zmqclient::zmqclient(std::string &ipc_pub,std::string &ipc_sub)
    :zmqadd_pub_(ipc_pub),
    zmqadd_sub_(ipc_sub)
    {
    //define by urser  
        //zmqadd_pub_ = "tcp://localhost:6665" ;
       // zmqadd_sub_ =  "tcp://localhost:6666";
        topic_size=0;

        
        //if u use ROS ,it can be write like this
    
        //define socket
        connectflag = false;
        try{
        	zmqcontext = new zmq::context_t(1,1024);
       
            }
        catch(...){
                        std::cout << "zmqclient: an error_t was thrown->" << ERROR_context_t_INIT <<std::endl;  
            }
        zmqpub_=new zmq::socket_t(*zmqcontext,ZMQ_PUB);
        zmqsub_ =new zmq::socket_t(*zmqcontext,ZMQ_SUB);


        
}


zmqclient::~zmqclient() 
{
    delete zmqpub_;
    delete zmqsub_;
    delete zmqcontext;
} 
void zmqclient::insert_topic(std::string topic){
      topic_size++;
      boost::format fmt1("%1%") ;
      fmt1 %topic_size;
      zmqsubtopic_s.insert(std::make_pair(fmt1.str(), topic));   
}

void zmqclient::connect(void){
        int rc = -1;
        //connect to the server
        try{
        this->zmqpub_->connect(this->zmqadd_pub_.c_str() );
            }
        catch(...){
            std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_PUB <<std::endl;  
            }
         try{
        this->zmqsub_->connect(this->zmqadd_sub_.c_str() );
            }
        catch(...){
            std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_SUB <<std::endl;  
            }
        
        int topicn = zmqsubtopic_s.size();

        for(int index=0;index < topicn ; index ++){
                format fmt1("%1%");
                fmt1 % (index+1);
                this->zmqsub_->setsockopt(ZMQ_SUBSCRIBE, 
                this->zmqsubtopic_s[fmt1.str()].c_str() , 
                 this->zmqsubtopic_s[fmt1.str()].length());
               std::cout << "sub topic: [" << fmt1.str() <<"] " <<  this->zmqsubtopic_s[fmt1.str()].c_str() << std::endl;
            }
        
        boost::format fmt2("connect to PUB: %1% SUB: %2% is successed");
        fmt2 % this->zmqadd_pub_;
        fmt2 % this->zmqadd_sub_;
        std::cout <<fmt2.str() << std::endl;

        this ->connectflag = true;
}
void zmqclient::connect_pub(void){
        int rc = -1;
        //connect to the server
        try{
        this->zmqpub_->connect(this->zmqadd_pub_.c_str() );
            }
        catch(...){
            std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_PUB <<std::endl;  
            }
        
        boost::format fmt2("connect to PUB: %1%  is successed");
        fmt2 % this->zmqadd_pub_;
        std::cout <<fmt2.str() << std::endl;

        this ->connectflag = true;
}

void zmqclient::connect_sub(void){
        int rc = -1;
        //connect to the server
         try{
        this->zmqsub_->connect(this->zmqadd_sub_.c_str() );
            }
        catch(...){
            std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_SUB <<std::endl;  
            }
        
              int topicn = zmqsubtopic_s.size();

        for(int index=0;index < topicn ; index ++){
                boost::format fmt1("%1%");
                fmt1 % (index+1);
                this->zmqsub_->setsockopt(ZMQ_SUBSCRIBE, 
                this->zmqsubtopic_s[fmt1.str()].c_str() , 
                 this->zmqsubtopic_s[fmt1.str()].length());
               std::cout << "sub topic: [" << fmt1.str() <<"] " <<  this->zmqsubtopic_s[fmt1.str()].c_str() << std::endl;
            }
        
        boost::format fmt2("connect to  SUB: %1% is successed");
        fmt2 % this->zmqadd_sub_;
        std::cout <<fmt2.str() << std::endl;

        this ->connectflag = true;
}

void zmqclient::destory_sub(void){
    delete zmqsub_;
    delete zmqcontext;
}
void zmqclient::destory_pub(void){
   delete zmqpub_;
    delete zmqcontext;
}
void zmqclient::destory(void){
   delete zmqpub_;
   delete zmqsub_;
   delete zmqcontext;
}


bool zmqclient::isconnect(){
    return this ->connectflag;
}

void zmqclient::zmqrecvLoop( )
    {  


     std::cout << "start to thread "<<"["<<__FUNCTION__ << "]"<<std::endl;
    try{
            //ros::Rate loop_rate(50);
               //  Poll socket for a subscriber, with timeout
               int time_out_conter=0;
            zmq::pollitem_t items[] = { { *(this->zmqsub_), 0, ZMQ_POLLIN, 0 } };          
        while (ros::ok()) 
        {
           // loop_rate.sleep();
		  
             // 100 milliseconds  ¶¨ÒåÒ»ÊÂ¼þ
            zmq::poll (&items[0], 1,  20);
             
            if (items[0].revents & ZMQ_POLLIN) 
                {
                    //recv a messag
                    int more=0;
                    std::size_t more_size = sizeof (more);
                    //zmq::message_t message;
                     
                    //this -> zmqadd_sub_->recv(&message);
                    std::string topic = this->s_recv();
                  //  std::cout << "topic =" <<topic <<std::endl;
                    this -> zmqsub_->getsockopt( ZMQ_RCVMORE, &more, &more_size);
                    //do some job                                                              
                    std::string step = this->s_recv();
                     this -> zmqsub_->getsockopt( ZMQ_RCVMORE, &more, &more_size);
                    std::string context = this->s_recv();
                    this -> zmqsub_->getsockopt( ZMQ_RCVMORE, &more, &more_size);
                    if(!more)
                        {
                            //recv last message
                          unpackage_main(context);
                          //½ÓÊÕµ½ÃüÁî×ª·¢µ½ROSÄÚ²¿
				//dirv_ros_SendRpc(context);
			      topic.clear();
			      context.clear();		  
                        }
                }
            else{
                //  std::cout << "zmqclient_recv  zmq::poll (&items[0], 1,  100)is timeout ................." << std::endl;  
                //Ò»¶¨Ê±¼ä·¢ËÍÒ»´ÎÍøÂçÐÄÌ
             boost::this_thread::interruption_point();  
			       //std::string to=ZMQ_NAME_DRIVERCTRL;
		              //std::string from = ZMQ_NAME_DRIVERCTRL;
				//pacgagemessage_heart(to,from);
				}
        }
        }
    catch(...){
              std::cout << "zmqclient_recv Interrupt exception was thrown." << std::endl;  
        }
} 

void zmqclient::rospubtask(){  

    
} 

void zmqclient::zmqpubtask(){  

    std::cout << "start to thread "<<"["<<__FUNCTION__ << "]"<<std::endl;

    ros::Rate loop_rate(1);
    int index = 0;
   try{

           while (ros::ok())
		 {
              loop_rate.sleep();
		//std::string to=ZMQ_NAME_DRIVERCTRL;
		//std::string from = ZMQ_NAME_DRIVERCTRL;
		
		zmq_SendRpc(zmqheart_SendRpc(),ZMQ_NAME_DRIVERCTRL);

              //boost::this_thread::sleep(boost::posix_time::seconds(1));
		boost::this_thread::interruption_point();  
              // sleep 100ºÁÃë;  
             // boost::this_thread::sleep(boost::posix_time::millisec(100)); 
             }
    }
   catch(...){
         std::cout << "zmqclient_pub Interrupt exception was thrown." << std::endl;  
    }
   

} 

void zmqclient::print_zmq_version(){
    int major, minor, patch;
    zmq::version (&major, &minor, &patch);
    std::cout << "Current 0MQ version is " << major << "." << minor << "." << 
patch << std::endl;
	
}
bool zmqclient::s_sendmore (const std::string  &string) {

    zmq::message_t message(string.data(), string.size());
    bool rc = this->zmqpub_->send (message, ZMQ_SNDMORE);
    return (rc);
}

bool zmqclient::s_send(const std::string  &key){
	  zmq::message_t message(key.data(), key.size());
	 // memcpy (message.data(), key.data(), key.size());
	  bool rc = this->zmqpub_->send (message);
	  return rc;	
}

bool zmqclient::msg_send(zmq::message_t& msg){
	 // memcpy (message.data(), key.data(), key.size());
	  bool rc = this->zmqpub_->send (msg);
	  return rc;	
}
bool zmqclient::msg_sendmore (zmq::message_t& msg) {

    bool rc = this->zmqpub_->send (msg, ZMQ_SNDMORE);
    return (rc);
}

std::string zmqclient::s_recv () {

    zmq::message_t message;
    this->zmqsub_->recv(&message,ZMQ_DONTWAIT);
    //this->zmqsub_->recv(&message,0);
    return std::string(static_cast<char*>(message.data()), message.size());
}



std::string zmqclient::get_msg_string (zmq::message_t& message) {

   // zmq::message_t message;
   // socket.recv(&message);
    return std::string(static_cast<char*>(message.data()), message.size());
}


void zmqclient::unpackage_main(std::string fun)
    {
    
        
     // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;
      try{

     PackageMessage msgrecv;
     msgrecv.ParseFromString(fun);

     #if 0
     show_mainmsg(msgrecv);
     #endif
     
     std::string strto=msgrecv.to();
     //std::cout<<"strto="<<strto<<std::endl;
     
    // ROS_INFO("strto=%s", strto.c_str());
     if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //×ª·¢¸øZMQ_NAME_DRIVERCTRL
            
            dirv_ros_SendRpc(fun);
          
      }
      	}
   catch(...){
         std::cout << "unpackage_main Interrupt exception was thrown." << std::endl;  
    }
     
}

int  zmqclient::show_mainmsg(PackageMessage & msg)
{
		 
	    boost::format fmt1("SessionId: = [%1%] Time: = [%2%] from: = [%3%] ");
	    
	    fmt1 %msg.sessionid();
	    //std::cout<<"SessionId: "<<fmt.str()<<" ";
	    //fmt.clear();
	    fmt1 %msg.time();
	    //std::cout<<"Time: "<<fmt.str()<<" ";
	    // fmt.clear();
	    fmt1 %msg.from();
	   // std::cout<<"From: "<<fmt.str()<<" ";
	   //  fmt.clear();
	   // fmt %msg.to();
	    std::cout<<fmt1.str()<<std::endl;

	    if(msg.has_callmessage()){
			boost::format fmt2("function: = [%1%] parameters_size = [%2%]");
	            PackageMessage_Call *msgcall = msg.mutable_callmessage();
	           //  fmt.clear();
	            fmt2 %msgcall->function();
	          //  std::cout<<"function: "<<fmt.str()<<" ";
	            // fmt.clear();
	            fmt2 %msgcall->parameters_size();
	           // std::cout<<"parameters_size: "<<fmt.str()<<std::endl;
	           std::cout<<fmt2.str()<<std::endl;
	        }
	    if(msg.has_resultmessage()){
			boost::format fmt3("errorcode = [%1%]  has_resultdata = [%2%]  ");
	            PackageMessage_Result *msgres= msg.mutable_resultmessage();
	            // fmt.clear();
	            fmt3 %msgres->errorcode();
	           // std::cout<<"errorcode: "<<fmt.str()<<" ";
	            // fmt.clear();
	            fmt3 %msgres->has_resultdata();
	           // std::cout<<"has_resultdata: "<<fmt.str()<<" ";
	            std::cout<<fmt3.str()<<std::endl;
	        }
	


}

int zmqclient::proc_zmq_recvheart(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    PackageMessage msgrecv;
    msgrecv.ParseFromString(pack);
      int ret=0;
      #if DUBUG_ECHO
   ROS_INFO("zmqclient->proc_zmq_recvheart");
      #endif
    std::string strto=msgrecv.to();
    
    if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //×ª·¢¸øZMQ_NAME_DRIVERCTRL
            PackageMessage_Call* callmsg = msgrecv.mutable_callmessage();
            std::string fun=callmsg->function();
            std::map<std::string,int >::iterator it = zmqcmdlist.find(fun); 
            if( it == zmqcmdlist.end() ) {
                    std::cout<<"we can not find function name:"<<fun<<std::endl;
                    return -1;
                } 
         
            int funid= (int)(it->second);
            #if 1
            std::cout<<"recv a heart:"<<fun<<"["<<funid<<"]"<<std::endl;
            #endif
            switch(funid){
                    case 1:
                            connectflag = 1;
                        break;
                       default:
                            break;
                }
            return funid;
      }                
          return -1;      
}   

void zmqclient::dirv_ros_SendRpc(std::string msg)
{   
    //ros::NodeHandle npub;
    // ros::Publisher zmq_pub = npub.advertise<std_msgs::String>("dirv_recvcmd", 1);
   
    std_msgs::String stdmsg;
     stdmsg.data = msg;
   // ROS_INFO("ros pub: to=%s,size= [%d]", "dirv_recvcmd",stdmsg.data.size());
   #if DUBUG_ECHO
    std::cout<<"ros pub: to="
        <<"dirv_recvcmd"<<",size="
        <<stdmsg.data.size()<<std::endl;
   #endif
     rospub.publish(stdmsg);
     
     ros::spinOnce();
    // stdmsg.Clear();
}
void zmqclient::zmq_SendRpc(std::string msg,std::string topic)
{   
            #if DUBUG_ECHO
            boost::format fmt1("zmq pub : topic=%1% , size=[%2%] ");
            fmt1 %topic;
            fmt1 %msg.size();
            
            std::cout << fmt1.str() << std::endl;
            #endif
            
            this->s_sendmore(topic);
            this->s_sendmore("");
            this->s_send(msg);    
}

std::string zmqclient::Get_to(std::string fun){
        
     // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;
      try{
    PackageMessage msgrecv;
    bool ret2 = msgrecv.ParseFromString(fun);
    if(ret2){
		//std::cout << "fun parse success" << std::endl;  
    	}
	else{
		//std::cout << "fun parse fail" << std::endl;
		}
   // show_mainmsg(msgrecv);
    std::string strfrom=msgrecv.to();


    return   strfrom;
	      	}
   catch(...){
         std::cout << "Get_to Interrupt exception was thrown." << std::endl;  
    }
	  
}

void zmqclient::dirv_zmq_Callback(const std_msgs::String::ConstPtr& msg)
 {
         #if DUBUG_ECHO
        ROS_INFO("ros sub ->dirv_zmq: [%d]", msg->data.size());
         #endif
         if(connectflag == true){
                std::string to = Get_to(msg->data);
                zmq_SendRpc(msg->data,to);
            }
        
 }
 void zmqclient::dirv_update_Callback(const std_msgs::String::ConstPtr& msg)
  {
         #if DUBUG_ECHO
         ROS_INFO("ros sub ->update: [%d]", msg->data.size());
         #endif
         //std::string from = Get_from(msg->data);
         if(connectflag == true){
                zmq_SendRpc(msg->data,ZMQ_STAND_TOPIC_DRIVERUPDATE);
            }
   
  }
 
 std::string zmqclient::get_session(void)
     {
         
         boost::uuids::random_generator rgen;
         boost::uuids::uuid u= rgen();
         std::string str;
         assert(uuid::static_size() == 16);
         assert(u.size() == 16);
         std::stringstream ss;
         ss << u; //uuidÊä³öµ½×Ö·û´®Á÷
         ss>>str;//×Ö·û´®Á÷Êä³öµ½×Ö·û´®¶ÔÏó
         // std::cout<<"uuid="<<str<<std::endl;
         return str;
     }
std::string zmqclient::zmqheart_SendRpc(void)
{   
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION; 
	 
	PackageMessage packMSg;
	 try{
	packMSg.Clear();
	packMSg.set_sessionid(get_session());
	packMSg.set_from(ZMQ_NAME_DRIVERCTRL);
	packMSg.set_to(ZMQ_NAME_DRIVERCTRL);
	packMSg.set_time(time(NULL));

	PackageMessage_Call * call = packMSg.mutable_callmessage();
	call->set_function( MYGEARTRpcTopic);
	   	}
   catch(...){
         std::cout << "zmqheart_SendRpc Interrupt exception was thrown." << std::endl;  
		packMSg.Clear();	 
		packMSg.set_sessionid(get_session());
		 
    }

    std::string res;
       if (!packMSg.SerializeToString(&res)  ) {
           std::cerr << "Failed to write msg MYGEARTRpcTopic." << std::endl; 
          // return ""; 
       }  	
  return res;
}




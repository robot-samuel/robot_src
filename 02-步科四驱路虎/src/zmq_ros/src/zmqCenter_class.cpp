
#include "zmqCenter_class.hpp"

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

zmqcenter::zmqcenter(){
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
                        std::cout << "zmqcenter: an error_t was thrown->" << ERROR_context_t_INIT <<std::endl;  
            }
        zmqpub_=new zmq::socket_t(*zmqcontext,ZMQ_PUB);
        zmqsub_ =new zmq::socket_t(*zmqcontext,ZMQ_SUB);
		
}
zmqcenter::zmqcenter(std::string &ipc_pub,std::string &ipc_sub)
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
                        std::cout << "zmqcenter: an error_t was thrown->" << ERROR_context_t_INIT <<std::endl;  
            }
        zmqpub_=new zmq::socket_t(*zmqcontext,ZMQ_PUB);
        zmqsub_ =new zmq::socket_t(*zmqcontext,ZMQ_SUB);


        
}


zmqcenter::~zmqcenter() 
{
    delete zmqpub_;
    delete zmqsub_;
    delete zmqcontext;
} 
void zmqcenter::insert_topic(std::string topic){
      topic_size++;
      boost::format fmt1("%1%") ;
      fmt1 %topic_size;
      zmqsubtopic_s.insert(std::make_pair(fmt1.str(), topic));   
}

void zmqcenter::bindall(void){
        int rc = -1;
        //connect to the server
        try{
        this->zmqpub_->bind(this->zmqadd_pub_.c_str() );
            }
        catch(...){
            std::cout << "zmqcenter: an error_t was thrown->" << ERROR_connect_PUB <<std::endl;  
            }
         try{
        this->zmqsub_->bind(this->zmqadd_sub_.c_str() );
            }
        catch(...){
            std::cout << "zmqcenter: an error_t was thrown->" << ERROR_connect_SUB <<std::endl;  
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
void zmqcenter::bind_pub(void){
        int rc = -1;
        //connect to the server
        try{
        this->zmqpub_->bind(this->zmqadd_pub_.c_str() );
            }
        catch(...){
            std::cout << "zmqcenter: an error_t was thrown->" << ERROR_connect_PUB <<std::endl;  
            }
        
        boost::format fmt2("connect to PUB: %1%  is successed");
        fmt2 % this->zmqadd_pub_;
        std::cout <<fmt2.str() << std::endl;

        this ->connectflag = true;
}

void zmqcenter::bind_sub(void){
        int rc = -1;
        //connect to the server
         try{
        this->zmqsub_->bind(this->zmqadd_sub_.c_str() );
            }
        catch(...){
            std::cout << "zmqcenter: an error_t was thrown->" << ERROR_connect_SUB <<std::endl;  
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

void zmqcenter::destory_sub(void){
    delete zmqsub_;
    delete zmqcontext;
}
void zmqcenter::destory_pub(void){
   delete zmqpub_;
    delete zmqcontext;
}
void zmqcenter::destory(void){
   delete zmqpub_;
   delete zmqsub_;
   delete zmqcontext;
}


void zmqcenter::zmqproxyLoop( )
    {  


     std::cout << "start to thread "<<"["<<__FUNCTION__ << "]"<<std::endl;
    try{
        
        while (ros::ok()) 
        { 
          std::string context;
	   std::string topic;
	   std::string step;
          context.clear();
             
			while(1){
		            zmq::message_t message;
		            int more;
		            std::size_t more_size = sizeof (more);

		            //  Process all parts of the message
		            this ->zmqsub_->recv(&message,0);
		            this ->zmqsub_->getsockopt( ZMQ_RCVMORE, &more, &more_size);
		            this ->zmqpub_->send(message, more? ZMQ_SNDMORE: 0);
		            if (!more){
		                break;      //  Last message part
		                context = get_msg_string(message);
				   if(topic = MYGEARTRpcTopic){
						//收到心跳回复
						
				   	}
		            	}
			    else{
					if(message.size()>1){
							topic = get_msg_string(message);
						}
			    	}
		        }
			//处理内容
			
             boost::this_thread::interruption_point();  
        }
        }
    catch(...){
              std::cout << "zmqcenter_recv Interrupt exception was thrown." << std::endl;  
        }
} 

void zmqcenter::print_zmq_version(){
    int major, minor, patch;
    zmq::version (&major, &minor, &patch);
    std::cout << "Current 0MQ version is " << major << "." << minor << "." << 
patch << std::endl;
	
}
bool zmqcenter::s_sendmore (const std::string  &string) {

    zmq::message_t message(string.data(), string.size());
    bool rc = this->zmqpub_->send (message, ZMQ_SNDMORE);
    return (rc);
}

bool zmqcenter::s_send(const std::string  &key){
	  zmq::message_t message(key.data(), key.size());
	 // memcpy (message.data(), key.data(), key.size());
	  bool rc = this->zmqpub_->send (message);
	  return rc;	
}

bool zmqcenter::msg_send(zmq::message_t& msg){
	 // memcpy (message.data(), key.data(), key.size());
	  bool rc = this->zmqpub_->send (msg);
	  return rc;	
}
bool zmqcenter::msg_sendmore (zmq::message_t& msg) {

    bool rc = this->zmqpub_->send (msg, ZMQ_SNDMORE);
    return (rc);
}

std::string zmqcenter::s_recv () {

    zmq::message_t message;
    this->zmqsub_->recv(&message,ZMQ_DONTWAIT);
    //this->zmqsub_->recv(&message,0);
    return std::string(static_cast<char*>(message.data()), message.size());
}



std::string zmqcenter::get_msg_string (zmq::message_t& message) {

   // zmq::message_t message;
   // socket.recv(&message);
    return std::string(static_cast<char*>(message.data()), message.size());
}


void zmqcenter::unpackage_main(std::string fun)
    {
    
        
     // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;
     
     PackageMessage msgrecv;
     msgrecv.ParseFromString(fun);
     #if DUBUG_ECHO
     show_mainmsg(msgrecv);
     #endif
     
     std::string strto=msgrecv.to();
     //std::cout<<"strto="<<strto<<std::endl;
     
    // ROS_INFO("strto=%s", strto.c_str());
     if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //转发给ZMQ_NAME_DRIVERCTRL
            
            dirv_ros_SendRpc(fun);
          
      }

     
}

int  zmqcenter::show_mainmsg(PackageMessage & msg)
{

    boost::format fmt("%1%");
    std::cout<<"****************PackageMessage*******************"<<std::endl;
    fmt %msg.sessionid();
    std::cout<<"SessionId: "<<fmt.str()<<" ";
    fmt.clear();
    fmt %msg.time();
    std::cout<<"Time: "<<fmt.str()<<" ";
     fmt.clear();
    fmt %msg.from();
    std::cout<<"From: "<<fmt.str()<<" ";
     fmt.clear();
    fmt %msg.to();
    std::cout<<"To: "<<fmt.str()<<std::endl;

    if(msg.has_callmessage()){
            PackageMessage_Call *msgcall = msg.mutable_callmessage();
             fmt.clear();
            fmt %msgcall->function();
            std::cout<<"function: "<<fmt.str()<<" ";
             fmt.clear();
            fmt %msgcall->parameters_size();
            std::cout<<"parameters_size: "<<fmt.str()<<std::endl;
        }
    if(msg.has_resultmessage()){
            PackageMessage_Result *msgres= msg.mutable_resultmessage();
             fmt.clear();
            fmt %msgres->errorcode();
            std::cout<<"errorcode: "<<fmt.str()<<" ";
             fmt.clear();
            fmt %msgres->has_resultdata();
            std::cout<<"has_resultdata: "<<fmt.str()<<" ";
        }


}

int zmqcenter::proc_zmq_recvheart(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    PackageMessage msgrecv;
    msgrecv.ParseFromString(pack);
      int ret=0;
      #if DUBUG_ECHO
   ROS_INFO("zmqcenter->proc_zmq_recvheart");
      #endif
    std::string strto=msgrecv.to();
    
    if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //转发给ZMQ_NAME_DRIVERCTRL
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

void zmqcenter::dirv_ros_SendRpc(std::string msg)
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
}
void zmqcenter::zmq_SendRpc(std::string msg,std::string topic)
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

std::string zmqcenter::Get_to(std::string fun){
        
     // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    PackageMessage msgrecv;
    msgrecv.ParseFromString(fun);
   // show_mainmsg(msgrecv);
    std::string strfrom=msgrecv.to();

    return   strfrom;
}

void zmqcenter::dirv_zmq_Callback(const std_msgs::String::ConstPtr& msg)
 {
         #if DUBUG_ECHO
        ROS_INFO("ros sub ->dirv_zmq: [%d]", msg->data.size());
         #endif
         if(connectflag == true){
                std::string to = Get_to(msg->data);
                zmq_SendRpc(msg->data,to);
            }
        
 }
 void zmqcenter::dirv_update_Callback(const std_msgs::String::ConstPtr& msg)
  {
         #if DUBUG_ECHO
         ROS_INFO("ros sub ->update: [%d]", msg->data.size());
         #endif
         //std::string from = Get_from(msg->data);
         if(connectflag == true){
                zmq_SendRpc(msg->data,ZMQ_NAME_MANGSYS);
            }
   
  }
 
 std::string zmqcenter::get_session(void)
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
 void zmqcenter::pacgagemessage_heart(std::string& to,std::string& from)
{
      // Verify that the version of the library that we linked against is
     // compatible with the version of the headers we compiled against.
     GOOGLE_PROTOBUF_VERIFY_VERSION;

         PackageMessage msg;
         msg.set_sessionid(get_session());
         msg.set_from(from);
         msg.set_to(to);
         msg.set_time( time(NULL));
         
         PackageMessage_Call* callmsg = msg.mutable_callmessage();
         callmsg->set_function( MYGEARTRpcTopic);
        // callmsg->add_parameters(topic);
         
          //序列化到内存
          std::ostringstream stream;
          msg.SerializeToOstream(&stream);
          
          std::string text = stream.str();
          
          //char* buf = text.c_str();
          
         int length = msg.ByteSize();
         //char* buf = new char[length];
         //msg.SerializeToArray(buf,length);
         
         //printPackageMessage(buf,length);
         //发送出去
         zmq_SendRpc(text,MYGEARTRpcTopic);

         
        // delete buf;
}





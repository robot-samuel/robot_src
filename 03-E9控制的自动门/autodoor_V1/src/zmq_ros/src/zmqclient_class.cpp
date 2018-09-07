
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

	 logfile.init( CONFIGLOGPATH, "zmq");
	 logfile.out_info("======zmqclient init======");
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



	 logfile.init( CONFIGLOGPATH, "zmq");
	 
	 logfile.out_info(zmqadd_pub_);
	 logfile.out_info(zmqadd_sub_);   
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


int zmqclient::connect(void){
        int rc = -1;
        //connect to the server
        try{
        		this->zmqpub_->connect(this->zmqadd_pub_.c_str() );
            }
        catch(...){
           // std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_PUB <<std::endl;  
			 logfile.out_info("zmqclient: an error_t was thrown->ERROR_connect_PUB");
		   	return -1;
            }
		
         try{
        		this->zmqsub_->connect(this->zmqadd_sub_.c_str());
            }
        catch(...){
          //  std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_SUB <<std::endl;  
		 logfile.out_info("zmqclient: an error_t was thrown->ERROR_connect_SUB");
		  return -1;
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
        //std::cout <<fmt2.str() << std::endl;
	logfile.out_info(fmt2.str());
        this ->connectflag = true;
	return  1;
}
void zmqclient::connect_pub(void){
        int rc = -1;
        //connect to the server
        try{
        		this->zmqpub_->connect(this->zmqadd_pub_.c_str() );
            }
        catch(...){
           // std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_PUB <<std::endl;  
	      logfile.out_info("zmqclient: an error_t was thrown->ERROR_connect_PUB");
            }
        
        boost::format fmt2("connect to PUB: %1%  is successed");
        fmt2 % this->zmqadd_pub_;
        //std::cout <<fmt2.str() << std::endl;
	logfile.out_info(fmt2.str());
        this ->connectflag = true;
}

void zmqclient::connect_sub(void){
        int rc = -1;
        //connect to the server
         try{
        this->zmqsub_->connect(this->zmqadd_sub_.c_str() );
            }
        catch(...){
           // std::cout << "zmqclient: an error_t was thrown->" << ERROR_connect_SUB <<std::endl; 
			 logfile.out_info("zmqclient: an error_t was thrown->ERROR_connect_SUB");
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
       // std::cout <<fmt2.str() << std::endl;
	logfile.out_info(fmt2.str());

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
   connectflag = false;
}


int zmqclient::disconnect(void){

      connectflag = false;	
      std::cout <<"["<<__FUNCTION__ << "]"<< "disconnect the  connect"<<std::endl;
     try{
   		this->zmqpub_->disconnect(this->zmqadd_pub_.c_str());
		
   		
     	}
    catch(...){
           // std::cout << "zmqclient: an error_t was thrown->" << "disconnect" <<std::endl;  
			logfile.out_info("zmqclient: an error_t was thrown1->->disconnect");
		   	return -1;
           }
        try{   
	this->zmqsub_->disconnect(this->zmqadd_sub_.c_str() );
     	}
    catch(...){
           // std::cout << "zmqclient: an error_t was thrown->" << "disconnect" <<std::endl;  
			logfile.out_info("zmqclient: an error_t was thrown2->->disconnect");
		   	return -1;
           }
	return 1;
}


bool zmqclient::isconnect(){
    return this ->connectflag;
}

void zmqclient::zmqrecvLoop( )
    {  

//try{
     std::cout << "start to thread "<<"["<<__FUNCTION__ << "]"<<std::endl;
  
            //ros::Rate loop_rate(50);
               //  Poll socket for a subscriber, with timeout
               int time_out_conter=0;
         zmq::pollitem_t items[] = { { *(this->zmqsub_), 0, ZMQ_POLLIN, 0 } };          
        while (ros::ok()) 
        {
           // loop_rate.sleep();
	      if(connectflag == true){  
	             // 100 milliseconds  ¶¨ÒåÒ»ÊÂ¼þ
			zmq::poll (&items[0], 1,  20);
	       		std::string topic;
		  	std::string step;
			std::string context;

			topic.clear();
			step.clear();
			context.clear();
	            if (items[0].revents & ZMQ_POLLIN) 
	                {
	                    //recv a messag
	                    int more=0;
	                    std::size_t more_size = sizeof (more);
	                    //zmq::message_t message;
	                     
	                    //this -> zmqadd_sub_->recv(&message);
	                    try{
	                        topic = this->s_recv();
	                    	}
				catch(...){
				 // std::cout << "zmqclient_recv Interrupt exception was thrown." << std::endl;  
					logfile.out_info("zmqclient_recv std::string topic = this->s_recv()" );
				       continue;
				}			
	                  //  std::cout << "topic =" <<topic <<std::endl;
	                    this -> zmqsub_->getsockopt( ZMQ_RCVMORE, &more, &more_size);
	                                                                        
	                    
	                    try{
	                         step = this->s_recv();
	                    	}
				catch(...){
				 // std::cout << "zmqclient_recv Interrupt exception was thrown." << std::endl;  
					logfile.out_info("zmqclient_recv std::std::string step = this->s_recv();" );
				       continue;
				}	
	                     this -> zmqsub_->getsockopt( ZMQ_RCVMORE, &more, &more_size);
	                   
                    	try{
	                        context = this->s_recv();
	                    	}
				catch(...){
				 // std::cout << "zmqclient_recv Interrupt exception was thrown." << std::endl;  
					logfile.out_info("zmqclient_recv std::string context = this->s_recv();" );
				       continue;
				}
	                    this -> zmqsub_->getsockopt( ZMQ_RCVMORE, &more, &more_size);
	                    if(!more)
	                        {
	                            //recv last message
	                          unpackage_main(context);
	                          //½ÓÊÕµ½ÃüÁî×ª·¢µ½ROSÄÚ²¿
					//dirv_ros_SendRpc(context);
				      //topic.clear();
				      //context.clear();		  
	                        }
	                }
	            else{
		                //  std::cout << "zmqclient_recv  zmq::poll (&items[0], 1,  100)is timeout ................." << std::endl;  
		                //Ò»¶¨Ê±¼ä·¢ËÍÒ»´ÎÍøÂçÐÄÌ
		             boost::this_thread::interruption_point();  
					   
			}
	      	}
        }
   //     }
   // catch(...){
     //        // std::cout << "zmqclient_recv Interrupt exception was thrown." << std::endl;  
	//      logfile.out_info("zmqclient_recv Interrupt exception was thrown." );
    //    }
} 

void zmqclient::rospubtask(){  

    
} 

void zmqclient::zmqpubtask(){  

    std::cout << "start to thread "<<"["<<__FUNCTION__ << "]"<<std::endl;

    ros::Rate loop_rate(0.5);
    int index = 0;
   try{

           while (ros::ok())
		 {
              loop_rate.sleep();
		center_online.check_connect();
		if(connectflag == true){
				zmq_SendRpc(zmqheart_SendRpc(),ZMQ_NAME_AUTTODOOR);
			}

              //boost::this_thread::sleep(boost::posix_time::seconds(1));
		boost::this_thread::interruption_point();  
		index++;
		if((index>=60)&&(center_online.online==0)){
				logfile.out_info("zmqpubtask: zmq connect heart ....................reconnected ");
				connectflag = false;
				disconnect();
				boost::this_thread::sleep(boost::posix_time::seconds(1));
				//destory();
				connect();
				index = 0;
				
			}
		else if(index>=120){
					
				index = 0;
			}
		
             }
    }
   catch(...){
         //std::cout << "zmqclient_pub Interrupt exception was thrown." << std::endl;  
	  logfile.out_info("zmqpubtask Interrupt exception was thrown." );
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


int zmqclient::unpackage_main(std::string fun)
    {
    
        
     // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;


     PackageMessage msgrecv;
	try{
    			msgrecv.ParseFromString(fun);
		}
      catch(...){
		 
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "ParseFromString a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return -1;  
		    } 

     #if 1
     show_mainmsg(msgrecv);
     #endif
     
       std::string strto=msgrecv.to();
	PackageMessage_Call* callmsg = msgrecv.mutable_callmessage();
	 std::string function=callmsg->function();
	 
     //std::cout<<"strto="<<strto<<std::endl;
     
    // ROS_INFO("strto=%s", strto.c_str());
     if(strto.compare( ZMQ_NAME_AUTTODOOR) ==0){
            //×ª·¢¸øZMQ_NAME_DRIVERCTRL
            //std::ostringstream logstream;
	   // logstream<<"to=["<<strto<< "]"<< "function = "<<function<<std::endl; 
	   // std::string logtext = logstream.str();
          //  logfile.out_info(logtext);
			
            dirv_ros_SendRpc(fun);
	     if(function.compare( MYGEARTRpcTopic) ==0){
		 	//ÊÕµ½ÐÄÌø
		 	center_online.Set_connect();
	      }
          
      }
	return  1; 
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
    //msgrecv.ParseFromString(pack);
	try{
    			msgrecv.ParseFromString(pack);
		}
      catch(...){
		 
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "ParseFromString a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return -1;  
		    } 
	  
      int ret=0;

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
				center_online.Set_connect();
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
   
    PackageMessage msgrecv;
    // msgrecv.ParseFromString(fun);
	try{
    			msgrecv.ParseFromString(fun);
		}
      catch(...){
		 
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "ParseFromString a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return NULL;  
		    } 

    std::string strfrom=msgrecv.to();

    return   strfrom;

}

void zmqclient::dirv_zmq_Callback(const std_msgs::String::ConstPtr& msg)
 {
         #if DUBUG_ECHO
        ROS_INFO("ros sub ->dirv_zmq: [%d]", msg->data.size());
         #endif
         if(connectflag == true){
                std::string to = Get_to(msg->data);
		if(to.size()>2){
                		zmq_SendRpc(msg->data,to);
			}
            }
        
 }
 void zmqclient::weather_update_Callback(const std_msgs::String::ConstPtr& msg)
  {

         //std::string from = Get_from(msg->data);
         if(connectflag == true){
                zmq_SendRpc(msg->data,ZMQ_NAME_SERVER);
         #if 0
         //ROS_INFO("ros sub ->update: [%s][%d]", ZMQ_NAME_SERVER,msg->data.size());
	  std::cout<<"ros sub ->update:"<<"["<<"UploadWeatherInfo]"<<"to ="<<ZMQ_NAME_SERVER<<std::endl;

     		PackageMessage msgrecv;
     		msgrecv.ParseFromString(msg->data);
     		show_mainmsg(msgrecv);
         #endif
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

	packMSg.Clear();
	packMSg.set_sessionid(get_session());
	packMSg.set_from(ZMQ_NAME_AUTTODOOR);
	packMSg.set_to(ZMQ_NAME_AUTTODOOR);
	packMSg.set_time(time(NULL));

	PackageMessage_Call * call = packMSg.mutable_callmessage();
	call->set_function( MYGEARTRpcTopic);

          //ÐòÁÐ»¯µ½ÄÚ´æ
        std::ostringstream stream;
	try{
        	packMSg.SerializeToOstream(&stream);
		}
      catch(...){
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "SerializeToOstream a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return NULL;  
		    } 

        std::string text = stream.str();
  return text;
}




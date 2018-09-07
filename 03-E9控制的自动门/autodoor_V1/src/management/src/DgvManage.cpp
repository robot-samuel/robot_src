#include <string>
#include <iostream>


#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>



#include "DgvManage.h"


#define DUBUG_ECHO 0

Dgv_Manage::Dgv_Manage()
{

	int index=1;
	zmqcmdlist.insert(std::make_pair("HeartBeat",index++ ));
	zmqcmdlist.insert(std::make_pair("ctrlmode_autodoor",index++));
	zmqcmdlist.insert(std::make_pair("getstautes_autodoor",index++));

	   

	ctrl_ID = NONE; 
	AutoDoor_statues = "0" ;
	std::cout << "======Dgv_Manage init======"<<std::endl;

	cmdnew = -1;
	cmdold  = -1;
	AutoDoor_Itatues = 0;
	AutoDoor_oldtatues = 0;    
	
	logfile.init( CONFIGLOGPATH, "manage");
	logfile.out_info("======Dgv_Manage init======");
	 
}

Dgv_Manage::~Dgv_Manage()
 {


}


 void Dgv_Manage::extern_zmq_cmdCall(const std_msgs::String::ConstPtr& msg)
{
                // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
               // GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string cmdstr = msg->data;
	if(cmdstr.size()>30){
			//std::cout << "======extern_zmq_cmdCall======"<<std::endl;
			proc_zmq_recvcmd(cmdstr);
		}
                      
 } 
 void Dgv_Manage::gpio_cmdCall(const dgvmsg::E9gpioS::ConstPtr& msg)
{
	//GPIO给出的所有状态
	 //check_obt(msg);
	if( msg->in[0] == 1){
			AutoDoor_Itatues = 0;
			
			AutoDoor_statues = "1";  //门关
		}
	else if( msg->in[0] == 0){
			AutoDoor_Itatues = 1;
			AutoDoor_statues = "0";  //门开
		}
	
        // std::cout<< "AutoDoor_statues= "<<AutoDoor_statues<<std::endl;              
 } 

void  Dgv_Manage::check_obt(const dgvmsg::E9gpioS::ConstPtr& msg)
{
	std::cout<< "intput1="<<msg->in[0];
	std::cout<< "  intput2="<<msg->in[1];
	std::cout<< " intput3="<<msg->in[2];
	std::cout<< "  intput4="<<msg->in[3];
	std::cout<<std::endl;

}

void Dgv_Manage::th_mainloop()
{
        
    ros::Rate loop_rate(10);
    int satas_count = 0;
    int configup_count = 0;
	std::cout << "======th_mainloop======"<<std::endl;
	
    while (ros::ok()){

			if(AutoDoor_oldtatues  !=  AutoDoor_Itatues){
					ros_pub_ctrl_autodoor_SendRpc(-1);  //门状态发生反转后恢复门的空闲状态
					AutoDoor_oldtatues = AutoDoor_Itatues;
					configup_count = 0;
					cmdnew = -1;
					cmdold  = -1;
				}
			if(cmdnew != cmdold){
					configup_count ++;
					if(configup_count>=50){
						configup_count=0;
						cmdold = cmdnew;
						ros_pub_ctrl_autodoor_SendRpc(-1); 
						cmdnew = -1;
						cmdold  = -1;
						}

				}
        		loop_rate.sleep();
      			
		
            }		

}   

int Dgv_Manage::printPackageMessage(std::string strmsg)
    {
       std::cout << "==================This is PackageMessage"
        <<"message.================"<<std::endl;
       
        PackageMessage pmsg;
        
	try{
    			pmsg.ParseFromString(strmsg);
		}
      catch(...){
		 
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "ParseFromString a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return -1;  
		    } 
	 
         std::cout << "PackageMessage  message{" << std::endl;
        if(pmsg.has_token())
            {
                std::cout << "token: " << pmsg.token()<<std::endl;
            }
        if(pmsg.has_sessionid())
            {
                std::cout << "sessionid: " << pmsg.sessionid()<<std::endl;
            }
        if(pmsg.has_time())
            {
                std::cout << "time: " << pmsg.time()<<std::endl;
            }
        if(pmsg.has_from())
            {
                std::cout << "from: " << pmsg.from()<<std::endl;
            }
        if(pmsg.has_to())
            {
                std::cout << "to: " << pmsg.to()<<std::endl;
            }
        if(pmsg.has_callmessage())
            {
                std::cout << "  call message{" << std::endl;
                if(pmsg.callmessage().has_service())
                   {
                       std::cout << "  service: " << pmsg.callmessage().service()<<std::endl;
                   }
                  if(pmsg.callmessage().has_function())
                   {
                       std::cout << "  function: " << pmsg.callmessage().function()<<std::endl;
                   }
                  if(pmsg.callmessage().parameters_size()!=0)
                   {
                       int size= pmsg.callmessage().parameters_size();
                       for(int index=0;  index<size;index++)
                           {
                                std::cout << "  parameters[" <<index<<"]= "<< pmsg.callmessage().parameters(index)<<std::endl;
                           }  
                   } 

                std::cout << "  }" << std::endl;
            }
        if(pmsg.has_resultmessage())
            {
                std::cout << "  resultmessage{" << std::endl;
                   PackageMessage_Result *presult =
                        pmsg.mutable_resultmessage();
                   if(presult->has_errorcode()){
                        std::cout << "  errorcode=" <<presult->errorcode()<< std::endl;
                    }
                   if(presult->has_resultdata()){
                        std::cout << "  resultdata size=" 
                        <<presult->resultdata().size()<< std::endl;
                    }
             
                std::cout << "  }" << std::endl;
            }


        std::cout << "}" << std::endl;
	return 1; 
    }


int Dgv_Manage::proc_zmq_recvcmd(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::cout <<"["<<__FUNCTION__ << "]"<< "recv cmd"<<std::endl;  
    PackageMessage msgrecv;
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
    
    if(strto.compare( ZMQ_NAME_AUTTODOOR) ==0){
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
	     if(funid!=1){
            		std::cout<<"recv a zmqcmd:"<<fun<<"["<<funid<<"]"<<std::endl;
	     	}
            #endif
            switch(funid){
                    case FUN_HEART:
                        if( ctrl_ID <= NET_USER){

                            }
                        break;

		case FUN_CTRL_AUTODOOR:
			
			
			Zmq_Recv_ctrlmode_autudoor(msgrecv);
			break;
		case FUN_GETSTAUTES_AUTODOOR:
				{
				std::ostringstream logstream;
				logstream<<"recv a zmqcmd:"<<fun<<"["<<funid<<"]"<<std::endl; 
				std::string logtext = logstream.str();
				logfile.out_info( logtext);
				}
			Zmq_Recv_getstautes_autodoor(msgrecv);
			break;
			
            	}
            return funid;
      }                
          return -1;      
}   


int  Dgv_Manage::Zmq_Recv_ctrlmode_autudoor(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
        Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_AUTTODOOR);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//配置灯光开关
	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	if(size >= 1){
		 int cmd=0;
		    try{  
		        	cmd=boost::lexical_cast<int>(pcallv->parameters(0)); // 将字符串转化为整数  
		    	}  
		    catch(boost::bad_lexical_cast&)    // 转换失败会抛出一个异常  
		    {  
		        cmd=0;   
		    } 
		//cmd_IRQ = cmd;
		ros_pub_ctrl_autodoor_SendRpc(cmd);
		cmdnew = cmd;
		}
          //序列化到内存
        std::ostringstream stream;
	try{
        	MSGSnd.SerializeToOstream(&stream);
		}
      catch(...){
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "SerializeToOstream a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return -1;  
		    } 

        std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_getstautes_autodoor(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        std::cout <<"["<<__FUNCTION__ << "]"<< "recv getstautes cmd"<<std::endl; 
	//装配状态数据 根据车体状态判断是否响应此命令
	Result->set_errorcode(0);
        // Result->clear_resultdata();
        Result->set_resultdata(AutoDoor_statues);   //门状态

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_AUTTODOOR);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//配置灯光开关
	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	pcallv ->set_function( "getstautes_autodoor");

	
          //序列化到内存
        std::ostringstream stream;
	try{
        	MSGSnd.SerializeToOstream(&stream);
		}
      catch(...){
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "SerializeToOstream a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return -1;  
		    } 

        std::string text = stream.str();
    
  // printPackageMessage(text);
    //发送出去
    ros_zmqreturn_SendRpc(text);
	
    return (int)(Result->errorcode());	

	
}
void Dgv_Manage::ros_zmqreturn_SendRpc(std::string msg)
{   
    //ros::NodeHandle npub;
    // ros::Publisher zmq_pub = npub.advertise<std_msgs::String>("dirv_recvcmd", 1);
   
    std_msgs::String stdmsg;
     stdmsg.data = msg;
     #if DUBUG_ECHO
    ROS_INFO("ros pub: to=%s,size= [%d]", "zmqreturn",stdmsg.data.size());
     #endif
     rospub_mode_zmqreturn.publish(stdmsg);
     ros::spinOnce();
}

void Dgv_Manage::ros_pub_ctrl_autodoor_SendRpc( int  cmd){


   
	dgvmsg::E9gpioCtrl cmdmsg;
	cmdmsg.out1 = cmd;
	rospub_ctrl_autodoor.publish(cmdmsg);
	ros::spinOnce();
}



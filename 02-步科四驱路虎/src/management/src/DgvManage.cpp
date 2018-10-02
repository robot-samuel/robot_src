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
            zmqcmdlist.insert(std::make_pair("gettoken",index++));
            zmqcmdlist.insert(std::make_pair( "getallstatues",index++));  
            zmqcmdlist.insert(std::make_pair( "getspeed",index++));  
            zmqcmdlist.insert(std::make_pair("ctrlforward",index++));    
            zmqcmdlist.insert(std::make_pair("ctrlbackward",index++));   
            zmqcmdlist.insert(std::make_pair("ctrlturnleft",index++));   
            zmqcmdlist.insert(std::make_pair( "ctrlturnright",index++));   
            zmqcmdlist.insert(std::make_pair("ctrlstop",index++));     //cmdstop
            zmqcmdlist.insert(std::make_pair("ctrlstopfree",index++)); //stop run=0
            zmqcmdlist.insert(std::make_pair("ctrlrun-speed-angle",index++));   
            zmqcmdlist.insert(std::make_pair("ctrlrun-left-right",index++));    
            zmqcmdlist.insert(std::make_pair("dissolutionalarm",index++));   
            zmqcmdlist.insert(std::make_pair("settoken",index++));   
            zmqcmdlist.insert(std::make_pair("configcar",index++));   
            zmqcmdlist.insert(std::make_pair("getcarconfig",index++));  
            zmqcmdlist.insert(std::make_pair("Dcupdataall",index++)); 
            
           zmqcmdlist.insert(std::make_pair("ctrlRealse",index++));
           zmqcmdlist.insert(std::make_pair("ctrlEnable",index++));
           zmqcmdlist.insert(std::make_pair("ClearError",index++));
           zmqcmdlist.insert(std::make_pair("EmergencyStop",index++)); //   

	cmd_speedVelocity=500; //��λmm
	cmd_angelVelocity=100;  //��λmm
	net_left=500;  //��λmm
	net_right=500;  //��λmm

	ctrl_ID = NONE;

	

           
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
 void Dgv_Manage::uartCtrl_ros_cmdCall(const dgvmsg::ctrl_ModeMessage::ConstPtr& msg)
{
    statues.User232Node.Set_connect();
	statues.UartCmdMsg.online = msg->online;
	statues.UartCmdMsg.twist.linear.x = msg->twist.linear.x;   //前进速度
	statues.UartCmdMsg.twist.linear.y = 0.0;   //
	statues.UartCmdMsg.twist.linear.z=  0.0;
	// heading driven by left joysticj left and right
	statues.UartCmdMsg.twist.angular.x = msg->twist.angular.x;  //转向速度
	//statues.UartCmdMsg.twist.angular.x = 0.0;
	statues.UartCmdMsg.twist.angular.y =  0.0;
	statues.UartCmdMsg.twist.angular.z =  0.0;
	
	#if 0
	statues.print_UartCmdMsg(msg);
	#endif
	
	
	if((ctrl_ID <= RS232_USER)&&(statues.UartCmdMsg.online == YES)){
		statues.User232connect = YES;
		statues.User232_count=0; 
		}
	else if((statues.UartCmdMsg.online == NO)){
		statues.User232connect = NO;
		statues.User232_count=0; 
		}
 } 

 
 void Dgv_Manage::Drivers_ros_cmdCall(const std_msgs::String::ConstPtr& msg)
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string cmdstr = msg->data;
	//printf("thread_navi:==%s==%d\n",__FUNCTION__,cmdstr.size());
	if(cmdstr.size()>20){
		
    		ControllerMessage_Dirver DriverInforecv;
    		DriverInforecv.ParseFromString(cmdstr);
		if(DriverInforecv.device_add() == DEV_DIR_ADD_LF){
			//std::cout<<"DEV_DIR_ADD_LF"<<std::endl;
			statues.Dirver[0].Clear();
			statues.Dirver[0].CopyFrom(DriverInforecv);
			statues.driverNode[0].Set_connect();
			statues.strDirver[0]=cmdstr;
		}
		else if(DriverInforecv.device_add() == DEV_DIR_ADD_RF){
			//std::cout<<"DEV_DIR_ADD_RF"<<std::endl;
			statues.Dirver[1].Clear();
			statues.Dirver[1].CopyFrom(DriverInforecv);
			statues.driverNode[1].Set_connect();
			statues.strDirver[1]=cmdstr;
		}
		else if(DriverInforecv.device_add() == DEV_DIR_ADD_LB){
			//std::cout<<"DEV_DIR_ADD_LB"<<std::endl;
			statues.Dirver[2].Clear();
			statues.Dirver[2].CopyFrom(DriverInforecv);
			statues.driverNode[2].Set_connect();
			statues.strDirver[2]=cmdstr;
		}
		else if(DriverInforecv.device_add() == DEV_DIR_ADD_RB){
			//std::cout<<"DEV_DIR_ADD_RB"<<std::endl;
			statues.Dirver[3].Clear();
			statues.Dirver[3].CopyFrom(DriverInforecv);
			statues.driverNode[3].Set_connect();
			statues.strDirver[3]=cmdstr;
		}		
	}
	                   
 } 



 void Dgv_Manage::print_joymsg(const sensor_msgs::Joy::ConstPtr& joy)
    {
            
            std::string buttonsstr;
            for(int index=0;index< joy->buttons.size();index++){
                    boost::format fmt("buttons[%d]=%f");
                    fmt %index;
                    fmt % joy->buttons[index];
                    buttonsstr+=fmt.str()+",";
                }
            //std::cout<<str<<std::endl;str.clear();
             std::string axesstr;
            for(int index=0;index< joy->axes.size();index++){
                    boost::format fmt("axes[%d]=%f");
                    fmt %index;
                    fmt %joy->axes[index];
                    axesstr+=fmt.str()+",";
                } 
            
              boost::format fmt("[%1%]");
              fmt %joy->header.stamp;
              
            std::cout<<fmt<<" "<<buttonsstr<<" "<<"axesstr"<<std::endl;
            // ROS_INFO("%s %s\n%s",fmt.str().c_str(),buttonsstr.c_str(),axesstr.c_str());
}
 
void Dgv_Manage:: joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
           double scale = 1. / (1. - 0.05) / 32767.;
           // print_joymsg(joy);
            if(ctrl_ID <=USBREMOTE_JOY){
          // X vel driven by left joystick for and aft
          statues.Joycmd_velocity.linear.x = joy->axes[1]*-1;
          statues.Joycmd_velocity.linear.y =  0.0;
          statues.Joycmd_velocity.linear.z=  0.0;
          // heading driven by left joysticj left and right
          statues.Joycmd_velocity.angular.x = joy->axes[3]*-1;
          statues.Joycmd_velocity.angular.y =  0.0;
          statues.Joycmd_velocity.angular.z = 0;
          //Joycmd_velocity.angular.z =  scale*joy->axes[6];
                }
            else{
                statues.Joycmd_velocity.linear.x = 0.0;
                statues.Joycmd_velocity.linear.y = 0.0;
                statues.Joycmd_velocity.linear.z=  0.0;
                // heading driven by left joysticj left and right
                statues.Joycmd_velocity.angular.x = 0.0;
                statues.Joycmd_velocity.angular.y =  0.0;
                statues.Joycmd_velocity.angular.z =  0.0;
                }
			
            if(joy->buttons[START] == YES){
                    //ctrl_ID = REMOTE_JOY;
                    statues.joyconnect = YES;
                }

            if(fabs(statues.Joycmd_velocity.linear.x)<0.1 ){
			statues.Joycmd_velocity.linear.x= 0.0;
                }
            if(fabs(statues.Joycmd_velocity.angular.x)<0.1 ){
			statues.Joycmd_velocity.angular.x = 0.0;
                }

          //print_joymsg(joy);
          statues.joystick_count =0;
          #if 0
          ROS_INFO("I heard SPEED =%f   ANGEL=%f",scale*joy->axes[1] , 
          scale*joy->axes[3] );
          #endif 

}
float Dgv_Manage::check_jody_speed(int id,float cmd)
{

	float velocity=0;
	if(id == 0){
		//speed
		velocity = cmd  ;
		velocity*=((de_JODYMAXSPEED*1.0)/(32767*1.0));	
		if(fabs(velocity)>de_JODYMAXSPEED){
			if(velocity>0){
				velocity = de_JODYMAXSPEED;
				}
			else if(velocity<0){
				velocity = 0-de_JODYMAXSPEED;
				}
			return velocity;
			}
	}
	else{
		//angle
		velocity = cmd  ;
		velocity*=((de_JODYMAXANGEL*1.0)/(32767*1.0));	
		if(fabs(velocity)>de_JODYMAXANGEL){
			if(velocity>0){
				velocity = de_JODYMAXANGEL;
				}
			else if(velocity<0){
				velocity = 0-de_JODYMAXANGEL;
				}
			return velocity;
			}
		}
	
	return velocity;
}

int Dgv_Manage::ros_pub_joy_autorun()
{
    geometry_msgs::Twist dirvcmd_velocity; 

     dirvcmd_velocity = statues.Joycmd_velocity;

     
     float  speed =check_jody_speed(0,dirvcmd_velocity.linear.x);
     float angel  =check_jody_speed(1,dirvcmd_velocity.angular.x);
	 
     float LEFT = (speed-angel/2);
     float RIGHT = (speed+angel/2);
	 
     int leftrpm=calculate_rpm_speed(DEV_DIR_ADD_LF,LEFT);
     int rightrpm=calculate_rpm_speed(DEV_DIR_ADD_RF,RIGHT); 
     ros_LaserCtrl_SendRpc("",leftrpm,rightrpm);
}

int Dgv_Manage::ros_pub_RS232_autorun()
{
    dgvmsg::ctrl_ModeMessage dirvcmd_velocity; 

     dirvcmd_velocity = statues.UartCmdMsg;

     
     float  speed =check_UartCtrl_speed(0,dirvcmd_velocity.twist.linear.x);
     float angel  =check_UartCtrl_speed(1,dirvcmd_velocity.twist.angular.x);
	 
     float LEFT = (speed-angel/2);
     float RIGHT = (speed+angel/2);
	 
     int leftrpm=calculate_rpm_speed(DEV_DIR_ADD_LF,LEFT);
     int rightrpm=calculate_rpm_speed(DEV_DIR_ADD_RF,RIGHT); 
	 
     ros_LaserCtrl_SendRpc("",leftrpm,rightrpm);
}

float Dgv_Manage::check_UartCtrl_speed(int id,float cmd)
{

	float velocity=0;
	if(id == 0){
		//speed 
		velocity = cmd  ;
		velocity*=1000.0;	
		if(fabs(velocity)>de_CTRLMAXSPEED){
			if(velocity>0){
				velocity = de_CTRLMAXSPEED;
				}
			else if(velocity<0){
				velocity = 0-de_CTRLMAXSPEED;
				}
			return velocity;
			}
	}
	else{ 
		//angle 
		velocity = cmd  ;
		velocity*=1000.0;	
		if(fabs(velocity)>de_CTRLMAXANGLE){
			if(velocity>0){
				velocity = de_CTRLMAXANGLE;
				}
			else if(velocity<0){
				velocity = 0-de_CTRLMAXANGLE;
				}
			return velocity;
			}
		}
	
	return velocity;
}


int Dgv_Manage::check_connect()
{
	//printf("thread_navi:==%s==\n",__FUNCTION__);
	//joystick
               if(statues.joyconnect == YES){
                  statues.joystick_count++;
                  if(statues.joystick_count >=90){             
                         //����һ�γ�ʱ����
                        #if DUBUG_ECHO
                         ROS_INFO("joystick is losted **********************************");
                        #endif
                         statues.joystick_count =0;
                         statues.joyconnect = NO;
     
                       statues.Joycmd_velocity.linear.x=0;
                       statues.Joycmd_velocity.angular.x=0;
			ros_Net2Ctrl_SendRpc("",0,0);
                         //ctrl_ID = NONE;
                         
                     }
                }
              else{
                    statues.joystick_count =0;
                }
	//Remote	  
                if(statues.Remoteconnect== YES){
                  statues.Remote_count++;
                  if(statues.Remote_count >=90){             
                         //����һ�γ�ʱ����
                        #if 1
                         ROS_INFO("Remoteconnect is losted **********************************");
                        #endif
                         statues.Remote_count =0;
                         statues.Remoteconnect = NO;
     			   // statues.msg_init();
			  ros_Net2Ctrl_SendRpc("",0,0);
                         //ctrl_ID = NONE;
                         
                     }
                }
              else{
                    statues.Remote_count =0;
		   //  statues.msg_init();
                }
	//User232	  
	if(statues.User232connect== YES){
		statues.User232_count++;
		if(statues.User232_count>90){
			statues.User232_count=0;
			statues.User232connect = NO;
                        #if DUBUG_ECHO
                         ROS_INFO("Laser Ctrl is losted **********************************");
                        #endif
			}
		}
	else{
		statues.User232_count=0;
		}

	//joystick
	if(statues.joyconnect == YES){
		if(ctrl_ID<=USBREMOTE_JOY){
			ctrl_ID = USBREMOTE_JOY;
			return ctrl_ID;
			}
		}
	//Remote
	if(statues.Remoteconnect == YES){
		if(ctrl_ID<=REMOTE_JOY){
			ctrl_ID = REMOTE_JOY;
			return ctrl_ID;
			}
		}
	//User232
	if(statues.User232connect== YES){
		if(ctrl_ID<=RS232_USER){
			ctrl_ID = RS232_USER;
			return ctrl_ID;
			}
		}
	//NET
	if(statues.center_isconnect == YES){
		if(ctrl_ID<=NET_USER){
			ctrl_ID = NET_USER;
			return ctrl_ID;
			}
		}
	ctrl_ID = NONE;
	return ctrl_ID;
	
}
int Dgv_Manage::is_carcanbectrl()
{
	if(!statues.is_CanContenct()){
			
			return 0;	
		}
	if(!statues.is_DriverOK()){
			
			return 0;	
		}
    return 1;
      
}

void Dgv_Manage::th_mainloop()
{
        
    ros::Rate loop_rate(de_CTRLRATE);
    int satas_count = 0;
    int configup_count = 0;
   // printf("thread_navi:==%s==\n",__FUNCTION__);	
    while (ros::ok()){
        loop_rate.sleep();
        check_connect();
        satas_count++;
        configup_count++;
        
        //update all satues per 1s
        if(ctrl_ID == NET_USER){
			
		//std::cout<<"ctrl_ID == NET_USER car_run_ptr="<<car_run_ptr<<std::endl;
                switch(car_run_ptr){
                        case CAR_CTRL_FORWARD:
                          if(is_carcanbectrl()){
				ros_Net2Ctrl_SendRpc("",cmd_speedVelocity,0);
                            }
                          else{
                                car_run_ptr =CAR_CTRL_DEFAUL; 
                            }
                            break;
                        case CAR_CTRL_BACKWARD:
                            if(is_carcanbectrl()){
				ros_Net2Ctrl_SendRpc("",cmd_speedVelocity,0);
                                }
                            else{
                                car_run_ptr =CAR_CTRL_DEFAUL; 


                              }

                            break;
                        case CAR_CTRL_TURNLEFT:
                              if(is_carcanbectrl()){
					ros_Net2Ctrl_SendRpc("",0,cmd_angelVelocity);
                                }
                          else{
                              car_run_ptr =CAR_CTRL_DEFAUL; 


                            }                            
                            break;
                        case CAR_CTRL_TURNRIGHT:
                             if(is_carcanbectrl()){
					ros_Net2Ctrl_SendRpc("",0,cmd_angelVelocity);
                                }
                           else{
                               car_run_ptr =CAR_CTRL_DEFAUL; 


                            }                           
                            break;
                        case CAR_CTRL_STOPFREE:
				ros_Net2Ctrl_SendRpc("",0,0);
                            break;
                        case CAR_CTRL_RUN_SPEED_ANGLE:
                              if(is_carcanbectrl()){
							  	
                                   ros_Net2Ctrl_SendRpc("",cmd_speedVelocity,cmd_angelVelocity);
                                }
                            else{
                               car_run_ptr =CAR_CTRL_DEFAUL; 


                            }                                
                            break;
                        case CAR_CTRL_RUN_LEFT_RIGHT:
                            if(is_carcanbectrl()){
                                     ros_Net2Ctrl_SendRpc("",net_left,net_right);
                                }
                            else{
                                car_run_ptr =CAR_CTRL_DEFAUL; 
                            
                            
                             }   

                            break;
                         case CAR_CTRL_STOP:
                              // if(is_carcanbectrl()){
                                {
                                    std::string cmd="CAR_CTRL_STOP";
					ros_Net2Ctrl_SendRpc(cmd,0,0);
                                   
                                }
                               // }
                              break;
                          case CAR_CTRL_EmergencyStop:
                             //if(is_carcanbectrl()){
                                {
                                std::string cmd="CAR_CTRL_EmergencyStop";
				  ros_Net2Ctrl_SendRpc(cmd,0,0);
                                //if(statues.check_isstop()){
                                       // car_run_ptr =CAR_CTRL_DEFAUL; 
                                    //}
                                }
                               // }
                             break;
                             case CAR_CTRL_Realse_Loop:
                                //if(is_carcanbectrl()){
                                    {
                                   std::string cmd="CAR_CTRL_Realse_Loop";
					ros_Net2Ctrl_SendRpc(cmd,0,0);
					
                                  // if(statues.check_isrelease()){
                                          // car_run_ptr =CAR_CTRL_DEFAUL; 
                                      // }
                                    }

                                 break;
                               case CAR_CTRL_Enable_Loop:
                                   // if(is_carcanbectrl())
                                   {
                                        
                                       std::string cmd="CAR_CTRL_Enable_Loop";
                                     ros_Net2Ctrl_SendRpc(cmd,0,0);
                                        //ros_pub_net_Scmd(PackScmd(cmd,par));
                                       //if(statues.check_isenable()){
                                               //car_run_ptr =CAR_CTRL_DEFAUL; 
                                          // }
                                       }
                                 
                                     break;
                                     case CAR_CTRL_ClearError:
                                         // if(is_carcanbectrl()){
                                            {
                                            std::string cmd="CAR_CTRL_ClearError";
                                            ros_Net2Ctrl_SendRpc(cmd,0,0);

                                            }
                                       break;
                         default :
                            cmd_angelVelocity = 0;
                            cmd_speedVelocity = 0;
                            ros_Net2Ctrl_SendRpc("",cmd_angelVelocity,cmd_speedVelocity);
                  //  car_run_ptr =CAR_CTRL_DEFAUL;                           
                            break;
                    }

            }
            else if(ctrl_ID == USBREMOTE_JOY){
				#if DUBUG_ECHO
		         std::cout<<"ctrl_ID == USBREMOTE_JOY"<<std::endl;
				#endif
                        car_run_ptr =CAR_CTRL_DEFAUL; 
                        if(is_carcanbectrl()){
	                       
				 ros_pub_joy_autorun();
	                        cmd_angelVelocity = 0;
	                        cmd_speedVelocity = 0;
				 //statues.JoycmdClear();
                            }
                        else{
	                             cmd_angelVelocity = 0;
	                            	cmd_speedVelocity = 0;
					statues.JoycmdClear();
	                             	ros_pub_joy_autorun();
                            #if 0
                                ROS_INFO("can not ctrl ros pub: to=%s,linear.x = [%f] linear.y=[%f]", "driver/joycmd",Joycmd_velocity.linear.x,Joycmd_velocity.linear.y);
                            #endif
                            }
                    }
		else if(ctrl_ID == RS232_USER){
                       #if DUBUG_ECHO
                        std::cout<<"ctrl_ID == RS232_USER"<<std::endl;
                        #endif
			  car_run_ptr =CAR_CTRL_DEFAUL; 
                        if(is_carcanbectrl()){
                        	ros_pub_RS232_autorun();
                        	cmd_angelVelocity = 0;
                       	 cmd_speedVelocity = 0;
                            }
			else{
                        	cmd_angelVelocity = 0;
                       	 cmd_speedVelocity = 0;
				statues.UartCmdClear();

                            	}
			}
            else{
                        #if DUBUG_ECHO
                        std::cout<<"ctrl_ID == NONE"<<std::endl;
                        #endif
                        car_run_ptr =CAR_CTRL_DEFAUL; 
                        cmd_angelVelocity = 0;
                        cmd_speedVelocity = 0; 
						
			statues.JoycmdClear();

                }

            if((configup_count%5)==0){
                    //upcarconfig();
                    configup_count=0;
                }
            
		
		for(int index=0;index<Dgv_statues::DRIVER_AMOUNT;index++){
            		statues.driverNode[index].check_connect();
	    	}

            }		

}   

void Dgv_Manage::printPackageMessage(std::string strmsg)
    {
       std::cout << "==================This is PackageMessage"
        <<"message.================"<<std::endl;
       
        PackageMessage pmsg;
        
        pmsg.ParseFromString(strmsg);
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
                       std::cout << "  service: " << pmsg.callmessage().
service()<<std::endl;
                   }
                  if(pmsg.callmessage().has_function())
                   {
                       std::cout << "  function: " << pmsg.callmessage().
function()<<std::endl;
                   }
                  if(pmsg.callmessage().parameters_size()!=0)
                   {
                       int size= pmsg.callmessage().parameters_size();
                       for(int index=0;  index<size;index++)
                           {
                                std::cout << "  parameters[" <<index<<"]= "<< 
pmsg.callmessage().parameters(index)<<std::endl;
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
    }


int Dgv_Manage::proc_zmq_recvcmd(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    PackageMessage msgrecv;
    msgrecv.ParseFromString(pack);
      int ret=0;
    std::string strto=msgrecv.to();
    
    if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //ת����ZMQ_NAME_DRIVERCTRL
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
                            statues.center_isconnect = YES;
                            statues.center_count=0; 
                            }
                        statues.ZmqTransNode.Set_connect();
                        break;
		    case FUN_CTRL_FORWARD:
			ret=Zmq_Recv_CAR_FORWARD(msgrecv);
                        if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_FORWARD;
                            }
			break;
		    case FUN_CTRL_BACKWARD:
				ret=Zmq_Recv_CAR_BACKWARD(msgrecv);
                            if(ret==0){
                            //
                                car_run_ptr = CAR_CTRL_BACKWARD;
                            }
			break;
		    case FUN_CTRL_TURNLEFT:
				Zmq_Recv_CAR_TURNLEFT(msgrecv);
                        if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_TURNLEFT;
                            }
			break;
		    case FUN_CTRL_TURNRIGHT:
				Zmq_Recv_CAR_TURNRIGHT(msgrecv);
                   if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_TURNRIGHT;
                            }  
			break;
		    case FUN_CTRL_STOP:
				Zmq_Recv_CAR_STOP(msgrecv);
                            car_run_ptr = CAR_CTRL_STOP;
			break;
		    case FUN_CTRL_STOPFREE:
				Zmq_Recv_CAR_STOPFREE(msgrecv);
				car_run_ptr = CAR_CTRL_STOPFREE;  
			break;
		    case FUN_CTRL_RUN_SPEED_ANGLE:
				Zmq_Recv_SPEED_ANGLE(msgrecv);
                         if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_RUN_SPEED_ANGLE;
                            }   
			break;
		    case FUN_Realse_Loop:
				Zmq_Recv_CAR_Realse_Loop(msgrecv);
				car_run_ptr = CAR_CTRL_Realse_Loop;  
			break;
		    case FUN_Enable_Loop:
				Zmq_Recv_CAR_Enable_Loop(msgrecv);
				car_run_ptr = CAR_CTRL_Enable_Loop;  
			break;
		    case FUN_ClearError:
				Zmq_Recv_CAR_ClearError(msgrecv);
				car_run_ptr = CAR_CTRL_ClearError;  
			break;
		    case FUN_EmergencyStop:
				Zmq_Recv_CAR_EmergencyStop(msgrecv);
				car_run_ptr = CAR_CTRL_EmergencyStop;  
			break;
			
            	}
            return funid;
      }                
          return -1;      
}   

int Dgv_Manage::proc_ros_recvcmd(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    PackageMessage msgrecv;
    msgrecv.ParseFromString(pack);
      int ret=0;
      #if DUBUG_ECHO
   ROS_INFO("proc_ros_recvcmd");
      #endif
    std::string strto=msgrecv.to();
    
    if(strto.compare( ZMQ_NAME_DRIVERCTRL) ==0){
            //ת����ZMQ_NAME_DRIVERCTRL
            PackageMessage_Call* callmsg = msgrecv.mutable_callmessage();
            std::string fun=callmsg->function();
            std::map<std::string,int >::iterator it = zmqcmdlist.find(fun); 
            if( it == zmqcmdlist.end() ) {
                    std::cout<<"we can not find function name:"<<fun<<std::endl;
                    return -1;
                } 
         
            int funid= (int)(it->second);
            #if 1
            std::cout<<"recv a roscmd:"<<fun<<"["<<funid<<"]"<<std::endl;
            #endif
    	}
          return -1;      
}   


std::string Dgv_Manage::get_session(void)
{
        
        boost::uuids::random_generator rgen;
        boost::uuids::uuid u= rgen();
        std::string str;
        assert(boost::uuids::uuid::static_size() == 16);
        assert(u.size() == 16);
        std::stringstream ss;
        ss << u; //uuid������ַ�����
        ss>>str;//�ַ�����������ַ�������
        // std::cout<<"uuid="<<str<<std::endl;
        return str;
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

void Dgv_Manage::ros_zmqupdate_SendRpc(std::string msg)
{   
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION; 
	PackageMessage packMSg;
	packMSg.Clear();
	packMSg.set_sessionid(get_session());
	packMSg.set_from(ZMQ_NAME_DRIVERCTRL);
	packMSg.set_to(ZMQ_NAME_ALLNODE);

	PackageMessage_Call * call = packMSg.mutable_callmessage();
	call->set_function( UPDATE_FUN_NAME);
	call->add_parameters(msg);

    std::string res;
       if (!packMSg.SerializeToString(&res)  ) {
           std::cerr << "Failed to write msg ControllerMessage." << std::endl; 
          // return ""; 
       }  	
	
    std_msgs::String stdmsg;
     stdmsg.data = res;
     #if DUBUG_ECHO
    ROS_INFO("ros pub: to=%s,size= [%d]", "dirv_Dcupdataall",stdmsg.data.size());
     #endif
     rospub_mode_uodateall.publish(stdmsg);
     ros::spinOnce();
}

void Dgv_Manage::ros_Net2Ctrl_SendRpc(std::string scmd,int speed,int angel)
{   
	//ros::NodeHandle npub;
	#if 0
	std::cout<<"scmd="<<scmd<<std::endl;
	std::cout<<"speed="<<speed<<std::endl;
	std::cout<<"angel="<<angel<<std::endl;
	#endif
	
	if(scmd == ""){
		//trans_4_velocity(speed,angel);
		transSA_4_velocity(speed,angel);
		dgvmsg::DriverVelocity Motec;
		Motec.driver.resize(4);
		
	#if 0
	std::cout<<"velocity[0]="<<velocity[0]<<std::endl;
	std::cout<<"velocity[1]="<<velocity[1]<<std::endl;
	std::cout<<"velocity[2]="<<velocity[2]<<std::endl;
	std::cout<<"velocity[3]="<<velocity[3]<<std::endl;
        #endif
	
		Motec.driver[0].name="DEV_DIR_ADD_LF";
		Motec.driver[0].add = DEV_DIR_ADD_LF;
		Motec.driver[0].VRPM = velocity[0];

		Motec.driver[1].name="DEV_DIR_ADD_LB";
		Motec.driver[1].add = DEV_DIR_ADD_LB;
		Motec.driver[1].VRPM = velocity[1];

		Motec.driver[2].name="DEV_DIR_ADD_RF";
		Motec.driver[2].add = DEV_DIR_ADD_RF;
		Motec.driver[2].VRPM = velocity[2];

		Motec.driver[3].name="DEV_DIR_ADD_RB";
		Motec.driver[3].add = DEV_DIR_ADD_RB;
		Motec.driver[3].VRPM = velocity[3];

		rospub_mode_net2ctrl.publish(Motec);
		}
	else{
			std_msgs::String stdmsg;
			
			std::string Sdrvcmd=PackScmd(scmd,"");
			stdmsg.data = Sdrvcmd;
			 rospub_mode_net2Sctrl.publish(stdmsg);
		}
     
     ros::spinOnce();
}
void Dgv_Manage::ros_LaserCtrl_SendRpc(std::string scmd,int speed,int angel)
{   
	//ros::NodeHandle npub;
	#if 0
	std::cout<<"scmd="<<scmd<<std::endl;
	std::cout<<"speed="<<speed<<std::endl;
	std::cout<<"angel="<<angel<<std::endl;
	#endif
	
	if(scmd == ""){
		trans_4_velocity(speed,angel);
		//transSA_4_velocity(speed,angel);
		dgvmsg::DriverVelocity Motec;
		Motec.driver.resize(4);
		
	#if 0
	std::cout<<"velocity[0]="<<velocity[0]<<std::endl;
	std::cout<<"velocity[1]="<<velocity[1]<<std::endl;
	std::cout<<"velocity[2]="<<velocity[2]<<std::endl;
	std::cout<<"velocity[3]="<<velocity[3]<<std::endl;
        #endif
	
		Motec.driver[0].name="DEV_DIR_ADD_LF";
		Motec.driver[0].add = DEV_DIR_ADD_LF;
		Motec.driver[0].VRPM = velocity[0];

		Motec.driver[1].name="DEV_DIR_ADD_LB";
		Motec.driver[1].add = DEV_DIR_ADD_LB;
		Motec.driver[1].VRPM = velocity[1];

		Motec.driver[2].name="DEV_DIR_ADD_RF";
		Motec.driver[2].add = DEV_DIR_ADD_RF;
		Motec.driver[2].VRPM = velocity[2];

		Motec.driver[3].name="DEV_DIR_ADD_RB";
		Motec.driver[3].add = DEV_DIR_ADD_RB;
		Motec.driver[3].VRPM = velocity[3];

		rospub_mode_net2ctrl.publish(Motec);
		}
	else{
			std_msgs::String stdmsg;
			
			std::string Sdrvcmd=PackScmd(scmd,"");
			stdmsg.data = Sdrvcmd;
			 rospub_mode_net2Sctrl.publish(stdmsg);
		}
     
     ros::spinOnce();
}
//�ٶȷ���
void Dgv_Manage::trans_4_velocity( float speed, float angel)
{
	float leftspeed;
	float rightspeed;
	//LEFT
	//leftspeed = (speed-angel/2);
	leftspeed = (speed);
       //LF
       
       //velocity[0]=calculate_rpm_speed(DEV_DIR_ADD_LF,leftspeed);
       velocity[0] = leftspeed;
         if(abs(velocity[0])>de_MAXRPM){
                 if((velocity[0])>0){
                        velocity[0]=de_MAXRPM;
                    }
                 else{
                        velocity[0]=0-de_MAXRPM;
                    }
            }
	
       //velocity[1]=calculate_rpm_speed(DEV_DIR_ADD_LB,leftspeed);
         velocity[1] = leftspeed;
         if(abs(velocity[1])>de_MAXRPM){
                 if((velocity[1])>0){
                        velocity[1]=de_MAXRPM;
                    }
                 else{
                        velocity[1]=0-de_MAXRPM;
                    }
            }	

	//RIGHT
	
	//rightspeed = (speed+angel/2);
	rightspeed =angel;
       //velocity[2]=calculate_rpm_speed(DEV_DIR_ADD_RF,rightspeed);
       velocity[2]=rightspeed;
         if(abs(velocity[2])>de_MAXRPM){
                 if((velocity[2])>0){
                        velocity[2]=de_MAXRPM;
                    }
                 else{
                        velocity[2]=0-de_MAXRPM;
                    }
            }
	
       //velocity[3]=calculate_rpm_speed(DEV_DIR_ADD_RB,rightspeed);
       velocity[3]=rightspeed;
         if(abs(velocity[3])>de_MAXRPM){
                 if((velocity[3])>0){
                        velocity[3]=de_MAXRPM;
                    }
                 else{
                        velocity[3]=0-de_MAXRPM;
                    }
            }	
          

}
void Dgv_Manage::transSA_4_velocity( float speed, float angel)
{
	float leftspeed;
	float rightspeed;
	//LEFT
	leftspeed = (speed-angel/2);
	//leftspeed = (speed);
       //LF
       
       velocity[0]=calculate_rpm_speed(DEV_DIR_ADD_LF,leftspeed);
       //velocity[0] = leftspeed;
         if(abs(velocity[0])>de_MAXRPM){
                 if((velocity[0])>0){
                        velocity[0]=de_MAXRPM;
                    }
                 else{
                        velocity[0]=0-de_MAXRPM;
                    }
            }
	
       velocity[1]=calculate_rpm_speed(DEV_DIR_ADD_LB,leftspeed);
         //velocity[1] = leftspeed;
         if(abs(velocity[1])>de_MAXRPM){
                 if((velocity[1])>0){
                        velocity[1]=de_MAXRPM;
                    }
                 else{
                        velocity[1]=0-de_MAXRPM;
                    }
            }	

	//RIGHT
	
	rightspeed = (speed+angel/2);
	//rightspeed =angel;
       velocity[2]=calculate_rpm_speed(DEV_DIR_ADD_RF,rightspeed);
       //velocity[2]=rightspeed;
         if(abs(velocity[2])>de_MAXRPM){
                 if((velocity[2])>0){
                        velocity[2]=de_MAXRPM;
                    }
                 else{
                        velocity[2]=0-de_MAXRPM;
                    }
            }
	
       velocity[3]=calculate_rpm_speed(DEV_DIR_ADD_RB,rightspeed);
       //velocity[3]=rightspeed;
         if(abs(velocity[3])>de_MAXRPM){
                 if((velocity[3])>0){
                        velocity[3]=de_MAXRPM;
                    }
                 else{
                        velocity[3]=0-de_MAXRPM;
                    }
            }	
          

}

int Dgv_Manage::calculate_rpm_speed(char dev_add, float speed)
{
       float rpm=0;
	 if(dev_add == DEV_DIR_ADD_LF){
       //LF
    	 rpm = (float)( 
        (speed/(1000.0*deLF_WheelSsize))*(de_ReductionRatiol*60));
        }
      else if(dev_add ==DEV_DIR_ADD_LB){
	  	//LB
          	rpm = (float)( 
              (speed/(1000.0*deLB_WheelSsize))*(de_ReductionRatiol*60));

        }
      else if(dev_add ==DEV_DIR_ADD_RF){
	  	//RF
          rpm = (float)( 
              (speed/(1000.0*deRF_WheelSsize))*(de_ReductionRatiol*60));

        }
      else if(dev_add ==DEV_DIR_ADD_RB){
	  	//RB
          rpm = (float)( 
              (speed/(1000.0*deRB_WheelSsize))*(de_ReductionRatiol*60));

        }

    if(fabs(rpm)<5){rpm=0;}
    return (int)rpm;
}

std::string Dgv_Manage::PackScmd(std::string cmd,std::string par)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION; 
    
    PackageMessage msg;
     msg.set_sessionid(get_session());
     msg.set_from("manager");
     msg.set_to("driver");
     msg.set_time( time(NULL));
	 
     PackageMessage_Call* callmsg = msg.mutable_callmessage();
     callmsg->set_function( cmd);
     callmsg->add_parameters(par);

   
   //���л����ڴ�
   std::ostringstream stream;
   msg.SerializeToOstream(&stream);
   
   std::string ctext = stream.str();
   
  return ctext;
}


int  Dgv_Manage::Zmq_Recv_CAR_FORWARD(PackageMessage &MsgRECV)
{
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));
    
	//����״ֵ̬
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	//pcallv->function()
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // ���ַ���ת��Ϊ����  
		    	}  
		    catch(boost::bad_lexical_cast&)    // ת��ʧ�ܻ��׳�һ���쳣  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_speedVelocity = recvspeed>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvspeed<0?0:recvspeed);
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   // printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	
	
}

int  Dgv_Manage::Zmq_Recv_CAR_BACKWARD(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//����״ֵ̬
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // ���ַ���ת��Ϊ����  
		    	}  
		    catch(boost::bad_lexical_cast&)    // ת��ʧ�ܻ��׳�һ���쳣  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_speedVelocity= recvspeed>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvspeed<0?0:recvspeed);
		   cmd_speedVelocity = 0-cmd_speedVelocity;
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}	
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
    //printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

}

int  Dgv_Manage::Zmq_Recv_CAR_TURNRIGHT(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//����״ֵ̬
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // ���ַ���ת��Ϊ����  
		    	}  
		    catch(boost::bad_lexical_cast&)    // ת��ʧ�ܻ��׳�һ���쳣  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_angelVelocity = recvspeed>de_CTRLMAXANGLE?de_CTRLMAXANGLE:(recvspeed<0?0:recvspeed);
		    cmd_angelVelocity = 0-cmd_angelVelocity;
			
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   // printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_TURNLEFT(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	int size = pcallv->parameters_size();
	
	if(size == 1){
		 int recvspeed=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // ���ַ���ת��Ϊ����  
		    	}  
		    catch(boost::bad_lexical_cast&)    // ת��ʧ�ܻ��׳�һ���쳣  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_angelVelocity = recvspeed>de_CTRLMAXANGLE?de_CTRLMAXANGLE:(recvspeed<0?0:recvspeed);
		   
		   
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_STOP(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_STOPFREE(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//����״ֵ̬
	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_EmergencyStop(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//����״ֵ̬
	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_SPEED_ANGLE(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));

	//����״ֵ̬
 	PackageMessage_Call * pcallv=MsgRECV.mutable_callmessage();
	//pcallv->function()
	int size = pcallv->parameters_size();

	if(size == 2){
		int recvspeed=0;
	  	int recvth=0;
		    try{  
		        	recvspeed=boost::lexical_cast<int>(pcallv->parameters(0)); // ���ַ���ת��Ϊ����  
		    	}  
		    catch(boost::bad_lexical_cast&)    // ת��ʧ�ܻ��׳�һ���쳣  
		    {  
		        recvspeed=0;  
		    } 
		   cmd_speedVelocity = recvspeed>de_CTRLMAXSPEED?de_CTRLMAXSPEED:(recvspeed<(0-de_CTRLMAXSPEED)?(0-de_CTRLMAXSPEED):recvspeed);

		    try{  
		        	recvth=boost::lexical_cast<int>(pcallv->parameters(1)); // ���ַ���ת��Ϊ����  
		    	}  
		    catch(boost::bad_lexical_cast&)    // ת��ʧ�ܻ��׳�һ���쳣  
		    {  
		        recvth=0;  
		    } 
		   cmd_angelVelocity = recvth>de_CTRLMAXANGLE?de_CTRLMAXANGLE:(recvth<(0-de_CTRLMAXANGLE)?(0-de_CTRLMAXANGLE):recvth);
		   
		   Result->set_errorcode(0);
		}
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}	
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   // printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_Realse_Loop(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//����״ֵ̬
	cmd_speedVelocity=0;
	cmd_angelVelocity=0;
	net_right=0;
	net_left=0;
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}

int  Dgv_Manage::Zmq_Recv_CAR_Enable_Loop(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));
		
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}

      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
  // printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}
int  Dgv_Manage::Zmq_Recv_CAR_ClearError(PackageMessage &MsgRECV)
{
	
       // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;	 
	PackageMessage MSGSnd;
        PackageMessage_Result *Result = MSGSnd.mutable_resultmessage();
        
	//װ��״̬���� ���ݳ���״̬�ж��Ƿ���Ӧ������
	Result->set_errorcode(0);
         Result->clear_resultdata();

        MSGSnd.clear_callmessage();

        MSGSnd.set_from( ZMQ_NAME_DRIVERCTRL);
        MSGSnd.set_to(MsgRECV.from());
        MSGSnd.set_sessionid(MsgRECV.sessionid());
        MSGSnd.set_time( time(NULL));


	//����״ֵ̬
	if((ctrl_ID!=NET_USER)||(!is_carcanbectrl())){
		Result->set_errorcode(0xD006);
	}
      //���л����ڴ�
    std::ostringstream stream;
    MSGSnd.SerializeToOstream(&stream);
    
    std::string text = stream.str();
    
   //printPackageMessage(text);
    //���ͳ�ȥ
    ros_zmqreturn_SendRpc(text);
    
    return (int)(Result->errorcode());	

	
}


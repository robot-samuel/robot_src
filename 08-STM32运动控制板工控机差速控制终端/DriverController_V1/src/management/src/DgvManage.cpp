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

#define PROTOBUFDUBUG_ECHO 1

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
           zmqcmdlist.insert(std::make_pair("EmergencyStop",index++));
		   	
	     zmqcmdlist.insert(std::make_pair("ctrlmode_light",index++));
	     zmqcmdlist.insert(std::make_pair("ctrlmode_bobstac",index++));
	     zmqcmdlist.insert(std::make_pair("ctrlmode_fobstac",index++));
	     zmqcmdlist.insert(std::make_pair("ctrlmode_autodoor",index++));
	   //  
	    zmqcmdlist.insert(std::make_pair("ctrlLed",index++));
	   zmqcmdlist.insert(std::make_pair("ctrlmode_Carsh",index++));
	   
	   


	cmd_speedVelocity=500; //单位mm
	cmd_angelVelocity=100;  //单位mm
	net_left=500;  //单位mm
	net_right=500;  //单位mm

	ctrl_ID = NONE;
	old_ctrl_ID = NONE;

	

	 remtorMAXSPEED = 1200 ;  //可被遥控器控制最大速度 寸mm/s
	 remtorMAXANGEL = 600;  //可被遥控器控制最大速度 寸mm/s	

	 logfile.init(CONFIGLOGPATH, "manage");
	 logfile.out_info("======Dgv_Manage init======");
           
}

Dgv_Manage::~Dgv_Manage()
 {


}

 void Dgv_Manage::Drivers_ros_cmdCall(const dgvmsg::DriverStatues::ConstPtr& msg)
{
	
	statues.Driverstatues.device_status = msg->device_status;
	
	if(msg->encounter.size()==2){
			statues.Driverstatues.encounter[0] = msg->encounter[0];
			statues.Driverstatues.encounter[1] = msg->encounter[1];
		}
	if(msg->Motor_Status.size()==2){
		statues.Driverstatues.Motor_Status[0] = msg->Motor_Status[0];
		statues.Driverstatues.Motor_Status[1] = msg->Motor_Status[1];
		}

	if(msg->errorcode.size()==2){
		statues.Driverstatues.errorcode[0] = msg->errorcode[0];
		statues.Driverstatues.errorcode[1] = msg->errorcode[1];
		}

	if(msg->Obt_dis.size()==4){
		statues.Driverstatues.Obt_dis[0] = msg->Obt_dis[0];
		statues.Driverstatues.Obt_dis[1] = msg->Obt_dis[1];
		statues.Driverstatues.Obt_dis[2] = msg->Obt_dis[2];
		statues.Driverstatues.Obt_dis[3] = msg->Obt_dis[3];
		}

	statues.Driverstatues.fcrash_io = msg->fcrash_io;
	statues.Driverstatues.bcrash_io = msg->bcrash_io;

	if(msg->Obt_en_Status.size()==4){
		statues.Driverstatues.Obt_en_Status[0] = msg->Obt_en_Status[0];
		statues.Driverstatues.Obt_en_Status[1] = msg->Obt_en_Status[1];
		statues.Driverstatues.Obt_en_Status[2] = msg->Obt_en_Status[2];
		statues.Driverstatues.Obt_en_Status[3] = msg->Obt_en_Status[3];
		}
	
	statues.Driverstatues.fObt_en_Status   = msg->fObt_en_Status;
	statues.Driverstatues.bObt_en_Status  = msg->bObt_en_Status;

	statues.Driverstatues.LMotor_com_Status   = msg->LMotor_com_Status;
	statues.Driverstatues.RMotor_com_Status  = msg->RMotor_com_Status;	

	if(msg->Obt_com_Status.size()==4){
		statues.Driverstatues.Obt_com_Status[0] = msg->Obt_com_Status[0];
		statues.Driverstatues.Obt_com_Status[1] = msg->Obt_com_Status[1];
		statues.Driverstatues.Obt_com_Status[2] = msg->Obt_com_Status[2];
		statues.Driverstatues.Obt_com_Status[3] = msg->Obt_com_Status[3];
		}

	if(msg->input_IO_Status.size()==4){
		statues.Driverstatues.input_IO_Status[0] = msg->input_IO_Status[0];
		statues.Driverstatues.input_IO_Status[1] = msg->input_IO_Status[1];
		statues.Driverstatues.input_IO_Status[2] = msg->input_IO_Status[2];
		statues.Driverstatues.input_IO_Status[3] = msg->input_IO_Status[3];
		}

	if(msg->out_IO_Status.size()==4){
		statues.Driverstatues.out_IO_Status[0] = msg->out_IO_Status[0];
		statues.Driverstatues.out_IO_Status[1] = msg->out_IO_Status[1];
		statues.Driverstatues.out_IO_Status[2] = msg->out_IO_Status[2];
		statues.Driverstatues.out_IO_Status[3] = msg->out_IO_Status[3];
		}

 }
 
 
 void Dgv_Manage::extern_zmq_cmdCall(const std_msgs::String::ConstPtr& msg)
{
                // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
               // GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::string cmdstr = msg->data;
	if(cmdstr.size()>30){
			
			proc_zmq_recvcmd(cmdstr);
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
              
            //std::cout<<str<<std::endl;
             ROS_INFO("%s %s\n%s",fmt.str().c_str(),buttonsstr.c_str(),axesstr.c_str());
}
 
void Dgv_Manage:: joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
           double scale = 1. / (1. - 0.05) / 32767.;
            //print_joymsg(joy);
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
		  
	statues.USBJostickNode.Set_connect();
	
     cmd_speedVelocity=(int)check_jody_speed(0,statues.Joycmd_velocity.linear.x);
     cmd_angelVelocity=(int)check_jody_speed(1,statues.Joycmd_velocity.angular.x);
          #if 0
	std::cout<<"statues.Joycmd_velocity.linear.x="<<statues.Joycmd_velocity.linear.x<<std::endl;
	std::cout<<"statues.Joycmd_velocity.angular.x="<<statues.Joycmd_velocity.angular.x<<std::endl;
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



void Dgv_Manage::record_joy_cmd(){
	
    geometry_msgs::Twist dirvcmd_velocity; 

     dirvcmd_velocity = statues.Joycmd_velocity;
	 
     float  speed =check_jody_speed(0,dirvcmd_velocity.linear.x);
     float angel  =check_jody_speed(1,dirvcmd_velocity.angular.x);
      //记录速度指令 
     statues.cmd_angelVelocity = angel;
     statues.cmd_speedVelocity = speed;	
}


void Dgv_Manage::ros_pub_joy_autorun()
{
    geometry_msgs::Twist dirvcmd_velocity; 

     dirvcmd_velocity = statues.Joycmd_velocity;

     float  speed =check_jody_speed(0,dirvcmd_velocity.linear.x);
     float angel  =check_jody_speed(1,dirvcmd_velocity.angular.x);

	 
     ros_Net2Ctrl_SendRpc("",(int)speed,(int)angel);
}


int Dgv_Manage::check_connect()
{
               if(statues.joyconnect == YES){
                  statues.joystick_count++;
                  if(statues.joystick_count >=90){             
                         //触发一次超时接收
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

               
              if(statues.center_isconnect == YES){
                    statues.center_count++;
                  if(statues.center_count >=150){             
                         //触发一次超时接收
                         #if DUBUG_ECHO
                         ROS_INFO("NET USER is losted **********************************");
                         #endif
                         statues.center_count =0;
                         statues.center_isconnect = NO;
			  
                     }
                }
              else{
                    statues.center_count =0;
                }
              
	if(statues.joyconnect == YES){
		if(ctrl_ID<=USBREMOTE_JOY){
			ctrl_ID = USBREMOTE_JOY;
			return ctrl_ID;
			}
		}
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

    return 1;
      
}

void Dgv_Manage::th_mainloop()
{
        
    ros::Rate loop_rate(de_CTRLRATE);
    int satas_count = 0;
    int configup_count = 0;
	std::cout << "======th_mainloop======"<<std::endl;
    while (ros::ok()){
        loop_rate.sleep();
        check_connect();
        satas_count++;
        configup_count++;
          boost::this_thread::interruption_point(); 
        //update all satues per 1s
        CtrlIDturn();
        if(ctrl_ID == NET_USER){
			

            }
            else if(ctrl_ID == USBREMOTE_JOY){
				#if DUBUG_ECHO
		         std::cout<<"ctrl_ID == USBREMOTE_JOY"<<std::endl;
				#endif
                        car_run_ptr =CAR_CTRL_DEFAUL; 
			  record_joy_cmd();
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
			
			ros_Net2Ctrl_SendRpc("",0,0);
			

                }
            if(satas_count>=de_CTRLRATE){
                   
                    satas_count=0;
                }

	

		caulate_speed();

		
		
            }		

}   


int Dgv_Manage::proc_zmq_recvcmd(std::string pack)
{
                  // Verify that the version of the library that we linked against is
                // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

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
	    // if(funid!=1){
            		std::cout<<"recv a zmqcmd:"<<fun<<"["<<funid<<"]"<<std::endl;
	     	//}
            #endif
            switch(funid){
                    case FUN_HEART:
                        if( ctrl_ID <= NET_USER){
				
                            statues.center_isconnect = YES;
                            statues.center_count=0; 
                            }
                        //statues.ZmqTransNode.Set_connect();
                        break;
		    case FUN_CTRL_FORWARD:

			break;
		    case FUN_CTRL_BACKWARD:
	
			break;
		    case FUN_CTRL_TURNLEFT:
		
			break;
		    case FUN_CTRL_TURNRIGHT:

			break;
		    case FUN_CTRL_STOP:
				
                            car_run_ptr = CAR_CTRL_STOP;
			break;
		    case FUN_CTRL_STOPFREE:
				
				car_run_ptr = CAR_CTRL_STOPFREE;  
			break;
		    case FUN_CTRL_RUN_SPEED_ANGLE:
				
                         if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_RUN_SPEED_ANGLE;
                            }   
			break;
		    case FUN_CTRL_RUN_LEFT_RIGHT:
				
                     if(ret==0){
                            //
                            car_run_ptr = CAR_CTRL_RUN_LEFT_RIGHT;
                            }  
			break;
		    case FUN_Realse_Loop:
				
				car_run_ptr = CAR_CTRL_Realse_Loop;  
			break;
		    case FUN_Enable_Loop:
				
				car_run_ptr = CAR_CTRL_Enable_Loop;  
			break;
		    case FUN_ClearError:
				
				car_run_ptr = CAR_CTRL_ClearError;  
			break;
		    case FUN_EmergencyStop:
				
				car_run_ptr = CAR_CTRL_EmergencyStop;  
			break;
		    case FUN_CTRL_LIGHT:
				
				
			break;
		    case FUN_CTRL_BOBSTAC:
				
			break;
		    case FUN_CTRL_FOBSTAC:
				
			break;
		case FUN_CTRL_AUTODOOR:
			
			break;
		case FUN_CTRL_LED:
			
			break;
		case FUN_CTRL_Carsh:
			
			break;
			
			
			
            	}
            return funid;
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
        ss << u; //uuid输出到字符串流
        ss>>str;//字符串流输出到字符串对象
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


void Dgv_Manage::ros_Net2Ctrl_SendRpc(std::string scmd,int speed,int angel)
{   
	//ros::NodeHandle npub;
	#if 0
	std::cout<<"scmd="<<scmd<<std::endl;
	std::cout<<"speed="<<speed<<std::endl;
	std::cout<<"angel="<<angel<<std::endl;
	#endif
	
	if(scmd == ""){
		trans_4_velocity(speed,angel);
		dgvmsg::DriverVelocity Motec;
		Motec.driver.resize(Dgv_statues::DRIVER_AMOUNT);
	#if 0
	std::cout<<"velocity[0]="<<velocity[0]<<std::endl;
	std::cout<<"velocity[1]="<<velocity[1]<<std::endl;
	#endif
            
		Motec.driver[0].name="DEV_DIR_ADD_LF";
		Motec.driver[0].add = DEV_DIR_ADD_LF;
		Motec.driver[0].VRPM = velocity[0];

		Motec.driver[1].name="DEV_DIR_ADD_RF";
		Motec.driver[1].add = DEV_DIR_ADD_RF;
		Motec.driver[1].VRPM = velocity[1];


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


//速度分配
void Dgv_Manage::trans_4_velocity( float speed, float angel)
{
	float leftspeed;
	float rightspeed;
	//LEFT
	leftspeed = (speed-angel/2);
       //LF
       
       velocity[0]=calculate_rpm_speed(DEV_DIR_ADD_LF,leftspeed);
         if(abs(velocity[0])>de_MAXRPM){
                 if((velocity[0])>0){
                        velocity[0]=de_MAXRPM;
                    }
                 else{
                        velocity[0]=0-de_MAXRPM;
                    }
            }
	


	//RIGHT
	
	rightspeed = (speed+angel/2);
       velocity[1]=calculate_rpm_speed(DEV_DIR_ADD_RF,rightspeed);
         if(abs(velocity[1])>de_MAXRPM){
                 if((velocity[1])>0){
                        velocity[1]=de_MAXRPM;
                    }
                 else{
                        velocity[1]=0-de_MAXRPM;
                    }
            }
	


}


int Dgv_Manage::calculate_rpm_speed(char dev_add, float speed)
{
       float rpm=0;
	 if(dev_add == DEV_DIR_ADD_LF){
       //LF
    	 rpm = (float)( 
        (speed/(1000.0*deLF_WheelSsize))*(de_ReductionRatiol*60*1));
        }
     
      else if(dev_add ==DEV_DIR_ADD_RF){
	  	//RF
          rpm = (float)( 
              (speed/(1000.0*deRF_WheelSsize))*(de_ReductionRatiol*60*1));

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

   
   //序列化到内存
	std::ostringstream stream;
	try{
        		msg.SerializeToOstream(&stream);
		}
      catch(...){
			std::ostringstream logstream;
			logstream<<"["<<__FUNCTION__ << "]"<< "SerializeToOstream a error"<<std::endl; 
			std::string logtext = logstream.str();
			logfile.out_info( logtext);
			return NULL;  
		    } 
    std::string ctext = stream.str();
   
  return ctext;
}


void Dgv_Manage::caulate_speed(){
	

}


void Dgv_Manage::CtrlIDturn()
{

	if(ctrl_ID !=old_ctrl_ID){
			//发送停车
                     std::string cmd="CAR_CTRL_STOP";
			ros_Net2Ctrl_SendRpc(cmd,0,0);
		}
	old_ctrl_ID = ctrl_ID;

}

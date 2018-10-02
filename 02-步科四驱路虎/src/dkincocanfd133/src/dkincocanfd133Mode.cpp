

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream> 
#include <vector>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>

#include "dkincocanfd133Mode.hpp"

#define DEBUG 0

kincocanfd133Mode::kincocanfd133Mode()
{
	ttyport ="can0";
 	
	for(int index=0;index<DRIVER_AMOUNT;index++){
		Fd133Driver[index].Set_add((unsigned char)1+index);
		Fd133Driver[index].set_dir(1);
		add[index] = 101+index;
		velocity[index] =0;
		onlineMsg[index].online=FN_LOST;
		}
	
	
}
kincocanfd133Mode::kincocanfd133Mode(std::string port,int *devadd,int *dir,int *driveadd,int cnt)
:ttyport(port)
{


 	for(int index=0;index<cnt;index++){
		Fd133Driver[index].Set_add(driveadd[index]);
		Fd133Driver[index].set_dir(dir[index]);
		add[index] = devadd[index];
		velocity[index] =0;
		onlineMsg[index].online=FN_LOST;
		std::cout<<"devadd["<<index<<"]= "<<(int)devadd[index]<<std::endl;
		std::cout<<"driveadd["<<index<<"]= "<<(int)driveadd[index]<<std::endl;
		std::cout<<"dir["<<index<<"]= "<<dir[index]<<std::endl;
		}
	
}
kincocanfd133Mode::~kincocanfd133Mode()
{
	DevCtrlDisEndable(mvecpa);
}


 //SUB 接收命令 
void kincocanfd133Mode::Net2Ctrl_driverCall(const dgvmsg::DriverVelocity::ConstPtr& cmdmsg)
{
        //ROS_INFO("ros sub:");
         int speed=0; 
		

        //前进速度
        int size = cmdmsg->driver.size();
	 #if 0
	 std::cout<<"Net2Ctrl_driverCall="<<size<<std::endl;
	 #endif
	for(int index=0;index<size;index++){
			//std::cout<<"cmdmsg->driver[index].add ="<<cmdmsg->driver[index].add<<std::endl;
			//std::cout<<"add="<<(int)add<<std::endl;
			if(cmdmsg->driver[index].add == add[0]){
					speed = cmdmsg->driver[index].VRPM;
				        SetSpeed(speed,0);
					//std::cout<<"add="<<(int)this->driverprco.Getadd();
					//std::cout<<"add="<<(int)add<<std::endl;
					//std::cout<<" speed="<<speed<<std::endl;
				}
			if(cmdmsg->driver[index].add == add[1]){
					speed = cmdmsg->driver[index].VRPM;
				        SetSpeed(speed,1);
					//std::cout<<"add="<<(int)this->driverprco.Getadd();
					//std::cout<<"add="<<(int)add<<std::endl;
					//std::cout<<" speed="<<speed<<std::endl;
				}
		}
  
    if((statues_ptr != FUN_SPEEDLOOP)&&(statues_ptr != FUN_REENABLE)){
                statues_ptr = FUN_REENABLE;
        }  
        
    
      //  ClearJoyFlag();  
}

void kincocanfd133Mode::Net2SCtrl_driverCall(const std_msgs::String::ConstPtr& ctrlcmd)
{

         // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;
		 
    std::string msg= ctrlcmd->data;
    if(msg.size()>50){
		PackageMessage msgrecv;
		msgrecv.ParseFromString(msg);
		PackageMessage_Call* callmsg = msgrecv.mutable_callmessage();
		if(callmsg->has_function()){
				std::string fun=callmsg->function();
			  	if(fun =="CAR_CTRL_STOP" ){
			            if((statues_ptr != FUN_STOPRUN)&&
			                (statues_ptr != FUN_RELEASE)&&
			               // (statues_ptr != FUN_EMERAGENCE)&&
			                (statues_ptr != FUN_ENABLE)){
			                        statues_ptr = FUN_STOPRUN;
			                }
			        }
			     else if(fun =="CAR_CTRL_EmergencyStop" ){
			            if(
			                //(statues_ptr != FUN_STOPRUN)&&
			                (statues_ptr != FUN_RELEASE)&&
			                (statues_ptr != FUN_EMERAGENCE)
			                //&&(statues_ptr != FUN_ENABLE)
			                ){
			                        statues_ptr = FUN_EMERAGENCE;
			                }            
			        }
			     else if(fun =="CAR_CTRL_Realse_Loop" ){
			            if((statues_ptr != FUN_RELEASE)
			                //&&(statues_ptr != FUN_RELEASE)&&
			               // (statues_ptr != FUN_EMERAGENCE)&&
			               // (statues_ptr != FUN_ENABLE)
			               ){
			                        statues_ptr = FUN_RELEASE;
			                }             
			         }
			     else if(fun =="CAR_CTRL_Enable_Loop" ){
			            if((statues_ptr != FUN_ENABLE)
			                &&(statues_ptr != FUN_RELEASE)&&
			                (statues_ptr != FUN_EMERAGENCE)&&
			                (statues_ptr != FUN_STOPRUN)
			                ){
			                        statues_ptr = FUN_ENABLE;
			                }            
			        }
			     else if(fun =="CAR_CTRL_ClearError" ){
			            if((statues_ptr != FUN_CLEARERROR)
			                //&&(statues_ptr != FUN_RELEASE)&&
			               // (statues_ptr != FUN_EMERAGENCE)&&
			                //(statues_ptr != FUN_ENABLE)
			                ){
			                        statues_ptr = FUN_CLEARERROR;
			                }        
			     	}
			     else if(fun =="CAR_CTRL_Config" ){
					if(callmsg->parameters_size()==1){
							std::string recvmsg = callmsg->parameters(0);
							if(recvmsg.size()>10){
									//Recv_driver_Config(recvmsg);
								}
						}
			        }
			}

    	}
  
    // ROS_INFO("recv SCMD msg :%s",msg.c_str());
 

}



void kincocanfd133Mode::kincocanfd133Mode_master(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{
	
       std::cout<< "hello can_master "<<std::endl;
        
	mvecpa = vecpa;
       
         int preokstatues_ptr=0;
         int errorcount=0;
	 int satas_count = 0;

        //初始化
            statues_ptr=FUN_INIT;
            preokstatues_ptr=statues_ptr;
			
	 SpeedClear();
	DriveInit(vecpa);
	
             
        ros::Rate loop_rate(10);
		 
         try{
                    while(ros::ok()){
             		//while(1){
                        switch(statues_ptr){
                                case FUN_INIT:
                                    //伺服使能
                                            if(DriveInit(vecpa)){
                                           
                                                    statues_ptr=FUN_SPEEDLOOP;
                                                    errorcount=0;
                                                }
                                    break;
                                 case FUN_SPEEDLOOP:
                                      //发送速度
                                     DriveSpeedCtrl(vecpa);
                                      errorcount=0;
                                    break;
                                case FUN_KEEPERROR:
                                    //伺服系统出错
                                    DevCtrlDisEndable(vecpa);
                                    DevGetDrivStatus(vecpa);
                                    
                                    SpeedClear();
                                    DevGetActPos(vecpa);
                                    DevGetCurCurent(vecpa);
                                    //statues_ptr=0;
                                    errorcount=0;
                                    break;
                                case FUN_RELEASE:
                                    //伺服系统出错
                                    if(DriveRelease(vecpa)){
                                            statues_ptr=FUN_DEFAULT;
                                        }

                                    errorcount=0;
                                    break; 
                                 case FUN_ENABLE:
                                     if(DriveEnable(vecpa)){
                                            statues_ptr=FUN_SPEEDLOOP;
                                        }
                                        errorcount=0;
                                       break; 

                                case FUN_EMERAGENCE:
                                         break; 
                                case FUN_STOPRUN:
                                          if(DriveStop(vecpa)){
                                                  statues_ptr=FUN_DEFAULT;
                                             }
                                         errorcount=0;
                                         break; 
                                case FUN_REENABLE:
                                          if(DriveREEnable(vecpa)){
                                                    statues_ptr=FUN_SPEEDLOOP;
                                              }
                                          errorcount=0;
                                         break; 

                                default:
                                    DevGetDrivStatus(vecpa);
                                    DevGetActPos(vecpa);
                                    DevGetCurCurent(vecpa);
					GetCruError1(vecpa);
                                    break;
                            }

			//驱动器报错
			for(int index=0;index<DRIVER_AMOUNT;index++){
		 	if(Fd133Driver[index].m_BitStatus.bit3_isFault ==1 ){
					statues_ptr=FUN_REENABLE;
					 std::cout << "Fd133Driver[index].m_BitStatus.bit3_isFault" << std:: endl;
		                }   
				}
                        //驱动器状态失联
                        for(int index=0;index<DRIVER_AMOUNT;index++){
				onlineMsg[index].check_connect();			
				if(onlineMsg[index].online!=FN_ONLINE){
	                          	statues_ptr=FUN_DEFAULT;
						      
	                       }
                        	}

			//状态反馈
			for(int index=0;index<DRIVER_AMOUNT;index++){
				dirv_pub_allstatues(index);
				dirv_pub_encounter(index);
				}		
				loop_rate.sleep();
                     }
		    DriveRelease(vecpa);
             }
        catch(boost::thread_interrupted&){
                 std::cout << "thread interrupt" << std:: endl;
             }
    
	
}

/*
void kincocanfd133Mode::kincocanfd133Mode_master(std::vector < boost::shared_ptr<socketcan> > &vecpa){


	std::cout<< "hello can_master "<<std::endl;
	ros::Rate loop_rate(1);
	 while(ros::ok()){

			loop_rate.sleep();
			DevGetUdc(vecpa);
	 	}


}
*/
void kincocanfd133Mode::print_dec(unsigned char *datBUF,int Num)
{

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<Num;index++){
                  int x=(unsigned  char)datBUF[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void kincocanfd133Mode::print_hex(unsigned char *datBUF,int Num)
 {

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
  void kincocanfd133Mode::print_hex(std::string str)
 {
	int Num = str.size();
	const char * datBUF =  str.c_str();
	
  std::cout<< "print_recv hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }

void kincocanfd133Mode::SpeedClear()
{
   for(int index=0;index<DRIVER_AMOUNT;index++){
   	 velocity[index] = 0;
   	}
}
void kincocanfd133Mode::SetSpeed(int speed,int cnt)
{
   
    velocity[cnt] = speed;
}
int kincocanfd133Mode::DriveInit(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{

       //std::cout << "DriveInit is running"<<std::endl;
	DevGetUdc(vecpa);
	DevGetCurCurent(vecpa);
	DevGetActPos(vecpa);
	DevGetVelocity(vecpa);
	GetCruError1(vecpa);
	GetSerialNo(vecpa);
	CmdStop(vecpa);
	DevCtrleEndable(vecpa);
	DevGetDrivStatus(vecpa);
	
	/*	
	if(driverprco.m_code.bit6_motor_enable !=1 ){
                    return 0;
                }

      if(onlineMsg.online !=FN_ONLINE ){
                     return 0;
                 }
                 
	*/
        return 1;
}
void kincocanfd133Mode::DriveSpeedCtrl(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{

        //static int listcounter=0;
        
        int num=0;
      //  int recvsize=0; 
      //  listcounter++;
	  #if 0
        std::cout << "DriveSpeedCtrl is running add="<<add<<std::endl;
	   std::cout << "velocity ="<<velocity<<std::endl;
	  #endif
        //设置速度
        //SetSpeed(vec_speed);
        DevSetspeed(vecpa,velocity);
        //查当前累计里程 单位为脉冲
        DevGetActPos(vecpa);
        DevGetCurCurent(vecpa); 
        DevGetDrivStatus(vecpa);
	DevGetVelocity(vecpa);
	GetCruError1(vecpa);
	
	
}

void kincocanfd133Mode::StatusRecord( boost::shared_ptr<socketcan> &vecpa,int cnt){

   
    int size = vecpa->recvbuf_size();

    for(int i=0;i<size;i++){
            vecpa->pop_cur_toarray();
            message msg( vecpa->cur_data.c_array(),13);
	    // std::cout<< "id="<<msg.get_fram_ID()<<"  add="<<(0x580+add)<<std::endl;
	    if((Fd133Driver[cnt].add()+0x580) == msg.get_fram_ID()){
			 Fd133Driver[cnt].signleRecord(msg);
	    	}   
        }

}

int kincocanfd133Mode::DriverecvDelay(
    boost::shared_ptr<socketcan> pa,
    int size,
    int timeout,
    std::string errprint){
    
    int count=0;
    while(pa->recvbuf_size()<size){
          count++;
          boost::this_thread::sleep(boost::posix_time::milliseconds(1));
          if(count>timeout){
                          std::cout<<errprint<<":"<<pa->recvbuf_size()<<'\a'<<std::endl;
                         // printf("\a");
                          count=0;
                         return pa->recvbuf_size();
                       }
            }
        return pa->recvbuf_size();
}


void kincocanfd133Mode::DevGetUdc(std::vector < boost::shared_ptr<socketcan> > &vecpa){

    //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevGetUdc "<<std::endl;
	#endif

	for(int index=0;index<DRIVER_AMOUNT;index++){
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetUdc());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		
		std::string errprint = ttyport+" Get DevGetUdc recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if((recvsize==RECV_DATA_SIZE)){
				//存储数据
				
				//message msg(pimpl->receivedData);
				StatusRecord(vecpa[0],index);
				//Fd133Driver.signleRecord(msg);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }    
		}
}

void kincocanfd133Mode::DevGetCurCurent(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{
    //获取实际电机电流
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevGetCurCurent "<<std::endl;
	#endif
	for(int index=0;index<DRIVER_AMOUNT;index++){
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetCurrent());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	

		std::string errprint = ttyport+"  DevGetCurCurent recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }  
		}

}

void kincocanfd133Mode::DevGetActPos(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{
    //获取实际位置
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevGetActPos "<<std::endl;
	#endif	
	for(int index=0;index<DRIVER_AMOUNT;index++){
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetCruPos());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	

		std::string errprint = ttyport+"  DevGetActPos recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }  
		}

}
void kincocanfd133Mode::DevGetVelocity(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{
    //获取实际速度
        static int recv_timeout=0;
	
	#if DEBUG
	std::cout<< "hello DevGetVelocity "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetVelocity());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	

		std::string errprint = ttyport+"  DevGetVelocity recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }  
		}

}
void kincocanfd133Mode::GetCruError1(std::vector < boost::shared_ptr<socketcan> > &vecpa){

   //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello GetCruError1 "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){   
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetError());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	

		std::string errprint = ttyport+"  GetCruError1 recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}

void kincocanfd133Mode::GetSerialNo(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{

   //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello GetSerialNo "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){      
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetDevserialN());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));

		std::string errprint = ttyport+"  GetSerialNo recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}


void kincocanfd133Mode::DevSetspeed(std::vector < boost::shared_ptr<socketcan> > &vecpa,int *vec_speed){
    
   
     //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevSetspeed "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){  	 
		vecpa[0]->clear();
		//std::cout<< "vec_speed =  "<<vec_speed[index]<<std::endl;
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_SetVelocity(vec_speed[index]));
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	
				   

		std::string errprint = ttyport+"  DevSetspeed recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
				 	}
	                   }  
		}
}

void kincocanfd133Mode::DevCtrleEndable(std::vector < boost::shared_ptr<socketcan> > &vecpa){
    //发送
        static int recv_timeout=0;
	
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){  	
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_SetEnable());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	
		
		std::string errprint = ttyport+" DevCtrleEndable recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}

void kincocanfd133Mode::DevCtrlDisEndable(std::vector < boost::shared_ptr<socketcan> > &vecpa){

    //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){ 	
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_SetRealse());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	
		
		std::string errprint = ttyport+" DevCtrlDisEndable recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}

void kincocanfd133Mode::DevSetClearError(std::vector < boost::shared_ptr<socketcan> > &vecpa){

    //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){ 
		
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_SetClearError());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	
		
		std::string errprint = ttyport+" DevSetClearError recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}

void kincocanfd133Mode::DevGetError(std::vector < boost::shared_ptr<socketcan> > &vecpa){

    //发送
        static int recv_timeout=0;
	
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){ 
		
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetError());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		
		std::string errprint = ttyport+" DevGetError recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}


void kincocanfd133Mode::CmdStop(std::vector < boost::shared_ptr<socketcan> > &vecpa){
    
        //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){		
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_SetStop());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));	
		
		std::string errprint = ttyport+" CmdStop recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}

void kincocanfd133Mode::DevEmergencyStop(std::vector < boost::shared_ptr<socketcan> > &vecpa){

        //发送
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){			
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_QuickStop());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));		
		
		std::string errprint = ttyport+" DevEmergencyStop recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}

}

void kincocanfd133Mode::DevGetDrivStatus(std::vector < boost::shared_ptr<socketcan> > &vecpa){
	
        static int recv_timeout=0;
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){			
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetStauts());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));

		std::string errprint = ttyport+"  DevGetDrivStatus recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }  
		}
        
}

void kincocanfd133Mode::GetSpeedCmd(std::vector < boost::shared_ptr<socketcan> > &vecpa){

    //发送
        static int recv_timeout=0;
		
	#if DEBUG
	std::cout<< "hello DevCtrleEndable "<<std::endl;
	#endif	
	
	for(int index=0;index<DRIVER_AMOUNT;index++){
		
		vecpa[0]->clear();
		vecpa[0]->sendRequest(Fd133Driver[index].Cmd_GetCmdVelocity());
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		
		std::string errprint = ttyport+"  GetSpeedCmd recv  error";  
		int recvsize=DriverecvDelay(vecpa[0],RECV_DATA_SIZE,RECV_WAITE_TIME,errprint);
	        if(recvsize==RECV_DATA_SIZE){
				//存储数据
				//message msg(pimpl->receivedData);
				//Fd133Driver.signleRecord(msg);
				StatusRecord(vecpa[0],index);
				onlineMsg[index].Set_connect();
	                   }
	         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg[index].online=FN_LOST;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
	                   }
		}
}

int kincocanfd133Mode::DriveRelease(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{

        std::cout << "DriveRelease is running"<<std::endl;
        
       
        
        DevCtrlDisEndable(vecpa);
		
     	DevSetClearError(vecpa);
		
        GetSpeedCmd(vecpa);

        DevGetActPos(vecpa);

        SpeedClear();

        DevGetUdc(vecpa);

        DevGetDrivStatus(vecpa);
	GetCruError1(vecpa);
  
        /*
            if(driverprco.m_code.bit6_motor_enable !=0 ){
                    //DevCtrleEndable(vecpa);
                    return 0;
                }
   */    
	for(int index=0;index<DRIVER_AMOUNT;index++){
	      if(onlineMsg[index].online !=FN_ONLINE ){
	                     return 0;
	                 }
		}
          
         
        return 1;
}

int kincocanfd133Mode::DriveEnable(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{

        //std::cout << "DriveEnable is running"<<std::endl;

        DevSetClearError(vecpa);
	
        GetSpeedCmd(vecpa);

        DevGetActPos(vecpa);

        //SpeedClear();
	 DevCtrleEndable(vecpa);
        DevGetUdc(vecpa);

        DevGetDrivStatus(vecpa);

        DevGetCurCurent(vecpa);

	GetCruError1(vecpa);
        

	for(int index=0;index<DRIVER_AMOUNT;index++){
		if(onlineMsg[index].online !=FN_ONLINE ){
	                     return 0;
	                 }
		}
              

        return 1;
        
}
int kincocanfd133Mode::DriveREEnable(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{
	#if 0
        std::cout << "DriveREEnable is running"<<std::endl;
        #endif


        DevSetClearError(vecpa);
        
        
        GetSpeedCmd(vecpa);

       DevGetActPos(vecpa);

        //SpeedClear();

        DevGetUdc(vecpa);

       

        
        DevGetCurCurent(vecpa);
	
        DevCtrleEndable(vecpa);
	SpeedClear();
        DevSetspeed(vecpa,velocity);
	
	 DevGetDrivStatus(vecpa);


        for(int index=0;index<DRIVER_AMOUNT;index++){
		if(Fd133Driver[index].m_BitStatus.bit3_isFault ==1 ){
	                    //DevCtrleEndable(vecpa);
	                     DevSetClearError(vecpa);
	                    return 0;
	                }
        	}

	  for(int index=0;index<DRIVER_AMOUNT;index++){
		if(onlineMsg[index].online !=FN_ONLINE ){
	                     return 0;
	                 }
	  	}
               
     
        return 1;
        
}

int kincocanfd133Mode::DriveEmergencyStop(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{
	 #if 0
    std::cout << "DriveEmergencyStop is running"<<std::endl;
	#endif
   
   SpeedClear();
	
   DevSetspeed(vecpa,velocity);   
     
   DevEmergencyStop(vecpa);
  
   GetSpeedCmd(vecpa); 

      DevGetActPos(vecpa);

   DevGetCurCurent(vecpa);
  
	  for(int index=0;index<DRIVER_AMOUNT;index++){
		if(onlineMsg[index].online !=FN_ONLINE ){
	                     return 0;
	                 }
	  	}
  /*
   for(int index=0 ; index < 1 ;index++){
             if(driverprco.cru_speed()!=0){
                     return 0;
                 }
         }

       */ 

   //DevSetspeed(vecpa,velocity);   
 
        return 1;
}

int kincocanfd133Mode::DriveStop(std::vector < boost::shared_ptr<socketcan> > &vecpa)
{

    //std::cout << "DriveStop is DriveStop"<<std::endl;
    
   CmdStop(vecpa);
    
   SpeedClear();

   DevSetspeed(vecpa,velocity);  
   
   GetSpeedCmd(vecpa); 
   GetCruError1(vecpa);

     DevGetActPos(vecpa);

   DevGetCurCurent(vecpa);

	  for(int index=0;index<DRIVER_AMOUNT;index++){
		if(onlineMsg[index].online !=FN_ONLINE ){
	                     return 0;
	                 }
	  	}
    /*
   for(int index=0 ; index < 1 ;index++){
             if(driverprco.cru_speed()!=0){
                     return 0;
                 }
         }
      
      */
        return 1;
}


void kincocanfd133Mode::dirv_pub_encounter(int cnt)
{   
	
	dgvmsg::Encounter encounter;
	
	//encounter.devadd = driverprco.add();
	encounter.devadd = add[cnt];
	encounter.encounter = Fd133Driver[cnt].CruPos();
	encounter.now = ros::Time::now();

	rospub_mode_encounter.publish(encounter);
	ros::spinOnce();
	
}
void kincocanfd133Mode::dirv_pub_allstatues(int cnt)
{   
    std_msgs::String stdmsg;
    
     stdmsg.data = Pack_driver_statues(cnt);
     
     rospub_mode_driverstatues.publish(stdmsg);
     ros::spinOnce();
}

std::string  kincocanfd133Mode::Pack_driver_statues(int cnt)
{       
         // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;

       ControllerMessage_Dirver driver_status;
      //driver_status.r
      driver_status.Clear();

        
       driver_status.set_device_status((::google::protobuf::int32)onlineMsg[cnt].online);
      // driver_status.set_device_add((::google::protobuf::int32)driverprco.add());
      driver_status.set_device_add((::google::protobuf::int32)add[cnt]);
       driver_status.set_actvelocity(Fd133Driver[cnt].cru_speed()*Fd133Driver[cnt].dir());
       driver_status.set_setvelocity(Fd133Driver[cnt].cru_speedcmd()*Fd133Driver[cnt].dir());
       driver_status.set_ctrl_dc_voltage(Fd133Driver[cnt].Udc());

       driver_status.set_ctrl_ipm_temperature(Fd133Driver[cnt].IpmTem());
       driver_status.set_actcurrent((float)Fd133Driver[cnt].effectiveTorque_I());
       
       driver_status.set_actualpos(Fd133Driver[cnt].CruPos()*Fd133Driver[cnt].dir());
       //driver_status.set_emergencyinputvalue(driverprco.emergencyinputvalue());
      //driver_status.clear_motec();
      
       driver_status.set_error(Fd133Driver[cnt].GetStrError());
 
       std::string res;
       if (!driver_status.SerializeToString(&res)  ) {
           std::cout << "Failed to write msg driver Message." << std::endl; 
           return ""; 
       }  
   //  std::cout<<"size="<<res.size()<<std::endl;
    return res;
}  




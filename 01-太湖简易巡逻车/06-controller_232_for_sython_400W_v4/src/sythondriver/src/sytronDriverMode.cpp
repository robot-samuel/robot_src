

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



#include "sytronDriverMode.hpp"


#define  DEBUG_ECHO 0

class MAsyncSerialImpl
{
public:
    CallbackAsyncSerial serial;
    std::string receivedData;
};


sytronDriverMode::sytronDriverMode(std::string& port, unsigned int rate)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;
	DEV_add = 1;
	driverprco.Set_add(1);

	
	
}
sytronDriverMode::sytronDriverMode(std::string& port, unsigned int rate,int devadd,int dir)
:pimpl(new MAsyncSerialImpl)
{
 
	open(port,rate);	
	ttyport = port;
	bandrate = rate;
	pimpl->receivedData.clear();

	//driverprco.Set_add((unsigned char)devadd);
	DEV_add = devadd;
	driverprco.Set_add(1);
	driverprco.set_dir(dir);
	
	std::cout<<"port= "<<port<<std::endl;
	std::cout<<"rate= "<<rate<<std::endl;
	std::cout<<"devadd= "<<(int)devadd<<std::endl;
	std::cout<<"dir= "<<dir<<std::endl;
	
	
}
sytronDriverMode::~sytronDriverMode()
{
  
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
int sytronDriverMode::open(std::string devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname,baudrate);
    } catch(boost::system::system_error&)
    {
        //Errors during open
        return 0;
    }
    pimpl->serial.setCallback(bind(&sytronDriverMode::readCallback,this, _1, _2));
	return 1;
}

void sytronDriverMode::close()
{
    pimpl->serial.clearCallback();
    try {
        pimpl->serial.close();
    } catch(boost::system::system_error&)
    {
        //Errors during port close
    }
    pimpl->receivedData.clear();//Clear eventual data remaining in read buffer
}

bool sytronDriverMode::isOpen()
{
    return pimpl->serial.isOpen();
}

bool sytronDriverMode::errorStatus()
{
    return pimpl->serial.errorStatus();
}

void sytronDriverMode::write(std::string data)
{
    pimpl->serial.writeString(data);
}

void sytronDriverMode::readCallback(const char *data, size_t size)
{
    //pimpl->receivedData+=QString::fromAscii(data,size);
        std::string res;
        res.clear();
        res.assign(data,size);
        pimpl->receivedData+=res;
	//const char* ch=pimpl->receivedData.c_str();
	#if 0
	print_hex(pimpl->receivedData);
	#endif
	/*
	comRecvFlag = procss_recv_buffer();
	if(comRecvFlag){
		onlineMsg.Set_connect();
		
		}
		*/

}

void sytronDriverMode::sytronDriverMode_master()
{
       std::cout<< "hello sytronDriverMode_master "<<std::endl;
        

       
         int preokstatues_ptr=0;
         int errorcount=0;
	int satas_count = 0;

        //初始化
            statues_ptr=FUN_INIT;
            preokstatues_ptr=statues_ptr;

            //设定速度
            //int vecspeed=1000;   

            SpeedClear();
            //获取参数
            InitGetDevParam();
             
         ros::Rate loop_rate(20);
		 
         try{
                    while(ros::ok()){
                     // time_now =  boost::posix_time::microsec_clock::universal_time();  
                      // std::cout<<"time_now:"<<time_now<<std::endl;
                        // boost::progress_timer pt; 
                        //伺服状态机                
                        switch(statues_ptr){
                                case FUN_INIT:
                                    //伺服使能
                                            if(DriveInit()){
                                           
                                                    statues_ptr=FUN_SPEEDLOOP;
                                                    errorcount=0;
                                                }
                                    break;
                                 case FUN_SPEEDLOOP:
                                      //发送速度
                                     DriveSpeedCtrl();
					DevGetDin();
                                      errorcount=0;
                                    break;
                                case FUN_KEEPERROR:
                                    //伺服系统出错
                                    DevSetClearError();
                                    DevCtrlDisEndable();
                                    DevGetDrivStatus();
                                    SpeedClear();
                                    DevGetActPos();
					DevGetDin();
                                    //statues_ptr=0;
                                    errorcount=0;
                                    break;
                                case FUN_RELEASE:
                                    //伺服系统出错
                                    if(DriveRelease()){
                                            statues_ptr=FUN_DEFAULT;
                                        }

                                    errorcount=0;
                                    break; 
                                 case FUN_ENABLE:
                                     if(DriveEnable()){
                                            statues_ptr=FUN_SPEEDLOOP;
                                        }
                                        errorcount=0;
                                       break; 
                                case FUN_CLEARERROR:
                                      if(DriveClearError()){
                                              statues_ptr=FUN_DEFAULT;
                                           }
                                        errorcount=0;
                                         break; 
                                case FUN_EMERAGENCE:
                                        if(DriveEmergencyStop()){
                                               statues_ptr=FUN_DEFAULT;
                                           }
                                         errorcount=0;
                                         break; 
                                case FUN_STOPRUN:
                                          if(DriveStop()){
                                                  statues_ptr=FUN_DEFAULT;
                                             }
                                         errorcount=0;
                                         break; 
                                case FUN_REENABLE:
                                          if(DriveREEnable()){
                                                    statues_ptr=FUN_SPEEDLOOP;
                                              }
                                          errorcount=0;
                                         break; 

                                default:
                                    DevGetDrivStatus();
                                    DevGetActPos();  //位置
                                    DevGetCurCurent();
				      GetCruError();
                                    break;
                            }
                        
                        //驱动器状态失联
			onlineMsg.check_connect();			
			if(onlineMsg.online!=FN_ONLINE){
                          	statues_ptr=FUN_DEFAULT;
					      
                       }
                         //错误报警
			 				 
                          //检查是否有报警
                        for(int index=0 ; index < 1 ;index++){
                                     if(driverprco.GetStrError() != "OK"){
						 std::cout<<(int)DEV_add<<":"<<driverprco.GetStrError()<<std::endl;
                                            //某一台电机出现故障
                                            statues_ptr=FUN_RELEASE;
                                           // if(errorcount%300==0){
                                                    // statues_ptr=0;
                                               // }
                                        }
                                     
                            } 
			
                        //所有状态OK
                        if((preokstatues_ptr != 0) &&(statues_ptr == 0)){
                                 int ok=0;
                            for(int index=0;index < 1 ;index++){
                               
                                if(onlineMsg.online!=FN_ONLINE){
                                             ok++;
                                              //std::cout<<'\a'<<std::endl;
                                    }

                                }  
                             if(ok == 1){
                                        // preokstatues_ptr  = 1;
                                         statues_ptr=preokstatues_ptr;
                                    }
                            }

			
                        //当速度控制模式时
                        if(statues_ptr ==FUN_SPEEDLOOP){
                             for(int index=0;index < 1 ;index++){
                                        if(driverprco.m_BitStatus.bit1_isENA_SRV !=1){
                                                //故障开环
                                                statues_ptr=FUN_INIT;
                                                
                                            }
                                    }                                  
                            }
                          
                            
			satas_count++;
			if(satas_count>=10){
				satas_count=0;
				//dirv_pub_allstatues();
				Pack_driver_msg_statues();
				}
                        //生成新的编码器数据包
                       dirv_pub_encounter();
                        
                        
                        loop_rate.sleep();
                     }
             }
        catch(boost::thread_interrupted&){
                 std::cout << "thread interrupt" << std:: endl;
             }
    

    
	
}


void sytronDriverMode::print_dec(unsigned char *datBUF,int Num)
{

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<Num;index++){
                  int x=(unsigned  char)datBUF[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void sytronDriverMode::print_hex(unsigned char *datBUF,int Num)
 {
	#if 0  
  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";
 	#endif 

 }
  void sytronDriverMode::print_hex(std::string str)
 {
 	#if 1   
	int Num = str.size();
	const char * datBUF =  str.c_str();
	
  std::cout<< "sytronDriverMode print_recv hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";
 	#endif   

 }

int sytronDriverMode::DriverecvDelay(  
 	int size,    
 	int timeout,    
 	std::string errprint){       
 	int count=0;    
	while(pimpl->receivedData.size()<size){          
		count++;          
		boost::this_thread::sleep(boost::posix_time::milliseconds(2));         
		if(count>timeout){                        
			#if DEBUG_ECHO                          
			std::cout<<errprint<<":"<<pimpl->receivedData.size()<<'\a'<<std::endl;	
			//print_hex(pimpl->receivedData);
			#endif                         // printf("\a");                          
			count=0;  
			int size = pimpl->receivedData.size();
			return size;                       
			}            
		}        
	return pimpl->receivedData.size();
}

void sytronDriverMode::DevCtrlDisEndable(){

    //发送
        static int recv_timeout=0;
	pimpl->receivedData.clear();
        write(driverprco.Cmd_SetRealse());
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));	
	
	std::string errprint = ttyport+" DevCtrlDisEndable recv  error";  
	int recvsize=DriverecvDelay(8,RECV_WAITE_TIME,errprint);
        if(recvsize>=8){
			//存储数据
			message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::FN010_ENABLE);
			onlineMsg.Set_connect();
			recv_timeout=0;
                   }
         else{
		 	if(recvsize==5){
				 //print_hex(pimpl->receivedData);
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }
}
void sytronDriverMode::DevCtrleEndable(){
    //发送
        static int recv_timeout=0;
	
	pimpl->receivedData.clear();
        write(driverprco.Cmd_SetEnable());
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));	
	
	std::string errprint = ttyport+" DevCtrleEndable recv  error";  
	int recvsize=DriverecvDelay(8,RECV_WAITE_TIME,errprint);
        if(recvsize>=8){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::FN010_ENABLE);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				 //print_hex(pimpl->receivedData);
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }
}


void sytronDriverMode::DevGetActPos()
{
    //获取实际位置
        static int recv_timeout=0;
	
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetCruPos());
	boost::this_thread::sleep(boost::posix_time::milliseconds(5));	
	//接收格式
	//|add|fun|num|N个数据|CRCH |CRCL
	std::string errprint = ttyport+"  DevGetActPos recv  error";  
	int recvsize=DriverecvDelay(9,RECV_WAITE_TIME,errprint);
        if(recvsize>=9){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN01D_CurPos);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				 //print_hex(pimpl->receivedData);
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }  

}

void sytronDriverMode::DevGetVelocity()
{
    //获取实际速度
        static int recv_timeout=0;
	
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetVelocity());
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));	

	std::string errprint = ttyport+"  DevGetVelocity recv  error";  
	int recvsize=DriverecvDelay(7,RECV_WAITE_TIME,errprint);
        if(recvsize>=7){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN000_ACTVelocity);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				// print_hex(pimpl->receivedData);
			
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }  

}
void sytronDriverMode::DevGetDin(){

    //发送
        static int recv_timeout=0;
	
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetDinStatues());
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	
	std::string errprint = ttyport+" Get DevGetDin recv  error";  
	int recvsize=DriverecvDelay(7,RECV_WAITE_TIME,errprint);
        if(recvsize>=7){
			//存储数据
			 message msg;
			#if 0 
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN030_DIN);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{

				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
                   }        
}


void sytronDriverMode::DevGetUdc(){

    //发送
        static int recv_timeout=0;
	
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetUdc());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));
	
	std::string errprint = ttyport+" Get DevGetUdc recv  error";  
	int recvsize=DriverecvDelay(7,RECV_WAITE_TIME,errprint);
        if(recvsize>=7){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN080_CurUDC);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{

				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
                   }        
}

void sytronDriverMode::GetCruError(){

   //发送
	static int recv_timeout=0;
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetError());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));	

	std::string errprint = ttyport+"  GetCruError1 recv  error";  
	int recvsize=DriverecvDelay(7,RECV_WAITE_TIME,errprint);
          if(recvsize>=7){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN036_ERROR);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
			
                   }
}

void sytronDriverMode::DevSetspeed(short int vec_speed){
    
   
     //发送
        static int recv_timeout=0;
		
                
	pimpl->receivedData.clear();
        write(driverprco.Cmd_SetVelocity(vec_speed));
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));	
			   

	std::string errprint = ttyport+"  DevSetspeed recv  error";  
	int recvsize=DriverecvDelay(8,RECV_WAITE_TIME,errprint);
        if(recvsize>=8){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::CN000_CMDVelocity);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				std::cout<<"DevSetspeed = recv 5 wirite protecting"<<std::endl;
				// print_hex(pimpl->receivedData);
				 
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }  
}

void sytronDriverMode::DevSetClearError(){

    //发送
        static int recv_timeout=0;
	pimpl->receivedData.clear();
        write(driverprco.Cmd_SetClearError());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));	
	
	std::string errprint = ttyport+" DevSetClearError recv  error";  
	int recvsize=DriverecvDelay(8,RECV_WAITE_TIME,errprint);
       if(recvsize>=8){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::FN011_CLEARERROR);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				 //print_hex(pimpl->receivedData);
				 std::cout<<"DevSetClearError = recv 5 wirite protecting"<<std::endl;
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }
}

void sytronDriverMode::CmdStop(){
    
        //发送
        static int recv_timeout=0;
		
	pimpl->receivedData.clear();
        write(driverprco.Cmd_SetStop());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));	
	
	std::string errprint = ttyport+" CmdStop recv  error";  
	int recvsize=DriverecvDelay(8,RECV_WAITE_TIME,errprint);
         if(recvsize>=8){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::CN000_CMDVelocity);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				 std::cout<<"CmdStop = recv 5 wirite protecting"<<std::endl;
				 //print_hex(pimpl->receivedData);
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }
}

void sytronDriverMode::DevEmergencyStop(){

        //发送
        static int recv_timeout=0;
		
	pimpl->receivedData.clear();
        write(driverprco.Cmd_QuickStop());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));	
	
	std::string errprint = ttyport+" DevEmergencyStop recv  error";  
	int recvsize=DriverecvDelay(8,RECV_WAITE_TIME,errprint);
        if(recvsize>=8){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::FN012_EMERGENCY);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				 //print_hex(pimpl->receivedData);
				  std::cout<<"DevEmergencyStop = recv 5 wirite protecting"<<std::endl;
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }

}

void sytronDriverMode::DevGetDrivStatus(){
	
        static int recv_timeout=0;
		
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetStauts());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));

	std::string errprint = ttyport+"  DevGetDrivStatus recv  error";  
	int recvsize=DriverecvDelay(7,RECV_WAITE_TIME,errprint);
         if(recvsize>=7){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN02B_APPS1);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
			 recv_timeout++;
			 if(recv_timeout>=RECV_NO_COUNTER){
                     			onlineMsg.online=0;
					recv_timeout=0;
					//print_test( vecpa[0]);
			 	}
                   }  
        
}

void sytronDriverMode::GetSpeedCmd(){

    //发送
        static int recv_timeout=0;
		
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetCmdVelocity());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));

	std::string errprint = ttyport+"  GetSpeedCmd recv  error";  
	int recvsize=DriverecvDelay(7,RECV_WAITE_TIME,errprint);
         if(recvsize>=7){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN007_CMDVelocity);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
			 recv_timeout++;
			 if(recv_timeout>=RECV_NO_COUNTER){
                     			onlineMsg.online=0;
					recv_timeout=0;
					//print_test( vecpa[0]);
			 	}
                   }
}

void sytronDriverMode::DevGetCurCurent()
{
    //获取实际电机电流
        static int recv_timeout=0;
	
	pimpl->receivedData.clear();
        write(driverprco.Cmd_GetCurrent());
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));	

	std::string errprint = ttyport+"  DevGetCurCurent recv  error";  
	int recvsize=DriverecvDelay(7,RECV_WAITE_TIME,errprint);
          if(recvsize>=7){
			//存储数据
			 message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::DN002_Current);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
			 recv_timeout++;
			 if(recv_timeout>=RECV_NO_COUNTER){
                     			onlineMsg.online=0;
					recv_timeout=0;
					//print_test( vecpa[0]);
			 	}
                   }  

}

void sytronDriverMode::DevSetLifeGuard(short int timeout){
    
   
     //发送
        static int recv_timeout=0;
		
                
	pimpl->receivedData.clear();
        write(driverprco.Cmd_SetLifeGuard(timeout));
	boost::this_thread::sleep(boost::posix_time::milliseconds(SEND_WAITE_TIME));	
			   

	std::string errprint = ttyport+"  DevSetLifeGuard recv  error";  
	int recvsize=DriverecvDelay(8,RECV_WAITE_TIME,errprint);
        if(recvsize>=8){
			//存储数据
			message msg;
			#if 0  
			print_hex(pimpl->receivedData);
			#endif
			msg.copy(pimpl->receivedData);
			driverprco.signleRecord(msg,sytronDriver::FN0E3_LIFEGUARD);
			recv_timeout=0;
			onlineMsg.Set_connect();
                   }
         else{
		 	if(recvsize==5){
				 //print_hex(pimpl->receivedData);
				 std::cout<<"DevSetLifeGuard = recv 5 wirite protecting"<<std::endl;
				 if(check_recv5bits(pimpl->receivedData)==1){
					recv_timeout=0;
					onlineMsg.Set_connect();
				 	}
		 		}
			else{
				 recv_timeout++;
				 if(recv_timeout>=RECV_NO_COUNTER){
	                     			onlineMsg.online=0;
						recv_timeout=0;
						//print_test( vecpa[0]);
				 	}
				}
                   }  
}

void sytronDriverMode::InitGetDevParam()
{
	DevGetActPos();
	DevGetVelocity();
	DevGetUdc();
	GetCruError();
	DevGetDrivStatus();
	DevCtrlDisEndable();
	DevSetspeed(0);
	DevGetDin();
}

int sytronDriverMode::DriveInit()
{
	#if DEBUG_ECHO
        std::cout << "DriveInit is running"<<std::endl;
	#endif
        //使能
        //DevCtrlDisEndable();
        //DevSetLifeGuard(0);
         DevGetCurCurent(); 
        DevSetClearError();
	//DevCtrlDisEndable();
		
	 GetSpeedCmd();
	 
        DevGetActPos();
		
        SpeedClear();
	
        DevGetUdc();
		

	//使能	
        DevCtrleEndable();
		
        DevGetDrivStatus();
		
	  DevGetActPos();	
		
      
	   
       GetCruError();
        DevSetspeed(0);
		
	DevSetLifeGuard(2000);
	
            if( driverprco.m_BitStatus.bit1_isENA_SRV != 1 ){//没有使能
                    //DevCtrleEndable(vecpa);
                    return 0;
                }

      if(onlineMsg.online !=FN_ONLINE ){  //通信错误
                     return 0;
                 }
    DevSetLifeGuard(2000);

  	if(driverprco.GetStrError()!="OK"){  //驱动器报警
			 return 0;
		}     
        return 1;
}

void sytronDriverMode::DriveSpeedCtrl(short int  vec_speed)
{
        static int listcounter=0;
        
        int num=0;
      //  int recvsize=0; 
        listcounter++;
        //std::cout << "DriveSpeedCtrl is running"<<std::endl;
        //设置速度
        SetSpeed(vec_speed);
        DevSetspeed(velocity);
        DevGetCurCurent();   //电流
        DevGetActPos();  //位置
        DevGetDrivStatus(); //状态
	 DevGetVelocity();  //实际速度
	 GetCruError();
	 DevSetLifeGuard(2000);
}
void sytronDriverMode::DriveSpeedCtrl()
{
       // static int listcounter=0;
	#if DEBUG_ECHO
        std::cout << "DriveSpeedCtrl is running"<<std::endl;
	#endif
      //  int num=0;
      //  int recvsize=0; 
      //  listcounter++;
        
        //设置速度
       // SetSpeed(vec_speed);
        DevSetspeed(velocity);
        DevGetCurCurent();   //电流
        DevGetActPos();  //位置
        DevGetDrivStatus(); //状态
	 DevGetVelocity();  //实际速度
	 GetCruError();
	 DevSetLifeGuard(2000);
}


int sytronDriverMode::DriveRelease()
{
	#if DEBUG_ECHO
        std::cout << "DriveRelease is running"<<std::endl;
        #endif
       
        //DevSetLifeGuard(0);
        GetCruError();
       DevCtrlDisEndable();
        DriveClearError();
        GetSpeedCmd();

       DevGetActPos();

        SpeedClear();

        DevGetUdc();

        DevGetDrivStatus();
	DevGetDin();
	

        DevSetLifeGuard(2000);
       
            if( driverprco.m_BitStatus.bit1_isENA_SRV != 0 ){
                    //DevCtrleEndable(vecpa);
                    return 0;
                }

      if(onlineMsg.online !=FN_ONLINE ){
                     return 0;
                 }
         
        return 1;
}

int sytronDriverMode::DriveEnable()
{
	#if DEBUG_ECHO
        std::cout << "DriveEnable is running"<<std::endl;
	#endif

        //DevSetLifeGuard(0);
        DevCtrleEndable();
        
        GetSpeedCmd();

        DevGetActPos();

        //SpeedClear();

        DevGetUdc();

        DevGetDrivStatus();

        DevGetCurCurent();
	DevGetDin();
        GetCruError();
        DevSetLifeGuard(2000);     
		
            if( driverprco.m_BitStatus.bit1_isENA_SRV !=1 ){
                    //DevCtrleEndable(vecpa);
                    return 0;
                }
	if(onlineMsg.online !=FN_ONLINE ){
                     return 0;
                 }
	if(driverprco.GetStrError()!="OK"){
			 return 0;
		}
        return 1;
        
}
int sytronDriverMode::DriveREEnable()
{
	#if DEBUG_ECHO
        std::cout << "DriveREEnable is running"<<std::endl;
        #endif
	//DevSetLifeGuard(0);
        
		
	DevCtrlDisEndable();
        DevSetClearError();
        GetSpeedCmd();

        //SpeedClear();

        DevGetUdc();
		
	DevCtrleEndable();
	DevGetActPos();
	DevSetspeed(0);
        //SetSpeed(vecspeed);
        GetCruError();
        DevGetCurCurent();
	DevGetDin();
	DevGetDrivStatus();
       DevSetLifeGuard(2000);
     
            if( driverprco.m_BitStatus.bit1_isENA_SRV !=1 ){
			#if DEBUG_ECHO
                    std::cout<<"DriveREEnable is  here #1 "<<std::endl;
			#endif
                 
                    return 0;
                }

	if(driverprco.GetStrError()!="OK"){
		#if DEBUG_ECHO
			 std::cout<<"DriveREEnable is  here #2 "<<driverprco.GetStrError()<<std::endl;
		#endif
			 return 0;
		}	
	
	if(onlineMsg.online !=FN_ONLINE ){
		#if DEBUG_ECHO
		      std::cout<<"DriveREEnable is  here #3 "<<std::endl;
		#endif
                     return 0;
                 }
        
        return 1;
        
}

int sytronDriverMode::DriveClearError()
{
	#if DEBUG_ECHO
       std::cout << "DriveClearError is running"<<std::endl;
	#endif
        
        //DevEncRst(vecpa);
        //DevSetLifeGuard(0);
        
	DevSetClearError();	
	DevCtrlDisEndable();

        DevGetActPos();

        DevGetUdc();

        DevGetDrivStatus();

        DevGetCurCurent();
	DevGetDin();
	GetCruError();
	//DevCtrleEndable();
       DevSetLifeGuard(2000);

	
            if( driverprco.m_BitStatus.bit1_isENA_SRV !=1 ){
                    //DevCtrleEndable(vecpa);
                    return 0;
                }
	if(driverprco.GetStrError()!="OK"){
			 return 0;
		}
	if(onlineMsg.online !=FN_ONLINE ){
                     return 0;
                 }
        
        return 1;
}

int sytronDriverMode::DriveEmergencyStop()
{
	
   // std::cout << "DriveEmergencyStop is running"<<std::endl;
	
   //DevSetLifeGuard(0);
   SpeedClear();
   
   DevSetspeed(0);   
     
   DevEmergencyStop();
  
   GetSpeedCmd(); 

   DevGetActPos();

   DevGetCurCurent();
   DevGetDin();
   GetCruError();
   DevSetLifeGuard(2000);
   if(onlineMsg.online !=FN_ONLINE ){
                     return 0;
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

int sytronDriverMode::DriveStop()
{
   // std::cout << "DriveStop is DriveStop"<<std::endl;
    
   CmdStop();
    
   SpeedClear();

   DevSetspeed(velocity);  
   
   GetSpeedCmd(); 

   DevGetActPos();

   DevGetCurCurent();

   GetCruError();
   DevSetLifeGuard(2000);
   if(onlineMsg.online !=FN_ONLINE ){
                     return 0;
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

#ifdef ROSIN
void sytronDriverMode::SetSpeed(short int speed)
{
   
    velocity = (speed>MAXSPEEDRPM?MAXSPEEDRPM:speed);
}

void sytronDriverMode::SpeedClear()
{
   
    velocity = 0;
}

//SUB 接收命令 
void sytronDriverMode::Net2Ctrl_driverCall(const dgvmsg::DriverVelocity::ConstPtr& cmdmsg)
{
        //ROS_INFO("ros sub:");
         short int speed=0; 

        //前进速度
        int size = cmdmsg->driver.size();
	 #if 0
	 std::cout<<"Net2Ctrl_driverCall="<<size<<std::endl;
	 #endif
	for(int index=0;index<size;index++){
			// std::cout<<"cmdmsg->driver[index].add ="<<cmdmsg->driver[index].add<<std::endl;
			if(cmdmsg->driver[index].add ==DEV_add){
					speed = cmdmsg->driver[index].VRPM;
					SetSpeed(speed);
					//std::cout<<"add="<<(int)this->driverprco.Getadd();
					//std::cout<<" speed="<<speed<<std::endl;
				}
		}
    
    if(statues_ptr != FUN_SPEEDLOOP){
                statues_ptr = FUN_REENABLE;
        }  
    
      //  ClearJoyFlag();  
}
void sytronDriverMode::Net2SCtrl_driverCall(const std_msgs::String::ConstPtr& ctrlcmd)
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


void sytronDriverMode::dirv_pub_encounter()
{   
	dgvmsg::Encounter encounter;
	
	//encounter.devadd = driverprco.add();
	encounter.devadd = DEV_add;
	encounter.encounter = driverprco.CruPos();
	encounter.now = ros::Time::now();

	rospub_mode_driverencounter.publish(encounter);
	ros::spinOnce();
	
}
/*
void sytronDriverMode::dirv_pub_allstatues()
{   
    std_msgs::String stdmsg;
    
     stdmsg.data = Pack_driver_statues();
     
     rospub_mode_driverstatues.publish(stdmsg);
     ros::spinOnce();
}
*/
/*
std::string  sytronDriverMode::Pack_driver_statues()
{       
         // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;

       ControllerMessage_Dirver driver_status;
      //driver_status.r

      for(int index=0;index<1;index++){
        
       driver_status.set_device_status((::google::protobuf::int32)onlineMsg.online);
      // driver_status.set_device_add((::google::protobuf::int32)driverprco.add());
      driver_status.set_device_add((::google::protobuf::int32)DEV_add);
       driver_status.set_actvelocity(driverprco.cru_speed()*driverprco.dir());
       //driver_status.set_setvelocity(driverprco.cru_speedcmd()*driverprco.dir());
       driver_status.set_setvelocity(velocity*driverprco.dir());
       
       driver_status.set_ctrl_dc_voltage(driverprco.Udc());

       driver_status.set_ctrl_ipm_temperature(driverprco.IpmTem());
       driver_status.set_actcurrent((float)driverprco.effectiveTorque_I());
       
       driver_status.set_actualpos(driverprco.CruPos()*driverprco.dir());
	 if(driverprco.Dins()==1){  //没急停
       		driver_status.set_emergencyinputvalue(0);
	 	}
	 else{
		driver_status.set_emergencyinputvalue(1);
	 	}
      driver_status.clear_motec();
       driver_status.set_error(driverprco.GetStrError());
        }

       
       std::string res;
       if (!driver_status.SerializeToString(&res)  ) {
           std::cerr << "Failed to write msg driver Message." << std::endl; 
           return ""; 
       }  
    return res;
}  
*/
void sytronDriverMode::Pack_driver_msg_statues()
{       
       dgvmsg::DriverStatues feedback;

	feedback.device_status = onlineMsg.online;
	feedback.device_add = DEV_add;
	feedback.actvelocityRPM = ((driverprco.cru_speed())*driverprco.dir());
	feedback.setvelocityRPM = velocity*driverprco.dir();
	feedback.dc_voltage = driverprco.Udc();
	feedback.ipm_temperature = driverprco.IpmTem();
	feedback.actcurrent =  (float)driverprco.effectiveTorque_I();
	feedback.actualpos = driverprco.CruPos()*driverprco.dir();
	feedback.emergencyinputvalue =driverprco.m_BitStatus.bit5_EMGING;
	feedback.errorcode	= driverprco.GetStrError();

     rospub_mode_driverstatues.publish(feedback);
     ros::spinOnce();
	
}  

int sytronDriverMode::check_recv5bits(std::string &data) 
{   
	if(data[0] != driverprco.add()){
		return -1;
		}
	if(data[1] != 0x86){
		return -1;
		}	
	if(data[2] != 0x04){
		return -1;
		}
	return 1;
}

#endif

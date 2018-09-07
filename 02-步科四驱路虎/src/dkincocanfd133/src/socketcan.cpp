
#include "socketcan.hpp"

#include <numeric>
#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>

#include <sys/time.h>   
#include <fcntl.h>


#include <linux/can.h>
#include <linux/can/raw.h>

#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#define DEBUG_ECHO 0

//using namespace boost::assign;  

socketcan::socketcan(std::string id):canport(id)
{ 
    //接收不到任何东西
        struct sockaddr_can addr;
        //struct canfd_frame frame;
        struct ifreq ifr; 
        //struct can_filter rfilter;   /*接收过滤*/
        
	/* open socket */
	if ((canFD = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		//return 1;
		throw "can open socket error";
	}
    
	strncpy(ifr.ifr_name, canport.c_str(), IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex");
		throw "can if_nametoindex  error";
	}   
    
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
        
	 //定义接收规则，只接收表示符等于0x11的报文  
	 //rfilter.can_id   = 0x123;  
	 //rfilter.can_mask = CAN_SFF_MASK; 
 	 //设置过滤规则  
   
	/* disable default receive filter on this RAW socket */
	/* This is obsolete as we do not read from the socket at all, but for */
	/* this reason we can remove the receive list in the Kernel to save a */
	/* little (really a very little!) CPU usage.                          */
	// setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));  
         //setsockopt(canFD, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0); 
     
	if (bind(canFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		//return 1;
		throw "can setsockopt  error";
	}
   m_recv_datecounter=0;
   m_send_datecounter = 0;
   
   buf_ptr = boost::shared_ptr<boost::circular_buffer<struct  can_frame> > (
    new boost::circular_buffer<struct  can_frame> (BUF_MAX_SIZE)
    );
     //创建接收线程
     
   boost::function0<void> f = boost::bind(&socketcan::th_sync_noblock_read, this);  
   canrecv_ptr = boost::shared_ptr<boost::thread>(new boost::thread (f));


    

}

socketcan::socketcan(std::string id,int filterid):canport(id),filter_id(filterid)
{ 
        struct sockaddr_can addr;
        //struct canfd_frame frame;
        struct ifreq ifr; 
        struct can_filter rfilter;   /*接收过滤*/
        
	/* open socket */
	if ((canFD = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		//return 1;
		throw "can open socket error";
	}
    
	strncpy(ifr.ifr_name, canport.c_str(), IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex");
		throw "can if_nametoindex  error";
	}   
    
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
        
	 //定义接收规则，只接收表示符等于0x11的报文  
	 rfilter.can_id   = filter_id;  
	 rfilter.can_mask = CAN_SFF_MASK; 
 	 //设置过滤规则  
   
	/* disable default receive filter on this RAW socket */
	/* This is obsolete as we do not read from the socket at all, but for */
	/* this reason we can remove the receive list in the Kernel to save a */
	/* little (really a very little!) CPU usage.                          */
	setsockopt(canFD, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));  
         //setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0); 
     
	if (bind(canFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		//return 1;
		throw "can setsockopt  error";
	}
    m_recv_datecounter=0;
   m_send_datecounter = 0;
   
    buf_ptr = boost::shared_ptr<boost::circular_buffer<struct  can_frame> > (
     new boost::circular_buffer<struct  can_frame> (BUF_MAX_SIZE)
     );
    //创建接收线程
    boost::function0<void> f = boost::bind(&socketcan::th_sync_noblock_read, this);  
    canrecv_ptr = boost::shared_ptr<boost::thread>(new boost::thread (f));



}
socketcan::socketcan(std::string id,int *filterid,int filtCnt):canport(id)
{ 
        struct sockaddr_can addr;
        //struct canfd_frame frame;
        struct ifreq ifr; 
        struct can_filter *rfilter = new  (struct can_filter[filtCnt]) ;   /*接收过滤*/
        
	/* open socket */
	if ((canFD = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		//return 1;
		throw "can open socket error";
	}
    
	strncpy(ifr.ifr_name, canport.c_str(), IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex");
		throw "can if_nametoindex  error";
	}   
    
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
        
	 //定义接收规则，只接收表示符等于0x11的报文  
	 for(int index =0 ; index <filtCnt ; index++ ){
	 rfilter[index].can_id   = filterid[index];  
	 rfilter[index].can_mask = CAN_SFF_MASK; 
       }
 	 //设置过滤规则  
            setsockopt(canFD, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));  
	/* disable default receive filter on this RAW socket */
	/* This is obsolete as we do not read from the socket at all, but for */
	/* this reason we can remove the receive list in the Kernel to save a */
	/* little (really a very little!) CPU usage.                          */
	
         //setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0); 
     
	if (bind(canFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		//return 1;
		throw "can setsockopt  error";
	}
    m_recv_datecounter=0;
   m_send_datecounter = 0;
   
    buf_ptr = boost::shared_ptr<boost::circular_buffer<struct  can_frame> > (
     new boost::circular_buffer<struct  can_frame> (BUF_MAX_SIZE)
     );
    //创建接收线程
    boost::function0<void> f = boost::bind(&socketcan::th_sync_noblock_read, this);  
    canrecv_ptr = boost::shared_ptr<boost::thread>(new boost::thread (f));



}



socketcan::~socketcan()
{
    canrecv_ptr->interrupt();
    canrecv_ptr->join();
    
    close(canFD);
}

int socketcan::send(const std::string &cmd){
    int id=0;
    struct  can_frame sendfram;
    
    id += cmd[1];
    id<<=8;
    id += cmd[2];
    id<<=8;
    id += cmd[3];
    id<<=8;
    id += cmd[4];
    
    sendfram.can_id &= CAN_SFF_MASK;
 
    sendfram.can_dlc =  cmd[0]&0x0f;
    
    for(int index =0 ;index<sendfram.can_dlc;index++){
            sendfram.data[index] = (unsigned char)cmd[5+index];
        }
    
    /* send frame */
    if (write(canFD, &sendfram, sizeof(struct  can_frame)) !=  sizeof(struct  can_frame)) {
        perror("write");
        throw "can send  error";
    }
    return sizeof(struct  can_frame);

    
}
void socketcan::sendRequest(const std::string &cmd){
    int id=0;
    struct  can_frame sendfram;
    
    id += cmd[1];
    id<<=8;
    id += cmd[2];
    id<<=8;
    id += cmd[3];
    id<<=8;
    id += cmd[4];
    
    sendfram.can_id &= CAN_SFF_MASK;
 
    sendfram.can_dlc =  cmd[0]&0x0f;
    
    for(int index =0 ;index<sendfram.can_dlc;index++){
            sendfram.data[index] = (unsigned char)cmd[5+index];
        }
    
    /* send frame */
    if (write(canFD, &sendfram, sizeof(struct  can_frame)) !=  sizeof(struct  can_frame)) {

        //perror("write");
        //throw "can send  error";
    }
    m_send_datecounter++;
}

int socketcan::send(boost::array <char ,13> &cmd){
    int id=0;
    struct  can_frame sendfram;
    
    id += cmd[1];
    id<<=8;
    id += cmd[2];
    id<<=8;
    id += cmd[3];
    id<<=8;
    id += cmd[4];
    
    sendfram.can_id &= CAN_SFF_MASK;
 
    sendfram.can_dlc =  cmd[0]&0x0f;
    
    for(int index =0 ;index<sendfram.can_dlc;index++){
            sendfram.data[index] = (unsigned char)cmd[5+index];
        }
    
    /* send frame */
    if (write(canFD, &sendfram, sizeof(struct  can_frame)) !=  sizeof(struct  can_frame)) {
        perror("write");
        throw "can send  error";
    }
    return sizeof(struct  can_frame);  
}

void socketcan::th_sync_block_read()
{

           int ret = -1;
           struct  can_frame frame;
           //boost::circular_buffer<struct  can_frame> frambuffer(100);
           while(1){
                   ret = read(canFD, &frame, sizeof(frame));  //接收报文 
                   if(ret > 0){
        		//printf("index=%d ID=D% DLC=%d data[0]=0x02X%\n", ret,frame.can_id,
        	               // frame.can_dlc, frame.data[0]);
        	                buf_ptr->push_back(frame);
                           m_recv_datecounter++;
        	                //boost::format fmt("index=%1% ID=%2%  DLC=%3% data=[%4%,%5%,%6%,%7%,%8%,%9%,%10%,%11%] ");
                    	 // fmt %buf_ptr->size();
                            //fmt % (boost::format("%04X ")%(frame.can_id&CAN_SFF_MASK));
                           // fmt %(int)frame.can_dlc;
                            //fmt %(boost::format("%02X ")%(int)frame.data[0]);
                           // fmt %(boost::format("%02X ")%(int)frame.data[1]);
                            //fmt %(boost::format("%02X ")%(int)frame.data[2]);
                            //fmt %(boost::format("%02X ")%(int)frame.data[3]);
                      	  //fmt %(boost::format("%02X ")%(int)frame.data[4]);
                            //fmt %(boost::format("%02X ")%(int)frame.data[5]);
                           // fmt %(boost::format("%02X ")%(int)frame.data[6]);
                            //fmt %(boost::format("%02X ")%(int)frame.data[7]);
                           
                           // std::cout<<fmt.str()<<std::endl;
                            
                            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
                 } 
            }
}
void socketcan::th_sync_noblock_read()
{
           fd_set inputs, testfds;
           struct timeval timeout; 
           
           
           int ret = -1;
            int result = -1;
           struct  can_frame frame;
  
           FD_ZERO(&inputs);//用select函数之前先把集合清零 
           FD_SET(canFD,&inputs);//把要检测的句柄——标准输入（0），加入到集合里。 
         try{       
           while(1){
                      testfds = inputs;   
                      timeout.tv_sec = 0;   
                      timeout.tv_usec = 5*1000;   
                      result = select(canFD+1, &testfds, NULL, NULL, &timeout); 
                      boost::this_thread::interruption_point();  
                      switch(result)   
                             {   
                             case 0:   
                                 //printf("timeout/n");   
                                 break;  
                             case -1:   
                                 perror("select");   
                                 //exit(1);   
                             default:   
                                 if(FD_ISSET(canFD,&testfds))   
                                 {   
                                 ret = read(canFD, &frame, sizeof(frame));  //接收报文 
                                  if(ret > 0){
                                         buf_ptr->push_back(frame);
                                         m_recv_datecounter++;
                                         //print_fram(frame);
                                         /*
            	                      boost::format fmt("index=%1% ID=%2%  DLC=%3% data=[%4%,%5%,%6%,%7%,%8%,%9%,%10%,%11%] ");
                                	    fmt %buf_ptr->size();
                                        fmt % (boost::format("%04X ")%(frame.can_id&CAN_SFF_MASK));
                                        fmt %(int)frame.can_dlc;
                                        fmt %(boost::format("%02X ")%(int)frame.data[0]);
                                        fmt %(boost::format("%02X ")%(int)frame.data[1]);
                                        fmt %(boost::format("%02X ")%(int)frame.data[2]);
                                        fmt %(boost::format("%02X ")%(int)frame.data[3]);
                                  	    fmt %(boost::format("%02X ")%(int)frame.data[4]);
                                        fmt %(boost::format("%02X ")%(int)frame.data[5]);
                                        fmt %(boost::format("%02X ")%(int)frame.data[6]);
                                        fmt %(boost::format("%02X ")%(int)frame.data[7]);
                               
                                        std::cout<<fmt.str()<<std::endl;     
                                        */
                                    }
                               }   
                               break;   
                            }   
            }
        }
       catch(...){
                  std::cout << "th_sync_noblock_read Interrupt exception was thrown." << std::endl;  
            }
}


int socketcan::is_recvbuf_empty(){

    if(buf_ptr->size()){
    return 0;
        }
    return 1;
}
int socketcan::is_recvbuf_full(){

    if(buf_ptr->size()>=BUF_MAX_SIZE){
    return 1;
        }
    return 0;
}
int socketcan::recvbuf_size(){

   return buf_ptr->size();

}

int socketcan::isresvdate(){
           return buf_ptr->size();
}
void socketcan::clear(){
     int index=0;
    while(buf_ptr->size()!=0){
            buf_ptr->pop_front();
            #if DEBUG_ECHO
            std::cout<<"clear "<<index<<std::endl;
            #endif
            index++;
        }
}

void socketcan::pop_cur_toarray(){
        struct  can_frame frame;
        
        if(buf_ptr->size()>0){
                    frame = (*buf_ptr->begin());
                    //print_fram(frame);
                    buf_ptr->pop_front();
                    
                    cur_data[0] = frame.can_dlc;
                    cur_data[1] =(char)((frame.can_id&0xff000000)>>24);
                    cur_data[2] =(char)((frame.can_id&0x00ff0000)>>16);
                    cur_data[3] =(char)((frame.can_id&0x0000ff00)>>8);
                    cur_data[4] =(char)((frame.can_id&0x000000ff));
                    cur_data[5] = frame.data[0];
                    cur_data[6] = frame.data[1];
                    cur_data[7] = frame.data[2];
                    cur_data[8] = frame.data[3];
                    cur_data[9] = frame.data[4];
                    cur_data[10] = frame.data[5];
                    cur_data[11] = frame.data[6];
                    cur_data[12] = frame.data[7];
                    //print_hex();
            }
}

void socketcan::print_fram(struct  can_frame frame)
{

    boost::format fmt("index=%1% ID=%2%  DLC=%3% data=[%4%,%5%,%6%,%7%,%8%,%9%,%10%,%11%] ");
      fmt %buf_ptr->size();
      fmt % (boost::format("%04X ")%(frame.can_id&CAN_SFF_MASK));
      fmt %(int)frame.can_dlc;
      fmt %(boost::format("%02X ")%(int)frame.data[0]);
      fmt %(boost::format("%02X ")%(int)frame.data[1]);
      fmt %(boost::format("%02X ")%(int)frame.data[2]);
      fmt %(boost::format("%02X ")%(int)frame.data[3]);
      fmt %(boost::format("%02X ")%(int)frame.data[4]);
      fmt %(boost::format("%02X ")%(int)frame.data[5]);
      fmt %(boost::format("%02X ")%(int)frame.data[6]);
      fmt %(boost::format("%02X ")%(int)frame.data[7]);

      std::cout<<fmt.str()<<std::endl;   

}
void socketcan::print_fram_back()
{
    if(buf_ptr->size()>0){
            struct  can_frame frame;
            frame = *(buf_ptr->begin()+(buf_ptr->size()-1));
            boost::format fmt("index=%1% ID=%2%  DLC=%3% data=[%4%,%5%,%6%,%7%,%8%,%9%,%10%,%11%] ");
              fmt %buf_ptr->size();
              fmt % (boost::format("%04X ")%(frame.can_id&CAN_SFF_MASK));
              fmt %(int)frame.can_dlc;
              fmt %(boost::format("%02X ")%(int)frame.data[0]);
              fmt %(boost::format("%02X ")%(int)frame.data[1]);
              fmt %(boost::format("%02X ")%(int)frame.data[2]);
              fmt %(boost::format("%02X ")%(int)frame.data[3]);
              fmt %(boost::format("%02X ")%(int)frame.data[4]);
              fmt %(boost::format("%02X ")%(int)frame.data[5]);
              fmt %(boost::format("%02X ")%(int)frame.data[6]);
              fmt %(boost::format("%02X ")%(int)frame.data[7]);

              std::cout<<fmt.str()<<std::endl;   
      }

}
void socketcan::print_fram_all()
{
 
            struct  can_frame frame;
            int size = buf_ptr->size();
           
            for(int index =0;index <size;index++){
                 frame = *(buf_ptr->begin()+index);
            boost::format fmt("index=%1% ID=%2%  DLC=%3% data=[%4%,%5%,%6%,%7%,%8%,%9%,%10%,%11%] ");
              fmt %buf_ptr->size();
              fmt % (boost::format("%04X ")%(frame.can_id&CAN_SFF_MASK));
              fmt %(int)frame.can_dlc;
              fmt %(boost::format("%02X ")%(int)frame.data[0]);
              fmt %(boost::format("%02X ")%(int)frame.data[1]);
              fmt %(boost::format("%02X ")%(int)frame.data[2]);
              fmt %(boost::format("%02X ")%(int)frame.data[3]);
              fmt %(boost::format("%02X ")%(int)frame.data[4]);
              fmt %(boost::format("%02X ")%(int)frame.data[5]);
              fmt %(boost::format("%02X ")%(int)frame.data[6]);
              fmt %(boost::format("%02X ")%(int)frame.data[7]);

              std::cout<<fmt.str()<<std::endl;   
                }


}

void socketcan::print_hex(){

 std::cout<< "socketcan::print_hex:";
 for(std::size_t index=0;index<13;index++){
                  int x=(unsigned  char)cur_data[index];
                   std::cout<<boost::format("%02X ")%x;
              }
std::cout<<"\n";

}



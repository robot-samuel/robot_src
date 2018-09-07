#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include <numeric>
#include <string>
#include <iostream>
#include <vector>


#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"

#include "geometry_msgs/Twist.h"
#include "dgvmsg/DriverVelocity.h"
#include "dgvmsg/DriverNode.h"
#include "dgvmsg/Encounter.h"
#include "dgvmsg/EncounterV.h"


#include "zmqclient.h"

#include "socketcan.hpp"
#include "kincocanfd133_Driver.h"
#include "message.hpp"




#define RECV_WAITE_TIME 20
#define RECV_DATA_SIZE 1
#define  RECV_NO_COUNTER 4
#define MAXSPEEDRPM 30000
#define DRIVER_AMOUNT 2

using namespace std;
using namespace boost;



class kincors232Fd133Driver;

class OnlineLogMessage
{
public:
       int online;
	int timeout;
       boost::posix_time::ptime time_recvnow;  
       boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse;
       OnlineLogMessage()
       	{
		online = 0; 
		timeout = 2;
		time_recvnow = boost::posix_time::microsec_clock::universal_time();  
       	}
	int check_connect()
	{   

	    if(online == 1)
	        {
	            //
	             boost::posix_time::ptime now;
	            // 这里为微秒为单位;这里可以将microsec_clock替换成second_clock以秒为单位;  
	            now = boost::posix_time::microsec_clock::universal_time();
	            time_elapse = now - time_recvnow;
	            // 类似GetTickCount，只是这边得到的是2个时间的ticket值的差，以微秒为单位;  
	            int ticks = time_elapse.ticks();  
	            // 得到两个时间间隔的秒数;   
	            int sec = time_elapse.total_seconds();
	            if(sec>=timeout){
	                    online = 0;
	                }
	            
	        
		}
        return online;
	        
	} 

	int Set_connect()
	{   
              online=1;
                    // 这里为微秒为单位;这里可以将microsec_clock替换成second_clock以秒为单位;  
               time_recvnow = boost::posix_time::microsec_clock::universal_time();  
	        
	} 
	
	void Set_timeout(int time)
	{   
              timeout=time;   
	}	
       
};

class kincocanfd133Mode
{
public:    
   enum dev_add{
            MYADD  =100,
            DEV_DIR_ADD_LF =1,
            DEV_DIR_ADD_LB =2,
            DEV_DIR_ADD_RF =3,
            DEV_DIR_ADD_RB =4,
    };
private:
    kincocanfd133Mode(const kincocanfd133Mode& rhs);
    kincocanfd133Mode& operator=(const kincocanfd133Mode& rhs);
    

public:
	enum DEV_FUN{
	    FUN_INIT=1,   
	    FUN_SPEEDLOOP, 
	    FUN_KEEPERROR, 
	    FUN_RELEASE,   
	    FUN_ENABLE,
	    FUN_CLEARERROR,
	    FUN_EMERAGENCE,
	    FUN_STOPRUN,
	    FUN_REENABLE,
	    FUN_DEFAULT,
	   }; 
    enum ONLINE{
	FN_ONLINE=1,
	FN_LOST=0
        };
	
	
   kincocanfd133Mode();
   kincocanfd133Mode(std::string port,int *devadd,int *dir,int *driveadd,int cnt);
	
    ~kincocanfd133Mode();

	void kincocanfd133Mode_master(std::vector < boost::shared_ptr<socketcan> > &vecpa);

	
	
	void print_dec(unsigned char *datBUF,int Num);
	void print_hex(unsigned char *datBUF,int Num);
	void print_hex(std::string str);

	int DriverecvDelay(
	    boost::shared_ptr<socketcan> pa,
	    int size,
	    int timeout,
	    std::string errprint);

	void SpeedClear();
	void SetSpeed(int speed,int cnt);
	
	int DriveInit(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DriveSpeedCtrl(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	int DriveRelease(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	int DriveEnable(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	int DriveREEnable(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	int DriveEmergencyStop(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	int DriveStop(std::vector < boost::shared_ptr<socketcan> > &vecpa);

	void DevGetUdc(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevGetCurCurent(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevGetActPos(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevGetVelocity(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void GetCruError1(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void GetSerialNo(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevSetspeed(std::vector < boost::shared_ptr<socketcan> > &vecpa,int *vec_speed);
	void DevCtrleEndable(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevCtrlDisEndable(std::vector < boost::shared_ptr<socketcan> > &vecpa);

	void CmdStop(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevEmergencyStop(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevGetDrivStatus(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void GetSpeedCmd(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevGetError(std::vector < boost::shared_ptr<socketcan> > &vecpa);
	void DevSetClearError(std::vector < boost::shared_ptr<socketcan> > &vecpa);


	void StatusRecord( boost::shared_ptr<socketcan> &vecpa,int cnt);
	void dirv_pub_encounter(int cnt);
	void dirv_pub_allstatues(int cnt);
	std::string  Pack_driver_statues(int cnt);
	
	
public:
//ros
	ros::Publisher rospub_mode_driverstatues;
	ros::Publisher rospub_mode_encounter;

	void Net2Ctrl_driverCall(const dgvmsg::DriverVelocity::ConstPtr& cmdmsg);
	void Net2SCtrl_driverCall(const std_msgs::String::ConstPtr& ctrlcmd);
private:
	std::string ttyport;
	kincors232Fd133Driver  Fd133Driver[DRIVER_AMOUNT];
	int  add[DRIVER_AMOUNT];
	OnlineLogMessage onlineMsg[DRIVER_AMOUNT];

	int statues_ptr;
	int   velocity[DRIVER_AMOUNT];  //速度指令
};






#pragma once


class laser_ctrl_Mode
{
    

private:
    laser_ctrl_Mode(const laser_ctrl_Mode& rhs);
    laser_ctrl_Mode& operator=(const laser_ctrl_Mode& rhs);
    
  enum { nomal_length = 12 };
   enum { HEAD = 0x88,
   		SEND = 0xAA};
   
   enum dev_add{
            MYADD  =100,
            DEV_DIR_ADD_LF =100,
            DEV_DIR_ADD_LB =102,
            DEV_DIR_ADD_RF =101,
            DEV_DIR_ADD_RB =103,
    };
public:


    laser_ctrl_Mode();
    
    ~laser_ctrl_Mode();

	
    bool procss_recv_buffer(std::string receivedData);
	unsigned short int CRC16(unsigned char *p,unsigned int len);
	int  Get_Sdata2();
	int  Get_Sdata1();
	int  Get_Sdata3();
	int  Get_Sdata4();
	int  Get_check();

    std::string  TC_return_package();
	std::string  TC_cmd_package(float speed,float th);
	void ByteClear(unsigned char * data,int size);
	
public:

	int recv_ptr;
	int recv_count;
	bool comRecvFlag;
    unsigned char Readarray[20];
	std::string recvbuffer;
	
	float vspeed;  //线速度
	float aspeed;  //角速度

	int LF_counter;
	int RF_counter;
	int LB_counter;
	int RB_counter;
	
	int DLF_counter;
	int DRF_counter;
	int DLB_counter;
	int DRB_counter;	

	
};






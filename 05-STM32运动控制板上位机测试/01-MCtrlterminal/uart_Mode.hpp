#pragma once


class uart_Mode
{
    

private:
    uart_Mode(const uart_Mode& rhs);
    uart_Mode& operator=(const uart_Mode& rhs);
    
  enum { nomal_length = 12,
  		nomal_recvlength = 25};
   enum { RECVHEAD1 = 0x88,
   		 RECVHEAD2 = 0xBB};
   

public:


    uart_Mode();
    
    ~uart_Mode();

	
    bool procss_recv_buffer(std::string receivedData);
	unsigned short int CRC16(unsigned char *p,unsigned int len);
	int  Get_Sdata2();
	int  Get_Sdata1();
	int  Get_Sdata3();
	int  Get_Sdata4();
	unsigned char  Get_check();

    std::string  TC_return_package();
	std::string  TC_cmd_package(float speed,float th);
	void ByteClear(unsigned char * data,int size);

	void  Get_data();
	
public:

	int recv_ptr;
	int recv_count;
	bool comRecvFlag;
	unsigned char Readarray[nomal_recvlength+1];
	std::string recvbuffer;
	
	float vspeed;  //线速度
	float aspeed;  //角速度

	int LF_counter;
	int RF_counter;
	int LB_counter;
	int RB_counter;

	unsigned char  LFMotor_Status;
	unsigned char  RFMotor_Status;
	unsigned char  LBMotor_Status;
	unsigned char  RBMotor_Status;

	unsigned short int LFMotor_error;
	unsigned short int RFMotor_error;
	unsigned short int LBMotor_error;
	unsigned short int RBMotor_error;

	unsigned char Obt1_dis;
	unsigned char Obt2_dis;
	unsigned char Obt3_dis;
	unsigned char Obt4_dis;

	
	unsigned char Obt_Status;
	unsigned char Com_Status;
	unsigned char IO_Status;
	
	
};






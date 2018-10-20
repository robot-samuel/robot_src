#pragma once


class uart_Mode
{
    

private:
    uart_Mode(const uart_Mode& rhs);
    uart_Mode& operator=(const uart_Mode& rhs);
    
  enum { nomal_length = 13,
  		nomal_recvlength = 39};
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


	unsigned char  LFMotor_Status;
	unsigned char  RFMotor_Status;


	unsigned short int LFMotor_error;
	unsigned short int RFMotor_error;


	unsigned char Obt1_dis;
	unsigned char Obt2_dis;
	
	char TEMA;
	unsigned char HUMA;
	char TEMB;
	unsigned char HUMB;
	
	unsigned char BOS;
	float VOT;
	float CRROUENT;
	
	float CO_vau;
	float EX_vau;
	float O2_vau;
	float H2S_vau;
	
	
	unsigned char Com_Status;
	unsigned char IO_Status;
	
	
};






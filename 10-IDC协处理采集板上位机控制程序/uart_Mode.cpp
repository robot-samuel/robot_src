

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream> 
#include <QString> 


#include "uart_Mode.hpp"

uart_Mode::uart_Mode()
{
	 recv_ptr = 0;
	 comRecvFlag = false;
	 recv_count=0;
	 

	 vspeed =0;  //线速度
	 aspeed =0;  //角速度


	 LF_counter=0;
	 RF_counter=0;




	LFMotor_Status=0;
	RFMotor_Status=0;


	LFMotor_error=0;
	RFMotor_error=0;


	Obt1_dis=0;
	Obt2_dis=0;

	TEMA=-100;
	HUMA = 0;
	TEMB=-100;
	HUMB = 0;
	
	BOS = 0;
	VOT = 48;
	CRROUENT = 0.0;
	
	CO_vau = 0.0;
	EX_vau = 0.0;
	O2_vau = 0.0;
	H2S_vau = 0.0;
	

	Com_Status=0;
	IO_Status=0;
	
}

uart_Mode::~uart_Mode()
{

}

bool uart_Mode::procss_recv_buffer(std::string receivedData)
{
    const char* ch=receivedData.c_str();
    int size = receivedData.size();
	
	
	for(int index=0;index<size;index++){
		unsigned char data=(unsigned char)ch[index];
        //QString str1;
        //str1.sprintf("data[%d]=%02X ",index,data);
        //std::cout<<str1.toStdString()<<std::endl;
	 	switch(recv_ptr){
			case 0:
				if(data==RECVHEAD1){
						recv_ptr=1;
						recv_count=0;
					}
				break;
			case 1:
				if(data==RECVHEAD2){
						recv_ptr=2;
						recv_count=0;
                        			ByteClear(Readarray,nomal_recvlength);
					}
				else{
						recv_ptr=0;
						recv_count=0;
                        			std::cout<<"recv wrong 2"<<std::endl;
					}
				break;
			case 2:
				if(data==(nomal_recvlength-1)){
						recv_ptr=3;
						recv_count=0;
                        			ByteClear(Readarray,nomal_recvlength);
					}
				else{
						recv_ptr=0;
						recv_count=0;
                        			std::cout<<"recv wrong 3"<<std::endl;
					}
				break;
			case 3:
				Readarray[recv_count]=data;
				recv_count++;
                		if(recv_count>=nomal_recvlength-3){
						//std::cout<<"recv_count = "<<recv_count<<std::endl;
						recv_ptr=0;
						recv_count=0;
						
						if(1==Get_check()){
							//
							Get_data();
							ByteClear(Readarray,nomal_recvlength);
                             			std::cout<<"recv a fram data"<<std::endl;
							return true;
							}
                        			std::cout<<"recv wrong 4"<<std::endl;
						return false;
					}
				break;				
			default:
				recv_ptr=0;
				recv_count=0;
                ByteClear(Readarray,nomal_recvlength);
                std::cout<<"recv wrong 1"<<std::endl;
				break;

	 		}
		}

	return false;

}
unsigned short int uart_Mode::CRC16(unsigned char *p,unsigned int len)
{
	unsigned char i;
	int j;
	unsigned short int uiCRC = 0xffff;

	for(j=0;j<len;j++)
	{
		uiCRC ^= (*p);
		p++;
		for( i=8; i != 0; i--)
		{
			if( uiCRC & 1 ){
				uiCRC >>= 1;
				uiCRC ^= 0xa001;
			}
			else{
				uiCRC>>=1;
			}
		}
	}
	return(uiCRC);
}
int  uart_Mode::Get_Sdata4()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[12];
	* (ps1+2) = Readarray[13];	
	* (ps1+1) = Readarray[14];	
	* (ps1+0) = Readarray[15];
	
	return Data1;
}
int  uart_Mode::Get_Sdata3()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[8];
	* (ps1+2) = Readarray[9];	
	* (ps1+1) = Readarray[10];	
	* (ps1+0) = Readarray[11];
	
	return Data1;
}
int  uart_Mode::Get_Sdata2()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[4];
	* (ps1+2) = Readarray[5];	
	* (ps1+1) = Readarray[6];	
	* (ps1+0) = Readarray[7];
	
	return Data1;
}

int  uart_Mode::Get_Sdata1()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[0];
	* (ps1+2) = Readarray[1];	
	* (ps1+1) = Readarray[2];	
	* (ps1+0) = Readarray[3];	
	
	return Data1;
}
unsigned char  uart_Mode::Get_check()
{
	unsigned char Data1=0;
	Data1 +=  RECVHEAD1;
	Data1 +=  RECVHEAD2;
	Data1 +=  (nomal_recvlength-1);
    for(int i =0 ;i<(nomal_recvlength-4);i++){
			Data1 +=Readarray[i];
		}

    unsigned char rsum= Readarray[nomal_recvlength-4];
	


         QString strData1;
         strData1.sprintf("%02X ",Data1);
         QString strrsum;
         strrsum.sprintf("%02X ",rsum);

    std::cout<<"strData1 = " <<strData1.toStdString();
    std::cout<<" strrsum = " <<strrsum.toStdString();
	std::cout<<std::endl;	 

    if(Data1 == rsum){return 1;}
    return 0;
}

void  uart_Mode::Get_data(){
	
	int index=0;
	unsigned char * ps1  = (unsigned char *)(&LF_counter);
	
	* (ps1+3) = Readarray[index++];//* (ps1+3) = Readarray[0];
	* (ps1+2) = Readarray[index++];//* (ps1+2) = Readarray[1];	
	* (ps1+1) = Readarray[index++];	//* (ps1+1) = Readarray[2];
	* (ps1+0) = Readarray[index++];//* (ps1+0) = Readarray[3];
	
	std::cout<<" LF_counter = " <<LF_counter<<std::endl;	

	unsigned char * ps2  = (unsigned char *)(&RF_counter);
	
	* (ps2+3) = Readarray[index++];  //* (ps2+3) = Readarray[4];
	* (ps2+2) = Readarray[index++];	
	* (ps2+1) = Readarray[index++];	
	* (ps2+0) = Readarray[index++];
	
	std::cout<<" RF_counter = " <<RF_counter<<std::endl;	

	LFMotor_Status = Readarray[index++];   //Readarray[8];
	RFMotor_Status = Readarray[index++];

	unsigned char * ps3  = (unsigned char *)(&LFMotor_error);
	
	* (ps3+1) = Readarray[index++];
	* (ps3+0) = Readarray[index++];	

	unsigned char * ps4  = (unsigned char *)(&RFMotor_error);
	
	* (ps4+1) = Readarray[index++];
	* (ps4+0) = Readarray[index++];	

	Obt1_dis = Readarray[index++];
	Obt2_dis = Readarray[index++];


	TEMA= Readarray[index++];
	HUMA= Readarray[index++];
	TEMB= Readarray[index++];
	HUMB= Readarray[index++];


	BOS = Readarray[index++];
	
	unsigned short int Ivot=0;
	ps4  = (unsigned char *)(&Ivot);
	* (ps4+1) = Readarray[index++];
	* (ps4+0) = Readarray[index++];	
	
    VOT = (Ivot*1.0)/100.0;
	
     short int Icurent=0;
	ps4  = (unsigned char *)(&Icurent);
	* (ps4+1) = Readarray[index++];
	* (ps4+0) = Readarray[index++];

    CRROUENT =(Icurent*1.0)/100.0;
	
	unsigned short int vau=0;

	ps4  = (unsigned char *)(&vau);
	* (ps4+1) = Readarray[index++];
	* (ps4+0) = Readarray[index++];	
	CO_vau = vau/100;

	ps4  = (unsigned char *)(&vau);
	* (ps4+1) = Readarray[index++];
	* (ps4+0) = Readarray[index++];	
	EX_vau = vau/100;

	ps4  = (unsigned char *)(&vau);
	* (ps4+1) = Readarray[index++];
	* (ps4+0) = Readarray[index++];	
	O2_vau = vau/100;

	ps4  = (unsigned char *)(&vau);
	* (ps4+1) = Readarray[index++];
	* (ps4+0) = Readarray[index++];	
	H2S_vau = vau/100;
	

	Com_Status = Readarray[index++];
	IO_Status = Readarray[index++];
	
}



void uart_Mode::ByteClear(unsigned char * data,int size)
{
	for(int index=0;index<size;index++){
		data[index]=0;
		}
}


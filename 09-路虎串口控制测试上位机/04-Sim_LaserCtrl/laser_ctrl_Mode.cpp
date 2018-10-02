

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream> 
#include <QString> 


#include "laser_ctrl_Mode.hpp"

laser_ctrl_Mode::laser_ctrl_Mode()
{
	 recv_ptr = 0;
	 comRecvFlag = false;
	 recv_count=0;
	 

	 vspeed =0;  //线速度
	 aspeed =0;  //角速度


	 LF_counter=0;
	 RF_counter=0;
	 LB_counter=0;
	 RB_counter=0;

	 DLF_counter = 0;
	 DRF_counter = 0;
	 DLB_counter = 0;
	 DRB_counter = 0;	
	
}

laser_ctrl_Mode::~laser_ctrl_Mode()
{

}

bool laser_ctrl_Mode::procss_recv_buffer(std::string receivedData)
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
				if(data==HEAD){
						recv_ptr=1;
						recv_count=0;
					}
				break;
			case 1:
				if(data==SEND){
						recv_ptr=2;
						recv_count=0;
                        ByteClear(Readarray,20);
					}
				else{
						recv_ptr=0;
						recv_count=0;
                        std::cout<<"recv wrong 2"<<std::endl;
					}
				break;
			case 2:
				Readarray[recv_count]=data;
				recv_count++;
                if(recv_count>=18){
						recv_ptr=0;
						recv_count=0;
						
						if(1==Get_check()){
							DLF_counter =Get_Sdata1() -LF_counter;LF_counter = Get_Sdata1();
							DRF_counter =Get_Sdata2() -RF_counter;RF_counter = Get_Sdata2();
							DLB_counter =Get_Sdata3() -LB_counter;LB_counter = Get_Sdata3();
							DRB_counter =Get_Sdata4() -RB_counter;RB_counter = Get_Sdata4();
							

							ByteClear(Readarray,20);
							//std::cout<<"speed="<<vspeed<<"angel speed = "<<aspeed<<std::endl;
							return true;
							}
                        std::cout<<"recv wrong 3"<<std::endl;
						return false;
					}
				break;				
			default:
				recv_ptr=0;
				recv_count=0;
                ByteClear(Readarray,20);
                std::cout<<"recv wrong 1"<<std::endl;
				break;

	 		}
		}

	return false;

}
unsigned short int laser_ctrl_Mode::CRC16(unsigned char *p,unsigned int len)
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
int  laser_ctrl_Mode::Get_Sdata4()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[12];
	* (ps1+2) = Readarray[13];	
	* (ps1+1) = Readarray[14];	
	* (ps1+0) = Readarray[15];
	
	return Data1;
}
int  laser_ctrl_Mode::Get_Sdata3()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[8];
	* (ps1+2) = Readarray[9];	
	* (ps1+1) = Readarray[10];	
	* (ps1+0) = Readarray[11];
	
	return Data1;
}
int  laser_ctrl_Mode::Get_Sdata2()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[4];
	* (ps1+2) = Readarray[5];	
	* (ps1+1) = Readarray[6];	
	* (ps1+0) = Readarray[7];
	
	return Data1;
}

int  laser_ctrl_Mode::Get_Sdata1()
{
	int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
	* (ps1+3) = Readarray[0];
	* (ps1+2) = Readarray[1];	
	* (ps1+1) = Readarray[2];	
	* (ps1+0) = Readarray[3];	
	
	return Data1;
}
int  laser_ctrl_Mode::Get_check()
{
	unsigned short int Data1;
	unsigned char * ps1  = (unsigned char *)(&Data1);
	
    * (ps1+1) = Readarray[16];
    * (ps1+0) = Readarray[17];

	unsigned short int crc=CRC16(Readarray,16);

	if(Data1!=crc){
        QString str1;
        str1.sprintf("Readarray[16]=%02X ",Readarray[16]);
        QString str2;
        str2.sprintf("Readarray[17]=%02X ",Readarray[17]);
         std::cout<<str1.toStdString()<<str2.toStdString()<<std::endl;
        std::cout<<"crc check = "<<crc<<" recv="<<Data1<<std::endl;
		return -1;
		}
	
	return 1;
}

std::string  laser_ctrl_Mode::TC_return_package()
{
	
    char data[20];
	 //char * pd=data.c_array();
	
    //data.assign(0);
	
	data[0] = 0x88;
	data[1] = 0xAA;
	
	unsigned char * ps1  = (unsigned char *)(&LF_counter);
	data[5]=  *(ps1);ps1++;  //低位
	data[4]=  *(ps1);ps1++; //高位
	data[3]=  *(ps1);ps1++; //高位
	data[2]=  *(ps1);ps1++; //高位
	
	ps1  = (unsigned char *)(&RF_counter);
	data[9]=  *(ps1);ps1++;  //低位
	data[8]=  *(ps1);ps1++; //高位
	data[7]=  *(ps1);ps1++; //高位
	data[6]=  *(ps1);ps1++; //高位

	ps1  = (unsigned char *)(&LB_counter);
	data[13]=  *(ps1);ps1++;  //低位
	data[12]=  *(ps1);ps1++; //高位
	data[11]=  *(ps1);ps1++; //高位
	data[10]=  *(ps1);ps1++; //高位

	ps1  = (unsigned char *)(&RB_counter);
	data[17]=  *(ps1);ps1++;  //低位
	data[16]=  *(ps1);ps1++; //高位
	data[15]=  *(ps1);ps1++; //高位
	data[14]=  *(ps1);ps1++; //高位
	//计算校验
     unsigned char* pd=(unsigned char *)data;
        unsigned short int check= CRC16(&pd[2],16);
		
	unsigned char * p  = (unsigned char *)(&check);
	data[19]=  *(p);p++;  //低位
	data[18]=  *(p);p++; //高位
	
	
    //print_hex(pd,20);
    std::string res;
    res.assign(data,20);
    return res;
	
}

std::string  laser_ctrl_Mode::TC_cmd_package(float speed,float th)
{
     char data[nomal_length];
	data[0] = 0xBB;
	data[1] = 0x66;

	//速度
     char * ps1  = (  char *)(&speed);
	data[5]=  *(ps1);ps1++;  //低位
	data[4]=  *(ps1);ps1++; //高位
	data[3]=  *(ps1);ps1++; //高位
	data[2]=  *(ps1);ps1++; //高位

	//角速度
     char * ps2  = (  char *)(&th);
	data[9]=  *(ps2);ps2++;  //低位
	data[8]=  *(ps2);ps2++; //高位
	data[7]=  *(ps2);ps2++; //高位
	data[6]=  *(ps2);ps2++; //高位	
	
    unsigned short int crc=CRC16((unsigned char *)(&data[2]),8);
	 char * ps3  = (  char *)(&crc);
	data[11]=  *(ps3);ps3++; //高位
	data[10]=  *(ps3);ps3++; //高位	

    std::string res;
    res.assign(data,nomal_length);
    return res;
}

void laser_ctrl_Mode::ByteClear(unsigned char * data,int size)
{
	for(int index=0;index<size;index++){
		data[index]=0;
		}
}

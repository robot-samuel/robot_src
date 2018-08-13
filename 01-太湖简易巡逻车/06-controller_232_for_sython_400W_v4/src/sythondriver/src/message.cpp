
#include "message.hpp"
#include "mbcrc.h"

#include <boost/format.hpp>

typedef      short                  int16; 
typedef      short                  INT16; 
typedef    unsigned short int       uint16; 
typedef    unsigned short int       UINT16; 

typedef    int                      int32; 
typedef    int                      INT32; 
typedef    unsigned int             uint32; 
typedef    unsigned int             UINT32;

message::message()
{
    m_cfram_info =0;
    m_Ifram_ID = 0;
	
    m_param1=0;
    m_address = 0 ; //地址
    m_length = 0; //读取时的数据长度
    m_crc_check = 0 ; //参数2	
    m_write_data = 0;
	
    for (std::size_t  i = 0; i < 8; i++)  
    {  
        //m_Afram_data[i] = _char[i];
        m_Afram_data[i]=0;
    } 

}


message::message(unsigned char * buf,int len)
{
    // std::cout << _int << std::endl;  
     std::size_t size = (len>=data_length)?data_length:len;
    for (std::size_t  i = 0; i < size; i++)  
    {  
        //m_Afram_data[i] = _char[i];
        m_Afram_data[i]=buf[i];
    } 
    m_Ifram_ID = m_Afram_data[0];
    m_cfram_info =  m_Afram_data[1];
/*

     unsigned char *p = m_Afram_data.c_array();
     unsigned char *ppt=(unsigned char *)&m_param1;

       *(ppt+1) = * p ;p++;
       *(ppt) = * p ;p++;

      ppt=(unsigned char *)&m_param2;

      *(ppt+1) = * p ;p++;
      *(ppt) = * p ;p++;

    ppt=(unsigned char *)&m_param3;
    *(ppt+1) = * p ;p++;
    *(ppt) = * p ;p++;

    ppt=(unsigned char *)&m_param4;
    *(ppt+1) = * p ;p++;
    *(ppt) = * p ;p++;
    */

}


message::message(std::string &str)
{
    // std::cout << _int << std::endl;  
    std::size_t len = str.size();
    
     std::size_t size = (len>=data_length)?data_length:len;
    for (std::size_t  i = 0; i < size; i++)  
    {  
        m_Afram_data[i]= str[i];
    } 
    
    m_Ifram_ID = m_Afram_data[0];
    m_cfram_info =  m_Afram_data[1];
/*
     
      unsigned char *p = m_Afram_data.c_array();
      unsigned char *ppt=(unsigned char *)&m_param1;
     
        *(ppt+1) = * p ;p++;
        *(ppt) = * p ;p++;
     
       ppt=(unsigned char *)&m_param2;
     
       *(ppt+1) = * p ;p++;
       *(ppt) = * p ;p++;
     
     ppt=(unsigned char *)&m_param3;
     *(ppt+1) = * p ;p++;
     *(ppt) = * p ;p++;
     
     ppt=(unsigned char *)&m_param4;
     *(ppt+1) = * p ;p++;
     *(ppt) = * p ;p++;

*/
}

message::~message()
{

}


void message::set_fram_data(unsigned char * _char)  
{  
    // std::cout << _int << std::endl;  
    for (int i = 0; i < data_length; i++)  
    {  
        m_Afram_data[i] = _char[i];
    } 
}

void message::set_fram_info(unsigned char info){
    
    m_cfram_info = info;
    m_Afram_data[1] = m_cfram_info;
    
}
void message::set_fram_ID(unsigned char id){

    	m_Ifram_ID = id;
	m_Afram_data[0] = m_Ifram_ID;
    
}

unsigned char message::get_fram_info(){
    return(this->m_cfram_info);
}

unsigned char message::get_fram_ID(){
      return(this->m_Ifram_ID);
}
unsigned char * message::get_fram_data(){
      return(this->m_Afram_data.c_array());
}


std::string message::to_string(){
    std::string res(m_Afram_data.begin(),m_Afram_data.end());
    return res;
}

std::string message::to_string(int leng){
    char * p=(char *)m_Afram_data.c_array();
    std::string res(p,leng);
    return res;
}


void message::print_dec(){

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<data_length;index++){
                  int x=(unsigned  char)m_Afram_data[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void message::print_hex(){

	std::cout<< "print_hex:";
	for(std::size_t index=0;index<data_length;index++){
                   int x=(unsigned  char)m_Afram_data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";

 }
 
 void message::set_data_m_param1(UINT16 para)
 {

	this->m_param1 =para;
        unsigned char * p = NULL;  
        p = (unsigned char *)(& this->m_param1);
        
        set_data_m_byte(*p,1);
        p++;
        //大端格式
        set_data_m_byte(*p,0);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */
        
    }
 
 void message::set_data_m_address(UINT16 para){

	this->m_address =para;
        unsigned char * p = NULL;  
        p = (unsigned char *)(& this->m_address);
        
        //大端格式
        set_data_m_byte(*p,3);
        p++;
        set_data_m_byte(*p,2);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */
}
 
 void message::set_data_m_length(UINT16 para){

	this->m_length =para;
	
        unsigned char * p = NULL;  
        p = (unsigned char *)(& this->m_length);
        
        //大端格式
        set_data_m_byte(*p,5);
        p++;
        set_data_m_byte(*p,4);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */
    }
 void message::set_data_m_write_data(UINT16 para){

	this->m_write_data =para;
	
        unsigned char * p = NULL;  
        p = (unsigned char *)(& this->m_write_data);
        
        //大端格式
        set_data_m_byte(*p,5);
        p++;
        set_data_m_byte(*p,4);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */
    }
 void message::set_data_m_crc_check(UINT16 para){
         this->m_crc_check =para;

        //int couter=0;
        unsigned char * p = NULL;  
        p = (unsigned char *)(& this->m_crc_check);
        
        //大端格式
        set_data_m_byte(*p,7);
        p++;
        set_data_m_byte(*p,6);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */

    }
 
 void message::set_data_m_byte(unsigned char para,int pos){

        this->m_Afram_data[pos]=para;
}
 
 UINT16 message::get_data_m_param1(){

     m_param1 =  this->m_Afram_data[0]*256+this->m_Afram_data[1];
     return this->m_param1;
}
 
UINT16 message::get_data_m_address(){
    m_address =  this->m_Afram_data[2]*256+this->m_Afram_data[3];
    return this->m_address;
}
 
UINT16 message::get_data_m_length(){
    m_length =  this->m_Afram_data[4]*256+this->m_Afram_data[5];
     return this->m_length;
}

UINT16 message::get_data_m_crc_check(){
     m_crc_check = this->m_Afram_data[6]*256+this->m_Afram_data[7];
    return this->m_crc_check;
}


unsigned short message::getCRC(){
    
	unsigned char *p = m_Afram_data.c_array();
	set_data_m_byte(0xff,6);
	set_data_m_byte(0xff,7);
	unsigned short check = crc16(p,6);
	

	return check;
}

int message::copy(std::string &str){

		
		m_Afram_data[0] = str[0];
		m_Afram_data[1] = str[1];
		m_Afram_data[2] = str[2];

		int size = m_Afram_data[2] ;
		if((size == 2)||(size == 4)){
			m_length = size;
			int p=3;
			while((p-3)<size){
				m_Afram_data[p++] = str[p-1];
				}
			m_Afram_data[p] = str[p];p++;
			m_Afram_data[p] = str[p];
			}
		else{
				size = str.size()-5;
				m_length = size;
				int p=3;
				while((p-3)<size){
				m_Afram_data[p++] = str[p-1];
				}
			m_Afram_data[p] = str[p];p++;
			m_Afram_data[p] = str[p];
			}
		

}

unsigned char message::recv_get_add(){

	return m_Afram_data[0];

}
unsigned char message::recv_get_cmd(){

	return m_Afram_data[1];

}
unsigned char message::recv_get_size(){

	return m_Afram_data[2];

}

 UINT16 message::recv_get_data1(){
	if(m_length>=2){
		UINT16 data = this->m_Afram_data[3]*256+this->m_Afram_data[4];
		return data;
		}
	return 0;
}

 UINT16 message::recv_get_data2(){
	if(m_length>=4){
		UINT16 data = this->m_Afram_data[5]*256+this->m_Afram_data[6];
		return data;
		}
	return 0;
}


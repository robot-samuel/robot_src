#pragma once

#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <numeric>
#include <string>
#include <iostream>

class message
{
public:

    typedef      short                  int16; 
    typedef      short                  INT16; 
    typedef    unsigned short int       uint16; 
    typedef    unsigned short int       UINT16; 
    
    typedef    int                      int32; 
    typedef    int                      INT32; 
    typedef    unsigned int             uint32; 
    typedef    unsigned int             UINT32;
    
   enum { data_length = 8,
   	MAX_LENGTH = 10};
   
private:
	unsigned char m_Ifram_ID;   //头
	unsigned char m_cfram_info; //命令
	
	boost::array<unsigned char,MAX_LENGTH> m_Afram_data;
	UINT16 m_address; //地址
	UINT16 m_length; //读取时的数据长度
	UINT16 m_write_data; //写数据
	UINT16 m_crc_check; //参数2	

	UINT16 m_param1; //参数1
    
public:
    

    
	message();
	message(unsigned char * buf,int len);
	message(std::string &str);
	~message();

	void set_fram_info(unsigned char info);
	void set_fram_ID(unsigned char id);
	void set_fram_data(unsigned char * _char) ;
    
	unsigned char get_fram_info();
	unsigned char get_fram_ID();
	unsigned char * get_fram_data();

	void set_data_m_param1(UINT16 para);
	
	 void set_data_m_address(UINT16 para);
	 void set_data_m_length(UINT16 para);
	  void set_data_m_write_data(UINT16 para);
	  
	 void set_data_m_crc_check(UINT16 para);
	 
	 
    
	void set_data_m_byte(unsigned char para,int pos);
    
	UINT16 get_data_m_param1();
	UINT16 get_data_m_address();
	UINT16 get_data_m_length();
	UINT16 get_data_m_crc_check();

	
	std::string to_string();
	std::string to_string(int leng);
   
	void print_dec();
	void print_hex();
	unsigned short getCRC();

	unsigned char recv_get_add();
	unsigned char recv_get_cmd();
	unsigned char recv_get_size();
	 UINT16 recv_get_data1();
	  UINT16 recv_get_data2();

	  int copy(std::string &str);
	  
};


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
    
   enum { fram_length = 13 };
   enum { data_length = 8 };
   
private:

    
    boost::array<char,fram_length> data;
    char m_cfram_info;
    UINT32 m_Ifram_ID;
    boost::array<char,data_length> m_Afram_data;

    UINT16 m_param1;
    UINT16 m_param2;
    UINT16 m_param3;
    UINT16 m_param4;
    
public:
    

    
    message();
    
    message(char * buf,int len);
   message(std::string &str);
    ~message();

    void set_fram_info(char info);
    void set_fram_ID(UINT32 id);
    void set_fram_data(char * _char) ;
    
    char get_fram_info();
    UINT32 get_fram_ID();
    char * get_fram_data();

    void set_data_m_param1(UINT16 para);
    void set_data_m_param2(UINT16 para);
    void set_data_m_param3(UINT16 para);
    void set_data_m_param4(UINT16 para);
    
    void set_data_m_byte(char para,int pos);
	void set_data_m_param34(INT32 data);
    
    UINT16 get_data_m_param1();
    UINT16 get_data_m_param2();
    UINT16 get_data_m_param3();
    UINT16 get_data_m_param4();
    UINT32 get_data_m_param34();
   std::string to_string();
   std::string data_to_string();
   unsigned char Get_sizetype();
     unsigned char get_data_m_byte(int pos);
   void print_dec();
    void print_hex();
   void getPRIMcheck();

    void Set_CMD(UINT32 cmd);
    UINT32 Get_CMD();

};


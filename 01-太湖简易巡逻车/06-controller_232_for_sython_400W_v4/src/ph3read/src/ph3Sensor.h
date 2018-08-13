#pragma once


#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include <numeric>
#include <string>
#include <iostream>
#include<map>
#include <boost/format.hpp>



class ph3Sensor{

public:
	
    typedef      short                  int16; 
    typedef      short                  INT16; 
    typedef    unsigned short int       uint16; 
    typedef    unsigned short int       UINT16; 
    
    typedef    int                      int32; 
    typedef    int                      INT32; 
    typedef    unsigned int             uint32; 
    typedef    unsigned int             UINT32;

    enum BIT{
 	SDATA_LENGTH  = 11,

        };


public:
//api
	ph3Sensor();
	ph3Sensor(unsigned char add);
	~ph3Sensor();


public :
	void print_hex(unsigned char * data);
	void print_hex(std::string &data);

	void Set_add(unsigned char  add);
	unsigned char add();
	void MyMemclear(const void *src,int count) ;
	std::string Cmd_GetPH3();

	void printfinfo();
	int recorddata(std::string &data);

	INT16 ph3Vau;   //
	int workstates;  //00 正常 01 超低段报警 02 超高段报警
	
private:

        unsigned char m_add;   //设备自身地址
        
        
	
	
};




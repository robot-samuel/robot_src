#pragma once


#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include <numeric>
#include <string>
#include <iostream>
#include<map>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


class E9Gpio{

public:
	
    typedef      short                  int16; 
    typedef      short                  INT16; 
    typedef    unsigned short int       uint16; 
    typedef    unsigned short int       UINT16; 
    
    typedef    int                      int32; 
    typedef    int                      INT32; 
    typedef    unsigned int             uint32; 
    typedef    unsigned int             UINT32;



	enum Direction
	{
	    Out=0,
	    In=1,
	};

public:
//api
	E9Gpio();
	E9Gpio(std::string num);
    
	E9Gpio(std::string num, int direction);

	~E9Gpio();

public :

	
public:
	bool gpio_isinit();
	bool gpio_setDirection(int dir);
	bool gpio_setvalue(int value);
	int gpio_getvalue();

	void set_GpioFilename(std::string str);
	std::string get_GpioFilename();
	int get_direction();
	void gpio_setdirection(int dir);
	
	
	
private:

	std::string GpioFilename;
	bool isGpioOpen;
	int value;
	int direction;
	
};




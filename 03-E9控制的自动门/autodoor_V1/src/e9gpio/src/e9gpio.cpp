 
#include "e9gpio.h"
#include <fstream>

 
#define  DEBUG_ECHO 0

E9Gpio::E9Gpio()
{

}

E9Gpio::E9Gpio(std::string filename)
:GpioFilename(filename)
{

}
E9Gpio::E9Gpio(std::string filename,int  dir)
:GpioFilename(filename)
{
	direction = dir;
}

E9Gpio::~E9Gpio(){



}
/*
void E9Gpio::MyMemclear(const void *src,int count)   
{   
 
    assert(src);   
    while (count--)   
    {   
       *(char *)src=0;   

        src = (char *)src + 1;   
    }   
} 


void E9Gpio::print_hex(unsigned char * data)
{

	std::cout<< "E9Gpio_cmd:print_hex:";
	for(std::size_t index=0;index<SDATA_LENGTH;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";

 }

void E9Gpio::print_hex(std::string &data)
{
	#if DEBUG_ECHO
	int Num = data.size();
	std::cout<< "E9Gpio_cmd:print_hex:";
	for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";
	#endif

 }
*/

void E9Gpio::set_GpioFilename(std::string str)
{

	GpioFilename = str;
}
std::string E9Gpio::get_GpioFilename()
{
	return GpioFilename;
}

int E9Gpio::get_direction()
{
     	#if 0
         std::cout<<"direction"<<direction<<std::endl;
	#endif  
	if(direction == In){
			return 1;

		}
	else if(direction == Out){
			return 0;
		}
	return -1;
}
bool E9Gpio::gpio_isinit()
{

	
	std::string rootpath =  "/sys/class/gpio/";
	std::string path = rootpath+GpioFilename;
	std::fstream _file;
	 
     _file.open(path,std::ios::in);
     if(!_file.is_open())
     {
     	#if DEBUG_ECHO
         std::cout<<path<<"not exist!"<<std::endl;
	#endif  
	 return 0;	
      }
	#if DEBUG_ECHO
          std::cout<<path<<"is already created!"<<std::endl;
	#endif  
	  _file.close();
	  return 1;      
}

bool E9Gpio::gpio_setDirection(int dir)
{

	std::string rootpath =  "/sys/class/gpio/";
	std::string path = rootpath+GpioFilename;
	std::string Vpath = path+"/direction";
	std::fstream _file;


        switch (dir)
        {
        case Out:
		  std::cout << "gpio_setDirection : out"<< std::endl;
            if (gpio_isinit())
            {
            	 _file.open(Vpath,std::ios::out);
                _file<<"out";
		  _file.close();
		  direction = Out;
                return true;
            }
            else
            {
            	#if DEBUG_ECHO
                std::cout <<GpioFilename<< ":set out gpio direction erro"<< std::endl;
		#endif  
                return false;
            }
            break;
        case In:
		std::cout << "gpio_setDirection: in"<< std::endl;
            if (gpio_isinit())
            {
            	 _file.open(Vpath,std::ios::out);
                _file<<"in";
		  _file.close();
		  direction = In;
                return true;
            }
            else
            {
            	#if DEBUG_ECHO
                std::cout <<GpioFilename<< ":set out gpio direction erro"<< std::endl;
		#endif  
                return false;
            }
            break;
        default:
		#if DEBUG_ECHO
            std::cout << GpioFilename<<":unkown erro of set gpio direction"<< std::endl;
		#endif 
            return false;
            break;
        }

}
void E9Gpio::gpio_setdirection(int dir)
{

	direction =dir;
}

bool E9Gpio::gpio_setvalue(int value)
{
	///sys/class/gpio/gpio30/value
	std::string rootpath =  "/sys/class/gpio/";
	std::string path = rootpath+GpioFilename;
	std::string Vpath = path+"/value";
	std::fstream _file;
	
    switch (get_direction())
    {
    case In:
        std::cout << "set gpio out please"<< std::endl;
        return false;
        break;
    case Out:
	   _file.open(Vpath);
	 if (_file.is_open()){
            	
	            if (value ==1)
	            {
	                _file << "1";
	                _file.close();
			std::cout << "path:"<<Vpath<<"  1"<<std::endl;
	                return true;
	            }
	            else if (value ==0)
	            {
	                _file << "0";
	                _file.close();
			std::cout << "path:"<<Vpath<<"  0"<<std::endl;
	                return true;
	            }
	            else
	            {
	            	
	                std::cout << "input  0 or 1  please"<< std::endl;
	                _file.close();
	                return false;
	            }
	 	}
        else
        {
            std::cout << "open gpio erro"<< std::endl;
            return false;
        }
        break;
    }
}


int E9Gpio::gpio_getvalue()
{
	std::string rootpath =  "/sys/class/gpio/";
	std::string path = rootpath+GpioFilename;
	std::string Vpath = path+"/value";
	std::fstream _file;
	std::string strvau;
	
    if (gpio_isinit())
    {
        switch (get_direction())
        {
        case In:
		 _file.open(Vpath,std::ios::in);
            if ( _file.is_open())
            {
                _file>>strvau;
		  int vau=boost::lexical_cast<int>(strvau);
                _file.close();
                return vau;
            }
            else
            {
            	 #if DEBUG_ECHO
                std::cout << "open gpio erro"<< std::endl;
		 #endif
                return -1;
            }
            break;
        case Out:
	    #if DEBUG_ECHO
            std::cout << "set gpio in please"<<std::endl;
		#endif
            return -1;
        default:
            std::cout << "unkown erro of get gpio value"<< std::endl;
            return -1;
        }
    }
    else
    {
        std::cout << "open gpio please\n"<< std::endl;
        return -1;
    }
}





#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream> 

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp> //for ref_list_of  
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>



#include "e9gpioMode.hpp"

#define  DEBUG_ECHO 0


E9GpioMode::E9GpioMode()
{
 	Gpio20.set_GpioFilename("gpio20");
	Gpio86.set_GpioFilename( "gpio86");
	Gpio30.set_GpioFilename( "gpio30");
	Gpio85.set_GpioFilename( "gpio85");

	Gpio94.set_GpioFilename("gpio94");
	Gpio27.set_GpioFilename( "gpio27");
	Gpio28.set_GpioFilename( "gpio28");
	Gpio80.set_GpioFilename( "gpio80");

	Gpio20.gpio_setdirection(E9Gpio::Out);
	Gpio86.gpio_setdirection(E9Gpio::Out);
	Gpio30.gpio_setdirection(E9Gpio::Out);
	Gpio85.gpio_setdirection(E9Gpio::Out);

	Gpio94.gpio_setdirection(E9Gpio::In);
	Gpio27.gpio_setdirection(E9Gpio::In);
	Gpio28.gpio_setdirection(E9Gpio::In);
	Gpio80.gpio_setdirection(E9Gpio::In);

	

	intput1=-1;
	intput2=-1;
	intput3=-1;
	intput4=-1;

	recvcmd = -1;
	doorstatues = 1;  //默认开门
	deley=0;
}

E9GpioMode::~E9GpioMode()
{

}

void E9GpioMode::gpio_cmdCall(const dgvmsg::E9gpioCtrl::ConstPtr& msg)
{
	//GPIO给出的所有状态
	E9GpioMode_Setoutvau(msg->out1);
	recvcmd = msg->out1;
	if((doorstatues == 0)&&(recvcmd ==1)){   //门是关着的，并且受到开门命令
			deley =1;
		}
	if(recvcmd ==0){
			deley = 0;
		}
	if(recvcmd ==2){
			deley = 0;
		}
	
                      
 } 
 
void E9GpioMode::E9GpioMode_master()
{
	
       std::cout<< "hello E9GpioMode_master "<<std::endl;
      
	E9GpioMode_init_dir();
        E9GpioMode_init_outvau();
         
         ros::Rate loop_rate(10);
	  int counter=0;
	   getinput();
	   doorstatues =  intput1;  //记录门的初始状态
         try{
                    while(ros::ok()){
					
				getinput();   //门状态采集
				//门状态为关门  并且收到了开门命令
				// std::cout << "doorstatues " << doorstatues << std:: endl;
				//std::cout << "recvcmd " << recvcmd << std:: endl;
				if(deley == 1){
						counter ++;
						if(counter>=200){   //延时30秒
								doorstatues = intput1;//刷新门状态
								counter = 0;
								deley = 0;
							}
					}
  				else{   //门的状态为开门 ，不管收到什么命令，都直接返回状态
						counter = 0;
						doorstatues = intput1;//刷新门状态
					}
				//check_obt();
				//上送io口状态
				ros_rospub_obt_SendRpc();
				//E9GpioMode_test_outvau(1);

				//判断是否处触发门运动状态
				
				loop_rate.sleep();
                     }
             }
        catch(boost::thread_interrupted&){
                 std::cout << "thread interrupt" << std:: endl;
             }
    

    
	
}


void E9GpioMode::print_dec(unsigned char *datBUF,int Num)
{

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<Num;index++){
                  int x=(unsigned  char)datBUF[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";

}
 void E9GpioMode::print_hex(unsigned char *datBUF,int Num)
 {

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
  void E9GpioMode::print_hex(std::string str)
 {
	int Num = str.size();
	const char * datBUF =  str.c_str();
	
  std::cout<< "E9GpioMode print_recv hex:";
  for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)datBUF[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }

bool E9GpioMode::E9GpioMode_init_dir()
{
	if(Gpio20.gpio_setDirection( E9Gpio::Out)){
			//return true;
		}
	if(Gpio86.gpio_setDirection( E9Gpio::Out)){
			//return true;
		}
	if(Gpio30.gpio_setDirection( E9Gpio::Out)){
			//return true;
		}
	if(Gpio85.gpio_setDirection( E9Gpio::Out)){
			//return true;
		}


	if(Gpio94.gpio_setDirection( E9Gpio::In)){
			//return true;
		}
	if(Gpio27.gpio_setDirection( E9Gpio::In)){
			//return true;
		}
	if(Gpio28.gpio_setDirection( E9Gpio::In)){
			//return true;
		}
	if(Gpio80.gpio_setDirection( E9Gpio::In)){
			//return true;
		}
	return false;
}

bool E9GpioMode::E9GpioMode_init_outvau()
{
	if(Gpio20.gpio_setvalue(1)){
			//return true;
		}
	if(Gpio86.gpio_setvalue( 1)){
			//return true;
		}
	if(Gpio30.gpio_setvalue( 1)){
			//return true;
		}
	if(Gpio85.gpio_setvalue( 1)){
			//return true;
		}

	return false;
}

bool E9GpioMode::E9GpioMode_test_outvau(int vau)
{
	if(Gpio20.gpio_setvalue(vau)){
			//return true;
		}
	if(Gpio86.gpio_setvalue( vau)){
			//return true;
		}
	if(Gpio30.gpio_setvalue( vau)){
			//return true;
		}
	if(Gpio85.gpio_setvalue( vau)){
			//return true;
		}

	return false;
}

bool E9GpioMode::E9GpioMode_Setoutvau(int cmd)
{
	switch(cmd){
			case 0:  //关
				if(Gpio20.gpio_setvalue(1)){
						//return true;
					}
				if(Gpio86.gpio_setvalue( 1)){
						//return true;
					}
				if(Gpio30.gpio_setvalue( 0)){
						//return true;
					}
				break;
			case 1:   //开
				if(Gpio20.gpio_setvalue(0)){
						//return true;
					}
				if(Gpio86.gpio_setvalue( 1)){
						//return true;
					}
				if(Gpio30.gpio_setvalue( 1)){
						//return true;
					}
				break;				
			case 2:   //停
				if(Gpio20.gpio_setvalue(1)){
						//return true;
					}
				if(Gpio86.gpio_setvalue( 0)){
						//return true;
					}
				if(Gpio30.gpio_setvalue( 1)){
						//return true;
					}
				break;
			case -1:   //空闲
				if(Gpio20.gpio_setvalue(1)){
						//return true;
					}
				if(Gpio86.gpio_setvalue( 1)){
						//return true;
					}
				if(Gpio30.gpio_setvalue( 1)){
						//return true;
					}
				break;

		}


	return false;
}



void  E9GpioMode::getinput(){

	intput1 = Gpio28.gpio_getvalue();    //上输入
	intput2 = Gpio80.gpio_getvalue();   //下输入

	intput3 = Gpio94.gpio_getvalue();    //上输入
	intput4 = Gpio27.gpio_getvalue();   //下输入

}

void  E9GpioMode::check_obt(){
	std::cout<< "doorstatues="<<doorstatues;
	std::cout<< "  intput2="<<intput2;
	std::cout<< " intput3="<<intput3;
	std::cout<< "  intput4="<<intput4;
	std::cout<<std::endl;

}



void E9GpioMode::ros_rospub_obt_SendRpc()
{   

	// std_msgs::String stdmsg;
	// stdmsg.data = msg;
	dgvmsg::E9gpioS gpiomsg;
	gpiomsg.in.resize(4);
	gpiomsg.in[0] = doorstatues;
	gpiomsg.in[1] = intput2;
	gpiomsg.in[2] = intput3;
	gpiomsg.in[3] = intput4;
	
	rospub_obt.publish(gpiomsg);
	ros::spinOnce();
}



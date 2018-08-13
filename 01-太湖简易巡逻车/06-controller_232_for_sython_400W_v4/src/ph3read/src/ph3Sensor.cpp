 
#include "ph3Sensor.h"


 
#define  DEBUG_ECHO 1

ph3Sensor::ph3Sensor(){

	m_add=1;   //默认为广播地址
	ph3Vau=0;
	
	

}
ph3Sensor::ph3Sensor(unsigned char add)
    :m_add(add)
{
	ph3Vau=0;
}

ph3Sensor::~ph3Sensor(){



}

void ph3Sensor::MyMemclear(const void *src,int count)   
{   
 
    assert(src);   
    while (count--)   
    {   
       *(char *)src=0;   

        src = (char *)src + 1;   
    }   
} 

std::string ph3Sensor::Cmd_GetPH3()
{
	//FF FF 01 01 05 01 00 6C 07 74 DD   //输入开口状态
	     boost::array<unsigned char,SDATA_LENGTH> m_Afram_data;

		m_Afram_data[0] = 0xff;
		m_Afram_data[1] = 0xff;
		m_Afram_data[2] = 0x01;
		m_Afram_data[3] = 0x01;
		m_Afram_data[4] = 0x05;
		m_Afram_data[5] = 0x01;
		m_Afram_data[6] = 0x00;
		
		m_Afram_data[7] = 0x6c;
		m_Afram_data[8] = 0x07;
		m_Afram_data[9] = 0x74;
		m_Afram_data[10] = 0xdd;
		
		std::string res(m_Afram_data.begin(),m_Afram_data.end());
	//print_hex(res);
	 return res;
	 
}




void ph3Sensor::Set_add(unsigned char  add)
{
	m_add = add;
 }

unsigned char ph3Sensor::add()
{
	return m_add;
 }



void ph3Sensor::print_hex(unsigned char * data)
{

	std::cout<< "ph3Sensor_cmd:print_hex:";
	for(std::size_t index=0;index<SDATA_LENGTH;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";

 }

void ph3Sensor::print_hex(std::string &data)
{
	#if DEBUG_ECHO
	int Num = data.size();
	std::cout<< "ph3Sensor_cmd:print_hex:";
	for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";
	#endif

 }

int ph3Sensor::recorddata(std::string &data)
{
	//FF FF 01 01 0C A1 00 6C 07 00 00 00 00 00 0F 02 25 DD
	boost::array<unsigned char,18> recvbuf;
	for(int index=0;index<18;index++){
			recvbuf[index]= ((unsigned  char)data[index]);
		}

	if(recvbuf[0] != 0xFF){
			return -1;
		}
	if(recvbuf[1] != 0xFF){
			return -1;
		}	
	if(recvbuf[17] != 0xDD){
			return -1;
		}
		workstates = recvbuf[9];
		
		ph3Vau = (recvbuf[10]&0xf0)*100000;
		ph3Vau += (recvbuf[10]&0x0f)*10000;
		ph3Vau += (recvbuf[11]&0xf0)*1000;
		ph3Vau += (recvbuf[11]&0x0f)*100;
		ph3Vau += (recvbuf[12]&0xf0)*10;
		ph3Vau += (recvbuf[12]&0x0f);

	#if 0
		printfinfo();
     #endif
	return ph3Vau;
 }

void ph3Sensor::printfinfo(){
	
	std::cout<< "printfinfo:"<<std::endl;
	std::cout<< "workstates:"<<workstates<<std::endl;
	std::cout<< "ph3Vau:"<<ph3Vau<<std::endl;
	
}



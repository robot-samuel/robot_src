 
#include "bmsSensor.h"
#include "mbcrc.h"

 
#define  DEBUG_ECHO 1

bmsSensor::bmsSensor(){

	m_add=1;   //默认为广播地址
	MyMemclear((void *)&BmsOrgInfo,sizeof(struct _BmsWork));

}
bmsSensor::bmsSensor(unsigned char add)
    :m_add(add)
{
	MyMemclear((void *)&BmsOrgInfo,sizeof(struct _BmsWork));
}

bmsSensor::~bmsSensor(){



}

void bmsSensor::MyMemclear(const void *src,int count)   
{   
 
    assert(src);   
    while (count--)   
    {   
       *(char *)src=0;   

        src = (char *)src + 1;   
    }   
} 

std::string bmsSensor::Cmd_GetInfo()
{
	//03  03  00  00  00  28  44  36 //03  03  00  00  00  29  85  F6
	     boost::array<unsigned char,SDATA_LENGTH> m_Afram_data;

		m_Afram_data[0] = 0x03;
		m_Afram_data[1] = 0x03;
		m_Afram_data[2] = 0x00;
		m_Afram_data[3] = 0x00;
		m_Afram_data[4] = 0x00;
		m_Afram_data[5] = 0x28;
		m_Afram_data[6] = 0xff;		
		m_Afram_data[7] = 0xff;

		unsigned char *p = m_Afram_data.c_array();

		unsigned short check = getCRC(p,8);
		
		m_Afram_data[6] = (unsigned char)((check>>8)&0x00ff);
		m_Afram_data[7] = (unsigned char)(check&0x00ff);
		
		std::string res(m_Afram_data.begin(),m_Afram_data.end());
	//print_hex(res);
	 return res;
	 
}




void bmsSensor::Set_add(unsigned char  add)
{
	m_add = add;
 }

unsigned char bmsSensor::add()
{
	return m_add;
 }



void bmsSensor::print_hex(unsigned char * data)
{

	std::cout<< "bmsSensor_cmd:print_hex:";
	for(std::size_t index=0;index<SDATA_LENGTH;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";

 }

void bmsSensor::print_hex(std::string &data)
{
	#if DEBUG_ECHO
	int Num = data.size();
	std::cout<< "bmsSensor_cmd:print_hex:";
	for(std::size_t index=0;index<Num;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
	std::cout<<"\n";
	#endif

 }

unsigned short bmsSensor::getCRC(unsigned char *p,int num){    	
	//unsigned char *p = m_Afram_data.c_array();	
	//set_data_m_byte(0xff,6);	
	//set_data_m_byte(0xff,7);
	//p[num-2]=0xff;
	//p[num-1]=0xff;
	unsigned short check = crc16(p,num-2);		
	return check;
}

int bmsSensor::infocheck(std::string &str){
	
	if(((unsigned  char)str[0]) != 0x03){
		return -1;
		}
	if(((unsigned  char)str[1]) != 0x03){
		return -1;
		}
	const char * datBUF =  str.c_str();
	int size = str.size();
	if(size<RDATA_LENGTH){
		return -1;	
		}
	unsigned short check = ((unsigned  char)str[RDATA_LENGTH-2]);
	check<<=8;
	check +=((unsigned  char)str[RDATA_LENGTH-1]);

	unsigned char recvbuf[RDATA_LENGTH];
	for(int index=0;index<RDATA_LENGTH;index++){
			recvbuf[index]= ((unsigned  char)str[index]);
		}
	unsigned short recvcheck = getCRC(recvbuf,RDATA_LENGTH);
	
	//std::cout<<"recvcheck="<<check;
	//std::cout<<"check="<<recvcheck<<std::endl;
	
	if(check != recvcheck){
			return -1;
		}
	
	return 1;
}

void bmsSensor::recordInfo(std::string &str){

	 boost::array<unsigned char,RDATA_LENGTH> recvbuf;
	for(int index=0;index<RDATA_LENGTH-2;index++){
			recvbuf[index]= ((unsigned  char)str[index]);
		}
		int counter=3;
		
		BmsOrgInfo.cell_overVoltage = recvbuf[counter++];
		BmsOrgInfo.cell_overVoltage<<=8;
		BmsOrgInfo.cell_overVoltage+= recvbuf[counter++];

		BmsOrgInfo.cell_overVol_release = recvbuf[counter++];
		BmsOrgInfo.cell_overVol_release<<=8;
		BmsOrgInfo.cell_overVol_release+= recvbuf[counter++];

		BmsOrgInfo.cell_lowVoltage = recvbuf[counter++];
		BmsOrgInfo.cell_lowVoltage<<=8;
		BmsOrgInfo.cell_lowVoltage+= recvbuf[counter++];

		BmsOrgInfo.cell_lowVol_release = recvbuf[counter++];
		BmsOrgInfo.cell_lowVol_release<<=8;
		BmsOrgInfo.cell_lowVol_release+= recvbuf[counter++];


		BmsOrgInfo.group_overVoltage = recvbuf[counter++];
		BmsOrgInfo.group_overVoltage<<=8;
		BmsOrgInfo.group_overVoltage+= recvbuf[counter++];

		BmsOrgInfo.group_overVol_release = recvbuf[counter++];
		BmsOrgInfo.group_overVol_release<<=8;
		BmsOrgInfo.group_overVol_release+= recvbuf[counter++];

		BmsOrgInfo.group_lowVoltage = recvbuf[counter++];
		BmsOrgInfo.group_lowVoltage<<=8;
		BmsOrgInfo.group_lowVoltage+= recvbuf[counter++];

		BmsOrgInfo.group_lowVol_release = recvbuf[counter++];
		BmsOrgInfo.group_lowVol_release<<=8;
		BmsOrgInfo.group_lowVol_release+= recvbuf[counter++];



		BmsOrgInfo.charge_HighTemp = recvbuf[counter++];
		BmsOrgInfo.charge_HighTemp<<=8;
		BmsOrgInfo.charge_HighTemp+= recvbuf[counter++];

		BmsOrgInfo.charge_HighTemp_release = recvbuf[counter++];
		BmsOrgInfo.charge_HighTemp_release<<=8;
		BmsOrgInfo.charge_HighTemp_release+= recvbuf[counter++];

		BmsOrgInfo.charge_lowTemp = recvbuf[counter++];
		BmsOrgInfo.charge_lowTemp<<=8;
		BmsOrgInfo.charge_lowTemp+= recvbuf[counter++];

		BmsOrgInfo.charge_lowTemp_release = recvbuf[counter++];
		BmsOrgInfo.charge_lowTemp_release<<=8;
		BmsOrgInfo.charge_lowTemp_release+= recvbuf[counter++];


		BmsOrgInfo.discharge_HighTemp = recvbuf[counter++];
		BmsOrgInfo.discharge_HighTemp<<=8;
		BmsOrgInfo.discharge_HighTemp+= recvbuf[counter++];

		BmsOrgInfo.discharge_HighTemp_release = recvbuf[counter++];
		BmsOrgInfo.discharge_HighTemp_release<<=8;
		BmsOrgInfo.discharge_HighTemp_release+= recvbuf[counter++];

		BmsOrgInfo.discharge_lowTemp = recvbuf[counter++];
		BmsOrgInfo.discharge_lowTemp<<=8;
		BmsOrgInfo.discharge_lowTemp+= recvbuf[counter++];

		BmsOrgInfo.discharge_lowTemp_release = recvbuf[counter++];
		BmsOrgInfo.discharge_lowTemp_release<<=8;
		BmsOrgInfo.discharge_lowTemp_release+= recvbuf[counter++];


		BmsOrgInfo.charge_overCurrent = recvbuf[counter++];
		BmsOrgInfo.charge_overCurrent<<=8;
		BmsOrgInfo.charge_overCurrent+= recvbuf[counter++];

		BmsOrgInfo.discharge_overCurrent = recvbuf[counter++];
		BmsOrgInfo.discharge_overCurrent<<=8;
		BmsOrgInfo.discharge_overCurrent+= recvbuf[counter++];


		BmsOrgInfo.Voltage = recvbuf[counter++];
		BmsOrgInfo.Voltage<<=8;
		BmsOrgInfo.Voltage+= recvbuf[counter++];

		BmsOrgInfo.Current = recvbuf[counter++];
		BmsOrgInfo.Current<<=8;
		BmsOrgInfo.Current+= recvbuf[counter++];

		BmsOrgInfo.Soc = recvbuf[counter++];
		BmsOrgInfo.Soc<<=8;
		BmsOrgInfo.Soc+= recvbuf[counter++];



		BmsOrgInfo.Cell_1 = recvbuf[counter++];
		BmsOrgInfo.Cell_1<<=8;
		BmsOrgInfo.Cell_1+= recvbuf[counter++];

		BmsOrgInfo.Cell_2 = recvbuf[counter++];
		BmsOrgInfo.Cell_2<<=8;
		BmsOrgInfo.Cell_2+= recvbuf[counter++];

		BmsOrgInfo.Cell_3 = recvbuf[counter++];
		BmsOrgInfo.Cell_3<<=8;
		BmsOrgInfo.Cell_3+= recvbuf[counter++];

		BmsOrgInfo.Cell_4 = recvbuf[counter++];
		BmsOrgInfo.Cell_4<<=8;
		BmsOrgInfo.Cell_4+= recvbuf[counter++];


		BmsOrgInfo.Cell_5 = recvbuf[counter++];
		BmsOrgInfo.Cell_5<<=8;
		BmsOrgInfo.Cell_5+= recvbuf[counter++];

		BmsOrgInfo.Cell_6 = recvbuf[counter++];
		BmsOrgInfo.Cell_6<<=8;
		BmsOrgInfo.Cell_6+= recvbuf[counter++];

		BmsOrgInfo.Cell_7 = recvbuf[counter++];
		BmsOrgInfo.Cell_7<<=8;
		BmsOrgInfo.Cell_7+= recvbuf[counter++];

		BmsOrgInfo.Cell_8 = recvbuf[counter++];
		BmsOrgInfo.Cell_8<<=8;
		BmsOrgInfo.Cell_8+= recvbuf[counter++];

		BmsOrgInfo.Cell_9 = recvbuf[counter++];
		BmsOrgInfo.Cell_9<<=8;
		BmsOrgInfo.Cell_9+= recvbuf[counter++];

		BmsOrgInfo.Cell_10 = recvbuf[counter++];
		BmsOrgInfo.Cell_10<<=8;
		BmsOrgInfo.Cell_10+= recvbuf[counter++];

		BmsOrgInfo.Cell_11 = recvbuf[counter++];
		BmsOrgInfo.Cell_11<<=8;
		BmsOrgInfo.Cell_11+= recvbuf[counter++];

		BmsOrgInfo.Cell_12 = recvbuf[counter++];
		BmsOrgInfo.Cell_12<<=8;
		BmsOrgInfo.Cell_12+= recvbuf[counter++];

		BmsOrgInfo.Cell_13 = recvbuf[counter++];
		BmsOrgInfo.Cell_13<<=8;
		BmsOrgInfo.Cell_13+= recvbuf[counter++];

		BmsOrgInfo.Cell_14 = recvbuf[counter++];
		BmsOrgInfo.Cell_14<<=8;
		BmsOrgInfo.Cell_14+= recvbuf[counter++];

		BmsOrgInfo.Cell_15 = recvbuf[counter++];
		BmsOrgInfo.Cell_15<<=8;
		BmsOrgInfo.Cell_15+= recvbuf[counter++];

		BmsOrgInfo.Cell_16 = recvbuf[counter++];
		BmsOrgInfo.Cell_16<<=8;
		BmsOrgInfo.Cell_16+= recvbuf[counter++];
		
		BmsOrgInfo.Temp1 = recvbuf[counter++];
		BmsOrgInfo.Temp1<<=8;
		BmsOrgInfo.Temp1+= recvbuf[counter++];

		BmsOrgInfo.Temp2 = recvbuf[counter++];
		BmsOrgInfo.Temp2<<=8;
		BmsOrgInfo.Temp2+= recvbuf[counter++];

		BmsOrgInfo.Soc_full = recvbuf[counter++];
		BmsOrgInfo.Soc_full<<=8;
		BmsOrgInfo.Soc_full+= recvbuf[counter++];

		//BmsOrgInfo.RSOC = recvbuf[counter++];
		//BmsOrgInfo.RSOC<<=8;
		//BmsOrgInfo.RSOC+= recvbuf[counter++];
		BmsOrgInfo.RSOC = (uint16)((BmsOrgInfo.Soc*1.0/BmsOrgInfo.Soc_full*1.0)*100);
		#if 0
		printMsg();
		#endif

}

void bmsSensor::printMsg(void){

	std::cout<< "bmsSensor_recv:print_hex:";
	
	std::cout<< "cell_overVoltage="<<BmsOrgInfo.cell_overVoltage<<std::endl;
	std::cout<< "cell_overVol_release="<<BmsOrgInfo.cell_overVol_release<<std::endl;
	std::cout<< "cell_lowVoltage="<<BmsOrgInfo.cell_lowVoltage<<std::endl;
	std::cout<< "cell_lowVol_release="<<BmsOrgInfo.cell_lowVol_release<<std::endl;

	std::cout<< "group_overVoltage="<<BmsOrgInfo.group_overVoltage<<std::endl;
	std::cout<< "group_overVol_release="<<BmsOrgInfo.group_overVol_release<<std::endl;
	std::cout<< "group_lowVoltage="<<BmsOrgInfo.group_lowVoltage<<std::endl;
	std::cout<< "group_lowVol_release="<<BmsOrgInfo.group_lowVol_release<<std::endl;

	std::cout<< "charge_HighTemp="<<BmsOrgInfo.charge_HighTemp<<std::endl;
	std::cout<< "charge_HighTemp_release="<<BmsOrgInfo.charge_HighTemp_release<<std::endl;
	std::cout<< "charge_lowTemp="<<BmsOrgInfo.charge_lowTemp<<std::endl;
	std::cout<< "charge_lowTemp_release="<<BmsOrgInfo.charge_lowTemp_release<<std::endl;		

	std::cout<< "discharge_HighTemp="<<BmsOrgInfo.discharge_HighTemp<<std::endl;
	std::cout<< "discharge_HighTemp_release="<<BmsOrgInfo.discharge_HighTemp_release<<std::endl;
	std::cout<< "discharge_lowTemp="<<BmsOrgInfo.discharge_lowTemp<<std::endl;
	std::cout<< "discharge_lowTemp_release="<<BmsOrgInfo.discharge_lowTemp_release<<std::endl;		

	std::cout<< "charge_overCurrent="<<BmsOrgInfo.charge_overCurrent<<std::endl;
	std::cout<< "discharge_overCurrent="<<BmsOrgInfo.discharge_overCurrent<<std::endl;		


	std::cout<< "Voltage="<<BmsOrgInfo.Voltage<<std::endl;
	std::cout<< "Current="<<BmsOrgInfo.Current<<std::endl;		


	std::cout<< "Soc="<<BmsOrgInfo.Soc<<std::endl;
	std::cout<< "Cell_1="<<BmsOrgInfo.Cell_1<<std::endl;	
	std::cout<< "Cell_2="<<BmsOrgInfo.Cell_2<<std::endl;
	std::cout<< "Cell_3="<<BmsOrgInfo.Cell_3<<std::endl;
	std::cout<< "Cell_4="<<BmsOrgInfo.Cell_4<<std::endl;
	std::cout<< "Cell_5="<<BmsOrgInfo.Cell_5<<std::endl;
	std::cout<< "Cell_6="<<BmsOrgInfo.Cell_6<<std::endl;
	std::cout<< "Cell_7="<<BmsOrgInfo.Cell_7<<std::endl;
	std::cout<< "Cell_8="<<BmsOrgInfo.Cell_8<<std::endl;
	std::cout<< "Cell_9="<<BmsOrgInfo.Cell_9<<std::endl;
	std::cout<< "Cell_10="<<BmsOrgInfo.Cell_10<<std::endl;
	std::cout<< "Cell_11="<<BmsOrgInfo.Cell_11<<std::endl;
	std::cout<< "Cell_12="<<BmsOrgInfo.Cell_12<<std::endl;
	std::cout<< "Cell_13="<<BmsOrgInfo.Cell_13<<std::endl;

	std::cout<< "Temp1="<<BmsOrgInfo.Temp1<<std::endl;
	std::cout<< "Temp2="<<BmsOrgInfo.Temp2<<std::endl;

	std::cout<< "Soc_full="<<BmsOrgInfo.Soc_full<<std::endl;
	std::cout<< "RSOC="<<BmsOrgInfo.RSOC<<std::endl;

}

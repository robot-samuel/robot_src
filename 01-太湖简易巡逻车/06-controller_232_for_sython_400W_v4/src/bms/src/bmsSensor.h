#pragma once


#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include <numeric>
#include <string>
#include <iostream>
#include<map>
#include <boost/format.hpp>





class bmsSensor{

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
 	SDATA_LENGTH  =8,
	RDATA_LENGTH  =85,
        };
	struct _BmsWork
	{
		uint16 cell_overVoltage;	//µ¥Ìå¹ıÑ¹
		uint16 cell_overVol_release;	//µ¥Î»ÊÇmV,µ¥Ìå¹ıÑ¹±£»¤×´Ì¬½â³ıµçÑ¹
		uint16 cell_lowVoltage;	//µ¥Î»ÊÇmV,µ¥½ÚÇ·Ñ¹±£»¤Öµ,×îµÍµ¥ÌåĞ¡ÓÚÓÚ¸ÃÖµÔò·¢Éú±£»¤£¬¹Ø±Õ·ÅµçMOS
		uint16 cell_lowVol_release;	//µ¥Î»ÊÇmV, µ¥ÌåÇ·Ñ¹Ñ¹±£»¤×´Ì¬½â³ıµçÑ¹

		
		uint16 group_overVoltage;	//Õû×é¹ıÑ¹¹
		uint16 group_overVol_release;	//µ¥Î»ÊÇmV,µ¥Ìå¹ıÑ¹±£»¤×´Ì¬½â³ıµçÑ¹
		uint16 group_lowVoltage;	//µ¥Î»ÊÇmV,µ¥½ÚÇ·Ñ¹±£»¤Öµ,×îµÍµ¥ÌåĞ¡ÓÚÓÚ¸ÃÖµÔò·¢Éú±£»¤£¬¹Ø±Õ·ÅµçMOS
		uint16 group_lowVol_release;	//µ¥Î»ÊÇmV, µ¥ÌåÇ·Ñ¹Ñ¹±£»¤×´Ì¬½â³ıµçÑ¹

		uint16 charge_HighTemp;	//µ¥Ìå¹ıÑ¹
		uint16 charge_HighTemp_release;	//µ¥Î»ÊÇmV,µ¥Ìå¹ıÑ¹±£»¤×´Ì¬½â³ıµçÑ¹
		uint16 charge_lowTemp;	//µ¥Î»ÊÇmV,µ¥½ÚÇ·Ñ¹±£»¤Öµ,×îµÍµ¥ÌåĞ¡ÓÚÓÚ¸ÃÖµÔò·¢Éú±£»¤£¬¹Ø±Õ·ÅµçMOS
		uint16 charge_lowTemp_release;	//µ¥Î»ÊÇmV, µ¥ÌåÇ·Ñ¹Ñ¹±£»¤×´Ì¬½â³ıµçÑ¹

		uint16  discharge_HighTemp;	//µ¥Ìå¹ıÑ¹
		uint16  discharge_HighTemp_release;	//µ¥Î»ÊÇmV,µ¥Ìå¹ıÑ¹±£»¤×´Ì¬½â³ıµçÑ¹
		uint16  discharge_lowTemp;	//µ¥Î»ÊÇmV,µ¥½ÚÇ·Ñ¹±£»¤Öµ,×îµÍµ¥ÌåĞ¡ÓÚÓÚ¸ÃÖµÔò·¢Éú±£»¤£¬¹Ø±Õ·ÅµçMOS
		uint16  discharge_lowTemp_release;	//µ¥Î»ÊÇmV, µ¥ÌåÇ·Ñ¹Ñ¹±£»¤×´Ì¬½â³ıµçÑ¹


		uint16  charge_overCurrent;	//µ¥Î»ÊÇmV,µ¥½ÚÇ·Ñ¹±£»¤Öµ,×îµÍµ¥ÌåĞ¡ÓÚÓÚ¸ÃÖµÔò·¢Éú±£»¤£¬¹Ø±Õ·ÅµçMOS
		uint16  discharge_overCurrent;	//µ¥Î»ÊÇmV, µ¥ÌåÇ·Ñ¹Ñ¹±£»¤×´Ì¬½â³ıµçÑ¹

		uint16 Voltage;// ×ÜµçÑ¹
		int16 Current;// ³ä·ÅµçÁ÷

		uint16 Soc;// Ê£ÓàÈİÁ¿

		uint16 Cell_1;// 1ºÅµç³ØµçÑ¹
		uint16 Cell_2;// 1ºÅµç³ØµçÑ¹
		uint16 Cell_3;// 1ºÅµç³ØµçÑ¹
		uint16 Cell_4;// 1ºÅµç³ØµçÑ¹
		uint16 Cell_5;// 1ºÅµç³ØµçÑ¹
		uint16 Cell_6;// 1ºÅµç³ØµçÑ¹

		uint16 Cell_7;// 1ºÅµç³ØµçÑ¹
		uint16 Cell_8;// 1ºÅµç³ØµçÑ¹
		uint16 Cell_9;// 9ºÅµç³ØµçÑ¹
		uint16 Cell_10;// 10ºÅµç³ØµçÑ¹
		uint16 Cell_11;// 11ºÅµç³ØµçÑ¹

		uint16 Cell_12;// 12ºÅµç³ØµçÑ¹
		uint16 Cell_13;// 13ºÅµç³ØµçÑ¹
		uint16 Cell_14;// 14ºÅµç³ØµçÑ¹
		uint16 Cell_15;// 15ºÅµç³ØµçÑ¹
		uint16 Cell_16;// 16ºÅµç³ØµçÑ¹

		uint16 Temp1;// Ê£ÓàÈİÁ¿
		uint16 Temp2;// Ê£ÓàÈİÁ¿

		uint16 Soc_full;// Ê£ÓàÈİÁ¿

		uint16 RSOC; //¸ß×Ö½ÚÔ¤Áô£¬µÍ×Ö½Ú±íÊ¾Ê£ÓàÈİÁ¿°Ù·Ö±È1¾Í±íÊ¾1%£¬×î´ó100¡£
	};  

public:
//api
	bmsSensor();
	bmsSensor(unsigned char add);
	~bmsSensor();


public :
	void print_hex(unsigned char * data);
	void print_hex(std::string &data);

	void Set_add(unsigned char  add);
	unsigned char add();
	void MyMemclear(const void *src,int count) ;
	std::string Cmd_GetInfo();

	unsigned short getCRC(unsigned char *p,int num);
	int infocheck(std::string &str);
	void recordInfo(std::string &str);
	void printMsg(void);
	
public:
	 struct _BmsWork BmsOrgInfo;
	
private:

        unsigned char m_add;   //Éè±¸×ÔÉíµØÖ·
      
	

	
	
	
};




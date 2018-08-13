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
		uint16 cell_overVoltage;	//�����ѹ
		uint16 cell_overVol_release;	//��λ��mV,�����ѹ����״̬�����ѹ
		uint16 cell_lowVoltage;	//��λ��mV,����Ƿѹ����ֵ,��͵���С���ڸ�ֵ�����������رշŵ�MOS
		uint16 cell_lowVol_release;	//��λ��mV, ����Ƿѹѹ����״̬�����ѹ

		
		uint16 group_overVoltage;	//�����ѹ�
		uint16 group_overVol_release;	//��λ��mV,�����ѹ����״̬�����ѹ
		uint16 group_lowVoltage;	//��λ��mV,����Ƿѹ����ֵ,��͵���С���ڸ�ֵ�����������رշŵ�MOS
		uint16 group_lowVol_release;	//��λ��mV, ����Ƿѹѹ����״̬�����ѹ

		uint16 charge_HighTemp;	//�����ѹ
		uint16 charge_HighTemp_release;	//��λ��mV,�����ѹ����״̬�����ѹ
		uint16 charge_lowTemp;	//��λ��mV,����Ƿѹ����ֵ,��͵���С���ڸ�ֵ�����������رշŵ�MOS
		uint16 charge_lowTemp_release;	//��λ��mV, ����Ƿѹѹ����״̬�����ѹ

		uint16  discharge_HighTemp;	//�����ѹ
		uint16  discharge_HighTemp_release;	//��λ��mV,�����ѹ����״̬�����ѹ
		uint16  discharge_lowTemp;	//��λ��mV,����Ƿѹ����ֵ,��͵���С���ڸ�ֵ�����������رշŵ�MOS
		uint16  discharge_lowTemp_release;	//��λ��mV, ����Ƿѹѹ����״̬�����ѹ


		uint16  charge_overCurrent;	//��λ��mV,����Ƿѹ����ֵ,��͵���С���ڸ�ֵ�����������رշŵ�MOS
		uint16  discharge_overCurrent;	//��λ��mV, ����Ƿѹѹ����״̬�����ѹ

		uint16 Voltage;// �ܵ�ѹ
		int16 Current;// ��ŵ���

		uint16 Soc;// ʣ������

		uint16 Cell_1;// 1�ŵ�ص�ѹ
		uint16 Cell_2;// 1�ŵ�ص�ѹ
		uint16 Cell_3;// 1�ŵ�ص�ѹ
		uint16 Cell_4;// 1�ŵ�ص�ѹ
		uint16 Cell_5;// 1�ŵ�ص�ѹ
		uint16 Cell_6;// 1�ŵ�ص�ѹ

		uint16 Cell_7;// 1�ŵ�ص�ѹ
		uint16 Cell_8;// 1�ŵ�ص�ѹ
		uint16 Cell_9;// 9�ŵ�ص�ѹ
		uint16 Cell_10;// 10�ŵ�ص�ѹ
		uint16 Cell_11;// 11�ŵ�ص�ѹ

		uint16 Cell_12;// 12�ŵ�ص�ѹ
		uint16 Cell_13;// 13�ŵ�ص�ѹ
		uint16 Cell_14;// 14�ŵ�ص�ѹ
		uint16 Cell_15;// 15�ŵ�ص�ѹ
		uint16 Cell_16;// 16�ŵ�ص�ѹ

		uint16 Temp1;// ʣ������
		uint16 Temp2;// ʣ������

		uint16 Soc_full;// ʣ������

		uint16 RSOC; //���ֽ�Ԥ�������ֽڱ�ʾʣ�������ٷֱ�1�ͱ�ʾ1%�����100��
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

        unsigned char m_add;   //�豸�����ַ
      
	

	
	
	
};




/*
 * publicfuc.h
 *
 *  Created on: 2009-11-5
 *      Author: letian
 */

#ifndef PUBLICFUC_H_
#define PUBLICFUC_H_

#include <QLineEdit>
#include <QCheckBox>
#include <stdio.h>
#include "DemoPublic.h"

#ifndef HPR_OK
#define HPR_OK 0
#endif

#ifndef HPR_ERROR
#define HPR_ERROR -1
#endif

#ifndef INT_LEN
#define INT_LEN 10  //the length of the string of 32 bit int
#endif

#ifndef IP_LEN
#define IP_LEN 16 //һ��IP��ַ����󳤶�
#endif

#define KIT_ERRPR_DIS_TYPE 1

typedef struct
{
	int iArrayChanInfo[MAX_CHANNUM_V30]; //-1��ʾ��Чͨ����; ��+��ʼͨ����
	char  cStartNO;          //��ʼͨ����
	char  cTotalNum;        //����ͨ������, Ҳ������iArrayChanInfo[]ֵ����-1������ͳ�Ƴ���
	char  cSupportIP;       //�Ƿ�֧��IPͨ��0����֧�֣�1��֧��
	char  cRes;
}KIT_CHAN_INFO, *PKIT_CHAN_INFO;

int KIT_editToInt(const QLineEdit *pEdit, int *pNum);

int KIT_intToEdit(QLineEdit *pEdit, int iNum);
int KIT_unintToEdit(QLineEdit *pEdit, unsigned int iNum);

int KIT_checkIP(char * ip);//check ip
int KIT_editToStr(const QLineEdit *pEdit, BYTE *pStr, int iLen);
int KIT_editToIP(const QLineEdit *pEditIP, BYTE *pIP, int iLen);

int KIT_qstringToStr(const QString *pString, BYTE *pStr, int iLen);
int KIT_qstringToInt(const QString *pString, int *pNum);

//ͨ���û�IDȥ��ȡͨ����Ϣ, iArrayChanelInfo�����СΪMAX_CHANNUM_V30
int KIT_GetChannelInfo(long lUserID,  PKIT_CHAN_INFO pChanInfo);

//ͨ���û�IDȥ����һ������ͨ����checkbox����; new�����ı��������ⲿ�ͷ�
//�������СΪ64, ���������һ�����ܱ����С�������������
int KIT_CreateCheckboxChanArr(long lUserID, QCheckBox *qArrChan[], int *piTotalNum);

//���ݱ��������ţ���ȡ�����������
int getAlarmOutInfo(int iAlarmOut, QCheckBox *pQCheckAlarmOut);
//��ȡ���ñ������ͨ��Checkbox���飻 new�����ı��������ⲿ�ͷ�
//�������СΪMAX_ALARMOUT_V30, ���������һ�����ܱ����С�������������
int KIT_CreateCheckboxAlarmOut(long lUserID, QCheckBox *qArrAlarmOut[], int *piTotalNum);

//��ʾ������ʾ��Ϣ
int KIT_DisplayErrorInfo(char *pInfoType, int iDisplayType);

void KIT_OutputDebugString(const char * );

#endif /* PUBLICFUC_H_ */

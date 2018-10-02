/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * �ļ����ƣ�catmnetparams.cpp
 * ��    ����
 * ��ǰ�汾��1.0
 * ��    �ߣ����Ƕ�
 * �������ڣ�2010��1��12��
 * �޸ļ�¼��
 */


#include "catmnetparams.h"
#include "publicfuc.h"


#include <QMessageBox>

/*******************************************************************
      Function:   CATMNetParams::CATMNetParams
     Parameter:   (IN)   QWidget *parent ������ָ��  
   Description:   ���캯��
**********************************************************************/
CATMNetParams::CATMNetParams(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	//ATM����
	QStringList listATMType;
	listATMType<<"NCR"<<"DIEBOLD"<<"WINCOR-NIXDORF"<<"SIEMENS"<<"OLIVETTI";
	listATMType<<"FUJITSU"<<"HITACHI"<<"SMI"<<"IBM"<<"BULL";
	listATMType<<"����"<<"����"<<"�����ͨ"<<"Mini-Banl"<<"����"<<"����";
	listATMType<<"��ͨ"<<"����"<<"����"<<"��������"<<"���̩����"<<"DRS918";
	listATMType<<"KALATEL"<<"NCR_2"<<"NXS";
	ui.comboATMType->addItems(listATMType);

	//��ѯ����
	QStringList listCodeType;
	listCodeType<<"Query"<<"Fetch"<<"Disposit"<<"Modify Passwd"<<"Transfer";		
		
	ui.comboCodeType->addItems(listCodeType);

	m_bCode = FALSE;
	memset(m_frameTypeCode, 0, sizeof(NET_DVR_FRAMETYPECODE)*10);

	connect(ui.comboCodeType, SIGNAL(activated(int)), this, SLOT(comboCodeTypeChanged(int)));
}

/*******************************************************************
      Function:   CATMNetParams::~CATMNetParams
     Parameter:   (IN)   �޲���  
   Description:   ��������
**********************************************************************/
CATMNetParams::~CATMNetParams()
{

}

/*******************************************************************
      Function:   CATMNetParams::getATMInfo
     Parameter:   (IN)   LPNET_DVR_FRAMEFORMAT pstruATMInfo  ATM����ָ��
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ���ⲿ��ȡ�Ĳ������õ�������
**********************************************************************/
int CATMNetParams::getATMInfo(LPNET_DVR_FRAMEFORMAT pstruATMInfo)
{
	if(NULL == pstruATMInfo)
	{
		m_bCode = FALSE;
		return HPR_ERROR;
	}

	//IP��ַ
	ui.editIP->setText(pstruATMInfo->sATMIP);
	//ATM����
	ui.comboATMType->setCurrentIndex((int)pstruATMInfo->dwATMType);

	//���ı�־λ����ʼλ��
	KIT_intToEdit(ui.editSignBeginPos, (int)pstruATMInfo->dwFrameSignBeginPos);
	//���ı�־λ������
	KIT_intToEdit(ui.editSignLength, (int)pstruATMInfo->dwFrameSignLength);
	//���ı�־λ������
        ui.editSignContent->setText((char *)pstruATMInfo->byFrameSignContent);

	//���ų�����Ϣ: ��ʼλ��
	KIT_intToEdit(ui.editCardLenBeginPos, (int)pstruATMInfo->dwCardLengthInfoBeginPos);
	//���ų�����Ϣ: ����
	KIT_intToEdit(ui.editCardLenLen, (int)pstruATMInfo->dwCardLengthInfoLength);

	//������Ϣ: ��ʼλ��
	KIT_intToEdit(ui.editCardPos, (int)pstruATMInfo->dwCardNumberInfoBeginPos);
	//������Ϣ: ����
	KIT_intToEdit(ui.editCardLen, (int)pstruATMInfo->dwCardNumberInfoLength);

	//������Ϣ: ��ʼλ��
	KIT_intToEdit(ui.editBusinessPos, (int)pstruATMInfo->dwBusinessTypeBeginPos);
	//������Ϣ: ��ʼλ��
	KIT_intToEdit(ui.editBusinessLen, (int)pstruATMInfo->dwBusinessTypeLength);

	//����
	m_bCode = TRUE;
	memcpy(m_frameTypeCode, pstruATMInfo->frameTypeCode, sizeof(NET_DVR_FRAMETYPECODE)*10);
	ui.comboCodeType->setCurrentIndex(0);
        ui.editCode->setText( (char *)pstruATMInfo->frameTypeCode[0].code);
	return HPR_OK;
}

/*******************************************************************
      Function:   CATMNetParams::setATMInfo
     Parameter:   (IN)   LPNET_DVR_FRAMEFORMAT pstruATMInfo ATM����ָ�� 
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   �������ϵ�����д��AMT����ָ����
**********************************************************************/
int CATMNetParams::setATMInfo(LPNET_DVR_FRAMEFORMAT pstruATMInfo)
{
	if(NULL == pstruATMInfo)
	{
		return HPR_ERROR;
	}

	//IP��ַ
        KIT_editToIP(ui.editIP, (BYTE *)pstruATMInfo->sATMIP, 16);
	//����ATM����
	pstruATMInfo->dwATMType = ui.comboATMType->currentIndex();

	//���ı�־λ����ʼλ��
	KIT_editToInt(ui.editSignBeginPos, (int *)&(pstruATMInfo->dwFrameSignBeginPos));
	//���ı�־λ������
	KIT_editToInt(ui.editSignLength, (int *)&(pstruATMInfo->dwFrameSignLength));
	//���ı�־λ������
	KIT_editToStr(ui.editSignContent, pstruATMInfo->byFrameSignContent, 12);

	//���ų�����Ϣ: ��ʼλ��
	KIT_editToInt(ui.editCardLenBeginPos, (int *)&(pstruATMInfo->dwCardLengthInfoBeginPos));
	//���ų�����Ϣ: ����
	KIT_editToInt(ui.editCardLenLen, (int *)&(pstruATMInfo->dwCardLengthInfoLength));

	//������Ϣ: ��ʼλ��
	KIT_editToInt(ui.editCardPos, (int *)&(pstruATMInfo->dwCardNumberInfoBeginPos));
	//������Ϣ: ����
	KIT_editToInt(ui.editCardLen, (int *)&(pstruATMInfo->dwCardNumberInfoLength));

	//������Ϣ: ��ʼλ��
	KIT_editToInt(ui.editBusinessPos, (int *)&(pstruATMInfo->dwBusinessTypeBeginPos));
	//������Ϣ: ��ʼλ��
	KIT_editToInt(ui.editBusinessLen, (int *)&(pstruATMInfo->dwBusinessTypeLength));

	//����
	int iCodeType;
	iCodeType = ui.comboCodeType->currentIndex();
	KIT_editToStr(ui.editCode, pstruATMInfo->frameTypeCode[iCodeType].code, 12);

	return HPR_OK;
}

/*******************************************************************
      Function:   CATMNetParams::comboCodeTypeChanged
     Parameter:   (IN)   int iType  ATM������code�ֶε����
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ATM������code�ֶε���Ÿı�ʱ����edit�ؼ���д����Ӧ��ֵ
**********************************************************************/
void CATMNetParams::comboCodeTypeChanged(int iType)
{
	//QMessageBox::information(this, "Please check!", "The error is");
	if(m_bCode)
	{
                ui.editCode->setText((char *)m_frameTypeCode[iType].code);
	}
}






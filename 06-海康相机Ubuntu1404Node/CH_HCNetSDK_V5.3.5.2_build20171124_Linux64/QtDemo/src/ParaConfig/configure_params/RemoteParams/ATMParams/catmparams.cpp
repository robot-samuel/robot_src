/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * �ļ����ƣ�catmparams.cpp
 * ��    ����
 * ��ǰ�汾��1.0
 * ��    �ߣ����Ƕ�
 * �������ڣ�2010��1��12��
 * �޸ļ�¼��
 */

#include "catmparams.h"
#include "publicfuc.h"
#include "DemoPublic.h"

#include <QMessageBox>

/*******************************************************************
      Function:   CATMParams::CATMParams
     Parameter:   (IN)   QWidget *parent  ������ָ��
   Description:   ���캯��
**********************************************************************/
CATMParams::CATMParams(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	m_pNetListen = new(std::nothrow) CATMNetParams;
	ui.widget->show();
	ui.vLayout->addWidget(m_pNetListen);
	ui.vLayout->addWidget(ui.widget);


	QStringList list;
	list<<"Sniff"<<"TCP/IP"<<"String From Serial Port"<<"ATM CMD From Serial Port";		
		
	ui.comboInputType->addItems(list);

	QStringList listATMType;
	listATMType<<"NCR"<<"DIEBOLD"<<"WINCOR-NIXDORF"<<"SIEMENS"<<"OLIVETTI";
	listATMType<<"FUJITSU"<<"HITACHI"<<"SMI"<<"IBM"<<"BULL";
	listATMType<<"����"<<"����"<<"�����ͨ"<<"Mini-Banl"<<"����"<<"����";
	listATMType<<"��ͨ"<<"����"<<"����"<<"��������"<<"���̩����"<<"DRS918";
	listATMType<<"KALATEL"<<"NCR_2"<<"NXS";
	ui.comboATMType->addItems(listATMType);

	connect(ui.comboInputType, SIGNAL(activated(int)), this,  SLOT(comboInputTypeChanged(int)));
}

/*******************************************************************
      Function:   CATMParams::~CATMParams
     Parameter:   (IN)   �޲���  
   Description:   ��������
**********************************************************************/
CATMParams::~CATMParams()
{
	delete m_pNetListen;
}

/*******************************************************************
      Function:   CATMParams::setUserID
     Parameter:   (IN)   long lUserID  �û�ID
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   �����û�ID
**********************************************************************/
int CATMParams::setUserID(long lUserID)
{
	if(lUserID < 0)
	{
		return HPR_ERROR;
	}
	m_lUserID = lUserID;

	return HPR_OK;
}

/*******************************************************************
      Function:   CATMParams::comboInputTypeChanged
     Parameter:   (IN)   int iType ATM��������  
        Return:   ��
   Description:   ��ATM�������͸ı�ʱ�������ú���������ҳ��
**********************************************************************/
void CATMParams::comboInputTypeChanged(int iType)
{
	switch(iType)
	{
	case 0: //�������(����)
		ui.widget->hide();
		m_pNetListen->show();
		ui.vLayout->addWidget(m_pNetListen);
		break;
	case 1: //����Э��(����)
		ui.widget->show();
		ui.wPort->show();
		ui.wType->hide();
		m_pNetListen->hide();
		break;
	case 2: //����ֱ������(����)
		ui.widget->show();
		ui.wPort->hide();
		ui.wType->show();
		m_pNetListen->hide();
		break;
	case 3: //����ATM��������(����)
		ui.widget->hide();
		m_pNetListen->hide();
		break;
	default: //�������
		break;
	}
}

/*******************************************************************
      Function:   CATMParams::getATMInfo
     Parameter:   (IN)   �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��ȡATM��������ˢ�½���
**********************************************************************/
int CATMParams::getATMInfo()
{
	if(m_lUserID < 0 )
	{
		return HPR_ERROR;
	}
	isATMDevice();

	int iRet;
	NET_DVR_FRAMEFORMAT struATMParams;
	iRet = NET_DVR_ClientGetframeformat(m_lUserID, &struATMParams );
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    ui.comboInputType->setCurrentIndex((int)struATMParams.dwInputMode);
    //���ý����ֵ
    switch(struATMParams.dwInputMode)
    {
    case 0: //��·����
    	//ATM��ַ
    	m_pNetListen->getATMInfo(&struATMParams);
    	break;
    case 1: //����Э��
    	//��ȡ�˿ں�
    	unsigned short  sPort;
    	if(NET_DVR_GetATMPortCFG(m_lUserID, &sPort))
    	{
    		KIT_intToEdit(ui.editPort, (int)sPort);
    	}
    	break;
    case 2: //���ڼ���
    	//ATM����
    	ui.comboATMType->setCurrentIndex((int)struATMParams.dwATMType);
    	break;
    case 3: //
    	break;
    default:
    	break;
    }

	return HPR_OK;
}

/*******************************************************************
      Function:   CATMParams::setATMInfo
     Parameter:   (IN)   �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ���ý�����������ǲ���ˢ�½���
**********************************************************************/
int CATMParams::setATMInfo()
{
	if(m_lUserID < 0 )
	{
		return HPR_ERROR;
	}

	int iRet;
	NET_DVR_FRAMEFORMAT struATMParams;
	iRet = NET_DVR_ClientGetframeformat(m_lUserID, &struATMParams );
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    int iInputType;
    iInputType = ui.comboInputType->currentIndex();
    struATMParams.dwInputMode = (unsigned int)iInputType;
    //��ȡ�����ֵ
	switch(iInputType)
	{
	case 0: //��·����
		//ATM��ַ
		m_pNetListen->setATMInfo(&struATMParams);
		break;
	case 1: //����Э��
		//��ȡ�˿ں�
		int  iPort;
		KIT_editToInt(ui.editPort, &iPort);
		if(!NET_DVR_SetATMPortCFG(m_lUserID, (unsigned short)iPort))
		{
			return HPR_ERROR;
		}
		break;
	case 2: //���ڼ���
		//ATM����
		struATMParams.dwATMType = (unsigned int)ui.comboATMType->currentIndex();
		break;
	case 3: //
		break;
	default:
		break;
	}

	//���õ��豸��ȥ
	iRet = NET_DVR_ClientSetframeformat(m_lUserID, &struATMParams );
    if(!iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    //�����󷽿���Ч
    QMessageBox::information(this, "Information", "Please restart device to enable the setting!");
	return HPR_OK;
}

int CATMParams::isATMDevice()
{
	if(m_lUserID < 0 )
	{
		return HPR_ERROR;
	}

	int iRet;
	DWORD paramsLen;
	NET_DVR_DEVICECFG deviceParams ;
	memset(&deviceParams, 0, sizeof(NET_DVR_DEVICECFG));
	iRet =     NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, -1, &deviceParams, sizeof(NET_DVR_DEVICECFG), &paramsLen);
    if(!iRet)
    {
    	KIT_DisplayErrorInfo("Get device information", KIT_ERRPR_DIS_TYPE);
    	return HPR_ERROR;
    }

    int iDeviceType;
    iDeviceType = deviceParams.byDVRType;
    if(iDeviceType != ATMDVR && iDeviceType != ATMDVR_S)
    {
    	setEnabled(FALSE);
    	return FALSE;
    }
    else
    {
    	setEnabled(TRUE);
    	return TRUE;
    }
}

/*******************************************************************
      Function:   CATMParams::showEvent
     Parameter:   (IN)   QShowEvent * event  ������ʾ�¼�
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��������ʾʱ���Զ����豸��ȡ���ݣ�ˢ�½���
**********************************************************************/
void CATMParams::showEvent ( QShowEvent * event )
{
	if(NULL == event)
	{
	}


	comboInputTypeChanged(0); //ˢ�½���
	getATMInfo(); //���´�������
}




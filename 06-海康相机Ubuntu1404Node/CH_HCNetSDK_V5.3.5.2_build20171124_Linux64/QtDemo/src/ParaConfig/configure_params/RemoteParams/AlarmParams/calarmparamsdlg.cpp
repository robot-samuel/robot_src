/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * �ļ����ƣ�calarmparamsdlg.cpp
 * ��    ����
 * ��ǰ�汾��1.0
 * ��    �ߣ����Ƕ�
 * �������ڣ�2010��1��15��
 * �޸ļ�¼��
 */

#include "calarmparamsdlg.h"
#include "ui_calarmparamsdlg.h"
#include "publicfuc.h"
#include "calarminhandletype.h"
#include "calarminptz.h"

CAlarmParamsDlg::CAlarmParamsDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CAlarmParamsDlg)
{
	m_lUserID = -1;
	m_bGetAlarmInInfo = FALSE;
	m_bGetAlarmOutInfo = FALSE;

    memset(&m_struIPAlarmInCfg, 0, sizeof(NET_DVR_IPALARMINCFG));
    memset(&m_struIPAlarmOutCfg, 0, sizeof(NET_DVR_IPALARMOUTCFG));
    m_ui->setupUi(this);
    connect(m_ui->comboBoxAlarmIn, SIGNAL(currentIndexChanged(int )), this, SLOT(changeAlarmInIndex(int )));
    connect(m_ui->comboBoxAlarmOut, SIGNAL(currentIndexChanged(int )), this, SLOT(changeAlarmOutIndex(int )));
    connect(m_ui->comboBoxAlarmDate,SIGNAL(currentIndexChanged(int )),this,SLOT(changeAlarmOutDateIndex(int )));
    //connect(m_ui->pushButtonAlarmInHandle,SIGNAL(clicked()),this,SLOT(AlarmInHandle()));
}

CAlarmParamsDlg::~CAlarmParamsDlg()
{
    delete m_ui;
}

void CAlarmParamsDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int CAlarmParamsDlg::setUserID(long lUserID)
 {
     if(lUserID >= 0)
     {
         m_lUserID = lUserID;
         return 1;
     }
     else
     {
         return 0;
     }
 }

void CAlarmParamsDlg::showEvent(QShowEvent * event)
{
	if(NULL == event)
	{
	}
	getAlarmInfo();

   changeAlarmInIndex(0);
   changeAlarmOutIndex(0);
}

void CAlarmParamsDlg::changeAlarmInIndex(int index)
{
    char szLan[128] = {0};
    char cTemp[100] = {0};
    unsigned int dwID = 0;
    m_iAlarmInIndex = m_ui->comboBoxAlarmIn->itemData(m_ui->comboBoxAlarmIn->currentIndex()).toInt();
    //������������
    memcpy(cTemp, m_struAlarmInCfg[m_iAlarmInIndex].sAlarmInName, NAME_LEN);
    m_ui->lineEditAlarmInName->setText(cTemp);
    m_ui->comboBoxAlarmInType->setCurrentIndex(m_struAlarmInCfg[m_iAlarmInIndex].byAlarmType);
    //�������봦��
    if(m_struAlarmInCfg[m_iAlarmInIndex].byAlarmInHandle == 1)
    {
        m_ui->checkBoxAlarmInHandle->setCheckState(Qt::Checked);
    }
    else
    {
        m_ui->checkBoxAlarmInHandle->setCheckState(Qt::Unchecked);
    }
    //�������봦��ť
    if (m_struAlarmInCfg[m_iAlarmInIndex].byAlarmInHandle ==1 )
    {
        m_ui->pushButtonAlarmInHandle->setEnabled(true);
    }
    else
    {
        m_ui->pushButtonAlarmInHandle->setEnabled(false);
    }

    //IP�豸��ַ
    //ģ�ⱨ������ͨ����ַ
    if (m_iAlarmInIndex >= 0 && m_iAlarmInIndex < MAX_ANALOG_ALARMIN)
    {
        m_ui->lineEditAlarmInIP->setText("Local");
        m_ui->lineEditAlarmInChan->setText("0");
    }
    //IP��������ͨ����ַ
    else if (m_iAlarmInIndex >= MAX_ANALOG_ALARMIN && m_iAlarmInIndex < MAX_ALARMIN_V30)
    {
        dwID = m_struIPAlarmInCfg.struIPAlarmInInfo[m_iAlarmInIndex-MAX_ANALOG_ALARMIN].byIPID;
        if (dwID <= 0)
        {
           m_ui->lineEditAlarmInIP->setText("Off line");
           m_ui->lineEditAlarmInChan->setText("0");
        }
        else
        {
            m_ui->lineEditAlarmInIP->setText(m_struIpAccessCfg.struIPDevInfo[dwID-1].struIP.sIpV4);
            m_ui->lineEditAlarmInChan->setText((char *)&(m_struIPAlarmInCfg.struIPAlarmInInfo[m_iAlarmInIndex-MAX_ANALOG_ALARMIN].byAlarmIn));
        }
    }
    m_ui->lineEditAlarmInIP->setEnabled(FALSE);
    m_ui->lineEditAlarmInChan->setEnabled(TRUE);

    memcpy(&m_struCurrAlarmInCfg, &(m_struAlarmInCfg[index]), sizeof(NET_DVR_ALARMINCFG_V30));
}

void CAlarmParamsDlg::changeAlarmOutIndex(int index)
{
    char szLan[128] = {0};
    char cTemp[100] = {0};
    unsigned int dwID = 0;
    m_iAlarmOutIndex = m_ui->comboBoxAlarmOut->itemData(m_ui->comboBoxAlarmOut->currentIndex()).toInt();
    memcpy(cTemp, m_struAlarmOutCfg[m_iAlarmOutIndex].sAlarmOutName, NAME_LEN);
    m_ui->lineEditAlarmOutName->setText(cTemp);

    if (m_iAlarmOutIndex >= 0 && m_iAlarmOutIndex < MAX_ANALOG_ALARMOUT)
    {
        m_ui->lineEditAlarmOutIp->setText("Local");
        m_ui->lineEditAlarmOutChan->setText("0");
    }
    else if (m_iAlarmOutIndex >= MAX_ANALOG_ALARMOUT && m_iAlarmOutIndex < MAX_ALARMOUT_V30)
    {
        dwID = m_struIPAlarmOutCfg.struIPAlarmOutInfo[m_iAlarmOutIndex-MAX_ANALOG_ALARMOUT].byIPID;
        if (dwID <= 0)
        {
           m_ui->lineEditAlarmOutIp->setText("Off line");
           m_ui->lineEditAlarmOutChan->setText("0");
        }
        else
        {
            m_ui->lineEditAlarmOutIp->setText(m_struIpAccessCfg.struIPDevInfo[dwID-1].struIP.sIpV4);
            m_ui->lineEditAlarmOutChan->setText((char *)&(m_struIPAlarmOutCfg.struIPAlarmOutInfo[m_iAlarmOutIndex-MAX_ANALOG_ALARMOUT].byAlarmOut));
        }

    }
    m_ui->comboBoxAlarmOutDelay->setCurrentIndex(m_struAlarmOutCfg[m_iAlarmOutIndex].dwAlarmOutDelay);
    m_ui->comboBoxAlarmDate->setCurrentIndex(0);
    changeAlarmOutDateIndex(0);

    return;
}

void CAlarmParamsDlg::changeAlarmOutDateIndex(int index)
{
    m_iAlarmOutIndex = m_ui->comboBoxAlarmOut->itemData(m_ui->comboBoxAlarmOut->currentIndex()).toInt();
    KIT_intToEdit(m_ui->lineEditHour1,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][0].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour2,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][0].byStopHour));
    KIT_intToEdit(m_ui->lineEditHour3,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][1].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour4,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][1].byStopHour));
    KIT_intToEdit(m_ui->lineEditHour5,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][2].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour6,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][2].byStopHour));
    KIT_intToEdit(m_ui->lineEditHour7,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][3].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour8,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][3].byStopHour));
    KIT_intToEdit(m_ui->lineEditHour9,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][4].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour10,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][4].byStopHour));
    KIT_intToEdit(m_ui->lineEditHour11,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][5].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour12,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][5].byStopHour));
    KIT_intToEdit(m_ui->lineEditHour13,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][6].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour14,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][6].byStopHour));
    KIT_intToEdit(m_ui->lineEditHour15,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][7].byStartHour));
    KIT_intToEdit(m_ui->lineEditHour16,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][7].byStopHour));
    KIT_intToEdit(m_ui->lineEditMinute1,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][0].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute2,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][0].byStopMin));
    KIT_intToEdit(m_ui->lineEditMinute3,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][1].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute4,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][1].byStopMin));
    KIT_intToEdit(m_ui->lineEditMinute5,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][2].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute6,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][2].byStopMin));
    KIT_intToEdit(m_ui->lineEditMinute7,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][3].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute8,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][3].byStopMin));
    KIT_intToEdit(m_ui->lineEditMinute9,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][4].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute10,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][4].byStopMin));
    KIT_intToEdit(m_ui->lineEditMinute11,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][5].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute12,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][5].byStopMin));
    KIT_intToEdit(m_ui->lineEditMinute13,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][6].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute14,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][6].byStopMin));
    KIT_intToEdit(m_ui->lineEditMinute15,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][7].byStartMin));
    KIT_intToEdit(m_ui->lineEditMinute16,(int)(m_struAlarmOutCfg[m_iAlarmOutIndex].struAlarmOutTime[index][7].byStopMin));
}

int CAlarmParamsDlg::getAlarmInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}

    int i = 0;
    DWORD dwReturned = -1;
    int AlarmInNum = 0;
    int AlarmOutNum = 0;
    int iAlarmInSel = 0;
    int iAlarmOutSel = 0;
    char strTemp[10];
    memset(&m_struIPAlarmInCfg, 0, sizeof(NET_DVR_IPALARMINCFG));
    memset(&m_struIPAlarmOutCfg, 0, sizeof(NET_DVR_IPALARMOUTCFG));
    NET_DVR_DEVICECFG m_struDeviceInfo;
    //�豸����
    NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, 0, &m_struDeviceInfo, sizeof(NET_DVR_DEVICECFG), &dwReturned);
    //IP�������
    //NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPPARACFG, 0, &m_struIpAccessCfg, sizeof(NET_DVR_IPPARACFG), &dwReturned);
    AlarmInNum = m_struDeviceInfo.byAlarmInPortNum;
    AlarmOutNum = m_struDeviceInfo.byAlarmOutPortNum;
    m_ui->comboBoxAlarmIn->clear();
    m_ui->comboBoxAlarmOut->clear();

    //9000�豸
    if(m_struDeviceInfo.byDVRType == DS90XX_HF_S)
    {
        if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPALARMINCFG, 0, &m_struIPAlarmInCfg, sizeof(NET_DVR_IPALARMINCFG), &dwReturned))
        {
            QMessageBox::information(this, tr("Error"),tr("NET_DVR_GET_IPALARMINCFG failed"));
            return HPR_ERROR;
        }
        if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPALARMOUTCFG, 0, &m_struIPAlarmOutCfg, sizeof(NET_DVR_IPALARMOUTCFG), &dwReturned))
        {
            QMessageBox::information(this, tr("Error"),tr("NET_DVR_GET_IPALARMOUTCFG failed"));
            return HPR_ERROR;
        }
    }

    //��ȡ�����������
	for(i = 0; i < MAX_ALARMIN_V30; i++)
	{
		m_bGetAlarmIn[i] = FALSE;
		if (i < (int)AlarmInNum || (i >= MAX_ANALOG_ALARMIN && m_struIPAlarmInCfg.struIPAlarmInInfo[i-MAX_ANALOG_ALARMIN].byIPID > 0))
		{
			if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMINCFG_V30, i, &m_struAlarmInCfg[i], sizeof(NET_DVR_ALARMINCFG_V30), &dwReturned))
			{
				m_bGetAlarmIn[i] = FALSE;
				return HPR_ERROR;
			}
			if (i<(int)AlarmInNum)
			{
				sprintf(strTemp,"AlarmIn%d",(i+1));
				m_ui->comboBoxAlarmIn->addItem(strTemp);
			}
			else
			{
				sprintf(strTemp,"IPAlarmIn%d",(i+1-MAX_ANALOG_ALARMIN));
				m_ui->comboBoxAlarmIn->addItem(strTemp);
			}
			 m_bGetAlarmIn[i] = TRUE;
			 m_ui->comboBoxAlarmIn->setItemData(iAlarmInSel, i);
			 iAlarmInSel ++;
		 }
	}
        m_ui->comboBoxAlarmIn->setCurrentIndex(0);
	m_iAlarmInIndex = m_ui->comboBoxAlarmIn->itemData(m_ui->comboBoxAlarmIn->currentIndex()).toInt();

	//�����������
	for(i = 0; i < MAX_ALARMOUT_V30; i++)
	{
		m_bGetAlarmOut[i] = FALSE;
		if (i<(int)AlarmOutNum || (i>=MAX_ANALOG_ALARMOUT && m_struIPAlarmOutCfg.struIPAlarmOutInfo[i-MAX_ANALOG_ALARMOUT].byIPID > 0))
		{
			if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMOUTCFG_V30, i, &m_struAlarmOutCfg[i], sizeof(NET_DVR_ALARMOUTCFG_V30), &dwReturned))
			{
				m_bGetAlarmOut[i] = FALSE;
				return HPR_ERROR;
			}
			if (i<(int)AlarmOutNum)
			{
				sprintf(strTemp,"AlarmOut%d",(i+1));
				m_ui->comboBoxAlarmOut->addItem(strTemp);
			}
			else if (i>=MAX_ANALOG_ALARMOUT)
			{
				sprintf(strTemp,"IPAlarmOut%d",(i+1-MAX_ANALOG_ALARMOUT));
				m_ui->comboBoxAlarmOut->addItem(strTemp);
			}

			m_bGetAlarmOut[i] = TRUE;
			m_ui->comboBoxAlarmOut->setItemData(iAlarmOutSel, i);
			iAlarmOutSel++;
		}
	}
        m_ui->comboBoxAlarmOut->setCurrentIndex(0);
	m_iAlarmOutIndex = m_ui->comboBoxAlarmOut->itemData(m_ui->comboBoxAlarmOut->currentIndex()).toInt();

	//���������������ȡ��
	m_bGetAlarmInInfo = TRUE;
	memcpy(&m_struCurrAlarmInCfg, &(m_struAlarmInCfg[0]), sizeof(NET_DVR_ALARMINCFG_V30));
	return HPR_OK;
}

//���ñ�����ز���
//����֮ǰ�Ĳ�����������������Ѿ���ȡ����m_struCurrAlarmInCfg
int CAlarmParamsDlg::setAlarmInInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	//***���������������***//
	int iAlarmInIndex;
	iAlarmInIndex = m_ui->comboBoxAlarmIn->currentIndex();
	QString stringTemp;
	stringTemp = m_ui->comboBoxAlarmIn->itemText(iAlarmInIndex);
	iAlarmInIndex = getAlarmInIndexByName(stringTemp.toLatin1().data());

	//������������
	KIT_editToStr(m_ui->lineEditAlarmInName, m_struCurrAlarmInCfg.sAlarmInName, NAME_LEN);
    //����������
	m_struCurrAlarmInCfg.byAlarmType = m_ui->comboBoxAlarmInType->currentIndex();
	//��������ʽ; ���ڴ�����ָ�룬��Ϊ����ʽ�������Ѿ������ù���
	m_struCurrAlarmInCfg.byAlarmInHandle = m_ui->checkBoxAlarmInHandle->isChecked();

	//���õ��豸��ȥ
	int iRet;
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMINCFG_V30, iAlarmInIndex,
				&m_struCurrAlarmInCfg, sizeof(NET_DVR_SET_ALARMINCFG_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get alarm in parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//***���������������***//

	return HPR_OK;
}

//���ñ����������
int CAlarmParamsDlg::setAlarmoutInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	int iAlarmOutIndex;
	iAlarmOutIndex = m_ui->comboBoxAlarmOut->currentIndex();
	QString stringTemp;
	stringTemp = m_ui->comboBoxAlarmIn->itemText(iAlarmOutIndex);
	iAlarmOutIndex = getAlarmInIndexByName(stringTemp.toLatin1().data());


	//�Ȼ�ȡ�����в�������
	DWORD dwReturned;
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMOUTCFG_V30, iAlarmOutIndex, &m_struCurrAlarmOutCfg, sizeof(NET_DVR_ALARMOUTCFG_V30), &dwReturned))
	{
		QMessageBox::information(this, tr("Please check!"), tr("Get alarm out parameters error! "));
		return HPR_ERROR;
	}

	//��ȡ�ؼ��е�ֵ
	//�����������
    KIT_editToStr(m_ui->lineEditAlarmOutName, m_struCurrAlarmOutCfg.sAlarmOutName, NAME_LEN);
    //����ź�״̬ά��ʱ��
    m_struCurrAlarmOutCfg.dwAlarmOutDelay = (unsigned int)m_ui->comboBoxAlarmOutDelay->currentIndex();
    //ʱ��
    int iTemp;
    int index ;
    index = m_ui->comboBoxAlarmDate->currentIndex();
    KIT_editToInt(m_ui->lineEditHour1, &iTemp);
    m_struCurrAlarmOutCfg.struAlarmOutTime[index][0].byStartHour = (BYTE)iTemp;
    KIT_editToInt(m_ui->lineEditHour2, &iTemp);
    m_struCurrAlarmOutCfg.struAlarmOutTime[index][0].byStopHour = (BYTE)iTemp;
    KIT_editToInt(m_ui->lineEditHour3, &iTemp);
    m_struCurrAlarmOutCfg.struAlarmOutTime[index][1].byStartHour = (BYTE)iTemp;
    KIT_editToInt(m_ui->lineEditHour4, &iTemp);
    m_struCurrAlarmOutCfg.struAlarmOutTime[index][1].byStopHour = (BYTE)iTemp;
    KIT_editToInt(m_ui->lineEditHour5, &iTemp);
    m_struCurrAlarmOutCfg.struAlarmOutTime[index][2].byStartHour = (BYTE)iTemp;
    KIT_editToInt(m_ui->lineEditHour6, &iTemp);
    m_struCurrAlarmOutCfg.struAlarmOutTime[index][2].byStopHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour7, &iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][3].byStartHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour8, &iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][3].byStopHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour9, &iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][4].byStartHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour10,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][4].byStopHour =  (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour11, &iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][5].byStartHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour12, &iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][5].byStopHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour13, &iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][6].byStartHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour14, &iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][6].byStopHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour15,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][7].byStartHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditHour16,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][7].byStopHour = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute1,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][0].byStartMin = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute2,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][0].byStopMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute3,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][1].byStartMin   = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute4,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][1].byStopMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute5,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][2].byStartMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute6,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][2].byStopMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute7,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][3].byStartMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute8,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][3].byStopMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute9,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][4].byStartMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute10,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][4].byStopMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute11,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][5].byStartMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute12,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][5].byStopMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute13,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][6].byStartMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute14,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][6].byStopMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute15,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][7].byStartMin  = (BYTE)iTemp;
   KIT_editToInt(m_ui->lineEditMinute16,&iTemp);
   m_struCurrAlarmOutCfg.struAlarmOutTime[index][7].byStopMin  = (BYTE)iTemp;

	//���ؼ��е�ֵ���õ��豸��ȥ
	int iRet;
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMOUTCFG_V30, iAlarmOutIndex,
				&m_struCurrAlarmOutCfg, sizeof(NET_DVR_ALARMOUTCFG_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Set alarm out parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}


//�Ƿ����ñ������봦��ʽ
void CAlarmParamsDlg::on_checkBoxAlarmInHandle_clicked()
{
	if(m_ui->checkBoxAlarmInHandle->isChecked())
	{
		m_ui->pushButtonAlarmInHandle->setEnabled(TRUE);
	}
	else
	{
		m_ui->pushButtonAlarmInHandle->setEnabled(FALSE);
	}
}

//���ñ������봦��ʽ
void CAlarmParamsDlg::on_pushButtonAlarmInHandle_clicked()
{
	int iAlarmIndex;
	iAlarmIndex = m_ui->comboBoxAlarmIn->currentIndex();
	if(m_lUserID < 0 || iAlarmIndex < 0)
	{
		return ;
	}

	//��ȡ�������봦��ʽ����
	if(!m_bGetAlarmIn[iAlarmIndex])
	{
		return ;
	}

	CAlarmInHandleType dlgHandleType;
	//�����û�ID
	dlgHandleType.setUserID(m_lUserID);
	//���봦��ʽ���
	dlgHandleType.setAlarmInInfo(&(m_struCurrAlarmInCfg));
	dlgHandleType.exec();
}

//���ñ����������button
void CAlarmParamsDlg::on_pushButtonAlarmInSave_clicked()
{
	setAlarmInInfo();
	getAlarmInfo();
}

//���ñ����������button
void CAlarmParamsDlg::on_pushButtonAlarmOutSave_clicked()
{
	   setAlarmoutInfo();
	   getAlarmInfo();
}

//���ݱ����������ƣ���ȡ�����������
int CAlarmParamsDlg::getAlarmInIndexByName(char * strName)
{
	int iLen;
	iLen = strlen(strName);
	if(strName == NULL || iLen <= 0)
	{
		return HPR_ERROR;
	}

    int bIP = FALSE; //IP����
    if(strName[0] == 'I')
    {
    	bIP = TRUE;
    }
    int i = 0;
    //AlarmIn1
    //�ҵ���ſ�ʼ�ĵط�
    while(strName[i] != 'n' && i < iLen)
    {
    	i++;
    }

    i++;
    int iIndex = -1;
    if(i < iLen)
    {
    	iIndex = atoi(&strName[i]);
    }

    iIndex -= 1;
	return iIndex;
}

int CAlarmParamsDlg::getAlarmOutIndexByName(char * strName)
{
	if(strName == NULL)
	{
		return HPR_ERROR;
	}

	int iLen;
	iLen = strlen(strName);
	if(strName == NULL || iLen <= 0)
	{
		return HPR_ERROR;
	}

    int bIP = FALSE; //IP����
    if(strName[0] == 'I')
    {
    	bIP = TRUE;
    }
    int i = 0;
    //AlarmIn1
    //�ҵ���ſ�ʼ�ĵط�
    while(strName[i] != 't' && i < iLen)
    {
    	i++;
    }

    i++;
    int iIndex = -1;
    if(i < iLen)
    {
    	iIndex = atoi(&strName[i]);
    }

    iIndex -= 1;
	return iIndex;
}

//PTZ����
void CAlarmParamsDlg::on_pushButtonPTZ_clicked()
{
	int iAlarmIndex;
	iAlarmIndex = m_ui->comboBoxAlarmIn->currentIndex();
	if(m_lUserID < 0 || iAlarmIndex < 0)
	{
		return ;
	}

	//��ȡ�������봦��ʽ����
	if(!m_bGetAlarmIn[iAlarmIndex])
	{
		return ;
	}

	CAlarmInPTZ dlgPTZ;
	//�����û�ID
	dlgPTZ.setUserID(m_lUserID);
	//���봦��ʽ���
	dlgPTZ.setAlarmInInfo(&(m_struCurrAlarmInCfg));
	dlgPTZ.exec();
}





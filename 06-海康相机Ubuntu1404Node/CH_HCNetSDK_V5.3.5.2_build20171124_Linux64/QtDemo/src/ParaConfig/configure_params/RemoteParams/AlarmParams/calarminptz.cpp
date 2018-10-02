/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * �ļ����ƣ�calarminptz.cpp
 * ��    ����
 * ��ǰ�汾��1.0
 * ��    �ߣ����Ƕ�
 * �������ڣ�2010��1��15��
 * �޸ļ�¼��
 */

#include "calarminptz.h"
#include "publicfuc.h"

CAlarmInPTZ::CAlarmInPTZ(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	m_pAlarmInInfo = NULL; //�ⲿ���룬�ⲿ�ͷ�
	connect(ui.comboChannel, SIGNAL(activated(int)), this, SLOT(changeChanIndex(int)));
}

CAlarmInPTZ::~CAlarmInPTZ()
{

}


int CAlarmInPTZ::setUserID(long lUserID)
 {
     if(lUserID >= 0)
     {
         m_lUserID = lUserID;
         return HPR_OK;
     }
     else
     {
         return HPR_ERROR;
     }
 }

int CAlarmInPTZ::setAlarmInInfo(LPNET_DVR_ALARMINCFG_V30 pAlarmInInfo)
{
	if(pAlarmInInfo == NULL)
	{
		return HPR_ERROR;
	}

	//���豸����д�����
	m_pAlarmInInfo = pAlarmInInfo;
	return HPR_OK;
}

//�������ݵ�����
int CAlarmInPTZ::getAlarmInPTZInfo()
{
	if(m_pAlarmInInfo == NULL || m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	//�Ȼ�ȡͨ��
	KIT_CHAN_INFO struChanInfo;
	if(KIT_GetChannelInfo(m_lUserID, &struChanInfo) == HPR_ERROR)
	{
		return HPR_ERROR;
	}
	//����ͨ��list���
	int i;
	int iChanIndex;
	QStringList list;
	char strTemp[100] = {0};
	for(i = 0; i < MAX_CHANNUM_V30; i++ )
	{
		iChanIndex = struChanInfo.iArrayChanInfo[i] - struChanInfo.cStartNO;
		if(iChanIndex < 0)
		{
			continue;
		}
		if(iChanIndex >= 0 && iChanIndex < MAX_ANALOG_CHANNUM)
		{
			sprintf(strTemp, "Channel%d", struChanInfo.iArrayChanInfo[i]);
			list<<strTemp;
		}
		if( iChanIndex >= MAX_ANALOG_CHANNUM && iChanIndex < MAX_CHANNUM_V30)
		{
			sprintf(strTemp, "IPChannel%d", struChanInfo.iArrayChanInfo[i]);
			list<<strTemp;
		}
	}
	ui.comboChannel->addItems(list);

	//Ĭ����ʾ��һ��ͨ��������
	changeChanIndex(0);


	return HPR_OK;
}

//�������ݵ��豸��
int CAlarmInPTZ::setAlarmInPTZInfo()
{
	if(m_pAlarmInInfo == NULL || m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	int iChanIndex;
	iChanIndex = ui.comboChannel->currentIndex();
	int iTemp = 0;

	//��ȡֵ���豸
	//Ԥ�õ�
	if(ui.rBtnPreset->isChecked())
	{
		m_pAlarmInInfo->byEnablePreset[iChanIndex] = 1;
		KIT_editToInt(ui.editPresetNo, &iTemp);
		m_pAlarmInInfo->byPresetNo[iChanIndex] = (BYTE)iTemp;
	}

	//Ԥ�õ�
	if(ui.rBtnCruise->isChecked())
	{
		m_pAlarmInInfo->byEnableCruise[iChanIndex] = 1;
		KIT_editToInt(ui.editCruiseNo, &iTemp);
		m_pAlarmInInfo->byCruiseNo[iChanIndex] = (BYTE)iTemp;
	}

	//Ԥ�õ�
	if(ui.rBtnPtzTrack->isChecked())
	{
		m_pAlarmInInfo->byEnablePtzTrack[iChanIndex] = 1;
		KIT_editToInt(ui.editPtzTrackNO, &iTemp);
		m_pAlarmInInfo->byPTZTrack[iChanIndex] = (BYTE)iTemp;
	}

	return HPR_OK;
}

void CAlarmInPTZ::changeChanIndex(int index)
{
	if(index < 0)
	{
		return ;
	}
	//�Ƿ����Ԥ�õ�,
	ui.rBtnPreset->setChecked(m_pAlarmInInfo->byEnablePreset[index]);
	ui.editPresetNo->setEnabled(m_pAlarmInInfo->byEnablePreset[index]);
	KIT_intToEdit(ui.editPresetNo, (int)m_pAlarmInInfo->byPresetNo[index]);

	//�Ƿ����Ԥ�õ�
	ui.rBtnCruise->setChecked(m_pAlarmInInfo->byEnableCruise[index]);
	ui.editCruiseNo->setEnabled(m_pAlarmInInfo->byEnableCruise[index]);
	KIT_intToEdit(ui.editCruiseNo, (int)m_pAlarmInInfo->byCruiseNo[index]);

	//�Ƿ���ù켣
	ui.rBtnPtzTrack->setChecked(m_pAlarmInInfo->byEnablePtzTrack[index]);
	ui.editPtzTrackNO->setEnabled(m_pAlarmInInfo->byEnablePtzTrack[index]);
	KIT_intToEdit(ui.editPtzTrackNO, (int)m_pAlarmInInfo->byPTZTrack[index]);

}

void CAlarmInPTZ::on_rBtnPreset_clicked()
{
	ui.editPresetNo->setEnabled(TRUE);
	ui.editCruiseNo->setEnabled(FALSE);
	ui.editPtzTrackNO->setEnabled(FALSE);
}

void CAlarmInPTZ::on_rBtnCruise_clicked()
{
	ui.editPresetNo->setEnabled(FALSE);
	ui.editCruiseNo->setEnabled(TRUE);
	ui.editPtzTrackNO->setEnabled(FALSE);
}

void CAlarmInPTZ::on_rBtnPtzTrack_clicked()
{
	ui.editPresetNo->setEnabled(FALSE);
	ui.editCruiseNo->setEnabled(FALSE);
	ui.editPtzTrackNO->setEnabled(TRUE);
}

void CAlarmInPTZ::on_btnSave_clicked()
{
	setAlarmInPTZInfo();
	close();
}

void  CAlarmInPTZ::on_btnExit_clicked()
{
	close();
}

void CAlarmInPTZ::showEvent(QShowEvent * event)
{
	if(NULL == event)
	{
	}
	getAlarmInPTZInfo();
}








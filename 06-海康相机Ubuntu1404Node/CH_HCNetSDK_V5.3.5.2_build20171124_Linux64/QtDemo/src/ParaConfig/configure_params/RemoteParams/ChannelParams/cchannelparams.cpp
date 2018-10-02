/*
 * Copyright(C) 2009,Hikvision Digital Technology Co., Ltd 
 * 
 * �ļ����ƣ�cchannelparams.cpp
 * ��    ����
 * ��ǰ�汾��1.0
 * ��    �ߣ����Ƕ�
 * �������ڣ�2009��11��23��
 * �޸ļ�¼��
 */

#include <QMessageBox>
#include "cchannelparams.h"
#include "publicfuc.h"
#include "cdlgremoterecordschedule.h"
#include "cvilost.h"
#include "cshelter.h"
#include "cmotion.h"
#include "chidealarm.h"

/*******************************************************************
      Function:  CChannelParams::CChannelParams
     Parameter:  
           (IN)     QWidget *parent
           
   Description:   ���캯��
**********************************************************************/
CChannelParams::CChannelParams(QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);

	m_lChannel = 0;
	m_lUserID = -1;
	m_iStartChan = 0;
	int i;
	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		m_iArryChannel[i] = -1;
	}
	m_iSimChannelNum = 0;

    memset(&m_struRecord, 0, sizeof(NET_DVR_RECORD_V30));
    m_bSetRecord = FALSE;
    memset(&m_struVILost, 0, sizeof(NET_DVR_VILOST_V30));
    m_bVILost = FALSE;
    memset(m_struShelter, 0, MAX_SHELTERNUM*sizeof(NET_DVR_SHELTER));
    m_bShelter = FALSE;
    memset(&m_struMotion, 0, sizeof(NET_DVR_MOTION_V30));
    m_bMotion = FALSE;
    memset(&m_struHide, 0, sizeof(NET_DVR_HIDEALARM_V30));
    m_bHide = FALSE;

    ui.btnVILost->setEnabled(FALSE);

	QStringList strings;
	//compress type
	strings << "Main stream" << "Sub stream" << "Event";
	ui.comboPressType->addItems(strings);
	strings.clear();
	//pictur quality
	strings << "Best" << "Much better" << "Better" << "Normal" << "Bad" << "Worse";
	ui.comboPicQuality->addItems(strings);
	strings.clear();
	//video frame rate
	strings << "All" << "1/16" << "1/8" << "1/4" << "1/2" << "1" << "2" << "4"
			<< "6" << "8" << "10" << "12" << "16" << "20" << "15" << "18"
			<< "22";
	ui.comboVideoFrameRate->addItems(strings);
	strings.clear();
	//encode type
	strings << "hik264" << "h264" << "mpeg4";
	ui.comboVideoEncType->addItems(strings);
	strings.clear();
	//stream type
	strings << "Video" << "Video&Audio";
	ui.comboStreamType->addItems(strings);
	strings.clear();
	//video bit. 2-12
	strings << "32K" << "48k" << "64K" << "80K" << "96K" << "128K" << "160k"
			<< "192K" << "224K" << "256K" << "320K";
	//13-23
	strings << "384K" << "448K" << "512K" << "640K" << "768K" << "896K"
			<< "1024K" << "1280K" << "1536K" << "1792K" << "2048K";
	strings << "Self definition";
	ui.comboVideoBit->addItems(strings);
	strings.clear();
	//Audio Encode Type
	strings << "OggVorbis";
	ui.comboAudioEncType->addItems(strings);
	strings.clear();
	//Resolution
	strings << "DCIF" << "CIF" << "QCIF" << "4CIF" << "2CIF" << "VGA" << "UXGA"
			<< "SVGA" << "HD720p" << "XVGA" << "HD900p";
	ui.comboResolution->addItems(strings);
	strings.clear();
	//byBitrateType
	strings << "VBR" << "CBR";
	ui.comboBitrateType->addItems(strings);
	strings.clear();
	//IntervalBPFrame
	strings << "BBP" << "BP" << "P";
	ui.comboIntervalBPFrame->addItems(strings);
	strings.clear();

	//Ԥ¼ʱ�䣺0-��Ԥ¼��1-5�룬2-10�룬3-15�룬4-20�룬5-25�룬6-30�룬7-0xffffffff(������Ԥ¼)
	strings<<"Never"<<"5s"<<"10s"<<"15s"<<"20s"<<"25s"<<"30s"<<"No limit";
	ui.comboPreRecordTime->addItems(strings);
	strings.clear();

	//¼����ʱʱ�䣬0-5�룬 1-20�룬 2-30�룬 3-1���ӣ� 4-2���ӣ� 5-5���ӣ� 6-10����
	strings<<"5s"<<"20s"<<"30s"<<"1min"<<"2min"<<"5min"<<"10min";
	ui.comboRecordDelay->addItems(strings);
	strings.clear();

	//OSD���ڸ�ʽ
	strings<<"year-month-day"<<"month-day-year"<<"XXXXyearXXmonthXXday"<<"XXmonthXXdayXXXXyear"<<"day-month-year"<<"XXdayXXmonthXXXXyear";
    ui.comboOSDType->addItems(strings);
    strings.clear();

    //OSD����
    strings<<"Transparent&Twinkle"<<"Transparent&Untwinkle"<<"Untarnsparent&Twinkle"<<"Untransparent&Untwinkle";
    ui.comboOSDAttrib->addItems(strings);
    strings.clear();

    //Сʱ��
    strings<<"24hours"<<"AM/PM";
    ui.comboHourOsdType->addItems(strings);
    strings.clear();

    //�ַ���������
    strings<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
    ui.comboStringNo->addItems(strings);
    strings.clear();

	//combox
	ui.editVideoBit->setEnabled(FALSE);
	connect(ui.comboChannelNo, SIGNAL(activated(int)), this, SLOT(comboChNoChanged(int)));
	connect(ui.comboPressType, SIGNAL(activated(int)), this, SLOT(comboCompChanged(int)));
	connect(ui.comboVideoBit,  SIGNAL(activated(int)), this, SLOT(comboVideoBitChanged (int)));
	connect(ui.comboStringNo,  SIGNAL(activated(int)), this, SLOT(comboStringNoChanged(int)));

	//button
	connect(ui.btnRemoteRecord, SIGNAL(clicked()), this, SLOT(clickBtnRemoteRecord()));
	connect(ui.btnVILost,  SIGNAL(clicked()), this, SLOT(clickBtnVILost()));
    connect(ui.btnShelter, SIGNAL(clicked()), this, SLOT(clickBtnShelter()));

	//checkbox
	connect(ui.checkboVideoRecord,  SIGNAL(clicked()), this, SLOT(clickCheckVideoRecord()));
	connect(ui.checkboShowChanName, SIGNAL(clicked()), this, SLOT(clickCheckShowChanName()));
    connect(ui.checkboVILost, SIGNAL(clicked()), this, SLOT(clickCheckVILost()));
    connect(ui.checkShelter,  SIGNAL(clicked()), this, SLOT(clickCheckShelter()));
    connect(ui.checkMotion, SIGNAL(clicked()), this, SLOT(clickCheckMotion()));
    connect(ui.checkHide,   SIGNAL(clicked()), this, SLOT(clickCheckHide()));
    connect(ui.checkShowOsd, SIGNAL(clicked()), this, SLOT(clickCheckOSD()));
    connect(ui.checkShowString, SIGNAL(clicked()), this, SLOT(clickCheckShowString()));
}


/*******************************************************************
      Function:  CChannelParams::~CChannelParams
     Parameter:  
           (IN)     �޲���  
   Description:  �������� 
**********************************************************************/
CChannelParams::~CChannelParams()
{

}

/*******************************************************************
      Function:  CChannelParams::getAllInfo
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��ȡѹ��������¼�������ͼ�������Ϣ
**********************************************************************/
int CChannelParams::getAllInfo()
{
	if (m_lUserID < 0) {
		return HPR_ERROR;
	}
	//��ȡͨ���б�combox��
	getChannelNum();

	//Compress Infomation.
	getCompressInfo(0);   //ͼ��ѹ������
	getRecordInfo();      //¼�����
	getPictureInfo();     //ͼ�����
	getShowStringInfo();  //�����ַ�����
	return HPR_OK;
}

/*******************************************************************
      Function:  CChannelParams::setAllInfo
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ���ؼ���ֵȡ���������õ��豸��ȥ
**********************************************************************/
int CChannelParams::setAllInfo()
{
	if (m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	setCompressInfo();
	setRecordInfo();
	setPictureInfo();
	setShowStringInfo();

	return HPR_OK;
}

/*******************************************************************
      Function:  CChannelParams::getChannelNum
     Parameter:  
           (IN)     �޲���  
        Return:  HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:  ��ȡģ��ͨ����IPͨ��������Ŀ
**********************************************************************/
int CChannelParams::getChannelNum() {
	NET_DVR_DEVICECFG struDeviceParams;
	DWORD paramsLen;
	int iRet;

	//***1.Get the channel number and IP channel number.***//
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, m_lChannel,
			&struDeviceParams, sizeof(NET_DVR_DEVICECFG), &paramsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error!"));
		return HPR_ERROR;
	}
	m_iStartChan = (int) (struDeviceParams.byStartChan);

	//***2.Get the channel which is enabled.***//
	NET_DVR_IPPARACFG struIPParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPPARACFG, m_lChannel,
			&struIPParams, sizeof(NET_DVR_IPPARACFG), &paramsLen);
	if (iRet == 0)
	{
		//iRet = NET_DVR_GetLastError();
		//QMessageBox::information(this, tr("Please check!"), tr("Get IP channel parameters error.\"%1\"").arg(iRet));
		//return HPR_ERROR;
	}

	int i, j;
	//����ͨ������Ϣ
	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		m_iArryChannel[i] = -1;
	}

	j = 0;
	if (iRet != 0)//if support ip channel
	{
		//channel
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)
		{
			if (struIPParams.byAnalogChanEnable[i])
			{
				m_iArryChannel[j] = i + m_iStartChan;
				j++;
			}
		}
		m_iSimChannelNum = j;
		//IP channel

		for (i = 0; i < MAX_IP_CHANNEL; i++)
		{
			if (struIPParams.struIPChanInfo[i].byIPID != 0)
			{
				m_iArryChannel[j] = i + m_iStartChan + MAX_ANALOG_CHANNUM;
				j++;
			}
		}
	}
	else//if not support ip channel
	{
		//simulate channel
		for (i = 0; (i < MAX_ANALOG_CHANNUM)
				&& (i < struDeviceParams.byChanNum); i++)
		{
			m_iArryChannel[j] = i + m_iStartChan;
			j++;
		}
		m_iSimChannelNum = j;
	}

	//***3. Add items to combox.***//
	char strTemp[100] = { 0 };
	QStringList strings;
	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		if (m_iArryChannel[i] >= 0) {
			if (m_iArryChannel[i] < MAX_ANALOG_CHANNUM) //channel
			{
				sprintf(strTemp, "Camora%d", m_iArryChannel[i]);
				strings << strTemp;
			} else//IP channel
			{
				if (iRet != 0)//supprot IP channel
				{
					sprintf(strTemp, "IPCamora%d", (m_iArryChannel[i] - MAX_ANALOG_CHANNUM));
					strings << strTemp;
				}
			}
		}
	}

	//QMessageBox::information(this, "Please check!", tr("getChannelNum() user id is \"%1\" ").arg(m_lUserID));
	ui.comboChannelNo->clear();
	ui.comboChannelNo->addItems(strings);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getCompressInfo
     Parameter:  
           (IN)     int iCompIndex  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��ȡѹ������
**********************************************************************/
int CChannelParams::getCompressInfo(int iCompIndex) {
	NET_DVR_COMPRESSIONCFG_V30 struCompress;
	int iRet;
	DWORD uiRetParamsLen;

	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_COMPRESSCFG_V30,
			m_lChannel, &struCompress, sizeof(NET_DVR_COMPRESSIONCFG_V30),
			&uiRetParamsLen);
	if (iRet == 0) {
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	ui.comboPressType->setCurrentIndex(iCompIndex);
	//Picture quality
	LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo;
	if (iCompIndex == 0)//video (major code stream)
	{
		pCompressInfo = &(struCompress.struNormHighRecordPara);
	}
	if (iCompIndex == 1)//net
	{
		pCompressInfo = &(struCompress.struNetPara);
	}
	if (iCompIndex == 2)//event
	{
		pCompressInfo = &(struCompress.struEventRecordPara);
	}
	setEachCompWidget(pCompressInfo);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setCompressInfo
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ����ѹ��
**********************************************************************/
int CChannelParams::setCompressInfo()
{
	NET_DVR_COMPRESSIONCFG_V30 struCompress;
	int iRet;
	DWORD uiRetParamsLen;

	//get the compress information
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_COMPRESSCFG_V30,
			m_lChannel, &struCompress, sizeof(NET_DVR_COMPRESSIONCFG_V30),
			&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//set the compress information
	int iCompIndex;
	iCompIndex = ui.comboPressType->currentIndex();

	LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo;
	if (iCompIndex == 0)//video (major code stream)
	{
		pCompressInfo = &(struCompress.struNormHighRecordPara);
	}
	if (iCompIndex == 1)//net
	{
		pCompressInfo = &(struCompress.struNetPara);
	}
	if (iCompIndex == 2)//event
	{
		pCompressInfo = &(struCompress.struEventRecordPara);
	}
	getEachCompWidget(pCompressInfo);
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_COMPRESSCFG_V30,
			m_lChannel, &struCompress, sizeof(NET_DVR_COMPRESSIONCFG_V30));
	if (iRet == 0) {
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Set press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getRecordInfo
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��ȡ¼�����
**********************************************************************/
int CChannelParams::getRecordInfo()
{
	NET_DVR_RECORD_V30 struRecord;
	int iRet;
	DWORD uiRetParamsLen;

	//get the record information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_RECORDCFG_V30,
				m_lChannel,
				&struRecord,
				sizeof(NET_DVR_RECORD_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//***set the value of each record widget***//
	//�Ƿ�¼��
	if(struRecord.dwRecord == 0)
	{
		ui.checkboVideoRecord->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkboVideoRecord->setCheckState(Qt::Checked);
	}
	//¼����ʱʱ��
	ui.comboRecordDelay->setCurrentIndex(int(struRecord.dwRecordTime));
	//Ԥ¼ʱ��
	ui.comboPreRecordTime->setCurrentIndex(int(struRecord.dwPreRecordTime ));
	//¼�񱣴���ʱ��
	KIT_intToEdit(ui.editDurationTime, int(struRecord.dwRecorderDuration));
	//�Ƿ�����¼��
	if(struRecord.byRedundancyRec  == 0)
	{
		ui.checkboRedundancy->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkboRedundancy->setCheckState(Qt::Checked);
	}
	//¼��ʱ����������ʱ�Ƿ��¼��Ƶ����
	if(struRecord.byAudioRec == 0)
	{
		ui.checkAudioRec->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkAudioRec->setCheckState(Qt::Checked);
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setRecordInfo
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ����¼�����
**********************************************************************/
int CChannelParams::setRecordInfo()
{
	NET_DVR_RECORD_V30 struRecord;
	int iRet;
	DWORD uiRetParamsLen;

	//1.get the record information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
				    NET_DVR_GET_RECORDCFG_V30,
					m_lChannel,
					&struRecord,
					sizeof(NET_DVR_RECORD_V30),
					&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}
	//2.ȡ�ؼ��е�ֵ
	//(1)�Ƿ�ʱ¼��
	if(ui.checkboVideoRecord->isChecked())
	{
		if(m_bSetRecord)
		{
			memcpy(&struRecord, &m_struRecord, sizeof(NET_DVR_RECORD_V30));
		}
		struRecord.dwRecord =1;
	}
	else
	{
		struRecord.dwRecord = 0;
	}
	//(2)��������
	int iTemp;
	KIT_editToInt(ui.editDurationTime, &iTemp);
	struRecord.dwRecorderDuration = (unsigned int)iTemp;
	//(3)Ԥ��ʱ��
	struRecord.dwPreRecordTime = ui.comboPreRecordTime->currentIndex();
	//(4)�ӳ�¼��
	struRecord.dwRecordTime = ui.comboRecordDelay->currentIndex();
	//(5)�Ƿ�����¼��
	if(ui.checkboRedundancy->isChecked())
	{
		struRecord.byRedundancyRec  = 1;
	}
	else
	{
		struRecord.byRedundancyRec = 0;
	}
	//(6)�Ƿ���Ƶ
	if(ui.checkAudioRec->isChecked())
	{
		struRecord.byAudioRec = 1;
	}
	else
	{
		struRecord.byAudioRec = 0;
	}

	//3.����
	iRet = NET_DVR_SetDVRConfig(m_lUserID,
				    NET_DVR_SET_RECORDCFG_V30,
					m_lChannel,
					&struRecord,
					sizeof(NET_DVR_RECORD_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Set press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getPictureInfo
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��ȡͼ�����
**********************************************************************/
int CChannelParams::getPictureInfo()
{
	NET_DVR_PICCFG_V30 struPictureParams;
	int iRet;
	DWORD uiRetParamsLen;

	//��ȡͼ�����
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
    {
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//***���ÿؼ��е�ֵ***/
        //ͨ������
        ui.editChannelName->setText((char *)struPictureParams.sChanName);

	//�Ƿ���ʾͨ������
	if(struPictureParams.dwShowChanName == 0)
	{
		ui.checkboShowChanName->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkboShowChanName->setCheckState(Qt::Checked);
	}
	//��ʾͨ�����Ƶ�����
	unsigned short usTemp;
	usTemp = (unsigned short)(struPictureParams.wShowNameTopLeftX);
	KIT_unintToEdit(ui.editShowNameTopLeftX, usTemp);
	usTemp = (unsigned short)(struPictureParams.wShowNameTopLeftY);
	KIT_unintToEdit(ui.editShowNameTopLeftY, usTemp);

	//��Ƶ�źŶ�ʧ����
	if(struPictureParams.struVILost.byEnableHandleVILost == 0)
	{
		ui.checkboVILost->setChecked(FALSE);
        ui.btnVILost->setEnabled(FALSE);
	}
	else
	{
		ui.checkboVILost->setChecked(TRUE);
		ui.btnVILost->setEnabled(TRUE);
	}

	//�Ƿ��ڵ�
	if(struPictureParams.dwEnableHide == 0)
	{
		ui.checkShelter->setChecked(FALSE);
		ui.btnShelter->setEnabled(FALSE);
	}
	else
	{
		ui.checkShelter->setChecked(TRUE);
		ui.btnShelter->setEnabled(TRUE);
	}

	//�Ƿ��ƶ����
	if(struPictureParams.struMotion.byEnableHandleMotion == 0)
	{
		ui.checkMotion->setChecked(FALSE);
		ui.btnMotion->setEnabled(FALSE);
	}
	else
	{
		ui.checkMotion->setChecked(TRUE);
		ui.btnMotion->setEnabled(TRUE);
	}

	//�ڵ�����
	if(struPictureParams.struHideAlarm.dwEnableHideAlarm == 0)
	{
		ui.checkHide->setChecked(FALSE);
		ui.btnHide->setEnabled(FALSE);
	}
	else
	{
		ui.checkHide->setChecked(TRUE);
		ui.btnHide->setEnabled(TRUE);
	}

	//OSD��ʾ
    if(struPictureParams.dwShowOsd == 0)
    {
    	ui.checkShowOsd->setChecked(FALSE);
    	clickCheckOSD();
    }
    else
    {
    	ui.checkShowOsd->setChecked(TRUE);
    	clickCheckOSD();
    }
    //X,Y��OSD����
    int iTemp;
    iTemp = (int)struPictureParams.wOSDTopLeftX;
    KIT_intToEdit(ui.editOSDTopLeftX, iTemp);
    iTemp = (int)struPictureParams.wOSDTopLeftY;
    KIT_intToEdit(ui.editOSDTopLeftY, iTemp);
    //OSD����
    ui.comboOSDType->setCurrentIndex(struPictureParams.byOSDType);
    //�Ƿ���ʾ����
    if(struPictureParams.byDispWeek == 0)
    {
    	ui.checkDispWeek->setChecked(FALSE);
    }
    else
    {
    	ui.checkDispWeek->setChecked(TRUE);
    }
    //OSD����
    ui.comboOSDAttrib->setCurrentIndex(struPictureParams.byOSDAttrib - 1);
    //Сʱ��
    ui.comboHourOsdType->setCurrentIndex(struPictureParams.byHourOSDType);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setPictureInfo
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ����ͼ�����
**********************************************************************/
int CChannelParams::setPictureInfo()
{
	NET_DVR_PICCFG_V30 struPictureParams;
	int iRet;
	DWORD uiRetParamsLen;

	//1.��ȡͼ�����
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_PICCFG_V30, m_lChannel,
			&struPictureParams, sizeof(NET_DVR_PICCFG_V30), &uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//2.��ȡ�ؼ���ֵ
	//ͨ������
	KIT_editToStr(ui.editChannelName, struPictureParams.sChanName, NAME_LEN);
        int iTemp = 0;

	//�Ƿ���ʾͨ�����ƣ�������ʾ����
	if(ui.checkboShowChanName->isChecked())
	{
		struPictureParams.dwShowChanName = 1;
		KIT_editToInt(ui.editShowNameTopLeftX, &iTemp);
		struPictureParams.wShowNameTopLeftX = (unsigned  short)iTemp;
		KIT_editToInt(ui.editShowNameTopLeftY, &iTemp);
		struPictureParams.wShowNameTopLeftY = (unsigned short)iTemp;
	}
	else
	{
		struPictureParams.dwShowChanName = 0;
	}

	//�Ƿ�������Ƶ�źŶ�ʧ����
	if(ui.checkboVILost->isChecked())
	{
		if(m_bVILost)
		{
			memcpy(&(struPictureParams.struVILost), &(m_struVILost), sizeof(NET_DVR_VILOST_V30));
		}
		struPictureParams.struVILost.byEnableHandleVILost = 1;
	}
	else
	{
		struPictureParams.struVILost.byEnableHandleVILost = 0;
	}

	//�Ƿ������ڵ�
	if(ui.checkShelter->isChecked())
	{
		if(m_bShelter)
		{
			memcpy(struPictureParams.struShelter, m_struShelter, MAX_SHELTERNUM*sizeof(NET_DVR_SHELTER));
		}
		struPictureParams.dwEnableHide = 1;
	}
	else
	{
		struPictureParams.dwEnableHide = 0;
	}

	//�Ƿ������ƶ����
	if(ui.checkMotion->isChecked())
	{
		if(m_bMotion)
		{
			memcpy(&(struPictureParams.struMotion), &(m_struMotion), sizeof(NET_DVR_MOTION_V30));
		}
		struPictureParams.struMotion.byEnableHandleMotion = 1;
	}
	else
	{
		struPictureParams.struMotion.byEnableHandleMotion = 0;
	}

	//�����ڵ�����
	if(ui.checkHide->isChecked())
	{
		if(m_bHide)
		{
			memcpy(&(struPictureParams.struHideAlarm), &(m_struHide), sizeof(NET_DVR_HIDEALARM_V30));
		}
		struPictureParams.struHideAlarm.dwEnableHideAlarm = 1;
	}
	else
	{
		struPictureParams.struHideAlarm.dwEnableHideAlarm = 0;
	}

	//����OSD
	if(ui.checkShowOsd->isChecked())
	{
		struPictureParams.dwShowOsd = 1;
		//����OSD��X��Y����
		KIT_editToInt(ui.editOSDTopLeftX, &iTemp);
		struPictureParams.wOSDTopLeftX = (unsigned short)iTemp;
		KIT_editToInt(ui.editOSDTopLeftY, &iTemp);
		struPictureParams.wOSDTopLeftY = (unsigned short)iTemp;
		//OSD����
		struPictureParams.byOSDType = ui.comboOSDType->currentIndex();
		//OSD����
		struPictureParams.byOSDAttrib = ui.comboOSDAttrib->currentIndex() + 1;
		//Сʱ��
		struPictureParams.byHourOSDType = ui.comboHourOsdType->currentIndex();
	}
	else
	{
		struPictureParams.dwShowOsd = 0;
	}
	//�Ƿ���ʾ����
	if(ui.checkDispWeek->isChecked())
	{
		struPictureParams.byDispWeek = 1;
	}
	else
	{
		struPictureParams.byDispWeek = 0;
	}

	//3.����ͼ�����
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_PICCFG_V30, m_lChannel,
				&struPictureParams, sizeof(NET_DVR_PICCFG_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get Picture parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}
	return HPR_OK;
}

//��ȡ�ַ����Ӳ���
int CChannelParams::getShowStringInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	comboStringNoChanged(0);

	return HPR_OK;
}

//�����ַ����Ӳ���
int CChannelParams::setShowStringInfo()
{
	//�Ȼ�ȡ�ַ����Ӳ���
	NET_DVR_SHOWSTRING_V30 struShowString;
	int iRet;
	DWORD uiRetParamsLen;

	//get show strings information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_SHOWSTRING_V30,
				m_lChannel,
				&struShowString,
				sizeof(NET_DVR_SHOWSTRING_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get show strings parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//��ȡ��ǰ���ַ������������
	int iShowStringNo;
	int iTemp;
	iShowStringNo = ui.comboStringNo->currentIndex();
	if(ui.checkShowString->isChecked())
	{
		struShowString.struStringInfo[iShowStringNo].wShowString = 1;
		//x,y����
		KIT_editToInt(ui.editShowStringTopLeftX, &iTemp);
		struShowString.struStringInfo[iShowStringNo].wShowStringTopLeftX = iTemp;
		KIT_editToInt(ui.editShowStringTopLeftY, &iTemp);
		struShowString.struStringInfo[iShowStringNo].wShowStringTopLeftY = iTemp;
		//strings
                KIT_editToStr(ui.editString, (BYTE *)struShowString.struStringInfo[iShowStringNo].sString, KIT_SHOWSTRING_LEN);
	}
	else
	{
		struShowString.struStringInfo[iShowStringNo].wShowString = 0;
	}

	//set show strings information
	iRet = NET_DVR_SetDVRConfig(m_lUserID,
			    NET_DVR_SET_SHOWSTRING_V30,
				m_lChannel,
				&struShowString,
				sizeof(NET_DVR_SHOWSTRING_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Set show strings parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setEachCompWidget
     Parameter:  
           (IN)     LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   �����豸��ȡ��ѹ������ֵ��д��ؼ���
**********************************************************************/
int CChannelParams::setEachCompWidget(LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo)
{
	ui.comboPicQuality->setCurrentIndex(pCompressInfo->byPicQuality);
	ui.comboVideoFrameRate->setCurrentIndex(
			int(pCompressInfo->dwVideoFrameRate));
	ui.comboVideoEncType->setCurrentIndex(pCompressInfo->byVideoEncType);

	ui.comboStreamType->setCurrentIndex(pCompressInfo->byStreamType);
	//bit rate
	unsigned int uiVideoBitrateIndex;
	uiVideoBitrateIndex = pCompressInfo->dwVideoBitrate;
	if (uiVideoBitrateIndex > 23)//define by myself.
	{
		ui.comboVideoBit->setCurrentIndex(22);
		uiVideoBitrateIndex = uiVideoBitrateIndex & 0x7fffffff;
		uiVideoBitrateIndex = uiVideoBitrateIndex/1024;
		KIT_intToEdit(ui.editVideoBit, (int)uiVideoBitrateIndex);
		ui.editVideoBit->setEnabled(TRUE);
	}
	else
	{
		ui.editVideoBit->setEnabled(FALSE);
		if (uiVideoBitrateIndex > 1)
		{
			ui.comboVideoBit->setCurrentIndex(uiVideoBitrateIndex - 2);
		}
	}
	//audio
	ui.comboAudioEncType->setCurrentIndex(pCompressInfo->byAudioEncType);

	//resolution
	int iResolution;
	iResolution = pCompressInfo->byResolution;
	if (iResolution > 4) {
		iResolution -= 11;
	}
	ui.comboResolution->setCurrentIndex(iResolution);
	//I frame interval time
	KIT_intToEdit(ui.editIntervalFrameI, int(pCompressInfo->wIntervalFrameI));

	//Bit rate type
	ui.comboBitrateType->setCurrentIndex(pCompressInfo->byBitrateType);
	//IntervalFrameI
	ui.comboIntervalBPFrame->setCurrentIndex(pCompressInfo->byIntervalBPFrame);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getEachCompWidget
     Parameter:  
           (IN)     LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��ȡ�ؼ���ֵ��д�뽫Ҫ�����豸�Ľṹ����
**********************************************************************/
int CChannelParams::getEachCompWidget(LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo) 
{
	pCompressInfo->byPicQuality = ui.comboPicQuality->currentIndex();
	pCompressInfo->dwVideoFrameRate = ui.comboVideoFrameRate->currentIndex();
	pCompressInfo->byVideoEncType = ui.comboVideoEncType->currentIndex();

	pCompressInfo->byStreamType = ui.comboStreamType->currentIndex();
	//bit rate
	int iVideoBitrate;
	if ((ui.comboVideoBit->currentIndex()) == 22)//define by myself
	{
		KIT_editToInt(ui.editVideoBit, &iVideoBitrate);
		if(iVideoBitrate <32)
		{
			iVideoBitrate = 32;
		}
		if(iVideoBitrate > 8192)
		{
			iVideoBitrate = 8192;
		}
		pCompressInfo->dwVideoBitrate = (unsigned int)iVideoBitrate*1024 + 0x80000000;
	} else {
		pCompressInfo->dwVideoBitrate = (ui.comboVideoBit->currentIndex() + 2);
	}
	//audio
	pCompressInfo->byAudioEncType = ui.comboAudioEncType->currentIndex();

	//resolution
	int iResolution;
	iResolution = ui.comboResolution->currentIndex();
	if (iResolution > 4)
	{
		pCompressInfo->byResolution = iResolution + 11;
	}
	else
	{
		pCompressInfo->byResolution = iResolution;
	}
	//I frame interval time
	int iIntervalFrameI;
	KIT_editToInt(ui.editIntervalFrameI, &iIntervalFrameI);
	pCompressInfo->wIntervalFrameI = iIntervalFrameI;

	//Bit rate type
	pCompressInfo->byBitrateType = ui.comboBitrateType->currentIndex();
	//IntervalFrameI
	pCompressInfo->byIntervalBPFrame = ui.comboIntervalBPFrame->currentIndex();

	return HPR_ERROR;
}

/*******************************************************************
      Function:   CChannelParams::comboChNoChanged
     Parameter:  
           (IN)     int iChannelIndex  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ͨ��ID���ı���
**********************************************************************/
void CChannelParams::comboChNoChanged(int iChannelIndex)
{
	m_lChannel = m_iArryChannel[iChannelIndex];
	//getAllInfo();

	//��ȡ����ͨ��������Ϣ��������Ĭ��Ϊ0����������Ϊ��ǰѡ��ֵ
	getCompressInfo(0);   //ͼ��ѹ������
	getRecordInfo();      //¼�����
	getPictureInfo();     //ͼ�����
	getShowStringInfo();  //�����ַ�����

}

/*******************************************************************
      Function:   CChannelParams::comboCompChanged
     Parameter:  
           (IN)     int iCompIndex  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ѹ������������ֵ�ı䣺�¼������磬..
**********************************************************************/
void CChannelParams::comboCompChanged(int iCompIndex)
{
	getCompressInfo(iCompIndex);
}

/*******************************************************************
      Function:   CChannelParams::comboVideoBitChanged
     Parameter:  
           (IN)     int iVideoBitIndex  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ¼������ʱ��ı�
**********************************************************************/
void CChannelParams::comboVideoBitChanged(int iVideoBitIndex)
{
	if (22 == iVideoBitIndex)
	{
		ui.editVideoBit->setEnabled(TRUE);
	}
	else
	{
		ui.editVideoBit->setEnabled(FALSE);
	}
}

//�ַ���������Ÿı�
void CChannelParams::comboStringNoChanged(int iStringNo)
{
	NET_DVR_SHOWSTRING_V30 struShowString;
	int iRet;
	DWORD uiRetParamsLen;

	//get show strings information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_SHOWSTRING_V30,
				m_lChannel,
				&struShowString,
				sizeof(NET_DVR_SHOWSTRING_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get show strings parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//�Ƿ���ʾ��������
	if(struShowString.struStringInfo[iStringNo].wShowString == 0)
	{
		ui.checkShowString->setChecked(FALSE);
		ui.editShowStringTopLeftX->setEnabled(FALSE);
		ui.editShowStringTopLeftY->setEnabled(FALSE);
		ui.editString->setEnabled(FALSE);
	}
	else
	{
		ui.checkShowString->setChecked(TRUE);
		ui.editShowStringTopLeftX->setEnabled(TRUE);
		ui.editShowStringTopLeftY->setEnabled(TRUE);
		ui.editString->setEnabled(TRUE);
	}

	//x,y����
	int iTemp;
	iTemp = (int)(struShowString.struStringInfo[iStringNo].wShowStringTopLeftX);
	KIT_intToEdit(ui.editShowStringTopLeftX, iTemp);
	iTemp = (int)(struShowString.struStringInfo[iStringNo].wShowStringTopLeftY);
	KIT_intToEdit(ui.editShowStringTopLeftY, iTemp);
	//�ַ���
	ui.editString->setText(struShowString.struStringInfo[iStringNo].sString);
}

/*******************************************************************
      Function:   CChannelParams::clickBtnRemoteRecord
     Parameter:  
           (IN)     �޲���  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   ��������ʱ��ƻ��Ľ���
**********************************************************************/
void CChannelParams::clickBtnRemoteRecord()
{
	NET_DVR_RECORD_V30 struRecord;
	int iRet;
	DWORD uiRetParamsLen;

	//get the record information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_RECORDCFG_V30,
				m_lChannel,
				&struRecord,
				sizeof(NET_DVR_RECORD_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//Create the dialog which is used to set the schedule.
	CDlgRemoteRecordSchedule dlgVedioRecord;
	dlgVedioRecord.SetStruRecord(&struRecord);
	dlgVedioRecord.exec();//��ģʽ�Ի���ʽ��ʾ
	memcpy(&m_struRecord, &struRecord, sizeof(NET_DVR_RECORD_V30));
	m_bSetRecord = TRUE;
}

/*******************************************************************
      Function:   CChannelParams::clickBtnVILost
     Parameter:
           (IN)     �޲���
        Return:   ��
   Description:   ��Ƶ�źŶ�ʧ��������
**********************************************************************/
void CChannelParams::clickBtnVILost()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//��ȡͼ������е��źŶ�ʧ������Ϣ
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//��ȡ�豸�����е���󱨾������
	NET_DVR_DEVICECFG struDeviceParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_DEVICECFG,
			    0,
				&struDeviceParams,
				sizeof(NET_DVR_DEVICECFG),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error1! \"%1\"").arg(iRet));
		return ;
	}

	memcpy(&(m_struVILost), &(struPictureParams.struVILost), sizeof(NET_DVR_VILOST_V30));
	//Create the dialog which is used to set the schedule.
	CVILost dlgVILost;
	dlgVILost.setStruVILost(&(m_struVILost)); //�źŶ�ʧ��������
	dlgVILost.setAlarmOutNum(struDeviceParams.byAlarmOutPortNum);
	dlgVILost.setUserID(m_lUserID);
	dlgVILost.exec();//��ģʽ�Ի���ʽ��ʾ
	m_bVILost = TRUE;
}

//�ڵ�����
void CChannelParams::clickBtnShelter()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//��ȡͼ������ֵ��ڵ���Ϣ
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//�����ڵ���Ϣ�����Ա��
	memcpy(m_struShelter, struPictureParams.struShelter, MAX_SHELTERNUM*sizeof(NET_DVR_SHELTER));

	//�����Ա���õ��Ի�����
	CShelter dlgShelter;
	dlgShelter.setShelter(m_struShelter);//�����ڵ�����ĵ�ַ
	dlgShelter.exec();

	m_bShelter = TRUE;
}

void CChannelParams::on_btnMotion_clicked()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//��ȡͼ������е��ƶ������Ϣ
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}
	//�����ƶ�����������ṹ��
	memcpy(&m_struMotion, &(struPictureParams.struMotion), sizeof(NET_DVR_MOTION_V30));

	//��ȡ�豸�����е���󱨾������
	NET_DVR_DEVICECFG struDeviceParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_DEVICECFG,
			    0,
				&struDeviceParams,
				sizeof(NET_DVR_DEVICECFG),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error! \"%1\"").arg(iRet));
		return ;
	}

	CMotion dlgMotion;
	//�����ƶ�������
	dlgMotion.setStruMotion(&m_struMotion);
	//�����û�ID, ��ȡ����ͨ��
	dlgMotion.setUserID(m_lUserID);
	//����ģ�ⱨ�������Ŀ
	dlgMotion.setAlarmOutNum(struDeviceParams.byAlarmOutPortNum);
	//���ÿɴ�����¼��ͨ����
	dlgMotion.setChannelID(m_iArryChannel);
	//������ʼͨ����
	dlgMotion.setStartChannelNo(m_iStartChan);
	dlgMotion.exec();
	m_bMotion = TRUE;
}

//�ڵ�����
void CChannelParams::on_btnHide_clicked()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//��ȡͼ������е��ƶ������Ϣ
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}
	//�����ƶ�����������ṹ��
	memcpy(&m_struHide, &(struPictureParams.struHideAlarm), sizeof(NET_DVR_HIDEALARM_V30));

	//��ȡ�豸�����е���󱨾������
	NET_DVR_DEVICECFG struDeviceParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_DEVICECFG,
			    0,
				&struDeviceParams,
				sizeof(NET_DVR_DEVICECFG),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error! \"%1\"").arg(iRet));
		return ;
	}

	CHideAlarm dlgHideAlarm;
	dlgHideAlarm.setStruHideAlarm(&m_struHide);
	dlgHideAlarm.setAlarmOutNum(struDeviceParams.byAlarmOutPortNum);
	dlgHideAlarm.setUserID(m_lUserID);
	dlgHideAlarm.exec();
	m_bHide = TRUE;
}

/*******************************************************************
      Function:   CChannelParams::clickCheckVideoRecord
     Parameter:  
           (IN)     �޲���  
        Return:  ��
   Description:   �Ƿ�ʱ¼��
**********************************************************************/
void CChannelParams::clickCheckVideoRecord()
{
	if(ui.checkboVideoRecord->isChecked())
	{
		ui.btnRemoteRecord->setEnabled(true);
	}
	else
	{
		m_bSetRecord = FALSE;
		ui.btnRemoteRecord->setEnabled(false);
	}
}

/*******************************************************************
      Function:   CChannelParams::clickCheckShowChanName
     Parameter:
           (IN)     �޲���
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   //(click check)�Ƿ�����Ļ����ʾͨ������
**********************************************************************/
 void CChannelParams::clickCheckShowChanName()
 {
	 if(ui.checkboShowChanName->isChecked())
	 {
		 ui.editShowNameTopLeftX->setEnabled(TRUE);
		 ui.editShowNameTopLeftY->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.editShowNameTopLeftX->setEnabled(FALSE);
		 ui.editShowNameTopLeftY->setEnabled(FALSE);
	 }
 }

 /*******************************************************************
       Function:   CChannelParams::clickCheckVILost
      Parameter:
            (IN)     ��
         Return:   ��
    Description:   �Ƿ�������Ƶ�źŶ�ʧ��������
 **********************************************************************/
 void CChannelParams::clickCheckVILost()
 {
	 if(ui.checkboVILost->isChecked())
	 {
		 ui.btnVILost->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnVILost->setEnabled(FALSE);
		 m_bVILost = FALSE;
	 }
 }


 void CChannelParams::clickCheckShelter()
 {
	 //QMessageBox::information(this, tr("Please check!"), tr("sss"));
	 if(ui.checkShelter->isChecked())
	 {
		 ui.btnShelter->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnShelter->setEnabled(FALSE);
		 m_bShelter = FALSE;
	 }
 }

 //(click check)�Ƿ������ƶ����
 void CChannelParams::clickCheckMotion()
 {
	 if(ui.checkMotion->isChecked())
	 {
		 ui.btnMotion->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnMotion->setEnabled(FALSE);
		 m_bMotion = FALSE;
	 }

 }
 //(click check)�Ƿ������ڵ�
 void CChannelParams::clickCheckHide()
 {
	 if(ui.checkHide->isChecked())
	 {
		 ui.btnHide->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnHide->setEnabled(FALSE);
		 m_bHide = FALSE;
	 }
 }

 //OSD��ʾ
 void CChannelParams::clickCheckOSD()
 {
	 if(ui.checkShowOsd->isChecked())
	 {
		 ui.editOSDTopLeftX->setEnabled(TRUE);
		 ui.editOSDTopLeftY->setEnabled(TRUE);
		 ui.checkDispWeek->setEnabled(TRUE);
		 ui.comboOSDAttrib->setEnabled(TRUE);
		 ui.comboOSDType->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.editOSDTopLeftX->setEnabled(FALSE);
		 ui.editOSDTopLeftY->setEnabled(FALSE);
		 ui.checkDispWeek->setEnabled(FALSE);
		 ui.comboOSDAttrib->setEnabled(FALSE);
		 ui.comboOSDType->setEnabled(FALSE);
	 }
 }

 //�Ƿ���ʾ�ַ�����
 void CChannelParams::clickCheckShowString()
 {
		//�Ƿ���ʾ��������
		if(ui.checkShowString->isChecked())
		{
			ui.editShowStringTopLeftX->setEnabled(TRUE);
			ui.editShowStringTopLeftY->setEnabled(TRUE);
			ui.editString->setEnabled(TRUE);
		}
		else
		{
			ui.editShowStringTopLeftX->setEnabled(FALSE);
			ui.editShowStringTopLeftY->setEnabled(FALSE);
			ui.editString->setEnabled(FALSE);
		}
 }

/*******************************************************************
      Function:   CChannelParams::setUserID
     Parameter:  
           (IN)     long lUserID  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   �����û�ID
**********************************************************************/
int CChannelParams::setUserID(long lUserID)
{
	m_lUserID = lUserID;
	return HPR_ERROR;
}

/*******************************************************************
      Function:   CChannelParams::showEvent
     Parameter:  
           (IN)     QShowEvent * event  
        Return:   HPR_OK--�ɹ���HPR_ERROR--ʧ�ܡ�
   Description:   �ڸô�����ʾʱ��ˢ��ҳ��Ĳ���
**********************************************************************/
void CChannelParams::showEvent(QShowEvent * event)
{
	//disable warning
	if (event == NULL)
	{
	}
	if (m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please login firstly!"), tr("User ID is not correct!."));
		return;
	}
	else
	{
		//QMessageBox::information(this, "Please check!", tr("showEvent() user id is \"%1\" ").arg(m_lUserID));
		getChannelNum(); //get channel list
		m_lChannel = m_iArryChannel[ui.comboChannelNo->currentIndex()];
		getAllInfo(); // update all data of widget
	}
}

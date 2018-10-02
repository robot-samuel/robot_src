/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   ptzcruise.cpp
 *        Description                   :   ��̨Ѳ��·��������ز���
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   none
*************************************************************************/
#include "ptzcruise.h"
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <stdio.h>

/************************************************************************
 *        Function            :  PtzCruise
 *        Description         :  ���캯��
 *        Input               :  int realhandle Ԥ�������,QWidget *parent
 								 QT widget����
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
PtzCruise::PtzCruise(int realhandle,QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	m_rpcurrentrealhandle = realhandle;

	QStringList stringlistforpreset;
	char strTemp[10];
	for (int i=1;i<256;i++)
	{
		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%d", i);
		stringlistforpreset <<strTemp;
	}
	ui.comboBox_ptzcruisepresetpoint->addItems(stringlistforpreset);
	ui.comboBox_ptzcruisetime->addItems(stringlistforpreset);

	QStringList stringlistforcruisespeed;
	//char strTemp[10];
	for (int i=1;i<16;i++)
	{
		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%d", i);
		stringlistforcruisespeed <<strTemp;
	}
	ui.comboBox_ptzcruisespeed->addItems(stringlistforcruisespeed);

	QStringList stringlistforcruise;
	for (int i=1;i<32;i++)
	{
		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%d", i);
		stringlistforcruise <<strTemp;
	}
	ui.comboBox_ptzcruisepath->addItems(stringlistforcruise);
	ui.comboBox_ptzcruisepoint->addItems(stringlistforcruise);

	m_ptzcruisepathindex = 1;
	m_ptzcruisepointindex = 1;
	m_ptzcruisepresetpointindex = 1;
	m_ptzcruisetimeindex = 1;
	m_ptzcruisespeedindex = 1;

}

/************************************************************************
 *        Function            :  ~PtzCruise
 *        Description         :  ��������
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
PtzCruise::~PtzCruise()
{

}

/************************************************************************
 *        Function            :  on_pushButton_addcruisepoint_clicked
 *        Description         :  ������̨Ѳ��Ԥ�õ�
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_pushButton_addcruisepoint_clicked()
{
 	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,FILL_PRE_SEQ, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisepresetpointindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,SET_SEQ_DWELL, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisetimeindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,SET_SEQ_SPEED, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisespeedindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
}

/************************************************************************
 *        Function            :  on_pushButton_deletecruisepoint_clicked
 *        Description         :  ɾ����̨Ѳ��Ԥ�õ�
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_pushButton_deletecruisepoint_clicked()
{
 	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,CLE_PRE_SEQ, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisepresetpointindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  �˳���̨Ѳ�����öԻ���
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_pushButton_exit_clicked()
{
	close();
}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisepath_currentIndexChanged
 *        Description         :  �޸���̨Ѳ��·�����
 *        Input               :  int index Ѳ��·�����
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisepath_currentIndexChanged(int index)
{
	m_ptzcruisepathindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisepoint_currentIndexChanged
 *        Description         :  �޸���̨Ԥ�õ���
 *        Input               :  int index ��̨Ԥ�õ���
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisepoint_currentIndexChanged(int index)
{
	m_ptzcruisepointindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisepresetpoint_currentIndexChanged
 *        Description         :  ������̨Ԥ�õ���
 *        Input               :  int index ��̨Ԥ�õ���
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisepresetpoint_currentIndexChanged(int index)
{
	m_ptzcruisepresetpointindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisetime_currentIndexChanged
 *        Description         :  �޸���̨Ѳ��ͣ��ʱ��ֵ
 *        Input               :  int index ͣ��ʱ��ֵ���
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisetime_currentIndexChanged(int index)
{
	m_ptzcruisetimeindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisespeed_currentIndexChanged
 *        Description         :  �޸���̨Ѳ���ٶ�ֵ
 *        Input               :  int index ��̨Ѳ���ٶ�ֵ
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisespeed_currentIndexChanged(int index)
{
	m_ptzcruisespeedindex =index+1;
}


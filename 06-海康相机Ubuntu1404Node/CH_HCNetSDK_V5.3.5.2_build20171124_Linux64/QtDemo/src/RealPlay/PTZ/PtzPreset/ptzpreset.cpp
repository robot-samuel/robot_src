/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   ptzpreset.cpp
 *        Description                   :   Ԥ����̨Ԥ�õ�������ز���
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   none
*************************************************************************/
#include "ptzpreset.h"
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <stdio.h>

/************************************************************************
 *        Function            :  PtzPreset
 *        Description         :  ���캯��
 *        Input               :  int realhandle Ԥ�������,QWidget *parent
 								 QT widget����
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
PtzPreset::PtzPreset(int realhandle,QWidget *parent)
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
	ui.comboBox_ptzpresetpointindex->addItems(stringlistforpreset);
}

/************************************************************************
 *        Function            :  ~PtzPreset
 *        Description         :  ��������
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
PtzPreset::~PtzPreset()
{

}

/************************************************************************
 *        Function            :  on_pushButton_addptzpresetpoint_clicked
 *        Description         :  ������̨Ԥ�õ�
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzPreset::on_pushButton_addptzpresetpoint_clicked()
{
//for(int m_presetpointindex=1;m_presetpointindex<=255;m_presetpointindex++)
//{
 	if (!NET_DVR_PTZPreset(m_rpcurrentrealhandle,SET_PRESET,m_presetpointindex))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
	//break;
	}
	else
	{
		 //QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
              //  tr("m_presetpointindex =%1").arg(m_presetpointindex));
   
	}
//}
}

/************************************************************************
 *        Function            :  on_pushButton_deleteptzpresetpoint_clicked
 *        Description         :  ɾ����̨Ԥ�õ�
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzPreset::on_pushButton_deleteptzpresetpoint_clicked()
{

 	if (!NET_DVR_PTZPreset(m_rpcurrentrealhandle,CLE_PRESET,m_presetpointindex))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  �˳��Ի���
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzPreset::on_pushButton_exit_clicked()
{
	close();

}

/************************************************************************
 *        Function            :  on_comboBox_ptzpresetpointindex_currentIndexChanged
 *        Description         :  �޸���̨Ԥ�õ���
 *        Input               :  int index ��̨Ԥ�õ���
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzPreset::on_comboBox_ptzpresetpointindex_currentIndexChanged(int index)
{
	m_presetpointindex =index+1;
}
	

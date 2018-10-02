/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       deviceattr.cpp
 *      Description     :       �豸����Դ�ļ�
 *      Modification    :       ��
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/
#include "deviceattr.h"

/************************************************************************
 *        Function            :  DeviceAttr
 *        Description         :  �豸���Խ���
 *        Input               :  parent
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
DeviceAttr::DeviceAttr(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.SID_lineEdit->setDisabled(1);
    ui.devType_lineEdit->setDisabled(1);
    ui.McastIP_lineEdit->setDisabled(1);
    ui.channelNum_lineEdit->setDisabled(1);
}

/************************************************************************
 *        Function            :  ~DeviceAttr
 *        Description         :  ��������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
DeviceAttr::~DeviceAttr()
{

}


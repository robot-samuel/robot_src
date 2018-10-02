/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       channelattr.cpp
 *      Description     :       ͨ������Դ�ļ�
 *      Modification    :       ��
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/
#include "channelattr.h"

/************************************************************************
 *        Function            :  ChannelAttr
 *        Description         :  ���캯��
 *        Input               :  parent
 *        Output              :  ��������
 *        Return              :  ��
*************************************************************************/
ChannelAttr::ChannelAttr(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.channNum_lineEdit->setDisabled(1);
    ui.devAddr_lineEdit->setDisabled(1);
    ui.devName_lineEdit->setDisabled(1);
}

/************************************************************************
 *        Function            :  ~ChannelAttr
 *        Description         :  ��������
 *        Input               :  ��
 *        Output              :  �رս���
 *        Return              :  ��
*************************************************************************/
ChannelAttr::~ChannelAttr()
{

}


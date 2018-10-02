/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       channeldata.cpp
 *      Description     :       ͨ��������ز���
 *      Modification    :       none
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 *****************************************************************************************/
#include "channeldata.h"

/************************************************************************
 *        Function            :  ChannelData
 *        Description         :  ���캯��
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
ChannelData::ChannelData()
{
    m_qchannelname = "";
    m_ichannelnum = -1;
	m_irealhandle = -1;
    m_eprotocoltype = TCP;
    m_estreamtype = MAINSTREAM;
}

/************************************************************************
 *        Function            :  ~ChannelData
 *        Description         :  ��������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
ChannelData::~ChannelData()
{

}

/************************************************************************
 *        Function            :  setChannelName
 *        Description         :  ����ͨ������
 *        Input               :  name
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
void ChannelData::setChannelName(QString name)
{
    m_qchannelname = name;
}

/************************************************************************
 *        Function            :  getChannelName
 *        Description         :  ��ȡͨ������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  ͨ������
*************************************************************************/
QString ChannelData::getChannelName()
{
    return m_qchannelname;
}

/************************************************************************
 *        Function            :  setChannelNum
 *        Description         :  ����ͨ����
 *        Input               :  num
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
void ChannelData::setChannelNum(int num)
{
    m_ichannelnum = num;
}

/************************************************************************
 *        Function            :  getChannelNum
 *        Description         :  ��ȡͨ����
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  m_iChannelNum
*************************************************************************/
int ChannelData::getChannelNum()
{
    return m_ichannelnum;
}

/************************************************************************
 *        Function            :  setProtocolType
 *        Description         :  ����Э������
 *        Input               :  type
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
void ChannelData::setProtocolType(PROTOCOL type)
{
    m_eprotocoltype = type;
}

/************************************************************************
 *        Function            :  getProtocolType
 *        Description         :  ��ȡЭ������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  Э������
*************************************************************************/
PROTOCOL ChannelData::getProtocolType()
{
    return m_eprotocoltype;
}

/************************************************************************
 *        Function            :  getProtocolTypeQstring
 *        Description         :  ��ȡЭ������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  �ַ���Э������
*************************************************************************/
QString ChannelData::getProtocolTypeQstring()
{
    QString protocol;
    switch (m_eprotocoltype)
    {
    case TCP:
        protocol="TCP";
        break;
    case UDP:
        protocol="UDP";
        break;
    case MCAST :
        protocol="MCAST";
        break;
    case RTP:
        protocol="RTP";
        break;
    case RTP_RTSP:
        protocol = "RTP/RTSP";
    default :
        protocol = "error";
        break;
    }
    return protocol;
}

/************************************************************************
 *        Function            :  setStreamType
 *        Description         :  ����Э������
 *        Input               :  Э������
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
void ChannelData::setStreamType(STREAMTYPE type)
{
    m_estreamtype = type;
}

/************************************************************************
 *        Function            :  getStreamType
 *        Description         :  ��ȡ��������������������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  ��������
*************************************************************************/
STREAMTYPE ChannelData::getStreamType()
{
    return m_estreamtype;
}

/************************************************************************
 *        Function            :  getStreamTypeQstring
 *        Description         :  ������������������������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  �ַ�����������
*************************************************************************/
QString ChannelData::getStreamTypeQstring()
{
    QString type;

    switch (m_estreamtype)
    {
    case MAINSTREAM:
        type="MAINSTREAM";
        break;
    case SUBSTREAM:
        type="SUBSTREAM";
        break;
    default :
        type = "error";
        break;
    }
    return type;
}


void ChannelData::setLinkMode()
{
    switch (m_estreamtype)
    {
    case MAINSTREAM:
            switch (m_eprotocoltype)
            {
            case TCP:
                m_ilinkmode =0x0;
                break;
            case UDP:
                m_ilinkmode=0x1;
                break;
            case MCAST :
                m_ilinkmode=0x2;
                break;
            case RTP:
                m_ilinkmode=0x3;
                break;
            case RTP_RTSP:
                m_ilinkmode=0x4;
                break;
            default :
                m_ilinkmode=0x0;
                break;
            }
            break;
    case SUBSTREAM:
            switch (m_eprotocoltype)
            {
            case TCP:
                m_ilinkmode =0x80000000;
                break;
            case UDP:
                m_ilinkmode=0x80000001;
                break;
            case MCAST :
                m_ilinkmode=0x80000002;
                break;
            case RTP:
                m_ilinkmode=0x80000003;
                break;
            case RTP_RTSP:
                m_ilinkmode=0x80000004;
                break;
            default :
                m_ilinkmode=0x80000000;
                break;
            }
            break;
        break;
    default :
        break;
    }
}

int ChannelData::getLinkMode()
{
    setLinkMode();
    return m_ilinkmode;
}


void ChannelData::setRealhandle(int num)
{
    m_irealhandle = num;
}

int ChannelData::getRealhandle()
{
    return m_irealhandle;
}
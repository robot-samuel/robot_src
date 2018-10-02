 /***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       channeldata.h
 *      Description     :       ͨ��������Ϣ
 *      Modification    :       �洢ͨ����ص���Ч���ݣ����ڴ�ȡ�ļ��ͽ�����ʾ��ʵʱ�޸�ͨ����Ϣ
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       panyd@hikvision.com  wanggp@hikvision.com
 *****************************************************************************************/
#ifndef CHANNELDATA_H_
#define CHANNELDATA_H_


#include <QString>
#include "devicedata.h"

enum PROTOCOL{ TCP = 0, UDP, MCAST, RTP, RTP_RTSP};
enum STREAMTYPE{ MAINSTREAM = 0, SUBSTREAM};


class ChannelData
{

public:
    friend class DeviceData;
    friend class QtClientDemo;
	friend class RealPlay;
    friend class PlayBack;
    ChannelData();
    ~ChannelData();

    void setChannelName(QString name);
    QString getChannelName();

    void setChannelNum(int num);
    int getChannelNum();

    void setProtocolType(PROTOCOL type);
    PROTOCOL getProtocolType();
    QString getProtocolTypeQstring();

    void setStreamType(STREAMTYPE type);
    STREAMTYPE getStreamType();
    QString getStreamTypeQstring();

    void setLinkMode();
    int getLinkMode();

	void setRealhandle(int num);
	int getRealhandle();


private:
    //ͨ������
    QString m_qchannelname; 
    //ͨ����
    int m_ichannelnum;  
    //����Э�飬��TCP,UDP���ಥ����RTP��
    PROTOCOL m_eprotocoltype;
    //�������ͣ�����������������
    STREAMTYPE m_estreamtype;  
    int m_ilinkmode;
	int m_irealhandle;
};
#endif /* CHANNELDATA_H_ */


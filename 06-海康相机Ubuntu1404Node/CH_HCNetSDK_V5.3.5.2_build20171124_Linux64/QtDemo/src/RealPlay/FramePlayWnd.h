/*
 * Copyright(C) 2010,Custom Co., Ltd 
 *    FileName: FramePlayWnd.h
 * Description: 
 *     Version: 1.0
 *      Author: panyadong
 * Create Date: 2010-6-24
 * Modification History��
 */
#ifndef FRAMEPLAYWND_H
#define FRAMEPLAYWND_H

#include <QFrame>
#include "ui_FramePlayWnd.h"
#include "DemoPublic.h"

//��������
typedef struct tagPlayRect
{
    int iX;
    int iY;
    int iWidth;
    int iHeight;
}CLIENT_PLAY_RECT, *LPCLIENT_PLAY_RECT;

//�ṩ���Ŵ��ڻ�����
class CFramePlayWnd : public QFrame
{
    Q_OBJECT

public:
    CFramePlayWnd(QWidget *parent = 0);
    ~CFramePlayWnd();

    WId GetPlayWndId();
    int GetPlayRect(CLIENT_PLAY_RECT* pPlayRect);

public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    //sigle click signal
    void SingleClickSig();

private:
    Ui::CFramePlayWndClass ui;

    QFrame *m_pframePlay; //windows����Ϊ���Ŵ���

    int m_iBorderWidth;
    int m_iBorderHeiht;

    int m_bSelected;
};

#endif // FRAMEPLAYWND_H

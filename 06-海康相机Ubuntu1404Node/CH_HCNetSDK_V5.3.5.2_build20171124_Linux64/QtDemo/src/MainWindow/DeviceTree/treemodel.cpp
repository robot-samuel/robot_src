/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :   treemodel.cpp
 *      Description     :   ������ʾģʽ����treeview���ʹ��
 *      Modification    :   ��  
 *      Version         :   V1.0.0
 *      Time            :   2009-10,11
 *      Author          :   wanggp@hikvision.com
 **************************************************************************************/
#include <QIcon>
#include <QtGui>
#include "treeitem.h"
#include "treemodel.h"
 
/************************************************************************
 *        Function            :  TreeModel
 *        Description         :  ���캯��
 *        Input               :  data parent
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
 TreeModel::TreeModel(const QString  &data, QObject *parent)
     : QStandardItemModel(parent)
 {
     QList<QVariant> rootData;
     rootData << "DeviceTree";
     rootItem = new TreeItem(rootData);
     setupModelData(data.split(QString("\n")), rootItem);
 }
 
/************************************************************************
 *        Function            :  ~TreeModel
 *        Description         :  ��������
 *        Input               :  ��
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
 TreeModel::~TreeModel()
 {
     delete rootItem;
 }

/************************************************************************
 *        Function            :  setupModelData
 *        Description         :  ���������б�
 *        Input               :  lines  �豸��ͨ����Ϣ
 								 parent �豸�ڵ�
 *        Output              :  ��
 *        Return              :  ��
*************************************************************************/
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
 {
    QList<TreeItem*> parents;
    parents << parent;
    QStandardItem *parentItem = invisibleRootItem();
 
	//QStandardItem *devicetreenode = new QStandardItem(QString("HIKVISION"));
	//devicetreenode->setEditable(0);
    
	
    //add by pyd
    QStandardItem *pDeviceRootItem = new QStandardItem(QString("device tree(right click to add device)"));
    //devicetreenode->appendRow(pDeviceRootItem);
    parentItem->appendRow(pDeviceRootItem);
	pDeviceRootItem->setIcon(QIcon(":/images/tree.bmp"));
	pDeviceRootItem->setEditable(0);
 
	QList<TreeItem*> parents1;
	QString deviceBegin("<device>");
	QString deviceEnd("</device>");
	QString channelBegin("<channel>");
	QString channelEnd("</channel>");

	QStandardItem *pDeviceItem = NULL;
	QStandardItem *pChannelItem = NULL;
    for ( QStringList::const_iterator it = lines.begin(); it != lines.end(); ++it )
    {   
     //�豸�ڵ�
        if ((*it)==deviceBegin)
        {
			++it;
			pDeviceItem = new QStandardItem(*it);
			pDeviceRootItem->appendRow(pDeviceItem);
			pDeviceItem->setIcon(QIcon(":/images/logout.bmp"));
			pDeviceItem->setEditable(0);
        }
 
        //ͨ����Ϣ
        QString chn =channelBegin;
        if ((*it)==chn)
        {
			++it;
			pChannelItem = new QStandardItem(*it);
			pDeviceItem->appendRow(pChannelItem);
			pChannelItem->setIcon(QIcon(":/images/camera.bmp"));
			pChannelItem->setEditable(0);
        }
    }
}


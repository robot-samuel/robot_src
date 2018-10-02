/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       treeitem.h
 *      Description     :       ������ʾ�ڵ�ͷ�ļ�
 *      Modification    :       ��
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/


#include <QStringList>
#include <QMessageBox>
#include "treeitem.h"

/************************************************************************
*        Function            :  TreeItem
*        Description         :  ���캯��
*        Input               :  data,parent
*        Output              :  ��
*        Return              :  ��
*************************************************************************/
TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}

/************************************************************************
*        Function            :  ~TreeItem
*        Description         :  ��������
*        Input               :  ��
*        Output              :  ��
*        Return              :  ��
*************************************************************************/
TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

/************************************************************************
*        Function            :  appendChild
*        Description         :  ��ӽڵ�
*        Input               :  item
*        Output              :  ��
*        Return              :  ��
*************************************************************************/
void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}

/************************************************************************
*        Function            :  child
*        Description         :  ȡ���ӽ��
*        Input               :  row
*        Output              :  ��
*        Return              :  childItems
*************************************************************************/
TreeItem *TreeItem::child(int row)
{
    return childItems.value(row);
}

/************************************************************************
*        Function            :  childCount
*        Description         :  �ӽڵ����
*        Input               :  ��
*        Output              :  ��
*        Return              :  ��
*************************************************************************/
int TreeItem::childCount() const
{
    return childItems.count();
}

/************************************************************************
*        Function            :  columnCount
*        Description         :  ��������
*        Input               :  ��
*        Output              :  ��
*        Return              :  �ӽڵ���ܸ���
*************************************************************************/
int TreeItem::columnCount() const
{
    return itemData.count();
}

/************************************************************************
*        Function            :  data
*        Description         :  ĳһ������
*        Input               :  column
*        Output              :  ��
*        Return              :  itemData.value
*************************************************************************/ 
QVariant TreeItem::data(int column) const
{
	QString data =itemData.value(column).toString();
	//QMessageBox::information(this,tr("TreeItem::data"),tr("data=%1").arg(data.toLatin1().data()));
	//printf("%s\n",data.toLatin1().data());
    return itemData.value(column);
}

bool TreeItem::setData(const QVariant &value)
{
	//printf("1232434534555=%s\n",value.toString().toLatin1().data());
  	//itemData.clear();
	//itemData.append(value);
	//printf("1232434534555=%s\n",value.toString().toLatin1().data());
		
	 	return true;
     
    // return false;


}

/************************************************************************
*        Function            :  parent
*        Description         :  ȡ�丸�ڵ�
*        Input               :  ��
*        Output              :  ��
*        Return              :  parentItem
*************************************************************************/
TreeItem *TreeItem::parent()
{
    return parentItem;
}

/************************************************************************
*        Function            :  row
*        Description         :  ͳ������
*        Input               :  ��
*        Output              :  ��
*        Return              :  0 
*************************************************************************/
int TreeItem::row() const
{
    if (parentItem)
    {
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}


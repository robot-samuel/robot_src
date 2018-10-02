/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       treeitem.h
 *      Description     :       ������ʾ�ڵ�ͷ�ļ�
 *      Modification    :       ��
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/

#ifndef TREEITEM_H_
#define TREEITEM_H_

#include <QList>
#include <QVariant>

class TreeItem
{
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
	bool setData(const QVariant &value);
    int row() const;
    TreeItem *parent();

private:
    //ÿ���ڵ㶼����Ϊ���ڵ�����������ӽڵ����
    QList<TreeItem*> childItems;
    //�ڵ����ݣ�������һ���豸���͵ڶ����豸���е�����ͨ�������Ϣ
    //QVariant����ת��Ϊ����ṹ����������
    QList<QVariant> itemData;
    TreeItem *parentItem;
};
#endif /* TREEITEM_H_ */


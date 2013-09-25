#ifndef SOFTALLKINDS_H
#define SOFTALLKINDS_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include "../Common/SqliteDb.h"

#include "SoftAllKindItem.h"
#include "SoftSortItem.h"
class SoftAllKindList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftAllKindList(QWidget *parent = 0);
    void Fill_Item_List(QStringList list_0,QStringList list_1,QList<int> list_2 );
    void add_tree_data();
    void add_branch_data();
signals:
    void currentRowChanged();
    
public slots:

public:
    QListWidget *list_classify;
    QListWidget *list_handpick;

    CSQLiteDb m_SQLiteDb;

    QStringList icon;
    QStringList text;
    QList<int> num;

    int count_handpick;
    int count_classify;
    QString lab_text;
    QStringList list_name;
    QStringList list_number;
    QList<int> list_type;
    QList<SoftAllKindItem *> *item_list;
    QList<QListWidgetItem *> *twi_list;
};

#endif // SOFTALLKINDS_H

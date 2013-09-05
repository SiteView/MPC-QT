#ifndef SOFTUNLOADLIST_H
#define SOFTUNLOADLIST_H

#include <QWidget>

#include <QListWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QStringList>
#include "SoftUnloadItem.h"
#include "../Common/SqliteDb.h"

class SoftUnloadList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUnloadList(QWidget *parent = 0);
    void DiffSelect(int);

signals:
    
public slots:

public:

    QStringList list_0;
    QStringList list_1;
    QStringList list_2;
    QList<qint64 > list_3;
    QStringList list_4;
    QStringList list_5;
    QStringList list_6;

    CSQLiteDb m_SQLiteDb;
    QListWidget *UnloadList;
    QListWidget *UnloadList2;
    QWidget *bottom;

    QList<SoftUnloadItem *> *item_list ;
    QList<QListWidgetItem*> *twi_list;

//    SoftUnloadItem *ani;
    QVBoxLayout *center_layout;
    QHBoxLayout *bottom_layout;
    int page_count; //总页数
    int page_count_point; //最后一页的显示个数
    int current_page; //当前为第几页

    QPushButton *first_page_button;
    QPushButton *previous_page_button;
    QPushButton *next_page_button;
    QPushButton *last_page_button;
    QString search_text;
    bool empty;

};

#endif // SOFTUNLOADLIST_H

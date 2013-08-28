#ifndef TESTUNLOADLIST_H
#define TESTUNLOADLIST_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include "testunloaditem.h"
#include "../Common/SqliteDb.h"

class TestUnloadList : public QWidget
{
    Q_OBJECT
public:
    explicit TestUnloadList(QWidget *parent = 0);
    void initCenter();
    void initBottom();

public:
    QVBoxLayout *center_layout;
    QHBoxLayout *bottom_layout;

    TestUnloadItem *item1;
    TestUnloadItem *item2;
    TestUnloadItem *item3;
    TestUnloadItem *item4;
    TestUnloadItem *item5;
    TestUnloadItem *item6;
    TestUnloadItem *item7;
    TestUnloadItem *item8;
    TestUnloadItem *item9;
    TestUnloadItem *item0;

    QPushButton *first_page_button;
    QPushButton *previous_page_button;
    QPushButton *next_page_button;
    QPushButton *last_page_button;

    QStringList name_list;
    QStringList time_list;
    QStringList size_list;
    QStringList detail_list;
    int page_count; //总页数
    int page_count_point; //最后一页的显示个数
    int current_page; //当前为第几页

    CSQLiteDb m_SQLiteDb;
    QWidget *WidgetContents;
    QScrollArea *area;


signals:
    
public slots:
    void showPage(QString page_count);

};

#endif // TESTUNLOADLIST_H

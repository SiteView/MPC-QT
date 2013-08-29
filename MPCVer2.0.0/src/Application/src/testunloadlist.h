#ifndef TESTUNLOADLIST_H
#define TESTUNLOADLIST_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QList>

#include "testunloaditem.h"
#include "../Common/SqliteDb.h"
#include "SoftUnloadItem.h"

class TestUnloadList : public QWidget
{
    Q_OBJECT
public:
    explicit TestUnloadList(QWidget *parent = 0);
    void initCenter();
    void initBottom();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QPoint move_point; //移动的距离
    bool mouse_press; //按下鼠标左键

public:
    QVBoxLayout *center_layout;
    QHBoxLayout *bottom_layout;

    QList<SoftUnloadItem *> *item_list ;

    //    TestUnloadItem *item;

    QPushButton *first_page_button;
    QPushButton *previous_page_button;
    QPushButton *next_page_button;
    QPushButton *last_page_button;

    QStringList icon_list;
    QStringList softname_list;
    QStringList softdetail_list;
    QStringList size_list;
    QStringList setuptime_list;
    QStringList progress_list;
    QStringList uninstallString_list;
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

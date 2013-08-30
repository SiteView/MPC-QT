#include <QListWidgetItem>
#include <QSignalMapper>
#include <QPainter>
#include "SoftUnloadList.h"
#include "mainwindow.h"

SoftUnloadList::SoftUnloadList(QWidget *parent) :
    QWidget(parent)
{
    UnloadList  = new QListWidget(this);
    UnloadList->resize(920,500);
    UnloadList->setFocusPolicy(Qt::NoFocus);
    bottom=new QWidget();

//    initBottom();
//    this->showPage(QString::number(current_page, 10));
}

void SoftUnloadList::DiffSelect(int cmd){

    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    switch(cmd)
    {
    case SELECT_ALL:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_NAME_UP:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by DisplayName desc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_NAME_DOWN:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by DisplayName asc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_SIZE_UP:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by EstimatedSize desc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_SIZE_DOWN:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by EstimatedSize asc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_TIME_UP:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by SetupTime desc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_TIME_DOWN:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by SetupTime asc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_PATH_UP:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by InstallLocation desc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SELECT_PATH_DOWN:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor order by InstallLocation asc  ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;

    }
    while ( SQLiteQuery.next() )
    {
        QVariant val0 = SQLiteQuery.value(0);
        QVariant val1 = SQLiteQuery.value(1);
        QVariant val2 = SQLiteQuery.value(2);
        QVariant val3 = SQLiteQuery.value(3);
        QVariant val4 = SQLiteQuery.value(4);
        QVariant val5 = SQLiteQuery.value(5);
        QVariant val6 = SQLiteQuery.value(6);

        QString pahtstr0 = val0.toString();
        QString pahtstr1 = val1.toString();
        QString pahtstr2 = val2.toString();
        QString pahtstr3 = val3.toString();
        QString pahtstr4 = val4.toString();
        QString pahtstr5 = val5.toString();
        QString pahtstr6 = val6.toString();
        pahtstr0.replace("\\","/");

        list_0<<pahtstr0;
        list_1<<pahtstr1;
        list_2<<pahtstr2;
        list_3<<pahtstr3;
        list_4<<pahtstr4;
        list_5<<pahtstr5;
        list_6<<pahtstr6;
    }
    SQLiteQuery.finish();

    item_list =new QList<SoftUnloadItem *>();
    twi_list =new QList<QListWidgetItem*>();
    for(int i=0; i<list_0.size(); i++)
    {
        SoftUnloadItem *item = new SoftUnloadItem();
        item_list->push_back(item);
        item_list->at(i)->takeText(list_0.at(i),list_1.at(i),list_2.at(i),list_3.at(i),
                                   list_4.at(i),list_5.at(i),list_6.at(i));
        QListWidgetItem *twi = new QListWidgetItem();
        twi->setSizeHint(QSize(400,59));
        twi_list->push_back(twi);
    }
    for(int k=0; k<list_0.size(); k++)
    {
        UnloadList->addItem(twi_list->at(k));
        UnloadList->setItemWidget(twi_list->at(k),item_list->at(k));
    }
}

void SoftUnloadList::initBottom()
{
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    QList<QPushButton *> *button_list = new QList<QPushButton *>();
    for(int i=0; i<page_count; i++)
    {
        QPushButton *page_button = new QPushButton();
        page_button->setFixedWidth(20);
        page_button->setText(QString::number(i+1, 10));
        page_button->setStyleSheet("color:rgb(0, 120, 230); background:transparent;");
        page_button->setCursor(Qt::PointingHandCursor);
        connect(page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
        signal_mapper->setMapping(page_button, page_button->text());
        button_list->push_back(page_button);
    }

    first_page_button = new QPushButton();
    previous_page_button = new QPushButton();
    next_page_button = new QPushButton();
    last_page_button = new QPushButton();
    first_page_button->setFixedWidth(50);
    previous_page_button->setFixedWidth(50);
    next_page_button->setFixedWidth(50);
    last_page_button->setFixedWidth(50);
    first_page_button->setCursor(Qt::PointingHandCursor);
    previous_page_button->setCursor(Qt::PointingHandCursor);
    next_page_button->setCursor(Qt::PointingHandCursor);
    last_page_button->setCursor(Qt::PointingHandCursor);
    first_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");
    previous_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");
    next_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");
    last_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");

    first_page_button->setText(tr("first"));
    previous_page_button->setText(tr("previous"));
    next_page_button->setText(tr("next"));
    last_page_button->setText(tr("last"));

    connect(first_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(previous_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(next_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(last_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    signal_mapper->setMapping(first_page_button, "first");
    signal_mapper->setMapping(previous_page_button, "previous");
    signal_mapper->setMapping(next_page_button, "next");
    signal_mapper->setMapping(last_page_button, "last");
    connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(showPage(QString)));

    bottom_layout = new QHBoxLayout();
    bottom_layout->addStretch();
    bottom_layout->addWidget(first_page_button, 0, Qt::AlignVCenter);
    bottom_layout->addWidget(previous_page_button, 0, Qt::AlignVCenter);
    for(int i=0; i<button_list->count(); i++)
    {
        QPushButton *page_button = button_list->at(i);
        bottom_layout->addWidget(page_button, 0, Qt::AlignVCenter);
    }
    bottom_layout->addWidget(next_page_button, 0, Qt::AlignVCenter);
    bottom_layout->addWidget(last_page_button, 0, Qt::AlignVCenter);
    bottom_layout->addStretch();
    bottom_layout->setSpacing(2);
    bottom_layout->setContentsMargins(0, 10, 0, 0);
}
void SoftUnloadList::showPage(QString current_skin)
{
    if(current_skin == "first")
    {
        current_page = 1;
    }
    else if(current_skin == "previous")
    {
        if(current_page > 2)
        {
            current_page = current_page - 1;
        }
        else
        {
            current_page=1;
        }
    }
    else if(current_skin == "next")
    {
        if(current_page < page_count)
        {
            current_page = current_page + 1;
        }
    }
    else if(current_skin == "last")
    {
        current_page = page_count;
    }
    else
    {
        bool ok;
        current_page = current_skin.toInt(&ok, 10);
    }

    if(current_page == 1)
    {
        next_page_button->show();
        last_page_button->show();
        first_page_button->hide();
        previous_page_button->hide();
    }
    else if(current_page == page_count)
    {
        first_page_button->show();
        previous_page_button->show();
        next_page_button->hide();
        last_page_button->hide();
    }
    else
    {
        first_page_button->hide();
        previous_page_button->show();
        next_page_button->show();
        last_page_button->hide();
    }
}

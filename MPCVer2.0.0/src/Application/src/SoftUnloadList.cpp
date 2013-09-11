#include <QListWidgetItem>
#include <QSignalMapper>
#include <QPainter>
#include "SoftUnloadList.h"
#include "mainwindow.h"

SoftUnloadList::SoftUnloadList(QWidget *parent) :
    QWidget(parent)
{
    empty=true;
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
    case SELECT_SEARCH:
        if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor where DisplayName like '%"+search_text+"%';" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }

        break;

    }
    while ( SQLiteQuery.next() )
    {
        empty=false;

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
        qint64 pahtstr3 = val3.toLongLong();
        QString pahtstr4 = val4.toString();
        QString pahtstr5 = val5.toString();
        QString pahtstr6 = val6.toString();
        pahtstr0.replace("\\","/");
        QStringList str=pahtstr4.split("T");
        list_0<<pahtstr0;
        list_1<<pahtstr1;
        list_2<<pahtstr2;
        list_3<<pahtstr3;
        list_4<<str.at(0);
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



#include <QListWidgetItem>

#include "softdownloadlist.h"
#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"
#include "mainwindow.h"
#include<QUrl>

SoftDownloadList::SoftDownloadList(QWidget *parent) :
    QWidget(parent)
{
    empty=true;
    DownloadList  = new QListWidget(this);
    DownloadList->setFocusPolicy(Qt::NoFocus);
    DownloadList->resize(760,510);
    DownloadList->setStyleSheet("QListView::item:selected{background-color:rgb(106,189,246)}");
}

void SoftDownloadList::selectDifType(int cmd)
{
    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB());
    switch(cmd)
    {
    case ALL:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case ONE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=1;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case TWO:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=2;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case THREE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=3;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case FOUR:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl from ServerAppInfo where type=4;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case FIVE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=5;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SIX:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=6;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SEVEN:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=7;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case EIGHT:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=8;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case NINE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=9;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case TEN:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=10;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case ELEVEN:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=11;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case TWELVE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where type=12;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SEARCH:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl  from ServerAppInfo where DisplayName like '%"+search_text+"%' ;" ) )
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
        QString pahtstr3 = val3.toString();
        qint64 pahtstr4 = val4.toLongLong();
        QString pahtstr5 = val5.toString();
        QString pahtstr6 = val6.toString();

        list_0<<pahtstr0;
        list_1<<pahtstr1;
        list_2<<pahtstr2;
        list_3<<pahtstr3;
        list_4<<pahtstr4;
        list_5<<pahtstr5;
        list_6<<pahtstr6;
    }
    SQLiteQuery.finish();

    item_list =new QList<SoftDownloadItem *>();
    twi_list =new QList<QListWidgetItem*>();
    for(int i=0; i<list_0.size(); i++)
    {
        SoftDownloadItem *item = new SoftDownloadItem();
        item_list->push_back(item);
        item->takeText(list_0.at(i),list_1.at(i),list_4.at(i),list_2.at(i),
                                   list_5.at(i));
        QListWidgetItem *twi = new QListWidgetItem();
        twi->setSizeHint(QSize(400,59));
        twi_list->push_back(twi);
    }
    for(int k=0; k<list_0.size(); k++)
    {
        DownloadList->addItem(twi_list->at(k));
        DownloadList->setItemWidget(twi_list->at(k),item_list->at(k));
    }
}

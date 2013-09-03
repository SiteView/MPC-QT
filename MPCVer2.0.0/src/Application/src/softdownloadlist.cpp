
#include <QListWidgetItem>

#include "softdownloadlist.h"
#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"
#include "mainwindow.h"
#include<QUrl>

SoftDownloadList::SoftDownloadList(QWidget *parent) :
    QWidget(parent)
{
    DownloadList  = new QListWidget(this);
    DownloadList->setFocusPolicy(Qt::NoFocus);
    DownloadList->resize(760,510);
    DownloadList->setStyleSheet("QListView::item:selected{background-color:rgb(106,189,246)}");

}

void SoftDownloadList::selectDifType(int cmd){

    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB());
    switch(cmd)
    {
    case ALL:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo ;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case ONE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=1;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case TWO:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=2;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case THREE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=3;" ) )
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
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=5;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SIX:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=6;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case SEVEN:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=7;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case EIGHT:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=8;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case NINE:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=9;" ) )
        {
            qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
        }
        break;
    case TEN:
        if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=10;" ) )
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

        list_0<<pahtstr0;
        list_1<<pahtstr1;
        list_2<<pahtstr2;
        list_3<<pahtstr3;
        list_4<<pahtstr4;
        list_5<<pahtstr5;
        list_6<<pahtstr6;

//        SoftDownloadItem *ani=new SoftDownloadItem (DownloadList);

//        ani->but_icon->setStyleSheet("border-image:url(./icons/"+pahtstr0+".ico)");
//        ani->but_softname->setText(pahtstr0);

//        ani->lab_softdetail->setText(pahtstr1+"...");

//        ani->lab_softdetail->setToolTip(pahtstr1);
//        ani->lab_upnum->setText(pahtstr3);
//        ani->lab_upnum_2->setText(pahtstr3);
//        ani->lab_size->setText(pahtstr4);
//        ani->lab_size_2->setText(pahtstr4);
//        ani->download->setText("download");
//        ani->urlprogram=pahtstr5;
//        //        ani->lab_prompt->setText(pahtstr2);
//        QUrl url = QUrl::fromEncoded(pahtstr5.toUtf8());
//        QString path_file = url.toString();
//        QStringList str=path_file.split("/");
//        int i=str.count();
//        ani->exename=str.at(i-1);

//        QListWidgetItem *twi = new QListWidgetItem(0);
//        twi->setSizeHint(QSize(400,59));
//        DownloadList->addItem(twi);
//        DownloadList->setItemWidget(twi,ani);
//        qDebug()<<"pahtstr2=="<<pahtstr2;
//        select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl ,Mark from ServerAppInfo where type=10;
    }
    SQLiteQuery.finish();

    item_list =new QList<SoftDownloadItem *>();
    twi_list =new QList<QListWidgetItem*>();
    for(int i=0; i<list_0.size(); i++)
    {
        SoftDownloadItem *item = new SoftDownloadItem();
        item_list->push_back(item);
        item_list->at(i)->takeText(list_0.at(i),list_1.at(i),list_4.at(i),list_2.at(i),
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

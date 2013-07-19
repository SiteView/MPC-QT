#include "softdownloadlist.h"
#include <QListWidgetItem>
#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"
SoftDownloadList::SoftDownloadList(QWidget *parent) :
    QWidget(parent)
{
    DownloadList  = new QListWidget(this);
    DownloadList->resize(805,420);
    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    if ( !SQLiteQuery.exec( "select DisplayName,Detailtext,ServerVersion,ResetServerVerSion,Size ,Downloadurl from ServerAppInfo ;" ) )
    {
        qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
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

        SoftDownloadItem *ani=new SoftDownloadItem (this);
        ani->icon->setText(pahtstr2);
        ani->softname->setText(pahtstr0);
        ani->softdetail->setText(pahtstr1);
        ani->setuptime->setText(pahtstr3);
        ani->progress->setText(pahtstr5);
        ani->size->setText(pahtstr4);
        ani->program=pahtstr6;

        QListWidgetItem *twi = new QListWidgetItem(0);
        twi->setSizeHint(QSize(400,59));
        DownloadList->addItem(twi);
        DownloadList->setItemWidget(twi,ani);
        DownloadList->setAlternatingRowColors(true);
    }
    SQLiteQuery.finish();
}


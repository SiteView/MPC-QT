
#include <QListWidgetItem>

#include "softdownloadlist.h"
#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"

#include "./curldownloadmanager.h"

SoftDownloadList::SoftDownloadList(QWidget *parent) :
    QWidget(parent)
{
    //    downloader    = new  CURLDownloadManager(this);
    DownloadList  = new QListWidget(this);

    downloader=new CURLDownloadManager(this);

    DownloadList->resize(760,510);
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

        QString pahtstr0 = val0.toString();
        QString pahtstr1 = val1.toString();
        QString pahtstr2 = val2.toString();
        QString pahtstr3 = val3.toString();
        QString pahtstr4 = val4.toString();
        QString pahtstr5 = val5.toString();


        ani=new SoftDownloadItem (this);

        ani->but_icon->setText(pahtstr2);
        ani->but_softname->setText(pahtstr0);        
        QString lab_softdetail2;
        if(pahtstr1.count()<15)
        {
            ani->lab_softdetail->setText(pahtstr1+"...");
        }
        else
        {
            for(int k=0;k<15;k++)
            {
                lab_softdetail2+=pahtstr1.at(k);
            }
            ani->lab_softdetail->setText(lab_softdetail2+"...");
        }
        ani->lab_softdetail->setToolTip(pahtstr1);
        ani->lab_size->setText(pahtstr4);
        ani->download->setText("download");
        ani->urlprogram=pahtstr5;

        QListWidgetItem *twi = new QListWidgetItem(0);
        twi->setSizeHint(QSize(400,59));
        DownloadList->addItem(twi);
        DownloadList->setItemWidget(twi,ani);



    }
    SQLiteQuery.finish();
    //    DownloadThread();
}
void SoftDownloadList::DownloadThread(){

    qDebug()<<ani->urlprogram<<"+++urlprogram";
    //    downloader=new CURLDownloadManager(this);
    downloader->start();
//    downloader->setUrl("http://dl.google.com/pinyin/v2/GooglePinyinInstaller.exe");
    downloader->setUrl(ani->urlprogram);
    downloader->setSavefileName("c:/GooglePinyinInstaller.exe");
    downloader->ready(true);

}


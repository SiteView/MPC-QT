
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
    //    filled=false;
    num=0;
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
    item_qlist =new QQueue<SoftDownloadItem *>();

    for(int i=0; i<list_0.size(); i++)
    {
        SoftDownloadItem *item = new SoftDownloadItem();//构造不同的对象
        item_list->push_back(item);
        item->takeText(list_0.at(i),list_1.at(i),list_4.at(i),list_2.at(i),
                       list_5.at(i));
        QListWidgetItem *twi = new QListWidgetItem();
        twi->setSizeHint(QSize(400,59));
        twi_list->push_back(twi);
        connect(item,SIGNAL(DownisBusy(QString)),this,SLOT(FillQueue(QString)));//获得点击过的下载对象的地址
        connect(item,SIGNAL(DownFinished()),this,SLOT(AddItemQueue()));//获得点击过的下载对象的地址

    }
    for(int k=0; k<list_0.size(); k++)
    {
        DownloadList->addItem(twi_list->at(k));
        DownloadList->setItemWidget(twi_list->at(k),item_list->at(k));
    }
}
void SoftDownloadList::FillQueue(QString Qurl)//int strcmp(const char *string1, const char *string2);
{
    if(queue.size()==0)
    {
        queue.append(Qurl);
        SoftDownloadItem* pDlg = qobject_cast<SoftDownloadItem*>(QObject::sender());//获得发送信号的源对象
        item_qlist->append(pDlg);
    }
    else if(queue.size()>0)
    {
        QList<int> result_list;
        int  product;
        for(int i=0;i<queue.size();i++)//将新获得的对象和以保存的逐一比较
        {
            int result = QString::compare(queue.at(i), Qurl, Qt::CaseInsensitive);
            result_list.append(result);
        }
        for(int i=0;i<result_list.size();i++)//将比较的结果相乘
        {
            product*=result_list.at(i);
        }
        if(product!=0)//如果不存在相同的对象就保存
        {
            queue.append(Qurl);
            SoftDownloadItem* pDlg = qobject_cast<SoftDownloadItem*>(QObject::sender());//获得发送信号的源对象
            item_qlist->append(pDlg);
        }
    }
}

void SoftDownloadList::AddItemQueue()
{
    if(num<item_qlist->size())
    {
        item_qlist->at(num)->on_download_clicked();
//        if(num<item_qlist->size()-1)
//        {
//            item_qlist->at(num+1)->download->setText("waiting");
//        }
        num++;
    }
}

#include <QUrl>
#include <QFileInfo>
#include <QCoreApplication>
#include <QListWidgetItem>
#include "SoftUpgradeItem.h"
#include "../Common/SqliteDb.h"

#include "SoftUpgradeList.h"
SoftUpgradeList::SoftUpgradeList(QWidget *parent) :
    QWidget(parent)
{
    list_softupgrade  = new QListWidget(this);
    list_softupgrade->resize(920,520);
    list_softupgrade->setFocusPolicy(Qt::NoFocus);
}

void SoftUpgradeList::selectDifType()
{
    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    if ( !SQLiteQuery.exec( "select a.[DisplayIcon] ,a.[DisplayName] ,b.[Detailtext] ,b.[ServerVersion] ,b.[ResetServerVerSion] ,b.[Size] ,b.[Downloadurl] from LocalAppInfor a , ServerAppInfo b  where a.[DisplayName] = b.[DisplayName] ;" ) )
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
        qint64 pahtstr5 = val5.toLongLong();
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

    item_list =new QList<SoftUpgradeItem *>();
    twi_list  =new QList<QListWidgetItem *>();
    for(int i=0; i<list_0.size(); i++)
    {
        SoftUpgradeItem *item = new SoftUpgradeItem();
        item_list->push_back(item);
        item->takeText(list_0.at(i),list_1.at(i),list_2.at(i),list_3.at(i),list_4.at(i),
                                   list_5.at(i),list_6.at(i));
        QListWidgetItem *twi = new QListWidgetItem();
        twi->setSizeHint(QSize(400,59));
        twi_list->push_back(twi);
    }
    for(int k=0; k<list_0.size(); k++)
    {
        list_softupgrade->addItem(twi_list->at(k));
        list_softupgrade->setItemWidget(twi_list->at(k),item_list->at(k));
    }
}

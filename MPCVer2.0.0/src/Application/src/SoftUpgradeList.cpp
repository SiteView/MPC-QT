#include <QListWidgetItem>
#include "../Common/SqliteDb.h"

#include "SoftUpgradeList.h"
#include "SoftUpgradeItem.h"
SoftUpgradeList::SoftUpgradeList(QWidget *parent) :
    QWidget(parent)
{
    list_softupgrade  = new QListWidget(this);
    list_softupgrade->resize(920,520);
    list_softupgrade->setFocusPolicy(Qt::NoFocus);

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
        QString pahtstr5 = val5.toString();
        QString pahtstr6 = val6.toString();

        SoftUpgradeItem *ani=new SoftUpgradeItem(list_softupgrade);
        ani->icon->setStyleSheet("border-image:url("+pahtstr1+"); background:transparent;");
        ani->softname->setText(pahtstr1);
        ani->softdetaile->setText(pahtstr2);
        ani->but_more->setText("more");
        ani->but_more->setStyleSheet("color:lightblue;background:transparent;");
        ani->old_versions->setText(pahtstr3);
        ani->new_versions->setText(pahtstr4);
        ani->size->setText(pahtstr5);
        ani->but_upgrade->setText("upgrade");
        QListWidgetItem *twi = new QListWidgetItem(0);
        twi->setSizeHint(QSize(400,60));
        list_softupgrade->addItem(twi);
        list_softupgrade->setItemWidget(twi,ani);
    }
    SQLiteQuery.finish();


}


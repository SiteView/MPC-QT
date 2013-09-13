#include <QListWidgetItem>
#include <QUrl>
#include <QFileInfo>
#include <QCoreApplication>
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
        qint64 pahtstr5 = val5.toLongLong();
        QString pahtstr6 = val6.toString();

        SoftUpgradeItem *ani=new SoftUpgradeItem(list_softupgrade);
        //        ani->icon->setStyleSheet("border-image:url(./icons/"+pahtstr1+".ico);");
        QString filename = QCoreApplication::applicationDirPath()+QString("/icons/")+pahtstr1+QString(".ico");
        QFileInfo iconfile(filename);
        if(iconfile.exists())
        {

            ani->icon->setStyleSheet("border-image:url("+filename+")");
        }
        else
        {
            ani->icon->setStyleSheet("border-image:url(:/images/default.png)");
        }
        ani->softname->setText(pahtstr1);
        ani->softname->setToolTip(pahtstr1);
        ani->softdetaile->setText(pahtstr2);
        QString strtooltip;
        for(int i=0;i<pahtstr2.size();i++)
        {
            strtooltip.append(pahtstr2.at(i));
            if(i%20==0&&i>19)
            {
                strtooltip.append("\n");
            }
        }
        ani->softdetaile->setToolTip(strtooltip);
        ani->but_more->setText("more");
        ani->but_more->setStyleSheet("color:lightblue;background:transparent;");
        ani->old_versions->setText(pahtstr3);
        ani->new_versions->setText(pahtstr4);
        ani->lab_size->setText(get_size(pahtstr5));
        ani->but_upgrade->setText("upgrade");
        qDebug()<<"pahtstr6----------6666------"<<pahtstr6;
        ani->urlprogram=pahtstr6;

        QUrl url = QUrl::fromEncoded(pahtstr6.toUtf8());
        QString path_file = url.toString();
        QStringList str=path_file.split("/");
        int i=str.count();
        ani->exename=str.at(i-1);

        QListWidgetItem *twi = new QListWidgetItem(0);
        twi->setSizeHint(QSize(400,60));
        list_softupgrade->addItem(twi);
        list_softupgrade->setItemWidget(twi,ani);

    }
    SQLiteQuery.finish();

}

QString SoftUpgradeList::get_size( qint64 byte )
{
    double kb=0,mb=0,gb=0;
    QString size;
    if ( byte > 1024 ) kb= byte/1024;
    if ( kb > 1024 ) mb=kb/1024;
    if ( mb > 1024 ) gb=mb/1024;
    size=tr("%1B").arg(byte);
    if ( kb != 0 ) size=tr("%1KB").arg(kb,0,'f',2);
    if ( mb != 0 ) size=tr("%1MB").arg(mb,0,'f',2);
    if ( gb != 0 ) size=tr("%1GB").arg(gb,0,'f',2);

    return size;
}

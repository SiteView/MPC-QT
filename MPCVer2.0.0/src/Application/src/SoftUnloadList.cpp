
#include <QListWidgetItem>

#include "SoftUnloadList.h"

SoftUnloadList::SoftUnloadList(QWidget *parent) :
    QWidget(parent)
{
    UnloadList  = new QListWidget(this);
    UnloadList->resize(920,510);

    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation,UninstallString from LocalAppInfor ;" ) )
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

        pahtstr0.replace("\\","/");
        qDebug()<<pahtstr0<<pahtstr1<<pahtstr2<<pahtstr3<<pahtstr4<<pahtstr5<<pahtstr6<<"*****123456+++++";
        SoftUnloadItem *ani=new SoftUnloadItem(this);
        ani->icon->setStyleSheet("border-image:url("+pahtstr0+")");
        ani->softname->setText(pahtstr1);
        ani->softdetail->setText(pahtstr2);


        ani->size->setText(pahtstr3);
        QString str1;
        for(int k=0;k<10;k++)
        {
            str1+=pahtstr4.at(k);
        }
        ani->setuptime->setText(str1);
        QString str;
        if(pahtstr5.count()<10)
        {
            ani->progress->setText(pahtstr5+"...");
        }
        else
        {
            for(int k=0;k<10;k++)
            {
                str+=pahtstr5.at(k);
            }
            ani->progress->setText(str+"...");
        }
        ani->progress->setToolTip(pahtstr5);
        ani->unload->setText("Uninstall");
        ani->program=pahtstr6;

        QListWidgetItem *twi = new QListWidgetItem(0);
        twi->setSizeHint(QSize(400,59));
        UnloadList->addItem(twi);
        UnloadList->setItemWidget(twi,ani);
        //        UnloadList->setAlternatingRowColors(true);
        UnloadList->setStyleSheet("QListView::item:selected{background-color:rgb(247,247,247)}");
    }
    SQLiteQuery.finish();
}

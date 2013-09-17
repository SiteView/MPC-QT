#include "SoftAllKindList.h"
#include "SoftAllKindItem.h"
SoftAllKindList::SoftAllKindList(QWidget *parent) :
    QWidget(parent),type(0)
{
    list_softallkind  = new QListWidget(this);
    list_softallkind->resize(160,560);
    list_softallkind->setFocusPolicy(Qt::NoFocus);
    list_softallkind->setStyleSheet("QListView::item:selected{background-color:rgb(101,205,113)}");

    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    if ( !SQLiteQuery.exec( "select TypeName,Ordernumber,Type from SoftType ;" ) )
    {
        qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
    }
    while ( SQLiteQuery.next() )
    {
        QVariant val0 = SQLiteQuery.value(0);
        QVariant val1 = SQLiteQuery.value(1);
        QVariant val2 = SQLiteQuery.value(2);

        QString pahtstr0 = val0.toString();
        QString pahtstr1 = val1.toString();
        int pahtstr2 = val2.toInt();
        type++;
        SoftAllKindItem *ani=new SoftAllKindItem(this);
        ani->ico->setStyleSheet("border-image:url(:/images/litter/"+pahtstr0+".png)");
        ani->text->setText(pahtstr0);
        ani->num->setText(pahtstr1);
        QListWidgetItem *twi = new QListWidgetItem(0);
        twi->setSizeHint(QSize(150,30));
        list_softallkind->addItem(twi);
        list_softallkind->setItemWidget(twi,ani);
        const QString tmpStyle = (
                    "QListView{"
                    "background: rgb(238,238,238);"
                    "}");
        list_softallkind->setStyleSheet(tmpStyle);
    }
    SQLiteQuery.finish();
}



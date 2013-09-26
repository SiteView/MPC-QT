#include "SoftAllKindList.h"
#include "SoftAllKindItem.h"
SoftAllKindList::SoftAllKindList(QWidget *parent) :
    QWidget(parent)
{
    item_list = new QList<SoftAllKindItem *>();
    twi_list  = new QList<QListWidgetItem *>();
    list_handpick  = new QListWidget(this);
    list_classify  = new QListWidget(this);
    list_handpick->setFrameStyle(0);
    list_classify->setFrameStyle(0);

}

void SoftAllKindList::add_tree_data()
{
    item_list->clear();
    twi_list->clear();

    icon<<"HotApps"<<"Popular";
    text<<" "<<" ";
    num<<1<<2;
    Fill_Item_List(icon,text,num);
    count_handpick=icon.size();
//    list_handpick  = new QListWidget(this);
    list_handpick->setFixedSize(160,30*count_handpick);
    list_handpick->setFocusPolicy(Qt::NoFocus);
    list_handpick->setStyleSheet("QListView::item:selected{background-color:lightblue;}");

    for(int k=0; k<count_handpick; k++)
    {
        list_handpick->addItem(twi_list->at(k));
        list_handpick->setItemWidget(twi_list->at(k),item_list->at(k));
    }

}

void SoftAllKindList::add_branch_data()
{
    item_list->clear();
    twi_list->clear();

    list_name<<"all";
    list_number<<"";
    list_type<<0;
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
        list_name<<pahtstr0;
        list_number<<pahtstr1;
        list_type<<pahtstr2;
    }
    SQLiteQuery.finish();

    count_classify=list_name.size();
    Fill_Item_List(list_name,list_number,list_type);

//    list_classify  = new QListWidget(this);
    list_classify->setFixedSize(160,30*count_classify);
    list_classify->setFocusPolicy(Qt::NoFocus);
    list_classify->setStyleSheet("QListView::item:selected{background-color:lightblue}");

    for(int k=0; k<count_classify; k++)
    {
        list_classify->addItem(twi_list->at(k));
        list_classify->setItemWidget(twi_list->at(k),item_list->at(k));
    }
}

void SoftAllKindList::Fill_Item_List(QStringList list_0,QStringList list_1,QList<int> list_2)
{
    for(int i=0; i<list_0.size(); i++)
    {
        SoftAllKindItem *item = new SoftAllKindItem();
        item->takeText(list_0.at(i),list_1.at(i),list_2.at(i));
        item_list->push_back(item);

        QListWidgetItem *twi = new QListWidgetItem();
        twi->setSizeHint(QSize(156,30));
        twi_list->push_back(twi);
    }
}


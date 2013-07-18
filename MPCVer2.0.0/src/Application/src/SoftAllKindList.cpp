#include "SoftAllKindList.h"

SoftAllKindList::SoftAllKindList(QWidget *parent) :
    QWidget(parent)
{
    list_softallkind  = new QListWidget(this);
//    network_list->setRootIsDecorated(false);
    list_softallkind->setAlternatingRowColors(true);
//    network_list->setHeaderHidden(true);
    list_softallkind->setObjectName(QString("WlanVisibleNetworkList"));
    list_softallkind->setFocusPolicy(Qt::NoFocus);
    list_softallkind->setStyleSheet("QListView::item:selected{background-color:rgb(49,106,197)}");

}

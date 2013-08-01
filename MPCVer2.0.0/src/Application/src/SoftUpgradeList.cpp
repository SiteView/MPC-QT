#include <qtablewidget.h>
#include "SoftUpgradeList.h"

SoftUpgradeList::SoftUpgradeList(QWidget *parent) :
    QWidget(parent)
{
    QTableWidget *widget=new QTableWidget(this);
}

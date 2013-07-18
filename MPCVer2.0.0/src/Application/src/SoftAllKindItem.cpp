#include "SoftAllKindItem.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QtUiTools/QUiLoader>
#include <QFile>
SoftAllKindItem::SoftAllKindItem(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    QSpacerItem *horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_3);
    ico = new QLabel();
    ico->setMaximumSize(QSize(20, 20));
    horizontalLayout_2->addWidget(ico);
    text = new QLabel();
    horizontalLayout_2->addWidget(text);
    num = new QLabel();
    horizontalLayout_2->addWidget(num);
    QSpacerItem *horizontalSpacer_4 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_4);
    this->setLayout(horizontalLayout_2);

}
void SoftAllKindItem::retranslateUi(){

}

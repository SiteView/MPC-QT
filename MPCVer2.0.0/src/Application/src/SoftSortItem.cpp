#include <QHBoxLayout>
#include "SoftSortItem.h"

SoftSortItem::SoftSortItem(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(160,40);
    ico = new QLabel();
    ico->setObjectName(QString::fromUtf8("ico"));
    ico->setFixedSize(QSize(6, 7));
//    ico->setStyleSheet("background:transparent");

    text = new QLabel();
    text->setObjectName(QString::fromUtf8("text"));
    text->setFixedSize(140,40);
//    text->setStyleSheet("background:transparent");

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_2->addWidget(ico, 0, Qt::AlignHCenter);
    horizontalLayout_2->addWidget(text, 0, Qt::AlignHCenter);
    this->setLayout(horizontalLayout_2);
    this->setStyleSheet("background-color:#e9e4e4;");
}

void SoftSortItem::takeText(QString Qico,QString Qname)
{
    ico->setStyleSheet("border-image:url(:/images/litter/"+Qico+".png)");
    text->setText(Qname);
}

#include <QHBoxLayout>
#include <QSpacerItem>
#include <QtUiTools/QUiLoader>
#include <QFile>
#include <QPainter>
#include <QMouseEvent>
#include "SoftAllKindItem.h"
SoftAllKindItem::SoftAllKindItem(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(160,30);
    ico = new QLabel();
    ico->setObjectName(QString::fromUtf8("ico"));
    ico->setFixedSize(QSize(16, 16));
    ico->setStyleSheet("background:transparent");
    text = new QLabel();
    text->setObjectName(QString::fromUtf8("text"));
    text->setFixedSize(100,20);
    text->setStyleSheet("background:transparent");

    num = new QLabel();
    num->setObjectName(QString::fromUtf8("num"));
    num->setFixedSize(20,20);
    num->setStyleSheet("background:transparent");

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_2->addWidget(ico, 0, Qt::AlignHCenter);
    horizontalLayout_2->addWidget(text, 0, Qt::AlignHCenter);
    horizontalLayout_2->addWidget(num, 0, Qt::AlignHCenter);
    this->setLayout(horizontalLayout_2);

}
void SoftAllKindItem::retranslateUi()
{

}

QString SoftAllKindItem::get_text()
{
    return text->text();
}
void SoftAllKindItem::takeText(QString Qtype_name,QString Qorder_number,int Qnum)
{
    ico->setStyleSheet("border-image:url(:/images/litter/"+Qtype_name+".png)");
    text->setText(Qtype_name);
    num->setText(Qorder_number);
}




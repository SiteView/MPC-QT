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
    ico = new QLabel();
    ico->setObjectName(QString::fromUtf8("ico"));
    ico->setGeometry(QRect(15,25,16,16));
    ico->setFixedSize(QSize(16, 16));
    text = new QLabel();
    text->setObjectName(QString::fromUtf8("text"));
    text->setGeometry(QRect(30,25,100,20));
    text->setFixedSize(100,20);

    num = new QLabel();
    num->setObjectName(QString::fromUtf8("num"));
    num->setGeometry(QRect(150,25,20,20));
    num->setFixedSize(20,20);

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




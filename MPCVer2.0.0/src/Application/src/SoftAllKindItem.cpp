
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QtUiTools/QUiLoader>
#include <QFile>
#include "SoftAllKindItem.h"
SoftAllKindItem::SoftAllKindItem(QWidget *parent) :
    QWidget(parent)
{

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
//    QSpacerItem *horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
//    horizontalLayout_2->addItem(horizontalSpacer_3);
    ico = new QLabel();
    ico->setObjectName(QString::fromUtf8("ico"));
    ico->setGeometry(QRect(15,25,16,16));
    ico->setFixedSize(QSize(16, 16));
    horizontalLayout_2->addWidget(ico, 0, Qt::AlignHCenter);
    text = new QLabel();
    text->setObjectName(QString::fromUtf8("text"));
    text->setGeometry(QRect(30,25,100,20));
    text->setFixedSize(100,20);
    horizontalLayout_2->addWidget(text, 0, Qt::AlignHCenter);

    num = new QLabel();
    num->setObjectName(QString::fromUtf8("num"));
    num->setGeometry(QRect(150,25,20,20));
    num->setFixedSize(20,20);
    horizontalLayout_2->addWidget(num, 0, Qt::AlignHCenter);
//    QSpacerItem *horizontalSpacer_4 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
//    horizontalLayout_2->addItem(horizontalSpacer_4);

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setContentsMargins(11, 11, 11, 11);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    verticalLayout_2->addLayout(horizontalLayout_2);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Maximum);

    verticalLayout_2->addItem(verticalSpacer);

    line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setMaximumSize(QSize(16777215, 1));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background: rgb(242,242,242);");
    verticalLayout_2->addWidget(line);

    this->setLayout(verticalLayout_2);


}
void SoftAllKindItem::retranslateUi()
{


}

QString SoftAllKindItem::get_text()
{
    return text->text();

}

#include <QHBoxLayout>

#include "testunloaditem.h"

TestUnloadItem::TestUnloadItem(QWidget *parent) :
    QWidget(parent)
{
    this->resize(920,70);
    this->setCursor(Qt::PointingHandCursor);

    softname   =new QLabel(this);
    softdetail =new QLabel(this);
    softsize   =new QLabel(this);
    setuptime  =new QLabel(this);
    unload     =new QPushButton(this);
    softname->setFixedSize(100,30);
    softdetail->setFixedSize(300,25);
    softsize->setFixedSize(100,23);
    setuptime->setFixedSize(100,25);
    unload->setFixedSize(70,25);
    softname->setObjectName(QString::fromUtf8("softname"));
    softdetail->setObjectName(QString::fromUtf8("softdetail"));
    softsize->setObjectName(QString::fromUtf8("softsize"));
    setuptime->setObjectName(QString::fromUtf8("setuptime"));
    unload->setObjectName(QString::fromUtf8("unload"));
    QHBoxLayout *horizon=new QHBoxLayout();
    horizon->addWidget(softname, 0, Qt::AlignHCenter);
    horizon->addWidget(softdetail, 0, Qt::AlignHCenter);
    horizon->addWidget(softsize, 0, Qt::AlignHCenter);
    horizon->addWidget(setuptime, 0, Qt::AlignHCenter);
    horizon->addWidget(unload, 0, Qt::AlignHCenter);
    this->setLayout(horizon);
}
void TestUnloadItem::takeText(QString name,QString datail,QString size,QString time){
    softname->setText(name);
    softdetail->setText(datail);
    softsize->setText(size);
    setuptime->setText(time);

}

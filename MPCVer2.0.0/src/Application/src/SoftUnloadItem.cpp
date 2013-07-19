#include "SoftUnloadItem.h"
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>
SoftUnloadItem::SoftUnloadItem(QWidget *parent) :
    QWidget(parent)
{
    icon       =new QLabel(this);
    softname   =new QLabel(this);
    softdetail =new QLabel(this);
    progress   =new QLabel(this);
    size       =new QLabel(this);
    setuptime  =new QLabel(this);
    unload     =new QPushButton(this);

    softname->setMaximumSize(QSize(300, 45));
    icon->setMaximumSize(QSize(45, 45));
    softdetail->setMaximumSize(QSize(300, 30));
    setuptime->setMaximumSize(QSize(90, 16777215));
    size->setMaximumSize(QSize(90, 16777215));
    progress->setMaximumSize(QSize(90, 16777215));
    unload->setMaximumSize(QSize(70, 25));
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->addWidget(softname);
    verticalLayout->addWidget(softdetail);
    QSpacerItem *verticalSpacer = new QSpacerItem(2, 10, QSizePolicy::Minimum, QSizePolicy::Maximum);
    verticalLayout->addItem(verticalSpacer);
    QSpacerItem *horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    horizontalLayout->addWidget(icon);
    horizontalLayout->addItem(horizontalSpacer_2);
    horizontalLayout->addLayout(verticalLayout);
    horizontalLayout->addWidget(size);
    horizontalLayout->addWidget(setuptime);
    horizontalLayout->addWidget(progress);
    horizontalLayout->addWidget(unload);
    horizontalLayout->addItem(horizontalSpacer);
    this->setLayout(horizontalLayout);
    connect(unload,SIGNAL(clicked()),this,SLOT(on_unload_clicked()));

}

void SoftUnloadItem::on_unload_clicked()
{

    QProcess *unload=new QProcess();
    unload->start(program,QStringList());    
    qDebug()<<program<<"cliked.....";

}

void SoftUnloadItem::SeekUnloadData(){

}



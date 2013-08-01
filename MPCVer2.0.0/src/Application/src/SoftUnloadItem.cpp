
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>

#include "SoftUnloadItem.h"

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
    uninstall  =new QLabel(this);
    icon->setObjectName(QString::fromUtf8("icon"));
    softname->setObjectName(QString::fromUtf8("softname"));
    softdetail->setObjectName(QString::fromUtf8("softdetail"));
    progress->setObjectName(QString::fromUtf8("progress"));
    size->setObjectName(QString::fromUtf8("size"));
    setuptime->setObjectName(QString::fromUtf8("setuptime"));
    unload->setObjectName(QString::fromUtf8("unload"));
    uninstall->setObjectName(QString::fromUtf8("uninstall"));

    softname->setMaximumSize(QSize(300, 45));
    icon->setMaximumSize(QSize(45, 65));
    softdetail->setMaximumSize(QSize(300, 30));
    setuptime->setMaximumSize(QSize(90, 16777215));
    size->setMaximumSize(QSize(90, 16777215));
    progress->setMaximumSize(QSize(90, 16777215));
    unload->setMaximumSize(QSize(70, 25));
    uninstall->setMaximumSize(QSize(70, 25));
    uninstall->setText("正在卸载");
    uninstall->hide();
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
    horizontalLayout->addWidget(uninstall);

    horizontalLayout->addItem(horizontalSpacer);



    QVBoxLayout *verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setContentsMargins(11, 11, 11, 11);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    QSpacerItem *verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Maximum);
    verticalLayout_2->addItem(verticalSpacer_2);
    verticalLayout_2->addLayout(horizontalLayout);

    QFrame *line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setMaximumSize(QSize(16777215, 1));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background: rgb(185,186,187);");
    verticalLayout_2->addWidget(line);

    this->setLayout(verticalLayout_2);
    connect(unload,SIGNAL(clicked()),this,SLOT(on_unload_clicked()));

}

void SoftUnloadItem::on_unload_clicked()
{

    QProcess *unloader=new QProcess();
    unloader->start(program,QStringList());
    qDebug()<<program<<"cliked.....";
    uninstall->show();
    unload->hide();
}

void SoftUnloadItem::SeekUnloadData()
{

}



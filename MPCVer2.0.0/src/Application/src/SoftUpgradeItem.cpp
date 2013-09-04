#include <QMessageBox>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QtUiTools/QUiLoader>
#include <QFile>

#include <QVBoxLayout>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QCoreApplication>

#include "SoftUpgradeItem.h"
#include "curldownloadmanager.h"
#include "CellClass.h"

SoftUpgradeItem::SoftUpgradeItem(QWidget *parent) :
    QWidget(parent)
{
    downloader=new CURLDownloadManager(this);

    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(6);
    horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    horizontalLayout_3->setContentsMargins(0, 0, 0, 0);

    frame_1 = new QFrame();
    frame_1->setObjectName(QString::fromUtf8("frame_1"));
    frame_1->setFixedSize(QSize(520, 60));

    icon = new QPushButton(frame_1);//图片
    icon->setObjectName(QString::fromUtf8("icon"));
    icon->setGeometry(QRect(10, 10, 50, 50));
    icon->setFixedSize(QSize(50, 50));

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    softname = new QLabel();
    softname->setObjectName(QString::fromUtf8("softname"));
    softname->setFixedSize(QSize(180, 30));
    verticalLayout->addWidget(softname);
    softdetaile = new QLabel();
    softdetaile->setObjectName(QString::fromUtf8("softdetaile"));
    softdetaile->setFixedSize(QSize(300, 20));
    verticalLayout->addWidget(softdetaile);

    but_more = new QPushButton(frame_1);//更多
    but_more->setObjectName(QString::fromUtf8("but_more"));
    but_more->setGeometry(QRect(470, 25, 40,20));
    but_more->setMaximumSize(QSize(40, 20));

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout();//新老版本
    verticalLayout_2->setGeometry(QRect(500,10,150,20));
    old_versions = new QLabel();
    old_versions->setObjectName(QString::fromUtf8("old_versions"));
    old_versions->setMaximumSize(QSize(150, 20));
    verticalLayout_2->addWidget(old_versions);
    new_versions = new QLabel();
    new_versions->setObjectName(QString::fromUtf8("new_versions"));
    new_versions->setMaximumSize(QSize(150, 20));
    verticalLayout_2->addWidget(new_versions);
    horizontalLayout_3->addWidget(frame_1);

    QHBoxLayout *horizontal = new QHBoxLayout(frame_1);
    horizontal->addWidget(icon ,0, Qt::AlignHCenter);
    horizontal->addLayout(verticalLayout);
    horizontal->addWidget(but_more , 0, Qt::AlignHCenter);
    horizontal->addLayout(verticalLayout_2);

    widget = new QWidget();//分页显示
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(520,0,400,60));
    widget->setFixedSize(QSize(400, 60));

    stackedWidget = new QStackedWidget(widget);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
    stackedWidget->setGeometry(QRect(0, 0, 400, 60));
    stackedWidget->setFixedSize(QSize(400, 60));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    frame_2 = new QFrame(page);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setGeometry(QRect(0, 0, 400, 60));
    frame_2->setFixedSize(QSize(400, 60));
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    lab_upnum = new QLabel(frame_2);
    lab_upnum->setObjectName(QString::fromUtf8("lab_upnum"));
    lab_upnum->setGeometry(QRect(110, 20, 101, 20));
    lab_upnum->setFixedSize(QSize(120, 20));
    lab_size = new QLabel(frame_2);
    lab_size->setObjectName(QString::fromUtf8("lab_size"));
    lab_size->setGeometry(QRect(10, 20, 50, 20));
    lab_size->setFixedSize(QSize(50, 20));
    but_upgrade = new QPushButton(frame_2);
    but_upgrade->setObjectName(QString::fromUtf8("download"));
    but_upgrade->setGeometry(QRect(280, 20, 80, 25));
    but_upgrade->setFixedSize(QSize(80, 25));
    stackedWidget->addWidget(page);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    frame_3 = new QFrame(page_2);
    frame_3->setObjectName(QString::fromUtf8("frame_3"));
    frame_3->setGeometry(QRect(0, 0, 400, 60));
    frame_3->setFixedSize(QSize(400, 60));
    frame_3->setFrameShape(QFrame::StyledPanel);
    frame_3->setFrameShadow(QFrame::Raised);

    but_suspend = new QPushButton(frame_3);
    but_suspend->setObjectName(QString::fromUtf8("but_suspend"));
    but_suspend->setGeometry(QRect(280, 20, 18, 14));
    but_cancel = new QPushButton(frame_3);
    but_cancel->setObjectName(QString::fromUtf8("but_cancel"));
    but_cancel->setGeometry(QRect(300, 20, 18, 14));
    progressBar = new QProgressBar(frame_3);
    progressBar->setObjectName(QString::fromUtf8("but_progress"));
    progressBar->setGeometry(QRect(20, 20, 200, 15));

    stackedWidget->addWidget(page_2);
    page_3 = new QWidget();
    page_3->setObjectName(QString::fromUtf8("page_3"));
    frame_4 = new QFrame(page_3);
    frame_4->setObjectName(QString::fromUtf8("frame_4"));
    frame_4->setGeometry(QRect(0, 0, 400, 60));
    frame_4->setFixedSize(QSize(400, 60));
    frame_4->setFrameShape(QFrame::StyledPanel);
    frame_4->setFrameShadow(QFrame::Raised);
    lab_upnum_2 = new QLabel(frame_4);
    lab_upnum_2->setObjectName(QString::fromUtf8("lab_upnum"));
    lab_upnum_2->setGeometry(QRect(150, 20, 101, 20));
    lab_upnum_2->setFixedSize(QSize(120, 20));
    lab_size_2 = new QLabel(frame_4);
    lab_size_2->setObjectName(QString::fromUtf8("lab_size"));
    lab_size_2->setGeometry(QRect(20, 20, 50, 20));
    lab_size_2->setFixedSize(QSize(50, 20));
    setup = new QPushButton(frame_4);
    setup->setObjectName(QString::fromUtf8("set_up"));
    setup->setGeometry(QRect(280, 20, 80, 25));
    setup->setFixedSize(QSize(80, 25));
    setup->setText("setup");
    stackedWidget->addWidget(page_3);
    horizontalLayout_3->addWidget(widget);
    this->setLayout(horizontalLayout_3);

    connect(but_upgrade,SIGNAL(clicked()),this,SLOT(on_but_upgrade_clicked()));
    connect(but_cancel,SIGNAL(clicked()),this,SLOT(cancelProgress_download()));
    connect(but_suspend,SIGNAL(clicked()),this,SLOT(suspendProgress_download()));
    connect(progressBar,SIGNAL(valueChanged(int)),this,SLOT(changevalued(int)));
    connect(setup,SIGNAL(clicked()),this,SLOT(suspendProgress_setup()));

}

void SoftUpgradeItem::on_but_upgrade_clicked()
{
    if(CURLDownloadManager::getThis()->isBusy())
    {
        CellClass *cell=new CellClass();
        cell->changeText("Upgrade","is busy","close");
        cell->show();
    }
    else
    {
        stackedWidget->setCurrentWidget(page_2);
        connect(CURLDownloadManager::getThis(),SIGNAL(Setvalue(int)),this,SLOT(startProgress(int)));
        connect(CURLDownloadManager::getThis(),SIGNAL(DownloadFinish(int)),this,SLOT(Downloadresult(int)));
        DownloadThread();
    }
}

void SoftUpgradeItem::DownloadThread()
{
    runPath = QCoreApplication::applicationDirPath();
    CURLDownloadManager::getThis()->start();
    CURLDownloadManager::getThis()->setUrl(urlprogram);
    CURLDownloadManager::getThis()->setSavefileName(runPath+"/tmp/"+exename);
    CURLDownloadManager::getThis()->ready(true);

}

void SoftUpgradeItem::startProgress(int i)//给进度条传值
{

    progressBar->setValue(i);

}
void SoftUpgradeItem::changevalued(int i)//判断下载是否完成
{
    if(i==100)
    {
        stackedWidget->setCurrentWidget(page_3);
    }
}

void SoftUpgradeItem::cancelProgress_download()//取消下载
{
    setup->destroyed();
    stackedWidget->setCurrentWidget(page);

}
void SoftUpgradeItem::suspendProgress_download()//暂停下载
{

}
void SoftUpgradeItem::cancelProgress_setup()//取消安装
{
    stackedWidget->setCurrentWidget(page);

}
bool SoftUpgradeItem::suspendProgress_setup()//暂停安装
{
    QString program=runPath+"/tmp/"+exename;
    QProcess *setup=new QProcess();
    setup->start(program,QStringList());
    if (!setup->waitForStarted()) // 检查是否可执行
    {
        CellClass *cell=new CellClass();
        cell->changeText("Setup","is fial","close");
        cell->show();
        return false;
    }
    if (!setup->waitForFinished()) // 检查是否可结束
    {
        CellClass *cell=new CellClass();
        cell->changeText("Setup","is fial","close");
        cell->show();
        return false;
    }
}

void SoftUpgradeItem::Downloadresult(int i)//获得下载返回值
{
    if (i!=0)
    {
        stackedWidget->setCurrentWidget(page);

    }
}

QString SoftUpgradeItem::get_size( qint64 byte )
{
    double kb=0,mb=0,gb=0;
    QString size;
    if ( byte > 1024 ) kb= byte/1024;
    if ( kb > 1024 ) mb=kb/1024;
    if ( mb > 1024 ) gb=mb/1024;
    size=tr("%1B").arg(byte);
    if ( kb != 0 ) size=tr("%1KB").arg(kb,0,'f',2);
    if ( mb != 0 ) size=tr("%1MB").arg(mb,0,'f',2);
    if ( gb != 0 ) size=tr("%1GB").arg(gb,0,'f',2);

    return size;
}


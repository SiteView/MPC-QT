#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "SoftDownloadItem.h"
#include "SoftDownloadList.h"
#include "curldownloadmanager.h"

SoftDownloadItem::SoftDownloadItem(QWidget *parent) :
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
    frame_1->setMaximumSize(QSize(340, 60));
    frame_1->setFrameShape(QFrame::StyledPanel);
    frame_1->setFrameShadow(QFrame::Raised);
    but_icon = new QPushButton(frame_1);
    but_icon->setObjectName(QString::fromUtf8("but_icon"));
    but_icon->setGeometry(QRect(10, 10, 50, 50));
    but_icon->setMaximumSize(QSize(50, 50));
    layoutWidget = new QWidget(frame_1);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(70, 0, 251, 61));
    QVBoxLayout *verticalLayout = new QVBoxLayout(layoutWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    but_softname = new QLabel(layoutWidget);
    but_softname->setObjectName(QString::fromUtf8("but_softname"));
    but_softname->setMaximumSize(QSize(120, 30));
    verticalLayout->addWidget(but_softname);
    lab_softdetail = new QLabel(layoutWidget);
    lab_softdetail->setObjectName(QString::fromUtf8("lab_softdetail"));
    lab_softdetail->setMaximumSize(QSize(220, 20));
    verticalLayout->addWidget(lab_softdetail);
    horizontalLayout_3->addWidget(frame_1);
    widget = new QWidget();
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setMaximumSize(QSize(310, 60));
    stackedWidget = new QStackedWidget(widget);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
    stackedWidget->setGeometry(QRect(0, 0, 310, 60));
    stackedWidget->setMaximumSize(QSize(310, 60));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    frame_2 = new QFrame(page);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setGeometry(QRect(-10, 0, 310, 60));
    frame_2->setMaximumSize(QSize(310, 60));
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    lab_upnum = new QLabel(frame_2);
    lab_upnum->setObjectName(QString::fromUtf8("lab_upnum"));
    lab_upnum->setGeometry(QRect(110, 20, 101, 20));
    lab_upnum->setMaximumSize(QSize(120, 20));
    lab_size = new QLabel(frame_2);
    lab_size->setObjectName(QString::fromUtf8("lab_size"));
    lab_size->setGeometry(QRect(10, 20, 50, 20));
    lab_size->setMaximumSize(QSize(50, 20));
    download = new QPushButton(frame_2);
    download->setObjectName(QString::fromUtf8("download"));
    download->setGeometry(QRect(220, 20, 80, 25));
    download->setMaximumSize(QSize(80, 25));
    stackedWidget->addWidget(page);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    frame_3 = new QFrame(page_2);
    frame_3->setObjectName(QString::fromUtf8("frame_3"));
    frame_3->setGeometry(QRect(0, 0, 310, 60));
    frame_3->setMaximumSize(QSize(310, 60));
    frame_3->setFrameShape(QFrame::StyledPanel);
    frame_3->setFrameShadow(QFrame::Raised);
    lab_prompt = new QLabel(frame_3);
    lab_prompt->setObjectName(QString::fromUtf8("lab_prompt"));
    lab_prompt->setGeometry(QRect(170, 20, 91, 16));
    lab_prompt->setMaximumSize(QSize(270, 60));
    but_suspend = new QPushButton(frame_3);
    but_suspend->setObjectName(QString::fromUtf8("but_suspend"));
    but_suspend->setGeometry(QRect(270, 20, 20, 20));
    but_cancel = new QPushButton(frame_3);
    but_cancel->setObjectName(QString::fromUtf8("but_cancel"));
    but_cancel->setGeometry(QRect(290, 20, 20, 20));
    progressBar = new QProgressBar(frame_3);
    progressBar->setObjectName(QString::fromUtf8("but_progress"));
    progressBar->setGeometry(QRect(0, 20, 200, 15));

    stackedWidget->addWidget(page_2);
    page_3 = new QWidget();
    page_3->setObjectName(QString::fromUtf8("page_3"));
    frame_4 = new QFrame(page_3);
    frame_4->setObjectName(QString::fromUtf8("frame_4"));
    frame_4->setGeometry(QRect(0, 0, 310, 60));
    frame_4->setMaximumSize(QSize(310, 60));
    frame_4->setFrameShape(QFrame::StyledPanel);
    frame_4->setFrameShadow(QFrame::Raised);
    lab_upnum_2 = new QLabel(frame_4);
    lab_upnum_2->setObjectName(QString::fromUtf8("lab_upnum"));
    lab_upnum_2->setGeometry(QRect(110, 20, 101, 20));
    lab_upnum_2->setMaximumSize(QSize(120, 20));
    lab_size_2 = new QLabel(frame_4);
    lab_size_2->setObjectName(QString::fromUtf8("lab_size"));
    lab_size_2->setGeometry(QRect(10, 20, 50, 20));
    lab_size_2->setMaximumSize(QSize(50, 20));
    setup = new QPushButton(frame_4);
    setup->setObjectName(QString::fromUtf8("set_up"));
    setup->setGeometry(QRect(220, 20, 80, 25));
    setup->setMaximumSize(QSize(80, 25));
    setup->setText("setup");
    stackedWidget->addWidget(page_3);

    horizontalLayout_3->addWidget(widget);
    this->setLayout(horizontalLayout_3);

    connect(download,SIGNAL(clicked()),this,SLOT(on_download_clicked()));

    connect(but_cancel,SIGNAL(clicked()),this,SLOT(cancelProgress_download()));
    connect(but_suspend,SIGNAL(clicked()),this,SLOT(suspendProgress_download()));
    connect(progressBar,SIGNAL(valueChanged(int)),this,SLOT(changevalued(int)));

//    connect(but_cancel_2,SIGNAL(clicked()),this,SLOT(cancelProgress_setup()));
    connect(setup,SIGNAL(clicked()),this,SLOT(suspendProgress_setup()));

}


void SoftDownloadItem::on_download_clicked()
{

    stackedWidget->setCurrentWidget(page_2);
    connect(CURLDownloadManager::getThis(),SIGNAL(Setvalue(int)),this,SLOT(startProgress(int)));
    connect(CURLDownloadManager::getThis(),SIGNAL(DownloadFinish(int)),this,SLOT(Downloadresult(int)));

    DownloadThread();

}
void SoftDownloadItem::startProgress(int i)
{

    progressBar->setValue(i);

}
void SoftDownloadItem::changevalued(int i)
{
    if(i==100)
    {
        stackedWidget->setCurrentWidget(page_3);
    }
}

void SoftDownloadItem::cancelProgress_download()
{

}
void SoftDownloadItem::suspendProgress_download()
{

}
void SoftDownloadItem::cancelProgress_setup()
{
    stackedWidget->setCurrentWidget(page);

}
void SoftDownloadItem::suspendProgress_setup()
{
    QString program="c:/"+exename;
    QProcess *setup=new QProcess();
    setup->start(program,QStringList());
}

void SoftDownloadItem::DownloadThread(){

    qDebug()<<"urlprogram---"<<urlprogram;
    downloader->start();
    downloader->setUrl(urlprogram);
    downloader->setSavefileName("c:/"+exename);
    downloader->ready(true);    
}

void SoftDownloadItem::Downloadresult(int i){

    if (i!=0)
    {
        stackedWidget->setCurrentWidget(page);
    }
}

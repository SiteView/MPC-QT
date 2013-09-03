#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QCoreApplication>

#include "SoftDownloadItem.h"
#include "SoftDownloadList.h"
#include "curldownloadmanager.h"

SoftDownloadItem::SoftDownloadItem(QWidget *parent) :
    QWidget(parent)
{
    downloader= new CURLDownloadManager(this);
    qDebug()<<"==downloader=="<<downloader;
    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(6);
    horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
    frame_1 = new QFrame();
    frame_1->setObjectName(QString::fromUtf8("frame_1"));
    frame_1->setFixedSize(QSize(450, 60));

    but_icon = new QPushButton(frame_1);
    but_icon->setObjectName(QString::fromUtf8("but_icon"));
    but_icon->setGeometry(QRect(10, 10, 50, 50));
    but_icon->setFixedSize(QSize(50, 50));

    layoutWidget = new QWidget(frame_1);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(70, 0, 440, 60));
    QVBoxLayout *verticalLayout = new QVBoxLayout(layoutWidget);
    but_softname = new QLabel(layoutWidget);
    but_softname->setObjectName(QString::fromUtf8("but_softname"));
    but_softname->setFixedSize(QSize(180, 30));
    verticalLayout->addWidget(but_softname);
    lab_softdetail = new QLabel(layoutWidget);
    lab_softdetail->setObjectName(QString::fromUtf8("lab_softdetail"));
    lab_softdetail->setFixedSize(QSize(400, 20));
    verticalLayout->addWidget(lab_softdetail);
    horizontalLayout_3->addWidget(frame_1);
    widget = new QWidget();
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setFixedSize(QSize(310, 60));
    stackedWidget = new QStackedWidget(widget);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
    stackedWidget->setGeometry(QRect(0, 0, 310, 60));
    stackedWidget->setFixedSize(QSize(310, 60));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    frame_2 = new QFrame(page);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setGeometry(QRect(-10, 0, 310, 60));
    frame_2->setFixedSize(QSize(310, 60));
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
    download = new QPushButton(frame_2);
    download->setObjectName(QString::fromUtf8("download"));
    download->setGeometry(QRect(220, 20, 80, 25));
    download->setFixedSize(QSize(80, 25));
    stackedWidget->addWidget(page);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    frame_3 = new QFrame(page_2);
    frame_3->setObjectName(QString::fromUtf8("frame_3"));
    frame_3->setGeometry(QRect(0, 0, 310, 60));
    frame_3->setFixedSize(QSize(310, 60));
    frame_3->setFrameShape(QFrame::StyledPanel);
    frame_3->setFrameShadow(QFrame::Raised);

    but_suspend = new QPushButton(frame_3);
    but_suspend->setObjectName(QString::fromUtf8("but_suspend"));
    but_suspend->setGeometry(QRect(240, 20, 18, 14));
    but_cancel = new QPushButton(frame_3);
    but_cancel->setObjectName(QString::fromUtf8("but_cancel"));
    but_cancel->setGeometry(QRect(260, 20, 18, 14));
    progressBar = new QProgressBar(frame_3);
    progressBar->setObjectName(QString::fromUtf8("but_progress"));
    progressBar->setGeometry(QRect(0, 20, 200, 15));

    stackedWidget->addWidget(page_2);
    page_3 = new QWidget();
    page_3->setObjectName(QString::fromUtf8("page_3"));
    frame_4 = new QFrame(page_3);
    frame_4->setObjectName(QString::fromUtf8("frame_4"));
    frame_4->setGeometry(QRect(-10, 0, 310, 60));
    frame_4->setFixedSize(QSize(310, 60));
    frame_4->setFrameShape(QFrame::StyledPanel);
    frame_4->setFrameShadow(QFrame::Raised);
    lab_upnum_2 = new QLabel(frame_4);
    lab_upnum_2->setObjectName(QString::fromUtf8("lab_upnum"));
    lab_upnum_2->setGeometry(QRect(110, 20, 101, 20));
    lab_upnum_2->setFixedSize(QSize(120, 20));
    lab_size_2 = new QLabel(frame_4);
    lab_size_2->setObjectName(QString::fromUtf8("lab_size"));
    lab_size_2->setGeometry(QRect(10, 20, 50, 20));
    lab_size_2->setFixedSize(QSize(50, 20));
    setup = new QPushButton(frame_4);
    setup->setObjectName(QString::fromUtf8("set_up"));
    setup->setGeometry(QRect(220, 20, 80, 25));
    setup->setFixedSize(QSize(80, 25));
    setup->setText("setup");
    stackedWidget->addWidget(page_3);

    horizontalLayout_3->addWidget(widget);
    this->setLayout(horizontalLayout_3);

    connect(download,SIGNAL(clicked()),this,SLOT(on_download_clicked()));

    connect(but_cancel,SIGNAL(clicked()),this,SLOT(cancelProgress_download()));
    connect(but_suspend,SIGNAL(clicked()),this,SLOT(suspendProgress_download()));
    connect(progressBar,SIGNAL(valueChanged(int)),this,SLOT(changevalued(int)));
    connect(setup,SIGNAL(clicked()),this,SLOT(suspendProgress_setup()));

}
void SoftDownloadItem::takeText(QString Qsoftname,
                              QString Qsoftdetail,QString Qsize,
                              QString Qupnum,QString Qurl )
{

    but_icon->setStyleSheet("border-image:url(./icons/"+Qsoftname+".ico)");
    but_softname->setText(Qsoftname);

    lab_softdetail->setText(Qsoftdetail+"...");

    lab_softdetail->setToolTip(Qsoftdetail);
    lab_upnum->setText(Qupnum);
    lab_upnum_2->setText(Qupnum);
    lab_size->setText(Qsize);
    lab_size_2->setText(Qsize);
    download->setText("download");
    urlprogram=Qurl;
    //        ani->lab_prompt->setText(pahtstr2);
    QUrl url = QUrl::fromEncoded(Qurl.toUtf8());
    QString path_file = url.toString();
    QStringList str=path_file.split("/");
    int i=str.count();
    exename=str.at(i-1);


}
void SoftDownloadItem::on_download_clicked()//触发下载按钮
{
    if(CURLDownloadManager::getThis()->isBusy())
    {
        QMessageBox::about(this,tr("inform"),tr("is busy"));
    }
    else
    {
    stackedWidget->setCurrentWidget(page_2);
    connect(CURLDownloadManager::getThis(),SIGNAL(Setvalue(int)),this,SLOT(startProgress(int)));
    connect(CURLDownloadManager::getThis(),SIGNAL(DownloadFinish(int)),this,SLOT(Downloadresult(int)));
    DownloadThread();
    }

}
void SoftDownloadItem::startProgress(int i)//给进度条传值
{
    progressBar->setValue(i);
}
void SoftDownloadItem::changevalued(int i)//判断下载是否完成
{
    if(i==100)
    {
        stackedWidget->setCurrentWidget(page_3);
    }
}

void SoftDownloadItem::cancelProgress_download()//取消下载
{
    setup->destroyed();
    stackedWidget->setCurrentWidget(page);
}
void SoftDownloadItem::suspendProgress_download()//暂停下载
{

}
void SoftDownloadItem::cancelProgress_setup()//取消安装
{
    stackedWidget->setCurrentWidget(page);

}
void SoftDownloadItem::suspendProgress_setup()//暂停安装
{
    QString program=runPath+"/tmp/"+exename;
    QProcess *setup=new QProcess();
    setup->start(program,QStringList());
}

void SoftDownloadItem::DownloadThread()//下载进程
{
    runPath = QCoreApplication::applicationDirPath();
    qDebug()<<runPath<<"---urlprogram---"<<urlprogram;
    CURLDownloadManager::getThis()->start();
    CURLDownloadManager::getThis()->setUrl(urlprogram);
    CURLDownloadManager::getThis()->setSavefileName(runPath+"/tmp/"+exename);
    CURLDownloadManager::getThis()->ready(true);
}

void SoftDownloadItem::Downloadresult(int i)//获得下载返回值
{
    if (i!=0)
    {
        stackedWidget->setCurrentWidget(page);
        qDebug()<<"Downloadresult->===i==="<<i;
    }
}

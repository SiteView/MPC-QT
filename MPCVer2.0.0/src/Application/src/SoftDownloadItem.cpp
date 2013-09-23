#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QCoreApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QDir>
#include "SoftDownloadItem.h"
#include "SoftDownloadList.h"
#include "curldownloadmanager.h"
#include "CellClass.h"
SoftDownloadItem::SoftDownloadItem(QWidget *parent) :
    QWidget(parent)
{
    QSpacerItem *horizontalSpacer = new QSpacerItem(15, 20, QSizePolicy::Maximum, QSizePolicy::Maximum);

    but_icon = new QPushButton();
    but_icon->setObjectName(QString::fromUtf8("but_icon"));
    but_icon->setFixedSize(QSize(50, 50));

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    but_softname = new QLabel();
    but_softname->setObjectName(QString::fromUtf8("but_softname"));
    but_softname->setFixedSize(QSize(180, 30));
    verticalLayout->addWidget(but_softname);
    lab_softdetail = new QLabel();
    lab_softdetail->setObjectName(QString::fromUtf8("lab_softdetail"));
    lab_softdetail->setFixedSize(QSize(360, 20));
    verticalLayout->addWidget(lab_softdetail);
    QHBoxLayout *horizontalLayout_1=new QHBoxLayout();
    horizontalLayout_1->addItem(horizontalSpacer);
    horizontalLayout_1->addWidget(but_icon);
    horizontalLayout_1->addLayout(verticalLayout);

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
    lab_size->setGeometry(QRect(10, 20, 60, 20));
    lab_size->setFixedSize(QSize(60, 20));
    download = new QPushButton(frame_2);
    download->setObjectName(QString::fromUtf8("download"));
    download->setGeometry(QRect(220, 18, 80, 25));
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

    but_continue = new QPushButton(frame_3);
    but_continue->setObjectName(QString::fromUtf8("but_continue"));
    but_continue->setGeometry(QRect(240, 20, 18, 14));
    but_continue->hide();
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
    lab_size_2->setGeometry(QRect(10, 20, 60, 20));
    lab_size_2->setFixedSize(QSize(60, 20));
    setup = new QPushButton(frame_4);
    setup->setObjectName(QString::fromUtf8("set_up"));
    setup->setGeometry(QRect(220, 18, 80, 25));
    setup->setFixedSize(QSize(80, 25));
    setup->setText("setup");
    stackedWidget->addWidget(page_3);

    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(6);
    horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_3->addLayout(horizontalLayout_1);
    horizontalLayout_3->addWidget(widget);
    this->setLayout(horizontalLayout_3);

    connect(download,SIGNAL(clicked()),this,SLOT(on_download_clicked()));
    connect(but_continue,SIGNAL(clicked()),this,SLOT(continueProgress_download()));
    connect(but_cancel,SIGNAL(clicked()),this,SLOT(cancelProgress_download()));
    connect(but_suspend,SIGNAL(clicked()),this,SLOT(suspendProgress_download()));
    connect(progressBar,SIGNAL(valueChanged(int)),this,SLOT(changevalued(int)));
    connect(setup,SIGNAL(clicked()),this,SLOT(suspendProgress_setup()));

}
void SoftDownloadItem::takeText(QString Qsoftname,
                                QString Qsoftdetail,qint64 Qsize,
                                QString Qupnum,QString Qurl )//加载文字到界面
{
    but_softname->setText(Qsoftname);
    but_softname->setToolTip(Qsoftname);
    lab_softdetail->setText(Qsoftdetail+"...");
    QString strtooltip;
    for(int i=0;i<Qsoftdetail.size();i++)
    {
        strtooltip.append(Qsoftdetail.at(i));
        if(i%20==0&&i>19)
        {
            strtooltip.append("\n");
        }
    }
    lab_softdetail->setToolTip(strtooltip);
    lab_upnum->setText(Qupnum);
    lab_upnum_2->setText(Qupnum);
    lab_size->setText(get_size( Qsize ));
    lab_size_2->setText(get_size( Qsize ));
    download->setText("download");
    urlprogram=Qurl;
    QUrl url = QUrl::fromEncoded(Qurl.toUtf8());//处理下载地址，截取有效段做为软件名称
    QString path_file = url.toString();
    QStringList str=path_file.split("/");
    int i=str.count();
    exename=str.at(i-1);

    QString filename = QCoreApplication::applicationDirPath()+QString("/icons/")+Qsoftname.trimmed()+QString(".ico");
    QFileInfo iconfile(filename);
    if(iconfile.exists())
    {
        but_icon->setStyleSheet("border-image:url("+filename+")");
    }
    else
    {
        but_icon->setStyleSheet("border-image:url(:/images/default.png)");
    }
}

void SoftDownloadItem::on_download_clicked()//触发下载按钮
{
    runPath=QCoreApplication::applicationDirPath();
    QString filetmp=runPath+QString("/tmp");//判断是否存在文件夹tmp，不存在即创建
    QDir *temp = new QDir;
    bool exist = temp->exists(filetmp);
    if(!exist)
    {
        bool ok = temp->mkdir(filetmp);
        if(!ok)
        {
            CellClass *cell=new CellClass();
            cell->changeText("creat files tmp","is fail","close");
            cell->show();
        }
    }
    QString filename =runPath +QString("/tmp/")+exename;//判断软件是否已经下载
    QFileInfo iconfile(filename);
    if(iconfile.exists())
    {
        CellClass *cell=new CellClass();
        cell->changeText("Download","is exists","close");
        cell->show();
        stackedWidget->setCurrentWidget(page_3);
    }
    else
    {
        if(CURLDownloadManager::getThis()->isBusy())//判断下载线程是否被占用
        {
            CellClass *cell=new CellClass();
            cell->changeText("Download","is busy","close");
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
    stackedWidget->setCurrentWidget(page);
    but_continue->hide();
    but_suspend->show();
    CURLDownloadManager::getThis()->CancelTask();
}

void SoftDownloadItem::continueProgress_download()//继续下载
{
    but_continue->hide();
    but_suspend->show();
    CURLDownloadManager::getThis()->ResumeTask();
}

void SoftDownloadItem::suspendProgress_download()//暂停下载
{
    but_continue->show();
    but_suspend->hide();
    CURLDownloadManager::getThis()->PauseTask();
}

void SoftDownloadItem::cancelProgress_setup()//取消安装
{
    stackedWidget->setCurrentWidget(page);
}

bool SoftDownloadItem::suspendProgress_setup()//暂停安装
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

void SoftDownloadItem::DownloadThread()//下载进程
{

    CURLDownloadManager::getThis()->start();
    CURLDownloadManager::getThis()->setUrl(urlprogram);
    CURLDownloadManager::getThis()->setSavefileName(runPath+"/tmp/"+exename);
    CURLDownloadManager::getThis()->ready(true);
    //    downloader.start();
    //    downloader.setUrl(urlprogram);
    //    downloader.setSavefileName(runPath+"/tmp/"+exename);
    //    downloader.ready(true);

}

void SoftDownloadItem::Downloadresult(int i)//获得下载返回值
{
    if (i!=0)
    {
        stackedWidget->setCurrentWidget(page);
        //        CellClass *cell=new CellClass();
        //        cell->changeText("Download","is fail","close");
        //        cell->show();
    }
}

QString SoftDownloadItem::get_size( qint64 byte )
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

void SoftDownloadItem::paintEvent(QPaintEvent *event)//绘制卸载界面
{
    //绘制边框
    QPainter painter2(this);
    QLinearGradient linear2(rect().topLeft(), rect().bottomLeft());
    linear2.setColorAt(0, Qt::white);
    linear2.setColorAt(0.5, Qt::white);
    linear2.setColorAt(1,  Qt::white);
    painter2.setPen(QColor(228,228,228)); //设定画笔颜色，到时侯就是边框颜色
    painter2.setBrush(linear2);
    painter2.drawRect(QRect(0.5, 0.5, this->width()-1, this->height()-1));
    if(mouse_enter)
    {
        //绘制边框
        QPainter painter2(this);
        QLinearGradient linear2(rect().topLeft(), rect().bottomLeft());
        linear2.setColorAt(0, QColor(247,247,247));
        linear2.setColorAt(0.5, QColor(247,247,247));
        linear2.setColorAt(1,  QColor(247,247,247));
        painter2.setPen(QColor(228,228,228)); //设定画笔颜色，到时侯就是边框颜色
        painter2.setBrush(linear2);
        painter2.drawRect(QRect(0.5, 0.5, this->width()-1, this->height()-1));
    }
}

void SoftDownloadItem::mousePressEvent(QMouseEvent * event)
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }
}

void SoftDownloadItem::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_press = false;
}

void SoftDownloadItem::enterEvent(QEvent *event)
{
    mouse_enter = true;
    update();
}

void SoftDownloadItem::leaveEvent(QEvent *event)
{
    mouse_enter = false;
    update();
}

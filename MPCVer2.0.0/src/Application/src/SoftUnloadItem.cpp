#include <QSpacerItem>
#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QPainter>
#include <QMouseEvent>
#include <QFileInfo>
#include <QCoreApplication>
#include "SoftUnloadItem.h"
#include "CellClass.h"
SoftUnloadItem::SoftUnloadItem(QWidget *parent) :
    QWidget(parent)
{
    this->resize(902,65);
    mouse_press = false;
    mouse_enter = false;
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
    icon->setCursor(Qt::PointingHandCursor);
    softname->setCursor(Qt::PointingHandCursor);
    icon->setFixedSize(QSize(45, 45));
    softname->setFixedSize(QSize(300, 30));
    softdetail->setFixedSize(QSize(300, 20));
    setuptime->setFixedSize(QSize(90, 20));
    size->setFixedSize(QSize(90, 20));
    progress->setFixedSize(QSize(90, 20));
    unload->setFixedSize(QSize(70, 25));
    uninstall->setFixedSize(QSize(70, 25));
    uninstall->setText("finish");
    uninstall->hide();
    unload->setText("Uninstall");
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(5, 5, QSizePolicy::Maximum, QSizePolicy::Minimum);
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


    this->setLayout(horizontalLayout);
    connect(unload,SIGNAL(clicked()),this,SLOT(on_unload_clicked()));

}
void SoftUnloadItem::takeText(QString Qicon,QString Qsoftname,
                              QString Qsoftdetail,qint64 Qsize,
                              QString Qsetuptime,QString Qprogress,
                              QString Quninstall)//加载文字
{

    softname->setText(Qsoftname);
    softname->setToolTip(Qsoftname);
    softdetail->setText(Qsoftdetail);
    softdetail->setToolTip(Qsoftdetail);
    size->setText(get_size(Qsize));
    setuptime->setText(Qsetuptime);
    progress->setText(Qprogress);
    progress->setToolTip(Qprogress);
    program=Quninstall;

    QString filename = QCoreApplication::applicationDirPath()+QString("/icons/")+Qsoftname.trimmed()+QString(".ico");
    QFileInfo iconfile(filename);
    bool bloaded = false;
    if(iconfile.exists())
    {

        icon->setStyleSheet("border-image:url("+filename+")");
        bloaded = true;
    }
    else
    {
        qDebug() <<Qicon  << Qsoftname.toLocal8Bit().data();

    }
    if(!bloaded)
    {
        QStringList str=Qicon.split(".");
        if(Qicon==""||str.at(1)!="ico")
        {
            icon->setStyleSheet("border-image:url(:/images/default.png)");

        }
        else
        {
            icon->setStyleSheet("border-image:url("+Qicon+")");

        }
    }
}
QString SoftUnloadItem::get_size( qint64 byte )
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
bool SoftUnloadItem::on_unload_clicked()//触发卸载事件
{
    QProcess *unloader=new QProcess();
    unloader->start(program,QStringList());
    uninstall->show();
    uninstall->setText("finish");
    unload->hide();
    connect(unloader,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(Unloadfinish(int , QProcess::ExitStatus )));
    connect(unloader,SIGNAL(error(QProcess::ProcessError )),this,SLOT(unloaderror(QProcess::ProcessError )));
    if (!unloader->waitForStarted()) // 检查是否可执行
    {
        unload->hide();
        uninstall->setText("failed");
        return false;
    }
    if (!unloader->waitForFinished()) // 检查是否可结束
    {
        uninstall->show();
        return false;
    }
}

void SoftUnloadItem::Unloadfinish(int exitCode, QProcess::ExitStatus exitStatus)//卸载完成
{
    if (exitStatus == QProcess::NormalExit)
    {
        uninstall->show();

    }
    else if (exitCode != 0)
    {
        unload->hide();
        uninstall->setText("error");
    }
    else
    {
        unload->show();
        uninstall->hide();
    }

    QSqlQuery sqlQuery( *m_SQLiteDb.getDB());
    m_SQLiteDb.getDB()->transaction();
    sqlQuery.prepare(QString("update LocalAppInfor set UninstallTime = ? where  DisplayName = ?"));
    sqlQuery.addBindValue(QDateTime::currentDateTime());
    sqlQuery.addBindValue(softname->text());
    if (!sqlQuery.exec()) {
        qDebug(sqlQuery.lastError().text().toLocal8Bit().data());
        qDebug() << softname->text();
        //        return false;
    }
    m_SQLiteDb.getDB()->commit();
}

void SoftUnloadItem::Unloaderror(QProcess::ProcessError error )//卸载出错
{
    unload->show();
    uninstall->hide();
}
void SoftUnloadItem::paintEvent(QPaintEvent *event)//绘制卸载界面
{
///*
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
//    */
}

void SoftUnloadItem::mousePressEvent(QMouseEvent * event)
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }
}

void SoftUnloadItem::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_press = false;
}

void SoftUnloadItem::enterEvent(QEvent *event)
{
    mouse_enter = true;
    update();
}

void SoftUnloadItem::leaveEvent(QEvent *event)
{
    mouse_enter = false;
    update();
}


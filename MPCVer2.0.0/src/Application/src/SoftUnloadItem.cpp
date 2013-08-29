
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QPainter>
#include <QMouseEvent>

#include "SoftUnloadItem.h"

SoftUnloadItem::SoftUnloadItem(QWidget *parent) :
    QWidget(parent)
{
    this->resize(920,70);

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

    icon->setStyleSheet("background:transparent;");
    softname->setStyleSheet("background:transparent;");
    softdetail->setStyleSheet("background:transparent;");
    progress->setStyleSheet("background:transparent;");
    size->setStyleSheet("background:transparent;");
    setuptime->setStyleSheet("background:transparent;");
    unload->setStyleSheet("background:transparent;");
    uninstall->setStyleSheet("background:transparent;");

    icon->setObjectName(QString::fromUtf8("icon"));
    softname->setObjectName(QString::fromUtf8("softname"));
    softdetail->setObjectName(QString::fromUtf8("softdetail"));
    progress->setObjectName(QString::fromUtf8("progress"));
    size->setObjectName(QString::fromUtf8("size"));
    setuptime->setObjectName(QString::fromUtf8("setuptime"));
    unload->setObjectName(QString::fromUtf8("unload"));
    uninstall->setObjectName(QString::fromUtf8("uninstall"));
    softname->setCursor(Qt::PointingHandCursor);
    icon->setFixedSize(QSize(45, 45));
    softname->setFixedSize(QSize(300, 30));
    softdetail->setFixedSize(QSize(300, 20));
    setuptime->setFixedSize(QSize(90, 20));
    size->setFixedSize(QSize(90, 20));
    progress->setFixedSize(QSize(90, 20));
    unload->setFixedSize(QSize(70, 25));
    uninstall->setFixedSize(QSize(70, 25));
    uninstall->setText("Uninstalling...");
    uninstall->hide();
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
void SoftUnloadItem::takeText(QString Qicon,QString Qsoftname,QString Qsoftdetail,QString Qsize,
                              QString Qsetuptime,QString Qprogress,QString QuninstallString)
{
    icon->setStyleSheet("border-image:url("+Qicon+")");
    softname->setText(Qsoftname);
    softdetail->setText(Qsoftdetail);
    size->setText(Qsize);
    setuptime->setText(Qsetuptime);
    progress->setText(Qprogress);
    program=QuninstallString;

}
void SoftUnloadItem::on_unload_clicked()
{

    QProcess *unloader=new QProcess();
    unloader->start(program,QStringList());
    qDebug()<<program<<"cliked.....";
    uninstall->show();
    unload->hide();
    connect(unloader,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(Unloadfinish(int , QProcess::ExitStatus )));
    connect(unloader,SIGNAL(error(QProcess::ProcessError )),this,SLOT(unloaderror(QProcess::ProcessError )));

}

void SoftUnloadItem::Unloadfinish(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug()<<exitCode<<"==exitCode";
    if (exitStatus == QProcess::NormalExit)
    {
        qDebug()<<exitCode<<"==exitStatus";
        uninstall->setText("finished");

    }
    else if (exitCode != 0)
    {
        qDebug()<<exitCode<<"==";

        unload->hide();
        uninstall->setText("0");
    }
    else
    {
        uninstall->show();
        uninstall->setText("...ing...");
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

void SoftUnloadItem::Unloaderror(QProcess::ProcessError error )
{
    qDebug()<<error<<"=error=";
    unload->show();
    uninstall->hide();
}
void SoftUnloadItem::paintEvent(QPaintEvent *event)
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


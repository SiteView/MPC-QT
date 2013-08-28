#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include "testunloaditem.h"

TestUnloadItem::TestUnloadItem(QWidget *parent) :
    QWidget(parent)
{
    this->resize(920,70);
    mouse_press = false;
    mouse_enter = false;
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
    softname->setStyleSheet("background:transparent;");
    softdetail->setStyleSheet("background:transparent;");
    softsize->setStyleSheet("background:transparent;");
    setuptime->setStyleSheet("background:transparent;");
    unload->setStyleSheet("background:lightblue;");

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

void TestUnloadItem::paintEvent(QPaintEvent *event)
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

void TestUnloadItem::mousePressEvent(QMouseEvent * event)
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }
}

void TestUnloadItem::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_press = false;
}

void TestUnloadItem::enterEvent(QEvent *event)
{
    mouse_enter = true;
    update();
}

void TestUnloadItem::leaveEvent(QEvent *event)
{
    mouse_enter = false;
    update();
}


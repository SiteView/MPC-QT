#include <QDebug>
#include <QMoveEvent>
#include <QPainter>
#include "ToolButton.h"

ToolButton::ToolButton(QWidget *parent) :
    QToolButton(parent)
{
}
void ToolButton::setImage(QString pic_name)
{


    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    //设置图标
    QPixmap pixmap(pic_name);
    setIcon(pixmap);
    setIconSize(pixmap.size());
    //设置大小
    setFixedSize(pixmap.width()+25, pixmap.height()+27);
    setAutoRaise(true);

    setStyleSheet("background:transparent;");

    mouse_over = false;
    mouse_press = false;

}



void ToolButton::enterEvent(QEvent *)
{
    mouse_over = true;
}
void ToolButton::leaveEvent(QEvent *)
{
    mouse_over = false;
}


void ToolButton::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

void ToolButton::setMousePress(bool mouse_press)
{
    this->mouse_press = mouse_press;
    update();
}

void ToolButton::paintEvent(QPaintEvent *event)
{
    if(mouse_over)
    {
        //绘制鼠标移到按钮上的按钮效果
        painterInfo(0, 100, 150);
    }
    else
    {
        if(mouse_press)
        {
            painterInfo(0, 100, 150);
        }
    }

    QToolButton::paintEvent(event);
}
void ToolButton::painterInfo(int top_color, int middle_color, int bottom_color)//参数决定透明度
{
    QPainter painter(this);
    QPen pen(Qt::NoBrush, 1);
    painter.setPen(pen);
    QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
    linear.setColorAt(0, QColor(230, 230, 230, top_color));
    linear.setColorAt(0.5, QColor(230, 230, 230, middle_color));
    linear.setColorAt(1, QColor(230, 230, 230, bottom_color));
    painter.setBrush(linear);
    painter.drawRect(rect());
}

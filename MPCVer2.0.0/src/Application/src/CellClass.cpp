#include <QVBoxLayout>
#include <QPainter>
#include <QPen>
#include <QMoveEvent>
#include <QDebug>
#include "CellClass.h"

CellClass::CellClass(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint );
    this->setFixedSize(140, 160);
    mouse_press = false;
    mouse_enter = false;

    skin_label = new QLabel();
    skin_name_label = new QLabel();
    use_skin_button = new QPushButton();
    this->setCursor(Qt::PointingHandCursor);

    use_skin_button->setStyleSheet("border-radius:3px; border:1px solid rgb(180, 190, 200); color:rgb(70, 70, 70);");
    skin_label->setScaledContents(true);
    skin_label->setFixedSize(100, 65);
    use_skin_button->setFixedSize(85, 25);
    skin_label->setAlignment(Qt::AlignCenter);
    skin_name_label->setAlignment(Qt::AlignCenter);
    use_skin_button->setObjectName(QString::fromUtf8("use_skin_button"));
    skin_label->setObjectName(QString::fromUtf8("skin_label"));

    QVBoxLayout *background_layout = new QVBoxLayout();
    background_layout->addWidget(skin_label, 0, Qt::AlignCenter);
    background_layout->addWidget(skin_name_label, 0, Qt::AlignCenter);
    background_layout->addWidget(use_skin_button, 0, Qt::AlignCenter);
    background_layout->setSpacing(5);
    background_layout->setContentsMargins(0, 10, 0, 10);

    this->setLayout(background_layout);
    connect(use_skin_button,SIGNAL(clicked()),this,SLOT(close()));   

}


void CellClass::changeText(QString pixmap_name, QString skin_name, QString download_count)
{
    skin_label->setText(pixmap_name);
    skin_name_label->setText(skin_name);
    use_skin_button->setText(download_count);
}
void CellClass::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(QColor(22,132,243));
    painter.setPen(pen);
    painter.drawRoundRect(0,0,this->width()-1, this->height()-1, 0, 0);


    if(mouse_enter)
    {
        QPainter painter2(this);
        QLinearGradient linear2(rect().topLeft(), rect().bottomLeft());
        linear2.setColorAt(0, QColor(245,245,245));
        linear2.setColorAt(0.5, QColor(245,245,245));
        linear2.setColorAt(1, QColor(245,245,245));
        painter2.setPen(QColor(125,186,243)); //设定画笔颜色，到时侯就是边框颜色
        painter2.setBrush(linear2);
        painter2.drawRect(QRect(0.5, 0.5, this->width()-1, this->height()-1));

    }
}

void CellClass::mousePressEvent(QMouseEvent * event)
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }
    moving = true;
    last = event->globalPos();
}

void CellClass::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_press = false;
}

void CellClass::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    if ((e->buttons() & Qt::LeftButton) && moving)
        move(x()+dx, y()+dy);
}

void CellClass::enterEvent(QEvent *event)
{
    mouse_enter = true;
    update();
}

void CellClass::leaveEvent(QEvent *event)
{
    mouse_enter = false;
    update();
}

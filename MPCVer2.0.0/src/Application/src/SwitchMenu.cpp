#include "SwitchMenu.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
SwitchMenu::SwitchMenu(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    m_bOn = false;
    m_bLBtnDown = false;
    mouse_enter = false;

}

void SwitchMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0), m_pixmapBk);
    QPoint ptFore;
    if (m_bOn) {
        ptFore = this->rect().topLeft();
        painter.drawPixmap(ptFore, m_pixmapBk);
        QRect rcOn;
        rcOn.setTopLeft(rect().topLeft());
        rcOn.setBottomRight(QPoint(m_pixmapBk.width(), m_pixmapBk.height()));
    }
    else
    {
        ptFore = QPoint(this->width() - m_pixmapFore.width(),
                        this->rect().top());
        painter.drawPixmap(ptFore, m_pixmapFore);
        QRect rcOff;
        rcOff.setTopLeft(QPoint(rect().width() - m_pixmapFore.width(), rect().top()));
        rcOff.setBottomRight(rect().bottomRight());
    }
}

void SwitchMenu::mousePressEvent(QMouseEvent *)
{
    m_bLBtnDown = true;
}

void SwitchMenu::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_bLBtnDown) {
        m_bOn = !m_bOn;
        m_bLBtnDown = false;
    }
    update();
}
void SwitchMenu::enterEvent(QEvent *event)
{
    mouse_enter = true;
    update();
}

void SwitchMenu::leaveEvent(QEvent *event)
{
    mouse_enter = false;
    update();
}

void SwitchMenu::initialize(const QString& strImageBk, const QString& strImageFore)
{
    m_pixmapBk.load(strImageBk);
    m_pixmapFore.load(strImageFore);
    int nWidth = m_pixmapBk.width();
    int nHeight = m_pixmapBk.height();
    setGeometry(0, 0, nWidth, nHeight);
}

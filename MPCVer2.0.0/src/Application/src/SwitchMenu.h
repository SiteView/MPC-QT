#ifndef SWITCHMENU_H
#define SWITCHMENU_H

#include <QWidget>

class SwitchMenu : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchMenu(QWidget *parent = 0);
    
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

signals:

public slots:

public:
    void initialize(const QString&, const QString&);

private:
    QPixmap     m_pixmapBk;
    QPixmap     m_pixmapFore;
    bool        m_bOn;
    bool        m_bLBtnDown;//按下按钮
    bool        mouse_enter;//进入按钮
};

#endif // SWITCHMENU_H

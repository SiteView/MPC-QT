#ifndef TESTUNLOADITEM_H
#define TESTUNLOADITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class TestUnloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit TestUnloadItem(QWidget *parent = 0);

    void  takeText(QString ,QString ,QString ,QString );
protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    bool mouse_press;
    bool mouse_enter;
signals:
    
public slots:
public:
    QLabel *softname;
    QLabel *softdetail;
    QLabel *softsize;
    QLabel *setuptime;
    QPushButton *unload;
    
};

#endif // TESTUNLOADITEM_H

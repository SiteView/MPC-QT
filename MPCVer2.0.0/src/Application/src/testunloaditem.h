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

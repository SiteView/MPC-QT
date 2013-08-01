#ifndef SOFTUNLOADITEM_H
#define SOFTUNLOADITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class SoftUnloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUnloadItem(QWidget *parent = 0);
    
signals:
    
public slots:
    void on_unload_clicked();

public:
    QLabel *icon;
    QLabel *softname;
    QLabel *softdetail;
    QLabel *size;
    QLabel *setuptime;
    QLabel *progress;
    QPushButton *unload;
    QLabel *uninstall;
    QString program;


public:
    void SeekUnloadData();

    
};

#endif // SOFTUNLOADITEM_H

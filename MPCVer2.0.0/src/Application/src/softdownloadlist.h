#ifndef SOFTDOWNLOADLIST_H
#define SOFTDOWNLOADLIST_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QString>
#include <QPushButton>
class SoftDownloadList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadList(QWidget *parent = 0);
public:
    QLabel *icon;
    QLabel *softname;
    QLabel *softdetail;
    QLabel *progress;
    QLabel *speed;
    QLabel *comment;
    QPushButton *download;
public:
    void retranslateUi();

signals:
    
public slots:
    
};

#endif // SOFTDOWNLOADLIST_H

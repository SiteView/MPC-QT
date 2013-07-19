#ifndef SOFTDOWNLOADITEM_H
#define SOFTDOWNLOADITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
class SoftDownloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadItem(QWidget *parent = 0);
public:

    QLabel *icon;    
    QLabel *softname;
    QLabel *softdetail;
    QLabel *size;
    QLabel *setuptime;
    QLabel *progress;
    QPushButton *download;
    QString program;
public:
    void retranslateUi();

signals:

public slots:
    void on_download_clicked();
};
#endif // SOFTDOWNLOADITEM_H

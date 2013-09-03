#ifndef SOFTDOWNLOADITEM_H
#define SOFTDOWNLOADITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QProgressBar>
#include <QStackedWidget>
class  CURLDownloadManager;

class SoftDownloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadItem(QWidget *parent = 0);
public:

    QFrame *frame_1 ;
    QPushButton *but_icon ;
    QWidget *layoutWidget;
    QLabel *but_softname;
    QLabel *lab_softdetail;
    QWidget *widget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;
    QWidget *page_3;

    QFrame *frame_3 ;
    QPushButton *but_suspend;
    QPushButton *but_cancel ;
    QFrame *frame_2;
    QLabel *lab_upnum;
    QLabel *lab_size;
    QPushButton *download;
    QFrame *frame_4;

    QLabel *lab_upnum_2;
    QLabel *lab_size_2;
    QPushButton *setup;
    QProgressBar *progressBar;
    QProgressBar *progressBar_2;

    QString urlprogram;
    QString exename;
    QString runPath;
    CURLDownloadManager *downloader;

public:
    void DownloadThread();

signals:

public slots:
    void on_download_clicked();
    void startProgress(int);
    void changevalued(int);
    void cancelProgress_download();
    void suspendProgress_download();
    void cancelProgress_setup();
    void suspendProgress_setup();
    void Downloadresult(int);
};
#endif // SOFTDOWNLOADITEM_H

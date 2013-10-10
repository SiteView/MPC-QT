#ifndef SOFTDOWNLOADITEM_H
#define SOFTDOWNLOADITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QProgressBar>
#include <QStackedWidget>
#include "SettingDialog.h"

class SoftDownloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadItem(QWidget *parent = 0);
    void DownloadThread(QString);
    void takeText(QString ,QString ,qint64 ,QString ,QString );
    QString get_size( qint64 byte );

public:

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
    QPushButton *but_continue;
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

    QString urlprogram;//软件下载路径
    QString exename;//软件保存名称
    QString runPath;//软件保存目录
//    CURLDownloadManager *downloader;
signals:
    void DownisBusy(QString);
    void DownFinished();

public slots:
    void on_download_clicked();
    void startProgress(int);
    void changevalued(int);
    void continueProgress_download();
    void cancelProgress_download();
    void suspendProgress_download();
    void cancelProgress_setup();
    bool suspendProgress_setup();
    void Downloadresult(int);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    bool mouse_press;
    bool mouse_enter;

};
#endif // SOFTDOWNLOADITEM_H

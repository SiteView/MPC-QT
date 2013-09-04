#ifndef SOFTALLKINDITEM_H
#define SOFTALLKINDITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QProgressBar>
#include <QStackedWidget>
class  CURLDownloadManager;

class SoftUpgradeItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUpgradeItem(QWidget *parent = 0);
    QString get_size( qint64 byte );

signals:

public slots:
   void on_but_upgrade_clicked();

   void startProgress(int);
   void changevalued(int);
   void cancelProgress_download();
   void suspendProgress_download();
   void cancelProgress_setup();
   bool suspendProgress_setup();
   void Downloadresult(int);
public:


   QFrame *frame_1 ;
   QPushButton *icon ;
   QLabel *softname;
   QLabel *softdetaile;
   QPushButton *but_more;
   QLabel *old_versions;
   QLabel *new_versions;

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
   QPushButton *but_upgrade;
   QFrame *frame_4;

   QLabel *lab_upnum_2;
   QLabel *lab_size_2;
   QPushButton *setup;

   QProgressBar *progressBar;

   QString urlprogram;
   QString exename;
   QString runPath;
   CURLDownloadManager *downloader;
public:
    void DownloadThread();
};

#endif // SOFTALLKINDITEM_H

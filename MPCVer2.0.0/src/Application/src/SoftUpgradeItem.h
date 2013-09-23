#ifndef SOFTUPGRADEITEM_H
#define SOFTUPGRADEITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QProgressBar>
#include <QStackedWidget>

class SoftUpgradeItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUpgradeItem(QWidget *parent = 0);
    void DownloadThread();
    void takeText(QString,QString,QString,QString,QString,qint64,QString);
    QString get_size( qint64 byte );

public:
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
   QPushButton *but_continue;
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

signals:

public slots:
   void on_but_upgrade_clicked();
   void on_icon_clicked();
   void on_but_more_clicked();
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

#endif // SOFTUPGRADEITEM_H

#ifndef SOFTUNLOADITEM_H
#define SOFTUNLOADITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include "../Common/SqliteDb.h"

class SoftUnloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUnloadItem(QWidget *parent = 0);


    void takeText(QString Qicon,QString Qsoftname,
                  QString Qsoftdetail,qint64 Qsize,
                  QString Qsetuptime,QString Qprogress,
                  QString QuninstallString);
    QString get_size( qint64 byte );

signals:

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    bool mouse_press;
    bool mouse_enter;
    bool uninstall_visible;
public slots:
    void on_unload_clicked();
    void Unloadfinish(int , QProcess::ExitStatus );
    void Unloaderror(QProcess::ProcessError );
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

    CSQLiteDb m_SQLiteDb;

};

#endif // SOFTUNLOADITEM_H

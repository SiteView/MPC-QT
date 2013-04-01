#ifndef START_H
#define START_H
#include <QUrl>
#include <QWidget>
#include "downloadthread.h"
#include <QStringList>
class start : public QWidget
{
    Q_OBJECT
public:
    explicit start(QWidget *parent = 0);
    
signals:
    
public slots:
    void OnBtnDownloadClicked();

private:
    DownloadThread *downloadThread;
    QList <DownloadThread *> taskThreadList;
    struct _NotDownload
    {
        int taskThreadListId;
        qint64 startPosition;
        qint64 endPosition;
    };
    typedef struct _NotDownload NotDownload;
    NotDownload *notDownload;
    struct _TaskInfomation
    {
        int taskInfoListId;
        int controlState;
        int state;
        QUrl url;
        QStringList urlsStringList;
        bool noProxyBool;
        bool singleProxyBool;
        int singleProxyId;
        bool mutipleProxyBool;
        QList <int> mutipleProxyIdsList;
        QString referrer;
        QString saveTo;
        QString rename;
        QString comment;
        QString user;
        QString password;
        int splite;
        int retry;
        qint64 size;
        qint64 completed;
        qint64 lastCompleted;
        qint64 startPosition;
        qint64 speed;
        qint64 elapsedTime;
        QList <qint64> speedPointList;
        QList <NotDownload *> notDownloadList;
        QList <DownloadThread *> taskThreadList;
    };
    typedef struct _TaskInfomation TaskInfomation;
    QList <TaskInfomation *> taskInfoList;
};

#endif // START_H

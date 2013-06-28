#ifndef DATA_H
#define DATA_H

#include "downloadthread.h"
struct _NotDownload
{
    int taskThreadListId;
    qint64 startPosition;
    qint64 endPosition;
};
typedef struct _NotDownload NotDownload;

struct _TaskInformation
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
    QString applicationName;
    QString silentParam;
    QString currentVersion;
    QString updateVersion;
    int     versionStatus;
    //aaron 0408 uninstall
    QString uninstallString;
};
typedef struct _TaskInformation TaskInformation;
#endif

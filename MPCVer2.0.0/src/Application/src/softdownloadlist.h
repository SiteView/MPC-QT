#ifndef SOFTDOWNLOADLIST_H
#define SOFTDOWNLOADLIST_H

#include <QWidget>
#include <QListWidget>
#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"

class  CURLDownloadManager;

class SoftDownloadList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadList(QWidget *parent = 0);

signals:

public slots:

public:
    CSQLiteDb m_SQLiteDb;
    QListWidget *DownloadList;
    void DownloadThread();
//    SoftDownloadItem downloaditem;
    CURLDownloadManager *downloader;
    SoftDownloadItem *ani;
};

#endif // SOFTDOWNLOADLIST_H

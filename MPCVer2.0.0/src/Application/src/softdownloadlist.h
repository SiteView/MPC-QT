#ifndef SOFTDOWNLOADLIST_H
#define SOFTDOWNLOADLIST_H

#include <QWidget>
#include <QListWidget>

#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"


class SoftDownloadList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadList(QWidget *parent = 0);
    void selectDifType(int);
signals:

public slots:

public:
    CSQLiteDb m_SQLiteDb;
    QListWidget *DownloadList;

    SoftDownloadItem *ani;

};

#endif // SOFTDOWNLOADLIST_H

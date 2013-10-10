#ifndef SOFTDOWNLOADLIST_H
#define SOFTDOWNLOADLIST_H

#include <QWidget>
#include <QListWidget>
#include <QStringList>
#include <QQueue>
#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"


class SoftDownloadList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadList(QWidget *parent = 0);
    void selectDifType(int);

signals:
    void Filled(bool);
public slots:
    void FillQueue(QString);
    void AddItemQueue();

public:
    CSQLiteDb m_SQLiteDb;
    QListWidget *DownloadList;

    SoftDownloadItem *ani;

    QStringList list_0;
    QStringList list_1;
    QStringList list_2;
    QStringList list_3;
    QList<qint64> list_4;
    QStringList list_5;
    QStringList list_6;

    QList<SoftDownloadItem *> *item_list ;
    QList<QListWidgetItem*> *twi_list;
    QList<SoftDownloadItem *> *item_qlist ;
    QQueue<QString> queue;

    QString search_text;
    bool empty;
    bool filled;
    int num;

};

#endif // SOFTDOWNLOADLIST_H

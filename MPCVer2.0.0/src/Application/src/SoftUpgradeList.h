#ifndef SOFTUPGRADELIST_H
#define SOFTUPGRADELIST_H

#include <QWidget>
#include <QListWidget>
#include <QStringList>
#include "SoftUpgradeItem.h"
#include "SoftDownloadItem.h"
#include "../Common/SqliteDb.h"

class SoftUpgradeList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUpgradeList(QWidget *parent = 0);
    void selectDifType();

signals:
    
public slots:

public:
    CSQLiteDb m_SQLiteDb;
    QListWidget *list_softupgrade;
    QString search_text;
    bool empty;

    QStringList list_0;
    QStringList list_1;
    QStringList list_2;
    QStringList list_3;
    QStringList list_4;
    QList<qint64> list_5;
    QStringList list_6;
    QList<SoftUpgradeItem *> *item_list;
    QList<QListWidgetItem *> *twi_list ;
};

#endif // SOFTUPGRADELIST_H

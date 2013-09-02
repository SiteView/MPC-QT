#ifndef SOFTUPGRADELIST_H
#define SOFTUPGRADELIST_H

#include <QWidget>
#include <QListWidget>

#include "../Common/SqliteDb.h"
class SoftUpgradeList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUpgradeList(QWidget *parent = 0);

signals:
    
public slots:

public:
    CSQLiteDb m_SQLiteDb;
    QListWidget *list_softupgrade;

};

#endif // SOFTUPGRADELIST_H

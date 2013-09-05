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
    QString  get_size( qint64 byte );

signals:
    
public slots:

public:
    CSQLiteDb m_SQLiteDb;
    QListWidget *list_softupgrade;
    QString search_text;
    bool empty;

};

#endif // SOFTUPGRADELIST_H

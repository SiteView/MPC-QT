#ifndef SOFTUNLOADLIST_H
#define SOFTUNLOADLIST_H

#include <QWidget>

#include <QListWidget>

#include <QDebug>

#include "SoftUnloadItem.h"
#include "../Common/SqliteDb.h"

class SoftUnloadList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUnloadList(QWidget *parent = 0);
    
signals:
    
public slots:

public:
    CSQLiteDb m_SQLiteDb;
    QListWidget *UnloadList;
//    SoftUnloadItem *ani;

};

#endif // SOFTUNLOADLIST_H

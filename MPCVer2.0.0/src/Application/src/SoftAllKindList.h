#ifndef SOFTALLKINDS_H
#define SOFTALLKINDS_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include "../Common/SqliteDb.h"

#include "SoftAllKindItem.h"
class SoftAllKindList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftAllKindList(QWidget *parent = 0);
    
signals:
    void currentRowChanged();
    
public slots:
    void changeCurrentItem();
public:
    QListWidget                   *list_softallkind;
//    SoftAllKindItem               *allkinditem;
    CSQLiteDb m_SQLiteDb;

public:
    int type;
    QString lab_text;
};

#endif // SOFTALLKINDS_H

#ifndef SOFTALLKINDS_H
#define SOFTALLKINDS_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include "SoftAllKindItem.h"
class SoftAllKindList : public QWidget
{
    Q_OBJECT
public:
    explicit SoftAllKindList(QWidget *parent = 0);
    
signals:
    
public slots:
public:
    QListWidget                   *list_softallkind;
    SoftAllKindItem               *allkinditem;
    
};

#endif // SOFTALLKINDS_H

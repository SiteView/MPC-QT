#ifndef SOFTALLKINDITEM_H
#define SOFTALLKINDITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class SoftUpgradeItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUpgradeItem(QWidget *parent = 0);

signals:

public slots:
public:

    QPushButton *icon;
    QLabel *softname;
    QLabel *softdetaile;
    QPushButton *but_more;
    QLabel *old_versions;
    QLabel *new_versions;
    QLabel *size;
    QPushButton *but_upgrade;

public:

};

#endif // SOFTALLKINDITEM_H

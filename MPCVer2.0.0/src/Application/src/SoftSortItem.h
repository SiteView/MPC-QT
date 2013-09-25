#ifndef SOFTSORTITEM_H
#define SOFTSORTITEM_H

#include <QWidget>
#include <QLabel>
class SoftSortItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftSortItem(QWidget *parent = 0);
    void takeText(QString ,QString);

signals:
    
public slots:

public:
    QLabel *ico;
    QLabel *text;
    
};

#endif // SOFTSORTITEM_H

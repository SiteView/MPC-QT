#ifndef SOFTALLKINDITEM_H
#define SOFTALLKINDITEM_H

#include <QWidget>
#include <QLabel>
class SoftAllKindItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftAllKindItem(QWidget *parent = 0);
    
signals:
    
public slots:
public:
    QLabel *ico;
    QLabel *text;
    QLabel *num;

public:
    void retranslateUi();
    QString get_text();

    void takeText(QString ,QString ,int);
protected:

};

#endif // SOFTALLKINDITEM_H

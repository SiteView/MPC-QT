#ifndef SOFTDOWNLOADITEM_H
#define SOFTDOWNLOADITEM_H

#include <QLabel>
#include <QEvent>
#include <QString>
#include <QPushButton>
#include "../Common/SqliteDb.h"
//#include "../Common/SytemLog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
class SoftDownloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadItem(QWidget *parent = 0);
public:
    //    QLabel *icon;
    //    QLabel *softname;
    //    QLabel *softdetail;
    //    QLabel *progress;
    //    QLabel *speed;
    //    QLabel *comment;
    //    QPushButton *download;
    //    QWidget *horizontalLayoutWidget;
    //    QHBoxLayout *horizontalLayout;
    QLabel *icon;
    //    QSpacerItem *horizontalSpacer_2;
    //    QVBoxLayout *verticalLayout;
    QLabel *softname;
    QLabel *softdetail;
    //    QSpacerItem *verticalSpacer;
    QLabel *size;
    QLabel *setuptime;
    QLabel *progress;
    QPushButton *download;
    //    QSpacerItem *horizontalSpacer;
public:
    void retranslateUi();
    CSQLiteDb m_SQLiteDb;

signals:

public slots:

};
#endif // SOFTDOWNLOADITEM_H

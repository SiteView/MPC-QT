#ifndef SOFTUPGRADEITEM_H
#define SOFTUPGRADEITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

class SoftUpgradeItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftUpgradeItem(QWidget *parent = 0);
    
signals:
    
public slots:
    void on_but_upgrade_clicked();
    void on_but_suspend_clicked();
    void on_but_cancel_clicked();
    void on_but_downorup_clicked();

public:
    QCheckBox *che_title;
    QPushButton *but_num;
    QLabel *lab_upgradeto;
    QLabel *lab_version;
    QPushButton *but_downorup;

    QFrame *frame_1;
    QCheckBox *check_content;
    QPushButton *but_icon;
    QWidget *layoutWidget;
    QPushButton *but_softname;
    QLabel *lab_softdetail;
    QPushButton *but_more;
    QWidget *layoutWidget1;
    QLabel *lab_curversion;
    QLabel *lab_newversion;
    QFrame *frame_3;
    QLabel *lab_prompt;
    QPushButton *but_suspend;
    QPushButton *but_cancel;
    QPushButton *but_progress;
    QFrame *frame_2;
    QLabel *lab_upnum;
    QLabel *lab_size;
    QPushButton *but_upgrade;
    QPushButton *pushButton;
public:
   void AddListTitleui();
   void AddListContentui();
    
};

#endif // SOFTUPGRADEITEM_H

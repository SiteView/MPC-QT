
#include <QSpacerItem>
#include <QHBoxLayout>

#include "SoftUpgradeItem.h"

SoftUpgradeItem::SoftUpgradeItem(QWidget *parent) :
    QWidget(parent)
{
    frame_3->setVisible(false);
    connect(but_downorup,SIGNAL(cliked()),this,SIGNAL(on_but_downorup_clicked()));
    connect(but_cancel,SIGNAL(cliked()),this,SIGNAL(on_but_cancel_clicked()));
    connect(but_suspend,SIGNAL(cliked()),this,SIGNAL(on_but_suspend_clicked()));
    connect(but_upgrade,SIGNAL(cliked()),this,SIGNAL(on_but_upgrade_clicked()));
}

void SoftUpgradeItem::AddListTitleui()//创建软件升级分类标题界面
{
    che_title = new QCheckBox();
    che_title->setMaximumSize(QSize(15, 15));
    che_title->setObjectName(QString::fromUtf8("che_title"));

    but_num = new QPushButton();
    but_num->setMaximumSize(QSize(35, 20));
    but_num->setObjectName(QString::fromUtf8("but_num"));

    lab_upgradeto = new QLabel();
    lab_upgradeto->setMaximumSize(QSize(60, 20));
    lab_upgradeto->setObjectName(QString::fromUtf8("lab_upgradeto"));

    lab_version = new QLabel();
    lab_version->setMaximumSize(QSize(60, 20));
    lab_version->setObjectName(QString::fromUtf8("lab_version"));

    but_downorup = new QPushButton();
    but_downorup->setMaximumSize(QSize(60, 20));
    but_downorup->setObjectName(QString::fromUtf8("but_downorup"));


    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_2->addWidget(che_title);
    horizontalLayout_2->addWidget(but_num);
    horizontalLayout_2->addWidget(lab_upgradeto);
    horizontalLayout_2->addWidget(lab_version);
    QSpacerItem *horizontalSpacer = new QSpacerItem(550, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer);
    horizontalLayout_2->addWidget(but_downorup);
    this->setLayout(horizontalLayout_2);
}

void SoftUpgradeItem::AddListContentui()//创建软件升级对象的显示界面
{

    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(6);
    horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
    frame_1 = new QFrame();
    frame_1->setObjectName(QString::fromUtf8("frame_1"));
    frame_1->setMaximumSize(QSize(500, 70));
    frame_1->setFrameShape(QFrame::StyledPanel);
    frame_1->setFrameShadow(QFrame::Raised);
    check_content = new QCheckBox(frame_1);
    check_content->setObjectName(QString::fromUtf8("check_content"));
    check_content->setGeometry(QRect(5, 28, 15, 15));
    check_content->setMaximumSize(QSize(15, 15));
    but_icon = new QPushButton(frame_1);
    but_icon->setObjectName(QString::fromUtf8("but_icon"));
    but_icon->setGeometry(QRect(20, 10, 50, 50));
    but_icon->setMaximumSize(QSize(50, 50));
    layoutWidget = new QWidget(frame_1);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(70, 0, 241, 71));
    QVBoxLayout *verticalLayout = new QVBoxLayout(layoutWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    but_softname = new QPushButton(layoutWidget);
    but_softname->setObjectName(QString::fromUtf8("but_softname"));
    but_softname->setMaximumSize(QSize(120, 30));

    verticalLayout->addWidget(but_softname);

    lab_softdetail = new QLabel(layoutWidget);
    lab_softdetail->setObjectName(QString::fromUtf8("lab_softdetail"));
    lab_softdetail->setMaximumSize(QSize(220, 20));

    verticalLayout->addWidget(lab_softdetail);

    but_more = new QPushButton(frame_1);
    but_more->setObjectName(QString::fromUtf8("but_more"));
    but_more->setGeometry(QRect(310, 30, 30, 20));
    but_more->setMaximumSize(QSize(30, 20));
    layoutWidget1 = new QWidget(frame_1);
    layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
    layoutWidget1->setGeometry(QRect(340, 0, 131, 71));
    QVBoxLayout *verticalLayout_2 = new QVBoxLayout(layoutWidget1);
    verticalLayout_2->setSpacing(0);
    verticalLayout_2->setContentsMargins(11, 11, 11, 11);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    lab_curversion = new QLabel(layoutWidget1);
    lab_curversion->setObjectName(QString::fromUtf8("lab_curversion"));
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(lab_curversion->sizePolicy().hasHeightForWidth());
    lab_curversion->setSizePolicy(sizePolicy);
    lab_curversion->setMaximumSize(QSize(120, 20));

    verticalLayout_2->addWidget(lab_curversion);

    lab_newversion = new QLabel(layoutWidget1);
    lab_newversion->setObjectName(QString::fromUtf8("lab_newversion"));
    sizePolicy.setHeightForWidth(lab_newversion->sizePolicy().hasHeightForWidth());
    lab_newversion->setSizePolicy(sizePolicy);
    lab_newversion->setMaximumSize(QSize(120, 20));

    verticalLayout_2->addWidget(lab_newversion);

    but_icon->raise();
    layoutWidget->raise();
    but_more->raise();
    layoutWidget->raise();
    check_content->raise();

    horizontalLayout_3->addWidget(frame_1);

    frame_3 = new QFrame();
    frame_3->setObjectName(QString::fromUtf8("frame_3"));
    frame_3->setMaximumSize(QSize(320, 70));
    frame_3->setFrameShape(QFrame::StyledPanel);
    frame_3->setFrameShadow(QFrame::Raised);
    lab_prompt = new QLabel(frame_3);
    lab_prompt->setObjectName(QString::fromUtf8("lab_prompt"));
    lab_prompt->setGeometry(QRect(140, 25, 91, 16));
    but_suspend = new QPushButton(frame_3);
    but_suspend->setObjectName(QString::fromUtf8("but_suspend"));
    but_suspend->setGeometry(QRect(270, 20, 20, 20));
    but_cancel = new QPushButton(frame_3);
    but_cancel->setObjectName(QString::fromUtf8("but_cancel"));
    but_cancel->setGeometry(QRect(290, 20, 20, 20));
    but_progress = new QPushButton(frame_3);
    but_progress->setObjectName(QString::fromUtf8("but_progress"));
    but_progress->setGeometry(QRect(0, 20, 100, 23));

    horizontalLayout_3->addWidget(frame_3);

    frame_2 = new QFrame();
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setMaximumSize(QSize(320, 70));
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    lab_upnum = new QLabel(frame_2);
    lab_upnum->setObjectName(QString::fromUtf8("lab_upnum"));
    lab_upnum->setGeometry(QRect(70, 30, 101, 20));
    lab_upnum->setMaximumSize(QSize(120, 20));
    lab_size = new QLabel(frame_2);
    lab_size->setObjectName(QString::fromUtf8("lab_size"));
    lab_size->setGeometry(QRect(10, 30, 50, 20));
    lab_size->setMaximumSize(QSize(50, 20));
    but_upgrade = new QPushButton(frame_2);
    but_upgrade->setObjectName(QString::fromUtf8("but_upgrade"));
    but_upgrade->setGeometry(QRect(180, 25, 80, 25));
    but_upgrade->setMaximumSize(QSize(80, 25));
    pushButton = new QPushButton(frame_2);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(270, 25, 40, 24));
    horizontalLayout_3->addWidget(frame_2);
    this->setLayout(horizontalLayout_3);

}
void SoftUpgradeItem::on_but_upgrade_clicked()
{
    frame_2->setVisible(false);
    frame_3->setVisible(true);

}
void SoftUpgradeItem::on_but_suspend_clicked()
{

}
void SoftUpgradeItem::on_but_cancel_clicked()
{


}

void SoftUpgradeItem::on_but_downorup_clicked()
{

}

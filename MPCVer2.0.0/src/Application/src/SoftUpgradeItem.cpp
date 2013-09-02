
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QtUiTools/QUiLoader>
#include <QFile>
#include "SoftUpgradeItem.h"
SoftUpgradeItem::SoftUpgradeItem(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);

    icon = new QPushButton(this);
    icon->setObjectName(QString::fromUtf8("icon"));
    icon->setMaximumSize(QSize(45, 45));

    horizontalLayout->addWidget(icon);

    QVBoxLayout *verticalLayout = new QVBoxLayout();

    softname = new QLabel(this);
    softname->setObjectName(QString::fromUtf8("softname"));
    softname->setMaximumSize(QSize(315, 25));

    verticalLayout->addWidget(softname);

    softdetaile = new QLabel(this);
    softdetaile->setObjectName(QString::fromUtf8("softdetaile"));
    softdetaile->setMaximumSize(QSize(320, 20));

    verticalLayout->addWidget(softdetaile);


    horizontalLayout->addLayout(verticalLayout);

    but_more = new QPushButton(this);
    but_more->setObjectName(QString::fromUtf8("but_more"));
    but_more->setMaximumSize(QSize(40, 20));

    horizontalLayout->addWidget(but_more);

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout();

    old_versions = new QLabel(this);
    old_versions->setObjectName(QString::fromUtf8("old_versions"));
    old_versions->setMaximumSize(QSize(150, 20));

    verticalLayout_2->addWidget(old_versions);

    new_versions = new QLabel(this);
    new_versions->setObjectName(QString::fromUtf8("new_versions"));
    new_versions->setMaximumSize(QSize(150, 20));

    verticalLayout_2->addWidget(new_versions);


    horizontalLayout->addLayout(verticalLayout_2);

    size = new QLabel(this);
    size->setObjectName(QString::fromUtf8("size"));
    size->setMaximumSize(QSize(180, 20));

    horizontalLayout->addWidget(size);

    but_upgrade = new QPushButton(this);
    but_upgrade->setObjectName(QString::fromUtf8("but_upgrade"));
    but_upgrade->setMaximumSize(QSize(80, 25));

    horizontalLayout->addWidget(but_upgrade);

}

/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QTableView *table_updateInform;
    QFrame *frame_2;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *but_logo;
    QLabel *lab_Informtitle;
    QSpacerItem *horizontalSpacer;
    QLabel *lab_set;
    QPushButton *but_close;
    QLabel *lab_suggestupdate;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *lab_prompt;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *but_checkall;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(400, 337);
        table_updateInform = new QTableView(Dialog);
        table_updateInform->setObjectName(QStringLiteral("table_updateInform"));
        table_updateInform->setGeometry(QRect(0, 80, 401, 201));
        frame_2 = new QFrame(Dialog);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(0, 0, 401, 31));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayoutWidget_2 = new QWidget(frame_2);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(0, 0, 401, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        but_logo = new QPushButton(horizontalLayoutWidget_2);
        but_logo->setObjectName(QStringLiteral("but_logo"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(30);
        sizePolicy.setVerticalStretch(30);
        sizePolicy.setHeightForWidth(but_logo->sizePolicy().hasHeightForWidth());
        but_logo->setSizePolicy(sizePolicy);
        but_logo->setMaximumSize(QSize(40, 40));

        horizontalLayout_3->addWidget(but_logo);

        lab_Informtitle = new QLabel(horizontalLayoutWidget_2);
        lab_Informtitle->setObjectName(QStringLiteral("lab_Informtitle"));
        lab_Informtitle->setMaximumSize(QSize(120, 40));

        horizontalLayout_3->addWidget(lab_Informtitle);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        lab_set = new QLabel(horizontalLayoutWidget_2);
        lab_set->setObjectName(QStringLiteral("lab_set"));

        horizontalLayout_3->addWidget(lab_set);

        but_close = new QPushButton(horizontalLayoutWidget_2);
        but_close->setObjectName(QStringLiteral("but_close"));
        but_close->setMaximumSize(QSize(40, 40));

        horizontalLayout_3->addWidget(but_close);

        lab_suggestupdate = new QLabel(Dialog);
        lab_suggestupdate->setObjectName(QStringLiteral("lab_suggestupdate"));
        lab_suggestupdate->setGeometry(QRect(0, 30, 401, 51));
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 279, 401, 61));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lab_prompt = new QLabel(horizontalLayoutWidget);
        lab_prompt->setObjectName(QStringLiteral("lab_prompt"));
        lab_prompt->setMaximumSize(QSize(120, 40));

        horizontalLayout->addWidget(lab_prompt);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        but_checkall = new QPushButton(horizontalLayoutWidget);
        but_checkall->setObjectName(QStringLiteral("but_checkall"));
        but_checkall->setMaximumSize(QSize(90, 30));

        horizontalLayout->addWidget(but_checkall);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "MPC", 0));
        but_logo->setText(QApplication::translate("Dialog", "PushButton", 0));
        lab_Informtitle->setText(QApplication::translate("Dialog", "TextLabel", 0));
        lab_set->setText(QApplication::translate("Dialog", "TextLabel", 0));
        but_close->setText(QApplication::translate("Dialog", "PushButton", 0));
        lab_suggestupdate->setText(QApplication::translate("Dialog", "TextLabel", 0));
        lab_prompt->setText(QApplication::translate("Dialog", "TextLabel", 0));
        but_checkall->setText(QApplication::translate("Dialog", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H

/********************************************************************************
** Form generated from reading UI file 'informdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFORMDIALOG_H
#define UI_INFORMDIALOG_H

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

class Ui_InformDialog
{
public:
    QTableView *table_updateInform;
    QFrame *frame_2;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_logo;
    QLabel *lab_Informtitle;
    QSpacerItem *horizontalSpacer;
    QLabel *lab_set;
    QPushButton *but_close;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *lab_notice;
    QLabel *lab_prompt;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *but_checkall;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *lab_update;
    QLabel *lab_suggestupdate;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *InformDialog)
    {
        if (InformDialog->objectName().isEmpty())
            InformDialog->setObjectName(QStringLiteral("InformDialog"));
        InformDialog->resize(400, 337);
        table_updateInform = new QTableView(InformDialog);
        table_updateInform->setObjectName(QStringLiteral("table_updateInform"));
        table_updateInform->setGeometry(QRect(0, 80, 401, 201));
        frame_2 = new QFrame(InformDialog);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(0, 0, 401, 31));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayoutWidget_2 = new QWidget(frame_2);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(0, 0, 401, 32));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        lab_logo = new QLabel(horizontalLayoutWidget_2);
        lab_logo->setObjectName(QStringLiteral("lab_logo"));
        lab_logo->setMinimumSize(QSize(30, 30));
        lab_logo->setMaximumSize(QSize(30, 30));

        horizontalLayout_3->addWidget(lab_logo);

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

        horizontalLayoutWidget = new QWidget(InformDialog);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 279, 401, 61));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lab_notice = new QLabel(horizontalLayoutWidget);
        lab_notice->setObjectName(QStringLiteral("lab_notice"));
        lab_notice->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(lab_notice);

        lab_prompt = new QLabel(horizontalLayoutWidget);
        lab_prompt->setObjectName(QStringLiteral("lab_prompt"));
        lab_prompt->setMaximumSize(QSize(300, 40));

        horizontalLayout->addWidget(lab_prompt);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        but_checkall = new QPushButton(horizontalLayoutWidget);
        but_checkall->setObjectName(QStringLiteral("but_checkall"));
        but_checkall->setMaximumSize(QSize(90, 30));

        horizontalLayout->addWidget(but_checkall);

        horizontalLayoutWidget_3 = new QWidget(InformDialog);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(0, 30, 401, 51));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        lab_update = new QLabel(horizontalLayoutWidget_3);
        lab_update->setObjectName(QStringLiteral("lab_update"));
        lab_update->setMaximumSize(QSize(40, 40));

        horizontalLayout_2->addWidget(lab_update);

        lab_suggestupdate = new QLabel(horizontalLayoutWidget_3);
        lab_suggestupdate->setObjectName(QStringLiteral("lab_suggestupdate"));
        lab_suggestupdate->setMaximumSize(QSize(300, 40));

        horizontalLayout_2->addWidget(lab_suggestupdate);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        retranslateUi(InformDialog);

        QMetaObject::connectSlotsByName(InformDialog);
    } // setupUi

    void retranslateUi(QDialog *InformDialog)
    {
        InformDialog->setWindowTitle(QApplication::translate("InformDialog", "MPC", 0));
        lab_logo->setText(QString());
        lab_Informtitle->setText(QApplication::translate("InformDialog", "360\350\275\257\344\273\266\347\256\241\345\256\266-\350\275\257\344\273\266\345\215\207\347\272\247", 0));
        lab_set->setText(QApplication::translate("InformDialog", "\350\256\276\347\275\256", 0));
        but_close->setText(QString());
        lab_notice->setText(QString());
        lab_prompt->setText(QApplication::translate("InformDialog", "\345\256\232\346\234\237\345\215\207\347\272\247\350\275\257\344\273\266\357\274\214\346\235\234\347\273\235\350\275\257\344\273\266\346\263\204\346\274\217\357\274\201", 0));
        but_checkall->setText(QApplication::translate("InformDialog", "\346\237\245\347\234\213\345\205\250\351\203\250", 0));
        lab_update->setText(QString());
        lab_suggestupdate->setText(QApplication::translate("InformDialog", "\345\217\221\347\216\260\350\275\257\344\273\266\346\234\211\351\234\200\350\246\201\345\215\207\347\272\247\347\232\204\350\275\257\344\273\266", 0));
    } // retranslateUi

};

namespace Ui {
    class InformDialog: public Ui_InformDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFORMDIALOG_H

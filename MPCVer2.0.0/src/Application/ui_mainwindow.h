/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *title;
    QPushButton *but_close;
    QPushButton *but_minimize;
    QWidget *title_page;
    QWidget *widget_body;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_SoftDownload;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_s_4;
    QLineEdit *lineEdit_s_4;
    QPushButton *but_clear_4;
    QPushButton *but_search_4;
    QWidget *widget_4;
    QStackedWidget *stack_download;
    QWidget *page_all;
    QWidget *page_type1;
    QWidget *page_type9;
    QWidget *page_null_default;
    QLabel *lab_null_caution;
    QLabel *lab_null_text;
    QWidget *page_type10;
    QWidget *page_type4;
    QWidget *page_type5;
    QWidget *page_type7;
    QWidget *page_type6;
    QWidget *page_type3;
    QWidget *page_type8;
    QWidget *page_type2;
    QWidget *page_search;
    QWidget *widget_2;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *but_return;
    QLabel *lab_softnum;
    QWidget *page_SoftUpgrade;
    QLabel *label_3;
    QWidget *widget_s_3;
    QLineEdit *lineEdit_s_3;
    QPushButton *but_clear_3;
    QPushButton *but_search_3;
    QWidget *widget_6;
    QWidget *page_SoftUnload;
    QWidget *widget_3;
    QStackedWidget *stackedWidget_2;
    QWidget *page_search_2;
    QWidget *page_up;
    QWidget *page_down;
    QWidget *page_2;
    QWidget *page_3;
    QWidget *page_null_unload;
    QLabel *lab_caution_unload;
    QLabel *lab_text_unload;
    QWidget *page_selall;
    QWidget *page_4;
    QWidget *page_5;
    QWidget *page_6;
    QWidget *page_7;
    QWidget *widget_5;
    QPushButton *but_sel_size;
    QPushButton *but_sel_time;
    QPushButton *but_sel_path;
    QPushButton *but_sel_operate;
    QPushButton *but_sel_name;
    QWidget *widget_s_2;
    QLineEdit *lineEdit_s_2;
    QPushButton *but_clear_2;
    QPushButton *but_search_2;
    QPushButton *but_return_2;
    QWidget *page_UpdateInform;
    QListWidget *list_Inform;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(920, 680);
        MainWindow->setMinimumSize(QSize(820, 604));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        title = new QWidget(centralWidget);
        title->setObjectName(QStringLiteral("title"));
        title->setGeometry(QRect(0, 0, 920, 100));
        but_close = new QPushButton(title);
        but_close->setObjectName(QStringLiteral("but_close"));
        but_close->setGeometry(QRect(895, 0, 25, 21));
        but_minimize = new QPushButton(title);
        but_minimize->setObjectName(QStringLiteral("but_minimize"));
        but_minimize->setGeometry(QRect(870, 0, 25, 21));
        title_page = new QWidget(title);
        title_page->setObjectName(QStringLiteral("title_page"));
        title_page->setGeometry(QRect(0, 0, 320, 100));
        widget_body = new QWidget(centralWidget);
        widget_body->setObjectName(QStringLiteral("widget_body"));
        widget_body->setGeometry(QRect(0, 100, 920, 580));
        stackedWidget = new QStackedWidget(widget_body);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 920, 560));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        stackedWidget->addWidget(page);
        page_SoftDownload = new QWidget();
        page_SoftDownload->setObjectName(QStringLiteral("page_SoftDownload"));
        widget = new QWidget(page_SoftDownload);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(600, 0, 315, 50));
        widget->setMaximumSize(QSize(315, 50));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widget_s_4 = new QWidget(widget);
        widget_s_4->setObjectName(QStringLiteral("widget_s_4"));
        widget_s_4->setMaximumSize(QSize(295, 25));
        lineEdit_s_4 = new QLineEdit(widget_s_4);
        lineEdit_s_4->setObjectName(QStringLiteral("lineEdit_s_4"));
        lineEdit_s_4->setGeometry(QRect(1, 1, 218, 23));
        lineEdit_s_4->setMaximumSize(QSize(218, 23));
        but_clear_4 = new QPushButton(widget_s_4);
        but_clear_4->setObjectName(QStringLiteral("but_clear_4"));
        but_clear_4->setGeometry(QRect(218, 5, 16, 16));
        but_search_4 = new QPushButton(widget_s_4);
        but_search_4->setObjectName(QStringLiteral("but_search_4"));
        but_search_4->setGeometry(QRect(238, 0, 56, 25));
        but_search_4->setMaximumSize(QSize(56, 25));

        horizontalLayout_2->addWidget(widget_s_4);

        widget_4 = new QWidget(page_SoftDownload);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setGeometry(QRect(0, 0, 160, 560));
        stack_download = new QStackedWidget(page_SoftDownload);
        stack_download->setObjectName(QStringLiteral("stack_download"));
        stack_download->setGeometry(QRect(160, 50, 760, 510));
        page_all = new QWidget();
        page_all->setObjectName(QStringLiteral("page_all"));
        stack_download->addWidget(page_all);
        page_type1 = new QWidget();
        page_type1->setObjectName(QStringLiteral("page_type1"));
        stack_download->addWidget(page_type1);
        page_type9 = new QWidget();
        page_type9->setObjectName(QStringLiteral("page_type9"));
        stack_download->addWidget(page_type9);
        page_null_default = new QWidget();
        page_null_default->setObjectName(QStringLiteral("page_null_default"));
        lab_null_caution = new QLabel(page_null_default);
        lab_null_caution->setObjectName(QStringLiteral("lab_null_caution"));
        lab_null_caution->setGeometry(QRect(180, 180, 24, 24));
        lab_null_text = new QLabel(page_null_default);
        lab_null_text->setObjectName(QStringLiteral("lab_null_text"));
        lab_null_text->setGeometry(QRect(220, 180, 450, 24));
        stack_download->addWidget(page_null_default);
        page_type10 = new QWidget();
        page_type10->setObjectName(QStringLiteral("page_type10"));
        stack_download->addWidget(page_type10);
        page_type4 = new QWidget();
        page_type4->setObjectName(QStringLiteral("page_type4"));
        stack_download->addWidget(page_type4);
        page_type5 = new QWidget();
        page_type5->setObjectName(QStringLiteral("page_type5"));
        stack_download->addWidget(page_type5);
        page_type7 = new QWidget();
        page_type7->setObjectName(QStringLiteral("page_type7"));
        stack_download->addWidget(page_type7);
        page_type6 = new QWidget();
        page_type6->setObjectName(QStringLiteral("page_type6"));
        stack_download->addWidget(page_type6);
        page_type3 = new QWidget();
        page_type3->setObjectName(QStringLiteral("page_type3"));
        stack_download->addWidget(page_type3);
        page_type8 = new QWidget();
        page_type8->setObjectName(QStringLiteral("page_type8"));
        stack_download->addWidget(page_type8);
        page_type2 = new QWidget();
        page_type2->setObjectName(QStringLiteral("page_type2"));
        stack_download->addWidget(page_type2);
        page_search = new QWidget();
        page_search->setObjectName(QStringLiteral("page_search"));
        stack_download->addWidget(page_search);
        widget_2 = new QWidget(page_SoftDownload);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setGeometry(QRect(175, 8, 150, 41));
        widget_2->setMaximumSize(QSize(150, 50));
        horizontalLayoutWidget = new QWidget(widget_2);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 151, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        but_return = new QPushButton(horizontalLayoutWidget);
        but_return->setObjectName(QStringLiteral("but_return"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(but_return->sizePolicy().hasHeightForWidth());
        but_return->setSizePolicy(sizePolicy);
        but_return->setMinimumSize(QSize(79, 29));
        but_return->setMaximumSize(QSize(79, 29));
        but_return->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(but_return);

        lab_softnum = new QLabel(horizontalLayoutWidget);
        lab_softnum->setObjectName(QStringLiteral("lab_softnum"));
        lab_softnum->setMaximumSize(QSize(130, 30));
        lab_softnum->setContextMenuPolicy(Qt::DefaultContextMenu);
        lab_softnum->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lab_softnum);

        stackedWidget->addWidget(page_SoftDownload);
        page_SoftUpgrade = new QWidget();
        page_SoftUpgrade->setObjectName(QStringLiteral("page_SoftUpgrade"));
        label_3 = new QLabel(page_SoftUpgrade);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 0, 551, 31));
        widget_s_3 = new QWidget(page_SoftUpgrade);
        widget_s_3->setObjectName(QStringLiteral("widget_s_3"));
        widget_s_3->setGeometry(QRect(620, 5, 295, 25));
        widget_s_3->setMaximumSize(QSize(295, 25));
        lineEdit_s_3 = new QLineEdit(widget_s_3);
        lineEdit_s_3->setObjectName(QStringLiteral("lineEdit_s_3"));
        lineEdit_s_3->setGeometry(QRect(1, 1, 218, 23));
        lineEdit_s_3->setMaximumSize(QSize(218, 23));
        but_clear_3 = new QPushButton(widget_s_3);
        but_clear_3->setObjectName(QStringLiteral("but_clear_3"));
        but_clear_3->setGeometry(QRect(218, 5, 16, 16));
        but_search_3 = new QPushButton(widget_s_3);
        but_search_3->setObjectName(QStringLiteral("but_search_3"));
        but_search_3->setGeometry(QRect(238, 0, 56, 25));
        widget_6 = new QWidget(page_SoftUpgrade);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        widget_6->setGeometry(QRect(0, 40, 920, 520));
        stackedWidget->addWidget(page_SoftUpgrade);
        page_SoftUnload = new QWidget();
        page_SoftUnload->setObjectName(QStringLiteral("page_SoftUnload"));
        widget_3 = new QWidget(page_SoftUnload);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setGeometry(QRect(0, 55, 920, 500));
        stackedWidget_2 = new QStackedWidget(widget_3);
        stackedWidget_2->setObjectName(QStringLiteral("stackedWidget_2"));
        stackedWidget_2->setGeometry(QRect(0, 0, 920, 500));
        page_search_2 = new QWidget();
        page_search_2->setObjectName(QStringLiteral("page_search_2"));
        stackedWidget_2->addWidget(page_search_2);
        page_up = new QWidget();
        page_up->setObjectName(QStringLiteral("page_up"));
        stackedWidget_2->addWidget(page_up);
        page_down = new QWidget();
        page_down->setObjectName(QStringLiteral("page_down"));
        stackedWidget_2->addWidget(page_down);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        stackedWidget_2->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        stackedWidget_2->addWidget(page_3);
        page_null_unload = new QWidget();
        page_null_unload->setObjectName(QStringLiteral("page_null_unload"));
        lab_caution_unload = new QLabel(page_null_unload);
        lab_caution_unload->setObjectName(QStringLiteral("lab_caution_unload"));
        lab_caution_unload->setGeometry(QRect(260, 230, 24, 24));
        lab_text_unload = new QLabel(page_null_unload);
        lab_text_unload->setObjectName(QStringLiteral("lab_text_unload"));
        lab_text_unload->setGeometry(QRect(300, 230, 520, 24));
        stackedWidget_2->addWidget(page_null_unload);
        page_selall = new QWidget();
        page_selall->setObjectName(QStringLiteral("page_selall"));
        stackedWidget_2->addWidget(page_selall);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        stackedWidget_2->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        stackedWidget_2->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QStringLiteral("page_6"));
        stackedWidget_2->addWidget(page_6);
        page_7 = new QWidget();
        page_7->setObjectName(QStringLiteral("page_7"));
        stackedWidget_2->addWidget(page_7);
        widget_5 = new QWidget(page_SoftUnload);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        widget_5->setGeometry(QRect(0, 35, 920, 20));
        but_sel_size = new QPushButton(widget_5);
        but_sel_size->setObjectName(QStringLiteral("but_sel_size"));
        but_sel_size->setGeometry(QRect(398, 0, 110, 20));
        but_sel_time = new QPushButton(widget_5);
        but_sel_time->setObjectName(QStringLiteral("but_sel_time"));
        but_sel_time->setGeometry(QRect(505, 0, 130, 20));
        but_sel_path = new QPushButton(widget_5);
        but_sel_path->setObjectName(QStringLiteral("but_sel_path"));
        but_sel_path->setGeometry(QRect(633, 0, 120, 20));
        but_sel_operate = new QPushButton(widget_5);
        but_sel_operate->setObjectName(QStringLiteral("but_sel_operate"));
        but_sel_operate->setGeometry(QRect(750, 0, 172, 20));
        but_sel_name = new QPushButton(widget_5);
        but_sel_name->setObjectName(QStringLiteral("but_sel_name"));
        but_sel_name->setGeometry(QRect(0, 0, 400, 20));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(but_sel_name->sizePolicy().hasHeightForWidth());
        but_sel_name->setSizePolicy(sizePolicy1);
        but_sel_name->setLayoutDirection(Qt::LeftToRight);
        widget_s_2 = new QWidget(page_SoftUnload);
        widget_s_2->setObjectName(QStringLiteral("widget_s_2"));
        widget_s_2->setGeometry(QRect(620, 5, 295, 25));
        widget_s_2->setMaximumSize(QSize(295, 25));
        lineEdit_s_2 = new QLineEdit(widget_s_2);
        lineEdit_s_2->setObjectName(QStringLiteral("lineEdit_s_2"));
        lineEdit_s_2->setGeometry(QRect(1, 1, 218, 23));
        lineEdit_s_2->setMaximumSize(QSize(218, 23));
        but_clear_2 = new QPushButton(widget_s_2);
        but_clear_2->setObjectName(QStringLiteral("but_clear_2"));
        but_clear_2->setGeometry(QRect(218, 5, 16, 16));
        but_search_2 = new QPushButton(widget_s_2);
        but_search_2->setObjectName(QStringLiteral("but_search_2"));
        but_search_2->setGeometry(QRect(238, 0, 56, 25));
        but_return_2 = new QPushButton(page_SoftUnload);
        but_return_2->setObjectName(QStringLiteral("but_return_2"));
        but_return_2->setGeometry(QRect(8, 5, 80, 28));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(but_return_2->sizePolicy().hasHeightForWidth());
        but_return_2->setSizePolicy(sizePolicy2);
        but_return_2->setMinimumSize(QSize(80, 28));
        but_return_2->setMaximumSize(QSize(80, 28));
        stackedWidget->addWidget(page_SoftUnload);
        page_UpdateInform = new QWidget();
        page_UpdateInform->setObjectName(QStringLiteral("page_UpdateInform"));
        list_Inform = new QListWidget(page_UpdateInform);
        list_Inform->setObjectName(QStringLiteral("list_Inform"));
        list_Inform->setGeometry(QRect(0, 30, 920, 530));
        stackedWidget->addWidget(page_UpdateInform);
        label = new QLabel(widget_body);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(8, 560, 191, 21));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);
        stack_download->setCurrentIndex(3);
        stackedWidget_2->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        but_close->setText(QString());
        but_minimize->setText(QString());
        but_clear_4->setText(QString());
        but_search_4->setText(QString());
        lab_null_caution->setText(QString());
        lab_null_text->setText(QString());
        but_return->setText(QApplication::translate("MainWindow", "return", 0));
        lab_softnum->setText(QString());
        label_3->setText(QString());
        but_clear_3->setText(QString());
        but_search_3->setText(QString());
        lab_caution_unload->setText(QString());
        lab_text_unload->setText(QString());
        but_sel_size->setText(QString());
        but_sel_time->setText(QString());
        but_sel_path->setText(QString());
        but_sel_operate->setText(QString());
        but_sel_name->setText(QString());
        lineEdit_s_2->setText(QString());
        but_clear_2->setText(QString());
        but_search_2->setText(QString());
        but_return_2->setText(QApplication::translate("MainWindow", "return", 0));
        label->setText(QApplication::translate("MainWindow", "Versions\357\274\2322.0.1.7", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

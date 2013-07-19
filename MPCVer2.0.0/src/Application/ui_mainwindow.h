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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *title;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QPushButton *SoftDownload;
    QLabel *label_SoftDownload;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QPushButton *SoftUpgrade;
    QLabel *label_SoftUpgrade;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QPushButton *SoftUnload;
    QLabel *label_SoftUnload;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_4;
    QPushButton *UpdateInform;
    QLabel *label_UpdateInform;
    QSpacerItem *horizontalSpacer;
    QPushButton *but_close;
    QPushButton *but_minimize;
    QPushButton *but_about;
    QWidget *widget_body;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *widget_2;
    QWidget *page_SoftDownload;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_download;
    QSpacerItem *horizontalSpacer_7;
    QLineEdit *lineEdit_2;
    QListWidget *list_Downloadleft;
    QWidget *widget_download;
    QWidget *page_SoftUpgrade;
    QLabel *label_3;
    QListWidget *list_Upgrade;
    QWidget *page_SoftUnload;
    QLabel *label_unload;
    QLineEdit *lineEdit;
    QWidget *widget_3;
    QWidget *page_UpdateInform;
    QListWidget *list_Inform;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(820, 604);
        MainWindow->setMinimumSize(QSize(820, 604));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        title = new QWidget(centralWidget);
        title->setObjectName(QStringLiteral("title"));
        title->setGeometry(QRect(0, 0, 821, 101));
        layoutWidget = new QWidget(title);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(4, 0, 431, 101));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_2->addItem(verticalSpacer);

        SoftDownload = new QPushButton(layoutWidget);
        SoftDownload->setObjectName(QStringLiteral("SoftDownload"));
        SoftDownload->setEnabled(true);
        SoftDownload->setMaximumSize(QSize(80, 80));
        SoftDownload->setIconSize(QSize(80, 80));

        verticalLayout_2->addWidget(SoftDownload);

        label_SoftDownload = new QLabel(layoutWidget);
        label_SoftDownload->setObjectName(QStringLiteral("label_SoftDownload"));
        label_SoftDownload->setMaximumSize(QSize(80, 15));
        label_SoftDownload->setLayoutDirection(Qt::LeftToRight);
        label_SoftDownload->setAlignment(Qt::AlignCenter);
        label_SoftDownload->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        verticalLayout_2->addWidget(label_SoftDownload);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_3->addItem(verticalSpacer_2);

        SoftUpgrade = new QPushButton(layoutWidget);
        SoftUpgrade->setObjectName(QStringLiteral("SoftUpgrade"));
        SoftUpgrade->setMaximumSize(QSize(80, 80));

        verticalLayout_3->addWidget(SoftUpgrade);

        label_SoftUpgrade = new QLabel(layoutWidget);
        label_SoftUpgrade->setObjectName(QStringLiteral("label_SoftUpgrade"));
        label_SoftUpgrade->setMaximumSize(QSize(80, 15));
        label_SoftUpgrade->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_SoftUpgrade);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalSpacer_3 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_4->addItem(verticalSpacer_3);

        SoftUnload = new QPushButton(layoutWidget);
        SoftUnload->setObjectName(QStringLiteral("SoftUnload"));
        SoftUnload->setMaximumSize(QSize(80, 80));

        verticalLayout_4->addWidget(SoftUnload);

        label_SoftUnload = new QLabel(layoutWidget);
        label_SoftUnload->setObjectName(QStringLiteral("label_SoftUnload"));
        label_SoftUnload->setMaximumSize(QSize(80, 15));
        label_SoftUnload->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_SoftUnload);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalSpacer_4 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_5->addItem(verticalSpacer_4);

        UpdateInform = new QPushButton(layoutWidget);
        UpdateInform->setObjectName(QStringLiteral("UpdateInform"));
        UpdateInform->setMaximumSize(QSize(80, 80));

        verticalLayout_5->addWidget(UpdateInform);

        label_UpdateInform = new QLabel(layoutWidget);
        label_UpdateInform->setObjectName(QStringLiteral("label_UpdateInform"));
        label_UpdateInform->setMaximumSize(QSize(80, 15));
        label_UpdateInform->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_UpdateInform);


        horizontalLayout->addLayout(verticalLayout_5);

        horizontalSpacer = new QSpacerItem(10, 10, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        but_close = new QPushButton(title);
        but_close->setObjectName(QStringLiteral("but_close"));
        but_close->setGeometry(QRect(795, 0, 25, 21));
        but_minimize = new QPushButton(title);
        but_minimize->setObjectName(QStringLiteral("but_minimize"));
        but_minimize->setGeometry(QRect(775, 0, 21, 21));
        but_about = new QPushButton(title);
        but_about->setObjectName(QStringLiteral("but_about"));
        but_about->setGeometry(QRect(755, 0, 21, 21));
        widget_body = new QWidget(centralWidget);
        widget_body->setObjectName(QStringLiteral("widget_body"));
        widget_body->setGeometry(QRect(0, 100, 821, 501));
        stackedWidget = new QStackedWidget(widget_body);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 821, 481));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        widget_2 = new QWidget(page);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setGeometry(QRect(10, 40, 781, 421));
        stackedWidget->addWidget(page);
        page_SoftDownload = new QWidget();
        page_SoftDownload->setObjectName(QStringLiteral("page_SoftDownload"));
        widget = new QWidget(page_SoftDownload);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(140, 0, 681, 51));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_download = new QLabel(widget);
        label_download->setObjectName(QStringLiteral("label_download"));

        horizontalLayout_2->addWidget(label_download);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);

        list_Downloadleft = new QListWidget(page_SoftDownload);
        list_Downloadleft->setObjectName(QStringLiteral("list_Downloadleft"));
        list_Downloadleft->setGeometry(QRect(0, 0, 141, 481));
        widget_download = new QWidget(page_SoftDownload);
        widget_download->setObjectName(QStringLiteral("widget_download"));
        widget_download->setGeometry(QRect(140, 50, 680, 430));
        stackedWidget->addWidget(page_SoftDownload);
        page_SoftUpgrade = new QWidget();
        page_SoftUpgrade->setObjectName(QStringLiteral("page_SoftUpgrade"));
        label_3 = new QLabel(page_SoftUpgrade);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 0, 821, 31));
        list_Upgrade = new QListWidget(page_SoftUpgrade);
        list_Upgrade->setObjectName(QStringLiteral("list_Upgrade"));
        list_Upgrade->setGeometry(QRect(0, 30, 821, 451));
        stackedWidget->addWidget(page_SoftUpgrade);
        page_SoftUnload = new QWidget();
        page_SoftUnload->setObjectName(QStringLiteral("page_SoftUnload"));
        label_unload = new QLabel(page_SoftUnload);
        label_unload->setObjectName(QStringLiteral("label_unload"));
        label_unload->setGeometry(QRect(0, 10, 171, 31));
        lineEdit = new QLineEdit(page_SoftUnload);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(650, 20, 161, 21));
        widget_3 = new QWidget(page_SoftUnload);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setGeometry(QRect(0, 50, 820, 420));
        stackedWidget->addWidget(page_SoftUnload);
        page_UpdateInform = new QWidget();
        page_UpdateInform->setObjectName(QStringLiteral("page_UpdateInform"));
        list_Inform = new QListWidget(page_UpdateInform);
        list_Inform->setObjectName(QStringLiteral("list_Inform"));
        list_Inform->setGeometry(QRect(0, 30, 821, 451));
        stackedWidget->addWidget(page_UpdateInform);
        label = new QLabel(widget_body);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 480, 191, 21));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        SoftDownload->setText(QString());
#ifndef QT_NO_TOOLTIP
        label_SoftDownload->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_SoftDownload->setText(QApplication::translate("MainWindow", "\350\275\257\344\273\266\344\270\213\350\275\275", 0));
        SoftUpgrade->setText(QString());
        label_SoftUpgrade->setText(QApplication::translate("MainWindow", "\350\275\257\344\273\266\345\215\207\347\272\247", 0));
        SoftUnload->setText(QString());
        label_SoftUnload->setText(QApplication::translate("MainWindow", "\350\275\257\344\273\266\345\215\270\350\275\275", 0));
        UpdateInform->setText(QString());
        label_UpdateInform->setText(QApplication::translate("MainWindow", "\346\233\264\346\226\260\351\200\232\347\237\245", 0));
        but_close->setText(QString());
        but_minimize->setText(QString());
        but_about->setText(QString());
        label_download->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        label_3->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        label_unload->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        label->setText(QApplication::translate("MainWindow", "\350\275\257\344\273\266\347\256\241\345\256\266\347\233\256\345\211\215\347\211\210\346\234\254\357\274\2321.0.0.1", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

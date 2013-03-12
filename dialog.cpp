#include "dialog.h"
#include "ui_dialog.h"
#include "httphandle.h"
#include "informationanalyze.h"
#include <QApplication>
#include <QIcon>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDebug>
#include <QPushButton>
#include <QModelIndex>
#include <QtNetwork>
#include <QProcess>

#define SERVERINI "d:/server.ini"
#define LOCALINI "d:/local.ini"
#define UPDATEINI "d:/update.ini"
#define SERVER "http://192.168.9.133/gshy/server.ini"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    m_pHttpHandle = new HttpHandle();

    m_pInformationAnalyze = new InformationAnalyze(SERVERINI, LOCALINI, UPDATEINI);
    m_pInformationAnalyze->vInitLocalInfomation();

    m_pHttpHandle = new HttpHandle();

    m_pHttpHandle->vDownloadFile(QUrl(SERVER), "D:/");


    m_pTableWidget = new QTableWidget(10,5);

    m_pTableWidget->setWindowTitle("QTableWidget & Item");
    m_pTableWidget->resize(600, 600);
    m_pTableWidget->setSelectionBehavior(QTableWidget::SelectRows);//一次选中一行

    m_pTableWidget->setShowGrid(false);
    m_pTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

    //在下载安装完毕后更新显示
    connect(m_pHttpHandle, SIGNAL(sig_vSetupfinish()), this, SLOT(slt_vUpdate()));


    QStringList l_sHeader;
    l_sHeader<<"Name"<<"Description"<<"Version"<<"url"<<"";
    m_pTableWidget->setHorizontalHeaderLabels(l_sHeader);

    m_pTableWidget->verticalHeader()->setVisible(false);


    QHBoxLayout * l_phbHBoxLayout = new QHBoxLayout;
    l_phbHBoxLayout->addWidget(m_pTableWidget);
    setLayout(l_phbHBoxLayout);

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::OnBtnDownloadClicked()
{
    //获取按钮所在行
    QPushButton * l_ppbSenderObj=qobject_cast<QPushButton *>(sender());
    if(l_ppbSenderObj == 0)
    {
         return;
    }
    QModelIndex l_miModelIndex = m_pTableWidget->indexAt(QPoint(l_ppbSenderObj->frameGeometry().x(),l_ppbSenderObj->frameGeometry().y()));
    int l_iRow = l_miModelIndex.row();

    QString l_Qurl = m_pTableWidget->item(l_iRow, 3)->text();


    m_pHttpHandle->vDownloadFile(l_Qurl);

}


void Dialog::slt_vUpdate()
{
    m_pInformationAnalyze->vRefresh();
    slt_vRefresh();
}

//重新获取数据并显示，在程序启动以及每次更新完毕后被调用
void Dialog::slt_vRefresh()
{
    //获取UPDATEINI里的数据以供显示
    QStringList totalSoftwareInformation;
    m_pInformationAnalyze->vGetUpdateInformation(totalSoftwareInformation);

    QStringList singleSoftwareInformation;


    for(int i = 0; i < totalSoftwareInformation.size(); i++)
    {

        singleSoftwareInformation = totalSoftwareInformation.at(i).split("|");

        QPushButton * pBtn = new QPushButton ("Downlaod");
        m_pTableWidget->setCellWidget(i,4,pBtn);

        QString status = singleSoftwareInformation.at(1);

        //如果程序不需要下载，则DOWNLOAD按钮变暗
        if("new" == status || "needupdate" == status)
        {
            connect(pBtn, SIGNAL(clicked()), this, SLOT(OnBtnDownloadClicked()));

        }
        else
        {
           pBtn->setEnabled(false);
        }


        m_pTableWidget->setItem(i,0,new QTableWidgetItem(singleSoftwareInformation.at(0)));
        m_pTableWidget->setItem(i,1,new QTableWidgetItem(singleSoftwareInformation.at(1)));
        m_pTableWidget->setItem(i,2,new QTableWidgetItem(singleSoftwareInformation.at(2)));
        m_pTableWidget->setItem(i,3,new QTableWidgetItem(singleSoftwareInformation.at(3)));

    }

    m_pTableWidget->show();
}

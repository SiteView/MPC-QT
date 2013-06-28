#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QIcon>
#include "allapplicationdialog.h"
#include "initialize.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(750,500);
    QMainWindow::setWindowTitle("Marketplace Client");
    //ui->tab-
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->resize(740,490);
    //QIcon("download.png");
    m_initInfo = new InitializeInfo();

    //QList <TaskInformation *> updateTaskInformationList = m_initInfo->GetUpdateTaskInformation();
    //QList <TaskInformation *> newTaskInformationList = m_initInfo->GetNewTaskInformation();
    //QList <TaskInformation *> normalTaskInformationList = m_initInfo->GetNormalTaskInformation();


    //qDebug() << "hello!";
    //qDebug() << normalTaskInformationList.size();
    //qDebug() << updateTaskInformationList.size();
    //qDebug() << newTaskInformationList.size();
    //qDebug() << "hello!";

    //AllApplicationDialog *hello1 = new AllApplicationDialog(updateTaskInformationList);
    //AllApplicationDialog *hello2= new AllApplicationDialog(newTaskInformationList);
    //AllApplicationDialog *hello3 = new AllApplicationDialog(normalTaskInformationList);

    //ui->tabWidget->addTab(hello1, QIcon("download.png"),"needupdate");
    //ui->tabWidget->addTab(hello2, "new");
    //ui->tabWidget->addTab(hello3, "normal");

    connect(m_initInfo, SIGNAL(sig_vUpdate()), this, SLOT(slt_vUpdate()));
    //ui->tabWidget->
}
void MainWindow::slt_vUpdate()
{
    QList <TaskInformation *> updateTaskInformationList = m_initInfo->GetUpdateTaskInformation();
    QList <TaskInformation *> newTaskInformationList = m_initInfo->GetNewTaskInformation();
    QList <TaskInformation *> normalTaskInformationList = m_initInfo->GetNormalTaskInformation();


    qDebug() << "hello!";
    qDebug() << normalTaskInformationList.size();
    qDebug() << updateTaskInformationList.size();
    qDebug() << newTaskInformationList.size();
    qDebug() << "hello!";

    AllApplicationDialog *hello4 = new AllApplicationDialog(updateTaskInformationList);
    AllApplicationDialog *hello5= new AllApplicationDialog(newTaskInformationList);
    AllApplicationDialog *hello6 = new AllApplicationDialog(normalTaskInformationList);

    ui->tabWidget->addTab(hello4,"NeedUpdate");
    ui->tabWidget->addTab(hello5, "New");
    ui->tabWidget->addTab(hello6, "Normal");
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
   //qDebug() << "hello!";
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <windows.h>
#include <Shellapi.h>
//#include "ExtractIcon.h"
#include <tchar.h>
#include <QTableWidgetItem>
#include <QtUiTools/QUiLoader>
#include <QStringList>

#include "src/SoftAllKindItem.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    createUnloadtableMenu();
    AddSoftSortList();
    inform = new InformDialog;


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::createUpgradeMenu(){



}
void MainWindow::createDownloadMenu(){

}

void MainWindow::createUnloadtableMenu(){
    list   =new SoftUnloadList();
    QHBoxLayout *hor_layout=new QHBoxLayout();
    hor_layout->addWidget(list);
    ui->widget_3->setLayout(hor_layout);

}
void MainWindow::AddSoftSortList()
{

}
void MainWindow::on_SoftDownload_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_SoftDownload);
}

void MainWindow::on_SoftUpgrade_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_SoftUpgrade);
}

void MainWindow::on_SoftUnload_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_SoftUnload);

}

void MainWindow::on_UpdateInform_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_UpdateInform);
    inform->showAsQQ();
}



void MainWindow::on_but_close_clicked()
{
    this->close();
}

void MainWindow::on_but_minimize_clicked()
{
    this->showMinimized();
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    moving = true;
    last = e->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    if ((e->buttons() & Qt::LeftButton) && moving)
        move(x()+dx, y()+dy);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    moving = false;
}

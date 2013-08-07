
#include <QMouseEvent>
#include <windows.h>
#include <Shellapi.h>
//#include "ExtractIcon.h"
#include <tchar.h>
#include <QTableWidgetItem>
#include <QtUiTools/QUiLoader>
#include <QStringList>

#include <QListWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    createUnloadtableMenu();
    createUpgradeMenu();
    createDownloadMenu();
    AddSoftSortMenu();
    inform = new InformDialog;
    item_allkind    = new SoftAllKindItem();

    ui->lineEdit_s->setFrame(false);//**
    connect(list_allkinds->list_softallkind,SIGNAL(itemSelectionChanged()),this,SLOT(changeCurrentItem()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::createUpgradeMenu()
{

}
void MainWindow::createDownloadMenu()
{
    list_download   = new SoftDownloadList(ui->widget_download);
}

void MainWindow::createUnloadtableMenu()
{
    list_unload     = new SoftUnloadList(ui->widget_3);
}
void MainWindow::AddSoftSortMenu()
{
    list_allkinds   = new SoftAllKindList(ui->widget_4);
}

void MainWindow::changeCurrentItem(){
    ui->lineEdit_s->setText(item_allkind->text->text());
    int current_row = list_allkinds->list_softallkind->currentRow();
    qDebug()<<current_row<<item_allkind->text->text()<<"----current ..row..text";
//    if(current_row=1)
//    {
//        list_download   = new SoftDownloadList(ui->widget_download);
//        list_unload     = new SoftUnloadList(ui->widget_download);



//    }

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
    //    inform->showAsQQ();
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

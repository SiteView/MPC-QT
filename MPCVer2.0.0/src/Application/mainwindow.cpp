
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
//    ui->lineEdit_2->setFrame(false);//**
    connect(list_allkinds,SIGNAL(currentRowChanged()),this,SLOT(changeCurrentItem()));
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
    ui->label_download->setText(list_allkinds->lab_text);
}

void MainWindow::changeCurrentItem(){
    item_allkind    = new SoftAllKindItem();

    qDebug()<<list_allkinds->list_softallkind->currentItem()->text()<<"----current ..row..text";
    QHBoxLayout *hor_layout=new QHBoxLayout();
    if(item_allkind->text->text()!="视频软件")
    {
        hor_layout->addWidget(list_download);

    }
    else
    {
        hor_layout->addWidget(list_download);

    }
    ui->widget_download->setLayout(hor_layout);

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

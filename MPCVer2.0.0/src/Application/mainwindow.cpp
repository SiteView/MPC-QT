
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
    ui(new Ui::MainWindow),flag(true)

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

    ui->but_sel_name->setStyleSheet("QPushButton{text-align:left;}");
    ui->but_sel_name->setText("softname");
    ui->but_sel_time->setText("setime");
    ui->but_sel_path->setText("setpath");
    ui->but_sel_size->setText("softsize");
    ui->but_sel_operate->setText("operate");

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
//    list_unload->DiffSelect();
//    list_unload     = new SoftUnloadList(ui->page_up);


//    list_unload2->DiffSelect2();
//    list_unload2     = new SoftUnloadList(ui->page_down);

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


void MainWindow::on_but_sel_name_clicked()
{

    if(flag)
    {
        list_unload     = new SoftUnloadList(ui->page_up);
        list_unload->DiffSelect(SELECT_NAME_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_up);
        ui->but_sel_name->setIcon(QIcon(":/images/down.png"));
        flag=false;

    }
    else
    {
        list_unload2     = new SoftUnloadList(ui->page_down);
        list_unload2->DiffSelect(SELECT_NAME_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_down);
        ui->but_sel_name->setIcon(QIcon(":/images/up.png"));
        flag=true;

    }
}

void MainWindow::on_but_sel_size_clicked()
{
    if(flag)
    {
        list_unload3     = new SoftUnloadList(ui->page_2);
        list_unload3->DiffSelect(SELECT_SIZE_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_2);
        ui->but_sel_size->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        list_unload4     = new SoftUnloadList(ui->page_3);
        list_unload4->DiffSelect(SELECT_SIZE_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_3);
        ui->but_sel_size->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }
}

void MainWindow::on_but_sel_time_clicked()
{
    if(flag)
    {
        list_unload5     = new SoftUnloadList(ui->page_4);
        list_unload5->DiffSelect(SELECT_TIME_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_4);
        ui->but_sel_time->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        list_unload6     = new SoftUnloadList(ui->page_5);
        list_unload6->DiffSelect(SELECT_TIME_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_5);
        ui->but_sel_time->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }

}

void MainWindow::on_but_sel_path_clicked()
{
    if(flag)
    {
        list_unload7     = new SoftUnloadList(ui->page_6);
        list_unload7->DiffSelect(SELECT_PATH_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_6);
        ui->but_sel_path->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        list_unload8     = new SoftUnloadList(ui->page_7);
        list_unload8->DiffSelect(SELECT_PATH_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_7);
        ui->but_sel_path->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }
}

void MainWindow::on_but_sel_operate_clicked()
{
    if(flag)
    {

        ui->but_sel_operate->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        ui->but_sel_operate->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }
}

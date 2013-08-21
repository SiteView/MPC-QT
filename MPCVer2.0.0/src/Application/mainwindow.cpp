
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
//#include "../src/new/skin_widget.h"

//class SkinWidget;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),flag(true)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->createUnloadtableMenu();
    this->createUpgradeMenu();
    this->createDownloadMenu();
    this->AddSoftSortMenu();
    inform        = new InformDialog;
    item_allkind  = new SoftAllKindItem();

    ui->lineEdit_s_4->setFrame(false);//**
    ui->lineEdit_s_2->setFrame(false);
    ui->lineEdit_s_3->setFrame(false);
    ui->lineEdit_s_2->setMaxLength(50);
    ui->lineEdit_s_3->setMaxLength(50);
    ui->lineEdit_s_4->setMaxLength(50);

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
    list_download   = new SoftDownloadList(ui->page_all);
    list_download->selectDifType(ALL);
    ui->stack_download->setCurrentWidget(ui->page_all);
}

void MainWindow::createUnloadtableMenu()
{
    list_unload     = new SoftUnloadList(ui->page_selall);
    list_unload->DiffSelect(SELECT_ALL);
    ui->stackedWidget_2->setCurrentWidget(ui->page_selall);

}
void MainWindow::AddSoftSortMenu()
{
    list_allkinds   = new SoftAllKindList(ui->widget_4);
}

void MainWindow::changeCurrentItem(){
    int current_row = list_allkinds->list_softallkind->currentRow();
    if(current_row==0)
    {
        ui->stack_download->setCurrentWidget(ui->page_all);
    }
    if(current_row==1)
    {
        list_download1     = new SoftDownloadList(ui->page_type2);
        list_download1->selectDifType(TWO);
        ui->stack_download->setCurrentWidget(ui->page_type2);

    }
    if(current_row==2)
    {
        list_download2     = new SoftDownloadList(ui->page_type3);
        list_download2->selectDifType(THREE);
        ui->stack_download->setCurrentWidget(ui->page_type3);
    }
    if(current_row==3)
    {
        list_download3     = new SoftDownloadList(ui->page_type8);
        list_download3->selectDifType(EIGHT);
        ui->stack_download->setCurrentWidget(ui->page_type8);
    }
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
//        layout_widget=new GridlayoutClass(ui->widget_6);

    layout_widget=new GridlayoutClass(this);
    layout_widget->show();

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
        list_unload1     = new SoftUnloadList(ui->page_up);
        list_unload1->DiffSelect(SELECT_NAME_UP);
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

void MainWindow::on_but_clear_4_clicked()
{
    ui->lineEdit_s_4->clear();

}

void MainWindow::on_but_search_4_clicked()
{

}

void MainWindow::on_but_clear_3_clicked()
{
    ui->lineEdit_s_3->clear();
}

void MainWindow::on_but_search_3_clicked()
{

}

void MainWindow::on_but_clear_2_clicked()
{
    ui->lineEdit_s_2->clear();
}

void MainWindow::on_but_search_2_clicked()
{

}

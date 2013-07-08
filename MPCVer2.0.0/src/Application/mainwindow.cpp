#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "src/softdownloadlist.h"
#include <QMouseEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    createUnloadtableMenu();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUnloadtableMenu(){

    SoftDownloadList *ani=new SoftDownloadList(this);

    ui->table_Unload->setColumnCount(8);
    ui->table_Unload->setRowCount(0);
    QStringList tableHeaders;
    tableHeaders << tr("State") << tr("File name") << tr("Size") << tr("Progress") <<tr("Elapsed time")<< tr("Time left") << tr("Retry") << tr("Comment");
    ui->table_Unload->setHorizontalHeaderLabels(tableHeaders);

    QListWidgetItem *twi = new QListWidgetItem(0);
    twi->setSizeHint(QSize(400,59));
    ui->list_Inform->addItem(twi);
    ui->list_Inform->setItemWidget(twi,ani);
    QTableWidgetItem *tw=new QTableWidgetItem(0);

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

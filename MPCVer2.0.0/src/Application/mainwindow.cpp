#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include "src/softdownloadlist.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QColor>
#include <QIcon>
#include <QStyle>
#include <QPalette>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QStringList>
#include <QPixmap>

#include <windows.h>
#include <Shellapi.h>
//#include "ExtractIcon.h"
#include <tchar.h>
#include <QIconEngine>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    createUnloadtableMenu();
    inform = new InformDialog;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUnloadtableMenu(){


    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    if ( !SQLiteQuery.exec( "select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime,InstallLocation from LocalAppInfor ;" ) )
    {
        qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
    }
    while ( SQLiteQuery.next() )
    {
        QVariant val0 = SQLiteQuery.value(0);
        QVariant val1 = SQLiteQuery.value(1);
        QVariant val2 = SQLiteQuery.value(2);
        QVariant val3 = SQLiteQuery.value(3);
        QVariant val4 = SQLiteQuery.value(4);
        QVariant val5 = SQLiteQuery.value(5);

        QString pahtstr0 = val0.toString();
        QString pahtstr1 = val1.toString();
        QString pahtstr2 = val2.toString();
        QString pahtstr3 = val3.toString();
        QString pahtstr4 = val4.toString();
        QString pahtstr5 = val5.toString();
        pahtstr0.replace("\\","/");

        SoftDownloadList *ani=new SoftDownloadList(this);
        ani->icon->setStyleSheet("border-image:url("+pahtstr0+")");
        ani->softname->setText(pahtstr1);
        ani->softdetail->setText(pahtstr2);
        ani->size->setText(pahtstr3);
        ani->setuptime->setText(pahtstr4);
        ani->progress->setText(pahtstr5);
        ani->download->setText("Download");

        QListWidgetItem *twi = new QListWidgetItem(0);
        twi->setSizeHint(QSize(400,59));
        ui->list_Inform->addItem(twi);
        ui->list_Inform->setItemWidget(twi,ani);
        ui->list_Inform->setAlternatingRowColors(true);
        QFont font;
        font.setPixelSize(14);
        font.setBold(true);
        ani->softname->setFont(font);
    }
    SQLiteQuery.finish();

        /*ICONINFO IconInfo;
        HICON hIconLarge = 0 , hIconSmall = 0;
        int list = ExtractIconEx(_T("C:\\Program Files\\HaoZip\\HaoZip.exe"),0,&hIconLarge,&hIconSmall,1);

    //	SaveIcon(hIconLarge,"c:\\tmp.ico",32);
        DestroyIcon(hIconLarge);
        DestroyIcon(hIconSmall);
    //	 QPixmap  ppk = qt_pixmapFromWinHICON(hIcon);

        //QPixmap pix = QPixmap::fromWinHICON(hIcon);
        //setWindowIcon(QIcon(pix));
    //	getHWNDForWidget(this);
        QString filepaht = 	QString::fromLocal8Bit("F:\\����ǿ\\HaoZip.exe");

        HICON* pIcons = (HICON*)LocalAlloc( LPTR, 100*sizeof(HICON) );
        int cIcons;
        if( pIcons != NULL )
        {

            // from winuser.h
            //#define LR_DEFAULTCOLOR     0x0000
            //#define LR_MONOCHROME       0x0001
            //#define LR_COLOR            0x0002
            cIcons = (int)ExtractIcons::Get( filepaht.toStdWString().data(), 0, 128, 128, pIcons, NULL, 100, LR_COLOR );

            //if( large )
            //cIcons = (int)ExtractIconEx( m_fileIco, 0, pIcons, NULL, MAX_ICON );
            //else
            //cIcons = (int)ExtractIconEx( m_fileIco, 0, NULL, pIcons, MAX_ICON );
        }

        SaveIcon(pIcons[0],_T("c:\\tmp.ico"),32);

    GlobalFree(pIcons);

    //	QPixmap  pp("C:\\Program Files\\HaoZip\\HaoZip.exe");
    //	this->setWindowIcon(QIcon(pp));
    */

}
void MainWindow::AddSoftSortList(){



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


#include <QTimer>
#include <QMouseEvent>

#include <QtUiTools/QUiLoader>
#include <QFile>

#include "informdialog.h"
#include "ui_informdialog.h"
#include "softdownloadlist.h"

InformDialog::InformDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏菜单栏
    timerShow=new QTimer(this);
    connect(timerShow,SIGNAL(timeout()),this,SLOT(myMove()));
    timerStay=new QTimer(this);
    connect(timerStay,SIGNAL(timeout()),this,SLOT(myStay()));
    timerClose=new QTimer(this);
    connect(timerClose,SIGNAL(timeout()),this,SLOT(myClose()));
}

InformDialog::~InformDialog()
{
    delete ui;
}

void InformDialog::on_but_close_clicked()
{
    this->close();
}

void InformDialog::showAsQQ()
{
    QDesktopWidget *deskTop=QApplication::desktop();
    deskRect=deskTop->availableGeometry();

    normalPoint.setX(deskRect.width()-rect().width());
    normalPoint.setY(deskRect.height()-rect().height());
    move(normalPoint.x(),768-1);
    show();
    timerShow->start(10);//滑出的时间
}
//平滑显示出来
void InformDialog::myMove()
{
    static int beginY=QApplication::desktop()->height();
    beginY--;
    move(normalPoint.x(),beginY);
    if(beginY<=normalPoint.y())
    {
        timerShow->stop();
        timerStay->start(1000);
    }
}
//停留显示
void InformDialog::myStay()
{
    static int timeCount=0;
    timeCount++;
    if(timeCount>=10)//停留时间
    {
        timerStay->stop();
        timerClose->start(200);//淡化时间
    }
}
//自动关闭时实现淡出效果
void InformDialog::myClose()
{
    static double tran=1.0;
    tran-=0.1;
    if(tran<=0.0)
    {
        timerClose->stop();
        emit close();
    }
    else
        setWindowOpacity(tran);
}

void InformDialog::mousePressEvent(QMouseEvent *e)
{
    moving = true;
    last = e->globalPos();
}

void InformDialog::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    if ((e->buttons() & Qt::LeftButton) && moving)
        move(x()+dx, y()+dy);
}

void InformDialog::mouseReleaseEvent(QMouseEvent *e)
{
    moving = false;
}

void InformDialog::additemlist()
{



}


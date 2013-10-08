#include "myslider.h"
#include "mouselabel.h"

MySliderMenu::MySliderMenu(QWidget * parent,int x, int y, QString imageBgPath_on,QString imageBgPath_off, QString imageSwPath,int nSliderID)
    :QLabel(parent)
{
    PixmapBg_on.load(imageBgPath_on);
    PixmapBg_off.load(imageBgPath_off);
    PixmapSw.load(imageSwPath);

    ImageBgLabel_on = new QLabel(parent);
    ImageBgLabel_off = new QLabel(parent);
    ImageSwLabel = new QLabel(parent);

    this->raise();//置顶
    TextSwLabel = new mouseLabel(parent,this);

    ImageBgLabel_on->setPixmap(PixmapBg_on);
    ImageSwLabel->setPixmap(PixmapSw);
    ImageBgLabel_off->setPixmap(PixmapBg_off);

    ImageBgLabel_on->setGeometry(x,y,PixmapBg_on.rect().width(),PixmapBg_on.rect().height());
    ImageBgLabel_off->setGeometry(x,y,PixmapBg_off.rect().width(),PixmapBg_off.rect().height());

    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    QString str1="",str2="";
    str1 += Str_On;
    str2 += Str_Off;
    this->setText(QObject::tr((str1+str2).toLocal8Bit().data()));
    this->setPalette(pal);
    this->setGeometry(x,y,PixmapBg_on.rect().width(),PixmapBg_on.rect().height());

    ImageSwLabel->setGeometry(x,y,PixmapSw.rect().width(),PixmapSw.rect().height());
    TextSwLabel->setGeometry(x,y,PixmapSw.rect().width(),PixmapSw.rect().height());
    TextSwLabel->setText(QObject::tr(Str_On));
    TextSwLabel->setAlignment(Qt::AlignCenter);
    TextSwLabel->setPalette(pal);
    //setMouseTracking(true);  //这是激活整个窗体的鼠标追踪
    //gpFrameMenu4->setMouseTracking(true);  //这是激活整个窗体的鼠标追踪
    //this->installEventFilter(this);
    xPos = x;
    yPos = y;
    State = MENU_ON;
    SliderId = nSliderID;
}
void MySliderMenu::setPosition(int x,int y)
{
    this->setGeometry(x,y,PixmapBg_on.rect().width(),PixmapBg_on.rect().height());
    ImageSwLabel->setGeometry(x,y,PixmapSw.rect().width(),PixmapSw.rect().height());
}
void MySliderMenu::moveImageSw(int pos)
{
    if(pos == MENU_ON)
    {
        ImageBgLabel_off->show();
        ImageBgLabel_on->hide();
        ImageSwLabel->setGeometry(xPos,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
        TextSwLabel->setText(QObject::tr(Str_On));
        TextSwLabel->setGeometry(xPos,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
        State = MENU_ON;
    }
    else if(pos ==MENU_OFF)
    {
        ImageBgLabel_on->show();
        ImageBgLabel_off->hide();
        ImageSwLabel->setGeometry(xPos+PixmapBg_on.rect().width()/2+1,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
        TextSwLabel->setText(QObject::tr(Str_Off));
        TextSwLabel->setGeometry(xPos+PixmapBg_on.rect().width()/2+1,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
        State =MENU_OFF ;
    }
}
void MySliderMenu::moveImageSwLittle(int pos)
{
    if(pos == MENU_ON)
    {
        ImageSwLabel->setGeometry(xPos+PixmapBg_on.rect().width()/2,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
        TextSwLabel->setText(QObject::tr(Str_Off));
        TextSwLabel->setGeometry(xPos+PixmapBg_on.rect().width()/2,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
    }
    else if(pos == MENU_OFF)
    {
        ImageSwLabel->setGeometry(xPos,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
        TextSwLabel->setText(QObject::tr(Str_On));
        TextSwLabel->setGeometry(xPos,yPos,PixmapSw.rect().width(),PixmapSw.rect().height());
    }
}
int MySliderMenu::getSliderID()
{
    return SliderId;
}
int MySliderMenu::getState()
{
    return State;
}
void MySliderMenu::mousePressEvent(QMouseEvent *e)
{
    if(State == MENU_OFF)
    {
        moveImageSw(MENU_ON);
    }else
    {
        moveImageSw(MENU_OFF);
    }
    // printf("press \n");
}
void MySliderMenu::mouseReleaseEvent(QMouseEvent *e)
{
    /*  if(State == 2)
    {
        moveImageSw(1);
    }else
    {
        moveImageSw(2);
    }
   */
    //  printf("release \n");
}
void MySliderMenu::mouseMoveEvent(QMouseEvent *ev)
{
    /*  if(State == 2)
    {
        moveImageSw(1);
    }else
    {
        moveImageSw(2);
    }*/
    // printf("mouve \n");
}

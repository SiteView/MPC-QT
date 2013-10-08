#include "mouselabel.h"
#include "myslider.h"
mouseLabel::mouseLabel(QWidget *parent,MySliderMenu *focusLabel):QLabel(parent)
{
    FocusLabel = focusLabel;
    releaseFlag = true;
}
void mouseLabel::mousePressEvent(QMouseEvent *e)
{

}
void mouseLabel::mouseReleaseEvent(QMouseEvent *e)
{
    releaseFlag = true;
    if(FocusLabel->getState() == MENU_OFF)
    {
        FocusLabel->moveImageSw(MENU_OFF);
    }
    else
    {
        FocusLabel->moveImageSw(MENU_ON);
    }
}
void mouseLabel::mouseMoveEvent(QMouseEvent *ev)
{
    static QPoint FirstPoint,LastPoint;
    if(releaseFlag)
    {
        FirstPoint = QCursor::pos();
        releaseFlag = false;
    }
    LastPoint = QCursor::pos();//获取当前光标的位置

    if(LastPoint.x() - FirstPoint.x() > 10)
    {
        if(FocusLabel->getState() == MENU_ON)
        {
            FocusLabel->moveImageSwLittle(MENU_OFF);
        }
   }
    if(FirstPoint.x() - LastPoint.x() > 10)
    {
        if(FocusLabel->getState() == MENU_OFF)
        {
            FocusLabel->moveImageSwLittle(MENU_ON);
        }
    }
    if(LastPoint.x() - FirstPoint.x() > 80)
    {
        if(FocusLabel->getState() == MENU_ON)
        {
            FocusLabel->moveImageSw(MENU_OFF);
        }
    }
    if(FirstPoint.x() - LastPoint.x() > 80)
    {
        if(FocusLabel->getState() == MENU_OFF)
        {
            FocusLabel->moveImageSw(MENU_ON);
        }
    }

    //  printf("x:%d,Y:%d\n",LastPoint.x(),LastPoint.y());
}

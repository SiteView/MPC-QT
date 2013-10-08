#ifndef MYSLIDER_H
#define MYSLIDER_H
#include <QLabel>

class mouseLabel;

class MySliderMenu :public QLabel
{
    friend class mouseLabel;
    Q_OBJECT
private:
    QPixmap PixmapBg_on;
    QPixmap PixmapBg_off;
    QPixmap PixmapSw;

    QLabel  *ImageBgLabel_on;
    QLabel  *ImageBgLabel_off;
    QLabel  *ImageSwLabel;
    mouseLabel  *TextSwLabel;
    int SliderId;
    int xPos;
    int yPos;
    int State;
public:
    const char *Str_On;
    const char *Str_Off;
public:
    MySliderMenu(QWidget * parent,int x, int y, QString imageBgPath_on,QString imageBgPath_off, QString imageSwPath,int nSliderID);
    void setPosition(int x,int y);
    void moveImageSw(int pos);
    void moveImageSwLittle(int pos);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *ev);
    int getSliderID();
    int getState();
};

#define MENU_ON  1
#define MENU_OFF 2

#endif // MYSLIDER_H

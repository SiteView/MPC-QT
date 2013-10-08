#ifndef MOUSELABEL_H
#define MOUSELABEL_H
#include "myslider.h"
#include <QLabel>
class MySliderMenu;

class mouseLabel : public QLabel
{
    friend class MySliderMenu;
    Q_OBJECT
private:
    QString ImageBgPath;
    QString ImageSwPath;
    MySliderMenu *FocusLabel;
    bool releaseFlag;
public:

    mouseLabel(QWidget *parent,MySliderMenu *focusLabel);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *ev);
    void setTwoImage(QString ImageBg,QString ImageSw);
};

#endif // MOUSELABEL_H

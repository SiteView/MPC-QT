#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButton(QWidget *parent = 0);

    void setImage(QString pic_name);
    void setMousePress(bool mouse_press);

public:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void painterInfo(int top_color, int middle_color, int bottom_color);

public:
    bool mouse_over; //鼠标是否移过
    bool mouse_press; //鼠标是否按下
    
};

#endif // TOOLBUTTON_H

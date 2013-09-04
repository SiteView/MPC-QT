#ifndef CELLCLASS_H
#define CELLCLASS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPoint>
class CellClass : public QWidget
{
    Q_OBJECT
public:
    explicit CellClass(QWidget *parent = 0);
    void changeText(QString pixmap_name, QString skin_name, QString download_count);

signals:
    
public slots:

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

public:

    QLabel *skin_label;
    QLabel *skin_name_label;
    QPushButton *use_skin_button;

    QString pixmap_name;
    bool mouse_press;
    bool mouse_enter;
    bool moving;
    QPoint last;

};

#endif // CELLCLASS_H

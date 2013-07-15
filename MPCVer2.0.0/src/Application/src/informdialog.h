#ifndef INFORMDIALOG_H
#define INFORMDIALOG_H

#include <QDialog>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QPoint>
#include <QTimer>
namespace Ui {
class InformDialog;
}

class InformDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InformDialog(QWidget *parent = 0);
    ~InformDialog();
    
    void showAsQQ();//在右下角显示

private slots:
    void on_but_close_clicked();

private:
    Ui::InformDialog *ui;

    QPoint normalPoint;//完全显示时左上角位置
    QRect deskRect;

    QTimer *timerShow;//平滑显示的定时器
    QTimer *timerStay;//停留时间的定时器 10s
    QTimer *timerClose;//关闭淡出的定时器

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool moving;
    QPoint last;
private slots:


    void myMove();
    void myStay();
    void myClose();

};

#endif // INFORMDIALOG_H

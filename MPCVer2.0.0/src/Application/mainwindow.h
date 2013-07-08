#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QListWidget"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_SoftDownload_clicked();

    void on_SoftUpgrade_clicked();

    void on_SoftUnload_clicked();

    void on_UpdateInform_clicked();
    void on_but_close_clicked();

    void on_but_minimize_clicked();
private:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool moving;
    QPoint last;

public:
    void createUnloadtableMenu();

    void add_networkitem(const QString &icon,const QString &soft_name,const QString &detail,const QString &comment);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

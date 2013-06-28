#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class InitializeInfo;

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
    void on_tabWidget_currentChanged(int index);
    void slt_vUpdate();
private:
    InitializeInfo* m_initInfo;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

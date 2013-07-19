#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QListWidget"
#include "../Common/SqliteDb.h"
//#include "../Common/SytemLog.h"
#include "src/informdialog.h"
#include "src/SoftUnloadList.h"

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
    void createUpgradeMenu();
    void createUnloadtableMenu();
    void createDownloadMenu();
    void AddSoftSortList();
    void add_networkitem(const QString &icon,const QString &soft_name,const QString &detail,const QString &comment);
public:
    CSQLiteDb m_SQLiteDb;
    InformDialog *inform;
    SoftUnloadList *list;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QListWidget"
#include "../Common/SqliteDb.h"
//#include "../Common/SytemLog.h"
#include "src/informdialog.h"
#include "src/SoftUnloadList.h"
#include "src/SoftDownloadList.h"
#include "src/SoftAllKindList.h"
#include "src/SoftAllKindItem.h"

//#include "../src/new/skin_widget.h"
//class SkinWidget;

namespace Ui {
class MainWindow;
}
enum SIGN
{
    SELECT_ALL
    ,SELECT_NAME_UP
    ,SELECT_NAME_DOWN
    ,SELECT_SIZE_UP
    ,SELECT_SIZE_DOWN
    ,SELECT_TIME_UP
    ,SELECT_TIME_DOWN
    ,SELECT_PATH_UP
    ,SELECT_PATH_DOWN

};
enum TYPE_FLAG{
    ALL
    ,ONE
    ,TWO
    ,THREE
    ,FOUR
    ,FIVE
    ,SIX
    ,SEVEN
    ,EIGHT
    ,NINE
    ,TEN
};
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
    void on_but_sel_name_clicked();

    void on_but_sel_size_clicked();

    void on_but_sel_time_clicked();

    void on_but_sel_path_clicked();

    void on_but_sel_operate_clicked();

    void on_but_clear_4_clicked();

    void on_but_search_4_clicked();

    void on_but_clear_3_clicked();

    void on_but_search_3_clicked();

    void on_but_clear_2_clicked();

    void on_but_search_2_clicked();

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
    void AddSoftSortMenu();
    void add_networkitem(const QString &icon,const QString &soft_name,const QString &detail,const QString &comment);

public:
    CSQLiteDb m_SQLiteDb;
    InformDialog *inform;
    SoftUnloadList *list_unload;
    SoftUnloadList *list_unload1;
    SoftUnloadList *list_unload2;
    SoftUnloadList *list_unload3;
    SoftUnloadList *list_unload4;
    SoftUnloadList *list_unload5;
    SoftUnloadList *list_unload6;
    SoftUnloadList *list_unload7;
    SoftUnloadList *list_unload8;

    SoftDownloadList *list_download;
    SoftDownloadList *list_download1;
    SoftDownloadList *list_download2;
    SoftDownloadList *list_download3;

    SoftAllKindList *list_allkinds;
    SoftAllKindItem *item_allkind;
//    CURLDownloadManager *downloader;
//    SkinWidget *skin_widget; //显示皮肤界面

    bool flag;
private:
    Ui::MainWindow *ui;
public slots:
    void changeCurrentItem();

};

#endif // MAINWINDOW_H

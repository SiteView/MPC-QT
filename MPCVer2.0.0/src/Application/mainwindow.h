#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include "QListWidget"
#include "../Common/SqliteDb.h"
#include "src/informdialog.h"
#include "src/SoftUnloadList.h"
#include "src/SoftDownloadList.h"
#include "src/SoftAllKindList.h"
#include "src/SoftAllKindItem.h"
#include "src/GridlayoutClass.h"
#include "src/CellClass.h"
#include "src/SoftUpgradeList.h"
#include "src/testunloadlist.h"
#include "src/ToolButton.h"

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
    ,SELECT_SEARCH

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
    ,SEARCH
};
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void TitlePage();

private slots:
    void on_SoftDownload_clicked();

    void on_SoftUpgrade_clicked();

    void on_SoftUnload_clicked();

//    void on_UpdateInform_clicked();
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
    void on_but_return_2_clicked();

    void on_but_return_clicked();

public slots:
    void turnPage(QString current_page);
    void changePage(int);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);

    bool moving;
    QPoint last;

public:
    void createUpgradeMenu();
    void createUnloadtableMenu();
    void createDownloadMenu();
    void AddSoftSortMenu();
//    void add_networkitem(const QString &icon,const QString &soft_name,const QString &detail,const QString &comment);

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
    SoftUnloadList *search_unload;


    SoftDownloadList *list_download;
    SoftDownloadList *list_download1;
    SoftDownloadList *list_download2;
    SoftDownloadList *list_download3;
    SoftDownloadList *search_download;

    SoftAllKindList *list_allkinds;
    SoftAllKindItem *item_allkind;

    SoftUpgradeList *list_upgrade;
//    CURLDownloadManager *downloader;
    TestUnloadItem *layout_widget; //显示皮肤界面
    TestUnloadList *testclass;
    GridlayoutClass *grid;
    bool flag;
    QList<ToolButton *> button_list;

private:
    Ui::MainWindow *ui;
public slots:
    void changeCurrentItem();

};

#endif // MAINWINDOW_H

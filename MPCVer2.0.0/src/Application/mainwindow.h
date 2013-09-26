#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include "QListWidget"
#include "../Common/SqliteDb.h"
#include "src/SoftUnloadList.h"
#include "src/SoftDownloadList.h"
#include "src/SoftAllKindList.h"
#include "src/SoftAllKindItem.h"
#include "src/GridlayoutClass.h"
#include "src/CellClass.h"
#include "src/SoftUpgradeList.h"
#include "src/PageModelList.h"
#include "src/ToolButton.h"
#include "src/SettingMenu.h"
#include "src/SoftSortItem.h"
#include "src/SettingDialog.h"
#include "src/informdialog.h"
class CURLDownloadManager;
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
    ,ELEVEN
    ,TWELVE
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

    void on_but_setting_clicked();

    void showAboutUs();
    void showNewCharacter();
    void showSettingDialog();

public slots:
    void turnPage(QString current_page);

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
    QLabel *lab_upnum;
    CSQLiteDb m_SQLiteDb;
//    InformDialog *inform;
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
    //    SoftDownloadList *list_download1;
    //    SoftDownloadList *list_download2;
    //    SoftDownloadList *list_download3;
    SoftDownloadList *search_download;

    SoftAllKindList *list_allkinds;
    SoftAllKindItem *item_allkind;

    SoftUpgradeList *list_upgrade;
    //    TestUnloadItem *layout_widget; //显示皮肤界面
    PageModelList *testclass;
    GridlayoutClass *grid;
    bool flag;
    int typecount;//软件分类总数
    int count_upgrade;//待升级软件数目
    QList<ToolButton *> button_list;//标题图标列表
    QList<QWidget *> *page_list ;//构造界面列表
    QList<SoftDownloadList *> *download_list;//构造下载对象列表
    CURLDownloadManager *downthread;//初始化一个下载对象
    SettingMenu *setting_menu;//设置菜单
    SettingDialog *setting_dialog;//设置界面


private:
    Ui::MainWindow *ui;
public slots:
    void changeCurrentItem();

};

#endif // MAINWINDOW_H

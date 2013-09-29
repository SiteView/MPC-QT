#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QFont>
#include <QToolButton>
#include "SwitchMenu.h"
class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);
//    ~SettingDialog();
    void translation();
    quint64 getDiskFreeSpace(QString );

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:

    void initTitle();
    void initCenter();
    void initBottom();
    void initTab1();
    void initTab2();
    void initTab3();
    void initTab4();

public:

    QPoint move_point; //移动的距离
    bool mouse_press; //按下鼠标左键
    QHBoxLayout *title_layout; //标题布局
    QLabel *title_icon_label; //标题图标
    QLabel *title_label; //标题
    QPushButton *close_button; //关闭按钮

    QHBoxLayout *bottom_layout; //底部布局
    QPushButton *ok_button; //确定按钮
    QPushButton *cancel_button; //取消按钮

    QTabWidget *tab_widget;

    QWidget *tab1; //升级方式
    QGroupBox *tab1_group_box;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *v_Layout;
    QHBoxLayout *Layout_1;
    QLabel *lab_soft_dir;
    QPushButton *but_open_dir;
    QLabel *lab_disc;
    QLabel *lab_disc_size;
    QSpacerItem *Spacer;
    QHBoxLayout *Layout_2;
    QLabel *lab_show_dir;
    QPushButton *but_chos_dir;
    QPushButton *but_recover_dir;
    QHBoxLayout *Layout_3;
    QLabel *lab_apply_dir;
    QPushButton *but_open_dir_2;
    QLabel *lab_disc_2;
    QLabel *lab_disc_size_2;
    QSpacerItem *Spacer_2;
    QHBoxLayout *Layout_4;
    QLabel *lab_show_dir_2;
    QPushButton *but_chos_dir_2;
    QPushButton *but_recover_dir_2;
    QGroupBox *tab1_group_box2;
    QLabel *lab_down_inform;
//    QToolButton *but_on_off;
    QLabel *but_on_off;

    QWidget *tab2; //高级设置
    QGroupBox *tab2_group_box1;
    QGroupBox *tab2_group_box2;
    QGroupBox *tab2_group_box3;
    QCheckBox *auto_start_check_box; //开机自启动
    QCheckBox *remove_own_check_box; //解除占有
    QCheckBox *strong_remove_check_box; //强力删除
    QCheckBox *mummy_kill_check_box; //木马云查杀
    QCheckBox *display_count_check_box; //显示软件升级数

    QWidget *tab3; //体验设置
    QGroupBox *tab3_group_box1;
    QGroupBox *tab3_group_box2;
    QRadioButton *auto_check_button; //每次启动自动体检
    QRadioButton *first_check_button; //首次开启体检
    QRadioButton *hand_check_button; //手动进行体检
    QRadioButton *select_quit_button; //选择退出方式
    QRadioButton *backstage_mode_button; //后台模式
    QRadioButton *immediacy_close_button; //直接关闭

    QWidget *tab4; //用户设置
    QGroupBox *tab4_group_box;
    QCheckBox *diaplay_experience_check_box; //完成任务显示获取经验值的动态提醒
    QCheckBox *diaplay_login_check_box; //自动登录后显示登录提示窗口
    QCheckBox *tray_quit_check_box; //托盘退出登录后显示提示窗口
    QCheckBox *new_character_check_box; //有新特权时显示通知窗口
    QCheckBox *rise_remind_check_box; //升级提醒通知窗口

public:
    QString direc_soft;
    QString direc_apply;
    QString filepath;

    SwitchMenu *switchMenu;

signals:
    
public slots:
    void on_but_chos_dir_clicked();
    void on_but_chos_dir2_clicked();
    void on_but_open_dir_clicked();
    void on_but_open_dir2_clicked();
    void on_but_recover_dir_clicked();
    void on_but_recover_dir2_clicked();
    QString on_but_ok_clicked();

    
};

#endif // SETTINGDIALOG_H

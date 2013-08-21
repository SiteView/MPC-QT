#ifndef GRIDLAYOUTCLASS_H
#define GRIDLAYOUTCLASS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
//#include <QPoint>
#include "CellClass.h"
class GridlayoutClass : public QWidget
{
    Q_OBJECT
public:
    explicit GridlayoutClass(QWidget *parent = 0);

signals:
    
private:

    void initTitle();
    void initCenter();
    void initBottom();

private:
    QString skin_name; //背景图片
    bool is_change; //是否改变背景
    QStringList skin_list;
//    QStringList tip_list;

    QHBoxLayout *title_layout;
    QGridLayout *center_layout;
    QHBoxLayout *bottom_layout;

//    QPoint move_point; //移动的距离
    bool mouse_press; //按下鼠标左键
    QLabel *title_label; //标题
    QLabel *title_icon_label; //标题图标
    QPushButton *close_button; //关闭按钮
    int page_count; //总页数
    int page_count_point; //最后一页的显示个数
    int current_page; //当前为第几页

    CellClass *cell_1;
    CellClass *cell_2;
    CellClass *cell_3;
    CellClass *cell_4;
    CellClass *cell_5;
    CellClass *cell_6;
    CellClass *cell_7;
    CellClass *cell_8;

    QPushButton *first_page_button;
    QPushButton *previous_page_button;
    QPushButton *next_page_button;
    QPushButton *last_page_button;
private slots:

    void showCell(QString page_count);
    
};

#endif // GRIDLAYOUTCLASS_H

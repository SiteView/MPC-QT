#ifndef CELLCLASS_H
#define CELLCLASS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
class CellClass : public QWidget
{
    Q_OBJECT
public:
    explicit CellClass(QWidget *parent = 0);
    
signals:
    
public slots:

private:

    QLabel *skin_label; //显示皮肤
    QLabel *skin_name_label; //显示皮肤名称
    QLabel *download_count_label; //显示下载次数
    QPushButton *use_skin_button; //使用此皮肤按钮

    QString pixmap_name;
    bool mouse_press;
    bool mouse_enter;
};

#endif // CELLCLASS_H

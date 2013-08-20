#include <QVBoxLayout>
#include "CellClass.h"

CellClass::CellClass(QWidget *parent) :
    QWidget(parent)
{

    this->setFixedSize(140, 160);
    mouse_press = false;
    mouse_enter = false;

    skin_label = new QLabel();
    skin_name_label = new QLabel();
    download_count_label = new QLabel();
    use_skin_button = new QPushButton();
    this->setCursor(Qt::PointingHandCursor);

    use_skin_button->setStyleSheet("border-radius:3px; border:1px solid rgb(180, 190, 200); color:rgb(70, 70, 70); background:transparent;");
    skin_label->setScaledContents(true);
    skin_label->setFixedSize(100, 65);
    use_skin_button->setFixedSize(85, 25);

    QVBoxLayout *background_layout = new QVBoxLayout();
    background_layout->addWidget(skin_label, 0, Qt::AlignCenter);
    background_layout->addWidget(skin_name_label, 0, Qt::AlignCenter);
    background_layout->addWidget(download_count_label, 0, Qt::AlignCenter);
    background_layout->addWidget(use_skin_button, 0, Qt::AlignCenter);
    background_layout->setSpacing(5);
    background_layout->setContentsMargins(0, 10, 0, 10);

    this->setLayout(background_layout);
}

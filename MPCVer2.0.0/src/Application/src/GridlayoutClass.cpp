#include <QVBoxLayout>
#include <QSignalMapper>
#include <QPainter>
#include "GridlayoutClass.h"

GridlayoutClass::GridlayoutClass(QWidget *parent) :
    QWidget(parent)
{

    this->resize(620, 445);
    mouse_press = false;
    skin_name = QString("");
    is_change = false;
    current_page = 1;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    initTitle();
    initCenter();
    initBottom();

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(title_layout);
    main_layout->addLayout(center_layout);
    main_layout->addLayout(bottom_layout);
    main_layout->addStretch();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(main_layout);
    this->showCell(QString::number(current_page, 10));

}

void GridlayoutClass::initTitle()
{
    title_label = new QLabel();
    title_icon_label = new QLabel();
    close_button = new QPushButton();
    title_label->setObjectName(QString::fromUtf8("title_label"));
    title_icon_label->setObjectName(QString::fromUtf8("title_icon_label"));
    close_button->setObjectName(QString::fromUtf8("close_button"));

    QPixmap title_pixmap(":/images/360small.png");
    title_icon_label->setPixmap(title_pixmap);
    title_icon_label->setFixedSize(16, 16);
    title_icon_label->setScaledContents(true);
    title_label->setFixedHeight(30);
//    close_button->setStyleSheet("border-image: url(:/images/but_close_hover.png)");

    title_layout = new QHBoxLayout();
    title_layout->addWidget(title_icon_label, 0, Qt::AlignVCenter);
    title_layout->addWidget(title_label, 0, Qt::AlignVCenter);
    title_layout->addStretch();
    title_layout->addWidget(close_button, 0, Qt::AlignTop);
    title_layout->setSpacing(5);
    title_layout->setContentsMargins(10, 0, 5, 0);

    title_label->setStyleSheet("color:white;");
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
}

void GridlayoutClass::initCenter()
{
    skin_list<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"
            <<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20"<<"21"
           <<"22"<<"23"<<"24"<<"25";

    tip_list<<tr("profound life")<<tr("blue sea")<<tr("red heart")<<tr("lovely baby")<<tr("transparent water")<<
        tr("flower")<<tr("great sunshine")<<tr("shadow amazement")<<tr("360 pet")<<tr("beautiful stone")<<
        tr("yellow energy")<<tr("magic world")<<tr("intense emotion")<<tr("dream sky")<<tr("angry bird")<<
        tr("graceful jazz")<<tr("card")<<tr("summer cool")<<tr("blue world")<<tr("woodwind")<<
        tr("pink mood")<<tr("across time")<<tr("six year");

    center_layout = new QGridLayout();
    center_layout->setSpacing(5);
    center_layout->setContentsMargins(5, 35, 5, 0);
    cell_1=new CellClass();
    cell_2=new CellClass();
    cell_3=new CellClass();
    cell_4=new CellClass();
    cell_5=new CellClass();
    cell_6=new CellClass();
    cell_7=new CellClass();
    cell_8=new CellClass();

    center_layout->addWidget(cell_1, 0, 0);
    center_layout->addWidget(cell_2, 0, 1);
    center_layout->addWidget(cell_3, 0, 2);
    center_layout->addWidget(cell_4, 0, 3);
    center_layout->addWidget(cell_5, 1, 0);
    center_layout->addWidget(cell_6, 1, 1);
    center_layout->addWidget(cell_7, 1, 2);
    center_layout->addWidget(cell_8, 1, 3);

    int skin_list_count = skin_list.size();
    page_count = skin_list_count / 8;
    page_count_point = skin_list_count % 8;
    if(page_count_point > 0)
    {
        page_count = page_count + 1;
    }
}

void GridlayoutClass::initBottom()
{
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    QList<QPushButton *> *button_list = new QList<QPushButton *>();
    for(int i=0; i<page_count; i++)
    {
        QPushButton *page_button = new QPushButton();
        page_button->setFixedWidth(20);
        page_button->setText(QString::number(i+1, 10));
        page_button->setStyleSheet("color:rgb(0, 120, 230); background:transparent;");
        page_button->setCursor(Qt::PointingHandCursor);
        connect(page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
        signal_mapper->setMapping(page_button, page_button->text());
        button_list->push_back(page_button);
    }

    first_page_button = new QPushButton();
    previous_page_button = new QPushButton();
    next_page_button = new QPushButton();
    last_page_button = new QPushButton();
    first_page_button->setFixedWidth(50);
    previous_page_button->setFixedWidth(50);
    next_page_button->setFixedWidth(50);
    last_page_button->setFixedWidth(50);
    first_page_button->setCursor(Qt::PointingHandCursor);
    previous_page_button->setCursor(Qt::PointingHandCursor);
    next_page_button->setCursor(Qt::PointingHandCursor);
    last_page_button->setCursor(Qt::PointingHandCursor);
    first_page_button->setStyleSheet("color:rgb(0, 120, 230); background:transparent;");
    previous_page_button->setStyleSheet("color:rgb(0, 120, 230); background:transparent;");
    next_page_button->setStyleSheet("color:rgb(0, 120, 230); background:transparent;");
    last_page_button->setStyleSheet("color:rgb(0, 120, 230); background:transparent;");

    first_page_button->setText(tr("first"));
    previous_page_button->setText(tr("previous"));
    next_page_button->setText(tr("next"));
    last_page_button->setText(tr("last"));

    connect(first_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(previous_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(next_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(last_page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    signal_mapper->setMapping(first_page_button, "first");
    signal_mapper->setMapping(previous_page_button, "previous");
    signal_mapper->setMapping(next_page_button, "next");
    signal_mapper->setMapping(last_page_button, "last");
    connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(showCell(QString)));

    bottom_layout = new QHBoxLayout();
    bottom_layout->addStretch();
    bottom_layout->addWidget(first_page_button, 0, Qt::AlignVCenter);
    bottom_layout->addWidget(previous_page_button, 0, Qt::AlignVCenter);
    for(int i=0; i<button_list->count(); i++)
    {
        QPushButton *page_button = button_list->at(i);
        bottom_layout->addWidget(page_button, 0, Qt::AlignVCenter);
    }
    bottom_layout->addWidget(next_page_button, 0, Qt::AlignVCenter);
    bottom_layout->addWidget(last_page_button, 0, Qt::AlignVCenter);
    bottom_layout->addStretch();
    bottom_layout->setSpacing(2);
    bottom_layout->setContentsMargins(0, 10, 0, 0);
}

void GridlayoutClass::showCell(QString current_skin)
{
    if(current_skin == "first")
    {
        current_page = 1;
    }
    else if(current_skin == "previous")
    {
        if(current_page > 2)
        {
            current_page = current_page - 1;
        }
    }
    else if(current_skin == "next")
    {
        if(current_page < page_count)
        {
            current_page = current_page + 1;
        }
    }
    else if(current_skin == "last")
    {
        current_page = page_count;
    }
    else
    {
        bool ok;
        current_page = current_skin.toInt(&ok, 10);
    }

    if(current_page == 1)
    {
        next_page_button->show();
        last_page_button->show();
        first_page_button->hide();
        previous_page_button->hide();
    }
    else if(current_page == page_count)
    {
        first_page_button->show();
        previous_page_button->show();
        next_page_button->hide();
        last_page_button->hide();
    }
    else
    {
        first_page_button->hide();
        previous_page_button->show();
        next_page_button->show();
        last_page_button->hide();
    }

    //第一页为0-7 显示至previous_total_page个
    int previous_total_page = (current_page - 1)*8;
    int tip_index = previous_total_page;
    if(previous_total_page > 0)
    {
        tip_index = previous_total_page - 1;
    }    
}


void GridlayoutClass::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(skin_name));

    QPainter painter2(this);
    QLinearGradient linear2(rect().topLeft(), rect().bottomLeft());
    linear2.setColorAt(0, Qt::white);
    linear2.setColorAt(0.5, Qt::white);
    linear2.setColorAt(1, Qt::white);
    painter2.setPen(Qt::white); //设定画笔颜色，到时侯就是边框颜色
    painter2.setBrush(linear2);
    painter2.drawRect(QRect(0, 30, this->width(), this->height()-30));

    QPainter painter3(this);
    painter3.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 30), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 30)};
    painter3.drawPolyline(points, 4);
}


void GridlayoutClass::mousePressEvent( QMouseEvent * event )
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }

    //窗口移动距离
    move_point = event->globalPos() - pos();
}

void GridlayoutClass::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}

void GridlayoutClass::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    if(mouse_press)
    {
        QPoint move_pos = event->globalPos();
        move(move_pos - move_point);
    }
}


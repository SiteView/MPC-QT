#include <QVBoxLayout>
#include <QSignalMapper>
#include <QPainter>
#include "testunloadlist.h"

TestUnloadList::TestUnloadList(QWidget *parent) :
    QWidget(parent)
{
    current_page = 1;
    mouse_press = false;
    WidgetContents=new QWidget();
    WidgetContents->setObjectName(QStringLiteral("WidgetContents"));
    WidgetContents->setStyleSheet("background:rgb(255,255,255);");
    initCenter();
    initBottom();
    area=new QScrollArea(this);
    area->setFixedSize(920,520);
    area->setObjectName(QStringLiteral("scrollArea"));
    area->setWidgetResizable(true);

    QVBoxLayout *main_layout = new QVBoxLayout(WidgetContents);
    main_layout->addLayout(center_layout);
    main_layout->addLayout(bottom_layout);
    main_layout->addStretch();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    WidgetContents->setLayout(main_layout);
    area->setWidget(WidgetContents);
    this->showPage(QString::number(current_page, 10));



}
void TestUnloadList::initCenter()
{
    QSqlQuery SQLiteQuery( *m_SQLiteDb.getDB() );
    if ( !SQLiteQuery.exec( "select DisplayName,DisplayVersion,EstimatedSize,SetupTime from LocalAppInfor ;" ) )
    {
        qDebug(SQLiteQuery.lastError().text().toLocal8Bit().data());
    }
    while ( SQLiteQuery.next() ){

        QVariant val0 = SQLiteQuery.value(0);
        QVariant val1 = SQLiteQuery.value(1);
        QVariant val2 = SQLiteQuery.value(2);
        QVariant val3 = SQLiteQuery.value(3);

        QString pahtstr0 = val0.toString();
        QString pahtstr1 = val1.toString();
        QString pahtstr2 = val2.toString();
        QString pahtstr3 = val3.toString();
        name_list<<pahtstr0;
        time_list<<pahtstr1;
        size_list<<pahtstr2;
        detail_list<<pahtstr3;
    }
    SQLiteQuery.finish();

    center_layout = new QVBoxLayout();

    item_list =new QList<TestUnloadItem *>();
    for(int i=0; i<10; i++)
    {
        TestUnloadItem *item = new TestUnloadItem(WidgetContents);
        center_layout->addWidget(item);
        item_list->push_back(item);
    }

    int skin_list_count = name_list.size();
    page_count = skin_list_count / 10;
    page_count_point = skin_list_count % 10;
    if(page_count_point > 0)
    {
        page_count = page_count + 1;
        qDebug()<<"page_count_point=="<<page_count_point;
    }

}
void TestUnloadList::initBottom()
{
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    QList<QPushButton *> *button_list = new QList<QPushButton *>();
    for(int i=0; i<page_count; i++)
    {
        QPushButton *page_button = new QPushButton(WidgetContents);
        page_button->setFixedWidth(20);
        page_button->setText(QString::number(i+1, 10));
        page_button->setStyleSheet("color:rgb(0, 120, 230); background:transparent;");
        page_button->setCursor(Qt::PointingHandCursor);
        connect(page_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
        signal_mapper->setMapping(page_button, page_button->text());
        button_list->push_back(page_button);
    }

    first_page_button = new QPushButton(WidgetContents);
    previous_page_button = new QPushButton(WidgetContents);
    next_page_button = new QPushButton(WidgetContents);
    last_page_button = new QPushButton(WidgetContents);
    first_page_button->setFixedWidth(50);
    previous_page_button->setFixedWidth(50);
    next_page_button->setFixedWidth(50);
    last_page_button->setFixedWidth(50);
    first_page_button->setCursor(Qt::PointingHandCursor);
    previous_page_button->setCursor(Qt::PointingHandCursor);
    next_page_button->setCursor(Qt::PointingHandCursor);
    last_page_button->setCursor(Qt::PointingHandCursor);
    first_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");
    previous_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");
    next_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");
    last_page_button->setStyleSheet("color:rgb(0, 120, 230); background:lightblue;");

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
    connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(showPage(QString)));

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


void TestUnloadList::showPage(QString current_skin)
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
        else
        {
            current_page=1;
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
    int tip_index = (current_page - 1)*10;
    if(current_page<page_count)
    {
        for(int i=0;i<10;i++)
        {
            item_list->at(i)->takeText(name_list.at(tip_index++), time_list.at(tip_index-1),size_list.at(tip_index-1) ,detail_list.at(tip_index-1));
        }
    }
    else
    {
        for(int i=0;i<page_count_point;i++)
        {
            item_list->at(i)->takeText(name_list.at(tip_index++), time_list.at(tip_index-1),size_list.at(tip_index-1) ,detail_list.at(tip_index-1));
        }
    }
}

void TestUnloadList::paintEvent(QPaintEvent *)
{
    QPainter painter2(WidgetContents);
    QLinearGradient linear2(rect().topLeft(), rect().bottomLeft());
    linear2.setColorAt(0, Qt::white);
    linear2.setColorAt(0.5, Qt::red);
    linear2.setColorAt(1, Qt::white);
    painter2.setPen(Qt::white); //设定画笔颜色，到时侯就是边框颜色
    painter2.setBrush(linear2);
    painter2.drawRect(QRect(0, 30, this->width(), this->height()));

    QPainter painter3(WidgetContents);
    painter3.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 0), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, this->height()-1)};
    painter3.drawPolyline(points, 4);
}


void TestUnloadList::mousePressEvent( QMouseEvent * event )
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }

    //窗口移动距离
    move_point = event->globalPos() - pos();
}

void TestUnloadList::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}


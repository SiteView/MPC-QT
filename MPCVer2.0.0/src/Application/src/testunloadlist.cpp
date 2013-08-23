#include <QVBoxLayout>
#include <QSignalMapper>
#include "testunloadlist.h"

TestUnloadList::TestUnloadList(QWidget *parent) :
    QWidget(parent)
{
    current_page = 1;
    WidgetContents=new QWidget();
    WidgetContents->setObjectName(QStringLiteral("WidgetContents"));
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

    item1=new TestUnloadItem(WidgetContents);
    item2=new TestUnloadItem(WidgetContents);
    item3=new TestUnloadItem(WidgetContents);
    item4=new TestUnloadItem(WidgetContents);
    item5=new TestUnloadItem(WidgetContents);
    item6=new TestUnloadItem(WidgetContents);
    item7=new TestUnloadItem(WidgetContents);
    item8=new TestUnloadItem(WidgetContents);
    item9=new TestUnloadItem(WidgetContents);
    item0=new TestUnloadItem(WidgetContents);

    center_layout->addWidget(item1);
    center_layout->addWidget(item2);
    center_layout->addWidget(item3);
    center_layout->addWidget(item4);
    center_layout->addWidget(item5);
    center_layout->addWidget(item6);
    center_layout->addWidget(item7);
    center_layout->addWidget(item8);
    center_layout->addWidget(item9);
    center_layout->addWidget(item0);

    int skin_list_count = name_list.size();
    page_count = skin_list_count / 10;
    page_count_point = skin_list_count % 10;
    if(page_count_point > 0)
    {
        page_count = page_count + 1;
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
    int previous_total_page = (current_page - 1)*10;
    int tip_index = previous_total_page;
    if(previous_total_page > 0)
    {
        tip_index = previous_total_page - 1;
    }
    item1->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item2->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item3->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item4->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item5->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item6->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item7->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item8->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item9->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));
    item0->takeText(name_list.at(tip_index++), time_list.at(tip_index++),size_list.at(tip_index++) ,detail_list.at(tip_index++));

}

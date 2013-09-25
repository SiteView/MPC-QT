#include <QMouseEvent>
#include <windows.h>
#include <QTableWidgetItem>
#include <QStringList>
#include <QPainter>
#include <QListWidget>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QStackedLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "curldownloadmanager.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),flag(true)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去除边框
    this->setWindowTitle("MarketPlace");//给窗体命名
    this->createUnloadtableMenu();
    this->createUpgradeMenu();
    this->createDownloadMenu();
    this->AddSoftSortMenu();
    downthread    = new CURLDownloadManager(this);//构造一个下载对象
//    inform        = new InformDialog;
    item_allkind  = new SoftAllKindItem();
    setting_menu  = new SettingMenu ();

    ui->lineEdit_s_4->setFrame(false);//搜索栏设为不可见
    ui->lineEdit_s_2->setFrame(false);
    ui->lineEdit_s_3->setFrame(false);
    ui->lineEdit_s_2->setMaxLength(50);//设置搜索栏的宽度
    ui->lineEdit_s_3->setMaxLength(50);
    ui->lineEdit_s_4->setMaxLength(50);

    connect(list_allkinds->list_classify,SIGNAL(itemSelectionChanged()),this,SLOT(changeCurrentItem()));

    ui->but_sel_name->setStyleSheet("QPushButton{text-align:left;}");
    ui->but_sel_name->setText("softname");
    ui->but_sel_time->setText("setime");
    ui->but_sel_path->setText("setpath");
    ui->but_sel_size->setText("softsize");
    ui->but_sel_operate->setText("operate");
    ui->but_return->hide();//返回按钮
    ui->but_return_2->hide();//返回按钮
//    ui->wdg_text->show();
    TitlePage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TitlePage()//加载标题图片
{
    QStringList string_list;
    string_list<<":/images/softdownload_no.png"<<":/images/softupgrade_no.png"<<":/images/uninstall_no.png";//<<":/images/manage_no.png";
    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->setSpacing(0);
    button_layout->setContentsMargins(0,0,0,0);
    QSignalMapper *signal_mapper = new QSignalMapper(this);
    for(int i=0; i<string_list.size(); i++)
    {
        ToolButton *tool_button = new ToolButton();
        if(i==1){
            lab_upnum = new QLabel(tool_button);
            lab_upnum->hide();
            lab_upnum->setObjectName(QString::fromUtf8("lab_upnum"));
            lab_upnum->setGeometry(QRect(38, 8, 26, 16));
            lab_upnum->setAlignment(Qt::AlignCenter|Qt::AlignCenter);
            lab_upnum->setStyleSheet("border-image:url(:/images/red.png);color:rgb(255,255,255); font: 75 12px 'Arial' ;font-weight:bold;");
            if(count_upgrade>0)
            {
                lab_upnum->setText(QString::number(count_upgrade,10));
                lab_upnum->show();
            }
        }
        tool_button->setImage(string_list.at(i));
        button_list.append(tool_button);
        connect(tool_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));//信号绑定，可以使其停留在选定的item上
        signal_mapper->setMapping(tool_button, QString::number(i, 10));
        button_layout->addWidget(tool_button, 0, Qt::AlignBottom);
    }
    connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(turnPage(QString)));//绑定改变item的信号
    button_list.at(0)->setObjectName(QString::fromUtf8("Download"));
    button_list.at(1)->setObjectName(QString::fromUtf8("Upgrade"));
    button_list.at(2)->setObjectName(QString::fromUtf8("Uninstall"));
    //    button_list.at(3)->setObjectName(QString::fromUtf8("Manage"));

    button_list.at(0)->setText("Download");
    button_list.at(1)->setText("Upgrade");
    button_list.at(2)->setText("Uninstall");
    //    button_list.at(3)->setText("Manage");
    ui->title_page->setLayout(button_layout);
    button_list.at(0)->mouse_press=true;
}

void MainWindow::turnPage(QString current_page)
{
    bool ok;
    int current_index = current_page.toInt(&ok, 10);
    for(int i=0; i<button_list.count(); i++)
    {
        ToolButton *tool_button = button_list.at(i);
        if(current_index == i)
        {
            tool_button->setMousePress(true);
        }
        else
        {
            tool_button->setMousePress(false);
        }
    }
    if(current_page=="0")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_SoftDownload);
    }
    else if(current_page=="1")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_SoftUpgrade);
    }
    else if(current_page=="2")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_SoftUnload);
    }
    else if(current_page=="3")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_UpdateInform);
    }
}

void MainWindow::paintEvent(QPaintEvent *)//画界面边框
{
    //    QPainter painter2(this);
    //    painter2.setPen(Qt::gray);
    //    static const QPointF points[4] = {QPointF(0, 100), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 100)};
    //    painter2.drawPolyline(points, 4);
    QPainter painter(this);
    QPen pen(Qt::gray);
    painter.setPen(pen);
    painter.drawRoundRect(0,0,this->width()-1, this->height()-1, 0, 0);
}

void MainWindow::createUpgradeMenu()//创建软件更新部分
{
    list_upgrade    = new SoftUpgradeList(ui->widget_6);
    list_upgrade->selectDifType();
    count_upgrade=list_upgrade->list_0.size();//更新的软件数量
}
void MainWindow::createDownloadMenu()//创建软件下载部分
{
    list_download   = new SoftDownloadList(ui->page_all);
    list_download->selectDifType(ALL);
    ui->stack_download->setCurrentWidget(ui->page_all);
    testclass=new PageModelList(ui->widget_7);
}

void MainWindow::createUnloadtableMenu()//创建软件卸载部分
{
    list_unload     = new SoftUnloadList(ui->page_selall);
    list_unload->DiffSelect(SELECT_ALL);
    ui->stackedWidget_2->setCurrentWidget(ui->page_selall);
}
void MainWindow::AddSoftSortMenu()//增加软件分类菜单
{
    SoftSortItem *soft_pick=new SoftSortItem();
    SoftSortItem *soft_sort=new SoftSortItem();
    soft_pick->takeText("litter_icon","handpick Software");
    soft_sort->takeText("litter_icon","Software classify");


    list_allkinds   = new SoftAllKindList();
    list_allkinds->add_tree_data();
    list_allkinds->add_branch_data();

    typecount=list_allkinds->count_classify;
    page_list = new QList<QWidget *>();//构造对应的page
    download_list =new QList<SoftDownloadList *>();//构造对应的Download对象
    for(int i=0;i<typecount;i++)
    {
        QWidget *page_type = new QWidget();
        ui->stack_download->addWidget(page_type);
        page_list->push_back(page_type);
        SoftDownloadList *download=new SoftDownloadList(page_type);
        download->selectDifType(i);
        download_list->push_back(download);
        qDebug()<<typecount<<"iii===="<<i;
    }

    QVBoxLayout *v_layout=new QVBoxLayout();
    v_layout->setSpacing(0);
    v_layout->setContentsMargins(11, 11, 11, 11);
    v_layout->setObjectName(QString::fromUtf8("v_layout"));
    v_layout->setContentsMargins(0, 0, 0, 0);
    v_layout->addWidget(soft_pick);
    v_layout->addWidget(list_allkinds->list_handpick);
    v_layout->addWidget(soft_sort);
    v_layout->addWidget(list_allkinds->list_classify);

    ui->scrollWidget->setLayout(v_layout);
    ui->scrollArea->setStyleSheet("background-color:#f5f5f5;");

}

void MainWindow::changeCurrentItem()//显示不同分类的软件
{
    ui->but_return->hide();
    ui->wdg_text->show();
    int current_row = list_allkinds->list_classify->currentRow();

    ui->lab_sortname->setText(list_allkinds->list_name.at(current_row));
    ui->lab_sortnum->setText(list_allkinds->list_number.at(current_row));
    ui->stack_download->setCurrentWidget(page_list->at(current_row));
    qDebug()<<current_row<<"----current_row----";
}

void MainWindow::on_but_close_clicked()//关闭窗口
{
    this->close();
}
void MainWindow::on_but_minimize_clicked()//缩小窗口
{
    this->showMinimized();
}
void MainWindow::mousePressEvent(QMouseEvent *e)//点击窗口
{
    moving = true;
    last = e->globalPos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)//移动窗口
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();

    last = e->globalPos();
    if ((e->buttons() & Qt::LeftButton) && moving)
        move(x()+dx, y()+dy);
    this->update();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)//释放窗口
{
    moving = false;
}

void MainWindow::on_but_sel_name_clicked()//卸载页以软件名称排序
{
    if(flag)
    {
        list_unload1     = new SoftUnloadList(ui->page_up);
        list_unload1->DiffSelect(SELECT_NAME_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_up);
        ui->but_sel_name->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        list_unload2     = new SoftUnloadList(ui->page_down);
        list_unload2->DiffSelect(SELECT_NAME_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_down);
        ui->but_sel_name->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }
}

void MainWindow::on_but_sel_size_clicked()//卸载页以软件大小排序
{
    if(flag)
    {
        list_unload3     = new SoftUnloadList(ui->page_2);
        list_unload3->DiffSelect(SELECT_SIZE_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_2);
        ui->but_sel_size->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        list_unload4     = new SoftUnloadList(ui->page_3);
        list_unload4->DiffSelect(SELECT_SIZE_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_3);
        ui->but_sel_size->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }
}

void MainWindow::on_but_sel_time_clicked()//卸载页以软件安装时间排序
{
    if(flag)
    {
        list_unload5     = new SoftUnloadList(ui->page_4);
        list_unload5->DiffSelect(SELECT_TIME_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_4);
        ui->but_sel_time->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        list_unload6     = new SoftUnloadList(ui->page_5);
        list_unload6->DiffSelect(SELECT_TIME_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_5);
        ui->but_sel_time->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }
}

void MainWindow::on_but_sel_path_clicked()//卸载页以软件安装路径排序
{
    if(flag)
    {
        list_unload7     = new SoftUnloadList(ui->page_6);
        list_unload7->DiffSelect(SELECT_PATH_UP);
        ui->stackedWidget_2->setCurrentWidget(ui->page_6);
        ui->but_sel_path->setIcon(QIcon(":/images/down.png"));
        flag=false;
    }
    else
    {
        list_unload8     = new SoftUnloadList(ui->page_7);
        list_unload8->DiffSelect(SELECT_PATH_DOWN);
        ui->stackedWidget_2->setCurrentWidget(ui->page_7);
        ui->but_sel_path->setIcon(QIcon(":/images/up.png"));
        flag=true;
    }
}

void MainWindow::on_but_sel_operate_clicked()
{

}

void MainWindow::on_but_clear_4_clicked()//软件下载页清除按钮
{
    ui->lineEdit_s_4->clear();
}

void MainWindow::on_but_search_4_clicked()//软件下载页搜索按钮
{
    search_download=new SoftDownloadList(ui->page_search);
    search_download->search_text=ui->lineEdit_s_4->text();
    search_download->selectDifType(SEARCH);
    if(search_download->empty)
    {
        QString str="Didn't find related software ,Please adjust the keywords to search again!";
        ui->lab_null_caution->setStyleSheet("border-image:url(:/images/caution.png)");
        ui->lab_null_text->setText(str);
        ui->stack_download->setCurrentWidget(ui->page_null_default);
    }
    else
    {
        ui->stack_download->setCurrentWidget(ui->page_search);
    }
    ui->but_return->show();
    ui->wdg_text->hide();
}

void MainWindow::on_but_clear_3_clicked()//软件更新页清除按钮
{
    ui->lineEdit_s_3->clear();
}

void MainWindow::on_but_search_3_clicked()//软件更新页搜索按钮
{

}

void MainWindow::on_but_clear_2_clicked()//软件卸载页清除按钮
{
    ui->lineEdit_s_2->clear();
}

void MainWindow::on_but_search_2_clicked()//软件卸载页搜索按钮
{
    SoftUnloadList *search_unload  = new SoftUnloadList(ui->page_search_2);
    search_unload->search_text=ui->lineEdit_s_2->text();
    search_unload->DiffSelect(SELECT_SEARCH);
    if(search_unload->empty)
    {
        QString str="Didn't find related software ,Please adjust the keywords to search again!";
        ui->lab_caution_unload->setStyleSheet("border-image:url(:/images/caution.png)");
        ui->lab_text_unload->setText(str);
        ui->stackedWidget_2->setCurrentWidget(ui->page_null_unload);
        ui->widget_5->hide();
    }
    else
    {
        ui->stackedWidget_2->setCurrentWidget(ui->page_search_2);
    }
    ui->but_return_2->show();
}

void MainWindow::on_but_return_2_clicked()//软件卸载页返回
{
    ui->but_return_2->hide();
    ui->stackedWidget_2->setCurrentWidget(ui->page_selall);
    ui->widget_5->show();
}

void MainWindow::on_but_return_clicked()//软件下载页返回
{
    ui->but_return->hide();
    ui->wdg_text->show();
    changeCurrentItem();
}

void MainWindow::on_but_setting_clicked()
{
    QPoint p = rect().topRight();
    p.setX(p.x() - 200);
    p.setY(p.y() + 15);
    setting_menu->exec(this->mapToGlobal(p));
}

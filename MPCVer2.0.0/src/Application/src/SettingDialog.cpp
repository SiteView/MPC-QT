#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>
#include <QtDebug>
#include <Windows.h>
#include "SettingDialog.h"
#include "myslider.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent)
{
    this->resize(560, 460);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->initTitle();
    this->initCenter();
    this->initTab1();
    this->initTab2();
    this->initTab3();
    this->initTab4();
    this->initBottom();
    filepath=QCoreApplication::applicationDirPath();
    dp_soft=filepath;
    dp_apply=filepath;
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(title_layout);
    main_layout->addWidget(tab_widget);
    main_layout->addLayout(bottom_layout);
    main_layout->addStretch();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(main_layout);
    mouse_press = false;
    translation();

//    SwitchMenu switchMenu;

//    switchMenu.initialize(":/images/button/btn_on_normal.png", ":/images/button/btn_off_normal.png");

//    switchMenu.show();
}

void SettingDialog::initTitle()
{
    title_label = new QLabel();
    title_icon_label = new QLabel();
    close_button = new QPushButton();
    close_button->setFixedSize(27,20);
    close_button->setObjectName(QString::fromUtf8("c_but"));
    close_button->setIcon(QIcon(":/images/but_close.png"));
    title_label->setStyleSheet("color:white;");
    QPixmap pixmap(":/images/logo.48.png");
    title_icon_label->setPixmap(pixmap);
    title_icon_label->setFixedSize(16, 16);
    title_icon_label->setScaledContents(true);
    title_layout = new QHBoxLayout();
    title_layout->addWidget(title_icon_label, 0, Qt::AlignVCenter);
    title_layout->addWidget(title_label, 0, Qt::AlignVCenter);
    title_layout->addStretch();
    title_layout->addWidget(close_button, 0, Qt::AlignTop);
    title_layout->setSpacing(5);
    title_layout->setContentsMargins(10, 0, 5, 10);
    connect(close_button, SIGNAL(clicked()), this, SLOT(hide()));
}

void SettingDialog::initCenter()
{
    tab_widget = new QTabWidget();
    tab_widget->setFixedSize(this->width(), this->height()-80);
    //    tab_widget->setGeometry(QRect(50, 80, this->width()-10,  this->height()-30));
    tab1 = new QWidget();
    tab2 = new QWidget();
    tab3 = new QWidget();
    tab4 = new QWidget();
    tab_widget->setStyleSheet("QTabWidget::pane{border: 5px;}"
                              "QTabWidget::tab-bar{alignment:center ;}"
                              "QTabBar::tab{background:transparent; color:white; min-width:30ex; min-height:10ex;}"
                              "QTabBar::tab:hover{background:rgb(255, 255, 255, 100);}"
                              "QTabBar::tab:selected{border-color: white;background:white;color:green;}");
    tab_widget->addTab(tab1, tr("rise style"));
    tab_widget->addTab(tab2, tr("advanced setting"));
    tab_widget->addTab(tab3, tr("physical setting"));
    tab_widget->addTab(tab4, tr("user setting"));
}

void SettingDialog::initBottom()
{
    ok_button = new QPushButton();
    cancel_button = new QPushButton();
    ok_button->setObjectName(QStringLiteral("ok_button"));
    cancel_button->setObjectName(QStringLiteral("ok_button"));
    ok_button->setFixedSize(75, 25);
    cancel_button->setFixedSize(75, 25);
    bottom_layout = new QHBoxLayout();
    bottom_layout->addStretch();
    bottom_layout->addWidget(ok_button);
    bottom_layout->addWidget(cancel_button);
    bottom_layout->setSpacing(20);
    bottom_layout->setContentsMargins(0, 10, 20, 0);
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ok_button, SIGNAL(clicked()), this, SLOT(on_but_ok_clicked()));
}

void SettingDialog::initTab1()
{
    tab1_group_box = new QGroupBox();
    tab1_group_box->setTitle("Download Setting");
    tab1_group_box->setStyleSheet("QGroupBox::title{color:green;}");
    tab1_group_box->setObjectName(QStringLiteral("tab1_group_box"));
    tab1_group_box->setGeometry(QRect(70, 20, 500, 160));
    tab1_group_box->setFixedSize(500,160);
    v_Layout = new QVBoxLayout(tab1_group_box);
    v_Layout->setSpacing(2);
    v_Layout->setObjectName(QStringLiteral("v_Layout"));
    v_Layout->setContentsMargins(25, 0, 25, 15);
    Layout_1 = new QHBoxLayout();
    Layout_1->setSpacing(6);
    Layout_1->setObjectName(QStringLiteral("Layout_1"));
    lab_soft_dir = new QLabel(tab1_group_box);
    lab_soft_dir->setObjectName(QStringLiteral("lab_soft_dir"));
    Layout_1->addWidget(lab_soft_dir);
    but_open_dir = new QPushButton(tab1_group_box);
    but_open_dir->setObjectName(QStringLiteral("but_open_dir"));
    but_open_dir->setStyleSheet("background:transparent;");
    but_open_dir->setCursor(Qt::PointingHandCursor);
    Layout_1->addWidget(but_open_dir);
    lab_disc = new QLabel(tab1_group_box);
    lab_disc->setObjectName(QStringLiteral("lab_disc"));
    Layout_1->addWidget(lab_disc);
    lab_disc_size = new QLabel(tab1_group_box);
    lab_disc_size->setObjectName(QStringLiteral("lab_disc_size"));
    Layout_1->addWidget(lab_disc_size);
    Spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    Layout_1->addItem(Spacer);
    v_Layout->addLayout(Layout_1);
    Layout_2 = new QHBoxLayout();
    Layout_2->setSpacing(6);
    Layout_2->setObjectName(QStringLiteral("Layout_2"));
    lab_show_dir = new QLabel(tab1_group_box);
    lab_show_dir->setObjectName(QStringLiteral("lab_show_dir"));
    lab_show_dir->setMaximumSize(QSize(275, 20));
    Layout_2->addWidget(lab_show_dir);
    but_chos_dir = new QPushButton(tab1_group_box);
    but_chos_dir->setObjectName(QStringLiteral("but_chos_dir"));
    but_chos_dir->setMaximumSize(QSize(72, 22));
    Layout_2->addWidget(but_chos_dir);
    but_recover_dir = new QPushButton(tab1_group_box);
    but_recover_dir->setObjectName(QStringLiteral("but_recover_dir"));
    but_recover_dir->setMaximumSize(QSize(88, 22));
    but_recover_dir->setStyleSheet("background:transparent;");
    but_recover_dir->setCursor(Qt::PointingHandCursor);
    Layout_2->addWidget(but_recover_dir);
    v_Layout->addLayout(Layout_2);
    Layout_3 = new QHBoxLayout();
    Layout_3->setSpacing(6);
    Layout_3->setObjectName(QStringLiteral("Layout_3"));
    lab_apply_dir = new QLabel(tab1_group_box);
    lab_apply_dir->setObjectName(QStringLiteral("lab_apply_dir"));
    Layout_3->addWidget(lab_apply_dir);
    but_open_dir_2 = new QPushButton(tab1_group_box);
    but_open_dir_2->setObjectName(QStringLiteral("but_open_dir_2"));
    but_open_dir_2->setStyleSheet("background:transparent;");
    but_open_dir_2->setCursor(Qt::PointingHandCursor);
    Layout_3->addWidget(but_open_dir_2);
    lab_disc_2 = new QLabel(tab1_group_box);
    lab_disc_2->setObjectName(QStringLiteral("lab_disc_2"));
    Layout_3->addWidget(lab_disc_2);
    lab_disc_size_2 = new QLabel(tab1_group_box);
    lab_disc_size_2->setObjectName(QStringLiteral("lab_disc_size_2"));
    Layout_3->addWidget(lab_disc_size_2);
    Spacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    Layout_3->addItem(Spacer_2);
    v_Layout->addLayout(Layout_3);
    Layout_4 = new QHBoxLayout();
    Layout_4->setSpacing(6);
    Layout_4->setObjectName(QStringLiteral("Layout_4"));
    lab_show_dir_2 = new QLabel(tab1_group_box);
    lab_show_dir_2->setObjectName(QStringLiteral("lab_show_dir_2"));
    lab_show_dir_2->setMaximumSize(QSize(275, 20));
    Layout_4->addWidget(lab_show_dir_2);
    but_chos_dir_2 = new QPushButton(tab1_group_box);
    but_chos_dir_2->setObjectName(QStringLiteral("but_chos_dir_2"));
    but_chos_dir_2->setMaximumSize(QSize(72, 22));
    Layout_4->addWidget(but_chos_dir_2);
    but_recover_dir_2 = new QPushButton(tab1_group_box);
    but_recover_dir_2->setObjectName(QStringLiteral("but_recover_dir_2"));
    but_recover_dir_2->setMaximumSize(QSize(88, 22));
    but_recover_dir_2->setStyleSheet("background:transparent;");
    but_recover_dir_2->setCursor(Qt::PointingHandCursor);
    Layout_4->addWidget(but_recover_dir_2);
    v_Layout->addLayout(Layout_4);
    tab1_group_box2 = new QGroupBox();
    tab1_group_box2->setTitle("Download Inform");
    tab1_group_box2->setStyleSheet("QGroupBox::title{color:green;}");
    tab1_group_box2->setObjectName(QStringLiteral("tab1_group_box2"));
    tab1_group_box2->setGeometry(QRect(70, 200, 500, 72));
    tab1_group_box2->setFixedSize(500,72);
    lab_down_inform = new QLabel(tab1_group_box2);
    lab_down_inform->setObjectName(QStringLiteral("lab_down_inform"));
    lab_down_inform->setGeometry(QRect(30, 30, 281, 16));
    but_on_off = new QLabel(tab1_group_box2);
    but_on_off->setObjectName(QStringLiteral("but_on_off"));
    but_on_off->setGeometry(QRect(360, 30, 68, 17));
//    switchMenu = new SwitchMenu(but_on_off);
//    switchMenu->initialize(":/images/but_about.png", ":/images/red.png");
    int nX = 0;//位置坐标
    int nY = 0;
    QString defImagePath = ":/images/button/on_bg.png";//背景及开关图片
    QString actImagePath = ":/images/button/niu.png";
    QString defImagePath_off = ":/images/button/off_bg.png";//背景及开关图片

    MySliderMenu *pMySlider = new MySliderMenu(but_on_off,nX,nY,defImagePath,defImagePath_off,actImagePath,0);

    QVBoxLayout *tab1_layout = new QVBoxLayout();
    tab1_layout->addWidget(tab1_group_box, 0, Qt::AlignCenter);
    tab1_layout->addWidget(tab1_group_box2, 0, Qt::AlignCenter);
    tab1_layout->setSpacing(10);
    tab1_layout->setContentsMargins(0, 20, 0, 0);
    tab1->setLayout(tab1_layout);

    connect(but_chos_dir,SIGNAL(clicked()),this,SLOT(on_but_chos_dir_clicked()));
    connect(but_chos_dir_2,SIGNAL(clicked()),this,SLOT(on_but_chos_dir2_clicked()));
    connect(but_open_dir,SIGNAL(clicked()),this,SLOT(on_but_open_dir_clicked()));
    connect(but_open_dir_2,SIGNAL(clicked()),this,SLOT(on_but_open_dir2_clicked()));
    connect(but_recover_dir,SIGNAL(clicked()),this,SLOT(on_but_recover_dir_clicked()));
    connect(but_recover_dir_2,SIGNAL(clicked()),this,SLOT(on_but_recover_dir2_clicked()));
}

void SettingDialog::initTab2()
{
    tab2_group_box1 = new QGroupBox();
    tab2_group_box2 = new QGroupBox();
    tab2_group_box3 = new QGroupBox();
    auto_start_check_box = new QCheckBox();
    remove_own_check_box = new QCheckBox();
    strong_remove_check_box = new QCheckBox();
    mummy_kill_check_box = new QCheckBox();
    display_count_check_box = new QCheckBox();
    tab2_group_box1->setStyleSheet("QGroupBox::title{color:green;}");
    tab2_group_box2->setStyleSheet("QGroupBox::title{color:green;}");
    tab2_group_box3->setStyleSheet("QGroupBox::title{color:green;}");
    QFont group_box_font = tab2_group_box1->font();
    group_box_font.setBold(true);
    tab2_group_box1->setFont(group_box_font);
    tab2_group_box2->setFont(group_box_font);
    tab2_group_box3->setFont(group_box_font);
    QHBoxLayout *group_box1_layout = new QHBoxLayout();
    group_box1_layout->addWidget(auto_start_check_box);
    group_box1_layout->setSpacing(0);
    group_box1_layout->setContentsMargins(30, 0, 0, 0);
    tab2_group_box1->setLayout(group_box1_layout);
    QVBoxLayout *group_box2_layout = new QVBoxLayout();
    group_box2_layout->addWidget(remove_own_check_box);
    group_box2_layout->addWidget(strong_remove_check_box);
    group_box2_layout->addWidget(mummy_kill_check_box);
    group_box2_layout->setSpacing(0);
    group_box2_layout->setContentsMargins(30, 0, 0, 0);
    tab2_group_box2->setLayout(group_box2_layout);
    QHBoxLayout *group_box3_layout = new QHBoxLayout();
    group_box3_layout->addWidget(display_count_check_box);
    group_box3_layout->setSpacing(0);
    group_box3_layout->setContentsMargins(30, 0, 0, 0);
    tab2_group_box3->setLayout(group_box3_layout);
    tab2_group_box1->setFixedSize(480, 60);
    tab2_group_box2->setFixedSize(480, 110);
    tab2_group_box3->setFixedSize(480, 60);
    QVBoxLayout *group_layout = new QVBoxLayout();
    group_layout->addWidget(tab2_group_box1, 0, Qt::AlignCenter);
    group_layout->addWidget(tab2_group_box2, 0, Qt::AlignCenter);
    group_layout->addWidget(tab2_group_box3, 0, Qt::AlignCenter);
    group_layout->addStretch();
    group_layout->setSpacing(10);
    group_layout->setContentsMargins(0, 20, 0, 0);
    tab2->setLayout(group_layout);
}

void SettingDialog::initTab3()
{
    tab3_group_box1 = new QGroupBox();
    tab3_group_box2 = new QGroupBox();
    auto_check_button = new QRadioButton();
    first_check_button = new QRadioButton();
    hand_check_button = new QRadioButton();
    select_quit_button = new QRadioButton();
    backstage_mode_button = new QRadioButton();
    immediacy_close_button = new QRadioButton();
    tab3_group_box1->setStyleSheet("QGroupBox::title{color:green;}");
    tab3_group_box2->setStyleSheet("QGroupBox::title{color:green;}");
    QFont group_box_font = tab3_group_box1->font();
    group_box_font.setBold(true);
    tab3_group_box1->setFont(group_box_font);
    tab3_group_box2->setFont(group_box_font);
    QVBoxLayout *group_box1_layout = new QVBoxLayout();
    group_box1_layout->addWidget(auto_check_button);
    group_box1_layout->addWidget(first_check_button);
    group_box1_layout->addWidget(hand_check_button);
    group_box1_layout->setSpacing(0);
    group_box1_layout->setContentsMargins(30, 0, 0, 0);
    tab3_group_box1->setLayout(group_box1_layout);
    QVBoxLayout *group_box2_layout = new QVBoxLayout();
    group_box2_layout->addWidget(select_quit_button);
    group_box2_layout->addWidget(backstage_mode_button);
    group_box2_layout->addWidget(immediacy_close_button);
    group_box2_layout->setSpacing(0);
    group_box2_layout->setContentsMargins(30, 0, 0, 0);
    tab3_group_box2->setLayout(group_box2_layout);
    tab3_group_box1->setFixedSize(480, 120);
    tab3_group_box2->setFixedSize(480, 120);
    QVBoxLayout *group_layout = new QVBoxLayout();
    group_layout->addWidget(tab3_group_box1, 0, Qt::AlignCenter);
    group_layout->addWidget(tab3_group_box2, 0, Qt::AlignCenter);
    group_layout->addStretch();
    group_layout->setSpacing(10);
    group_layout->setContentsMargins(0, 20, 0, 0);
    tab3->setLayout(group_layout);

    if(auto_check_button->isChecked())
    {

    }
    else if(first_check_button->isChecked())
    {

    }
    else if(hand_check_button->isChecked())
    {

    }

}

void SettingDialog::initTab4()
{
    tab4_group_box = new QGroupBox();
    diaplay_experience_check_box = new QCheckBox();
    diaplay_login_check_box = new QCheckBox();
    tray_quit_check_box = new QCheckBox();
    new_character_check_box = new QCheckBox();
    rise_remind_check_box = new QCheckBox();
    tab4_group_box->setStyleSheet("QGroupBox::title{color:green;}");
    QFont group_box_font = tab4_group_box->font();
    group_box_font.setBold(true);
    tab4_group_box->setFont(group_box_font);
    tab4_group_box->setFixedSize(480, 180);
    QVBoxLayout *group_layout = new QVBoxLayout();
    group_layout->addWidget(diaplay_experience_check_box);
    group_layout->addWidget(diaplay_login_check_box);
    group_layout->addWidget(tray_quit_check_box);
    group_layout->addWidget(new_character_check_box);
    group_layout->addWidget(rise_remind_check_box);
    group_layout->setSpacing(0);
    group_layout->setContentsMargins(30, 0, 0, 0);
    tab4_group_box->setLayout(group_layout);
    QVBoxLayout *tab4_layout = new QVBoxLayout();
    tab4_layout->addWidget(tab4_group_box, 0 , Qt::AlignCenter);
    tab4_layout->addStretch();
    tab4_layout->setSpacing(0);
    tab4_layout->setContentsMargins(0, 20, 0, 0);
    tab4->setLayout(tab4_layout);
}
//SettingDialog::~SettingDialog()
//{

//}

void SettingDialog::mousePressEvent( QMouseEvent * event )
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
    }

    //窗口移动距离
    move_point = event->globalPos() - pos();
}

void SettingDialog::mouseReleaseEvent(QMouseEvent *)
{
    mouse_press = false;
}

void SettingDialog::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    if(mouse_press)
    {
        QPoint move_pos = event->globalPos();
        move(move_pos - move_point);
    }
}
void SettingDialog::paintEvent(QPaintEvent *)
{
    QString skin_name=":/images/title.png";
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(skin_name));

    QPainter painter2(this);
    QLinearGradient linear2(rect().topLeft(), rect().bottomLeft());
    linear2.setColorAt(0, Qt::white);
    linear2.setColorAt(0.5, Qt::white);
    linear2.setColorAt(1, Qt::white);
    painter2.setPen(Qt::white); //设定画笔颜色，到时侯就是边框颜色
    painter2.setBrush(linear2);
    painter2.drawRect(QRect(0, 60, this->width(), this->height()-60));

    QPainter painter3(this);
    painter3.setPen(Qt::gray);
    static const QPointF points[4] = {QPointF(0, 60), QPointF(0, this->height()-1), QPointF(this->width()-1, this->height()-1), QPointF(this->width()-1, 60)};
    painter3.drawPolyline(points, 4);
}
void SettingDialog::translation()
{
    lab_soft_dir->setText(tr("Download directory ("));
    but_open_dir->setText(tr("open dir"));
    lab_disc->setText(tr(") | ffee disk:"));
    lab_show_dir->setText(filepath);
    but_chos_dir->setText(tr("choose dir"));
    but_recover_dir->setText(tr("default directory"));
    lab_apply_dir->setText(tr("Apply directory ("));
    but_open_dir_2->setText(tr("open dir"));
    lab_disc_2->setText(tr(") | ffee disk:"));
    lab_show_dir_2->setText(filepath);
    but_chos_dir_2->setText(tr("choose dir"));
    but_recover_dir_2->setText(tr("default directory"));
    lab_down_inform->setText(tr("Information such as tips before download software whether charges"));
    quint64 freeSpace =getDiskFreeSpace(QString(filepath));
    lab_disc_size->setText(QString::number(freeSpace,10)+"GB");
    lab_disc_size_2->setText(QString::number(freeSpace,10)+"GB");
    ok_button->setText(tr("ok"));
    cancel_button->setText(tr("cancel"));

    tab3_group_box1->setTitle(tr("Clean package"));
    tab3_group_box2->setTitle(tr("Close when download"));
    auto_check_button->setText(tr(""));
    first_check_button->setText(tr(""));
    hand_check_button->setText(tr(""));
    select_quit_button->setText(tr(""));
    backstage_mode_button->setText(tr(""));
    immediacy_close_button->setText(tr(" "));
}

void SettingDialog::on_but_chos_dir_clicked()
{
    direc_soft = QFileDialog::getExistingDirectory(
                this,
                "Select one folder to send",
                "",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(direc_soft!=NULL)
    {
        lab_show_dir->setText(direc_soft);
        qDebug()<<lab_show_dir->text()<<"---lab_show_dir---";
        dp_soft=direc_soft;
        quint64 freeSpace =getDiskFreeSpace(QString(direc_soft));
        lab_disc_size->setText(QString::number(freeSpace,10)+"GB");
    }
}

void SettingDialog::on_but_chos_dir2_clicked()
{
    direc_apply = QFileDialog::getExistingDirectory(
                this,
                "Select one folder to send",
                "/home",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(direc_apply!=NULL)
    {
        lab_show_dir_2->setText(direc_apply);
        qDebug()<<lab_show_dir_2->text()<<"---lab_show_dir_2---";
        dp_apply=direc_apply;
        quint64 freeSpace =getDiskFreeSpace(QString(direc_apply));
        lab_disc_size_2->setText(QString::number(freeSpace,10)+"GB");
    }
}

void SettingDialog::on_but_open_dir_clicked()
{
    QString Qurl=lab_show_dir->text();
    QDesktopServices::openUrl(QUrl("file:/"+Qurl, QUrl::TolerantMode));
}

void SettingDialog::on_but_open_dir2_clicked()
{
    QString Qurl=lab_show_dir_2->text();
    QDesktopServices::openUrl(QUrl("file:/"+Qurl, QUrl::TolerantMode));
}

void SettingDialog::on_but_recover_dir_clicked()
{
    lab_show_dir->setText(filepath);
    dp_soft=filepath;
    quint64 freeSpace =getDiskFreeSpace(QString(filepath));
    lab_disc_size->setText(QString::number(freeSpace,10)+"GB");
}

void SettingDialog::on_but_recover_dir2_clicked()
{
    lab_show_dir_2->setText(filepath);
    dp_apply=filepath;
    quint64 freeSpace =getDiskFreeSpace(QString(filepath));
    lab_disc_size_2->setText(QString::number(freeSpace,10)+"GB");
}

quint64 SettingDialog::getDiskFreeSpace(QString driver)
{
    LPCWSTR lpcwstrDriver=(LPCWSTR)driver.utf16();
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
        return 0;
    }
    return (quint64) liTotalFreeBytes.QuadPart/1024/1024/1024;
}

QString SettingDialog::on_but_ok_clicked()
{
    this->close();
    return lab_show_dir->text();
}


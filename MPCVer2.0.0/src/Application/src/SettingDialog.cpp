#include "SettingDialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent)
{
    this->resize(560, 400);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->initTitle();
    this->initCenter();
    this->initTab1();
    this->initTab2();
    this->initTab3();
    this->initTab4();
    this->initBottom();
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(title_layout);
    main_layout->addWidget(tab_widget);
    main_layout->addLayout(bottom_layout);
    main_layout->addStretch();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(main_layout);
    mouse_press = false;
}


void SettingDialog::initTitle()
{
    title_label = new QLabel();
    title_icon_label = new QLabel();
    close_button = new QPushButton();
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
//    setStyleSheet("background-color:lightblue");

    connect(close_button, SIGNAL(clicked()), this, SLOT(hide()));

}

void SettingDialog::initCenter()
{
    tab_widget = new QTabWidget();
    tab_widget->setFixedSize(this->width(), this->height()-80);
    tab1 = new QWidget();
    tab2 = new QWidget();
    tab3 = new QWidget();
    tab4 = new QWidget();
    tab_widget->setStyleSheet("QTabWidget::pane{border: 0px;}"
        "QTabWidget::tab-bar{alignment:center;}"
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

    ok_button->setFixedSize(75, 25);
    cancel_button->setFixedSize(75, 25);
    ok_button->setStyleSheet("QPushButton{border:1px solid lightgray; background:rgb(230,230,230);}"
        "QPushButton:hover{border-color:green; background:transparent;}");
    cancel_button->setStyleSheet("QPushButton{border:1px solid lightgray; background:rgb(230,230,230);}"
        "QPushButton:hover{border-color:green; background:transparent;}");

    bottom_layout = new QHBoxLayout();
    bottom_layout->addStretch();
    bottom_layout->addWidget(ok_button);
    bottom_layout->addWidget(cancel_button);
    bottom_layout->setSpacing(20);
    bottom_layout->setContentsMargins(0, 10, 20, 0);

    connect(cancel_button, SIGNAL(clicked()), this, SLOT(hide()));
}
void SettingDialog::initTab1()
{
    tab1_group_box = new QGroupBox();
    tab1_group_box2 = new QGroupBox();

    auto_rise_button = new QRadioButton();
    not_auto_rise_button = new QRadioButton();
    rise_mummy_check_box = new QCheckBox();
    game_check_box = new QCheckBox();
    g3_check_box = new QCheckBox();
    p2p_check_box = new QCheckBox();
    mummy_check_box = new QCheckBox();

    tab1_group_box->setTitle("Download Setting");
    tab1_group_box2->setTitle("Download Inform");
    tab1_group_box->setStyleSheet("QGroupBox::title{color:green;}");
    QFont group_box_font = tab1_group_box->font();
    group_box_font.setBold(true);
    tab1_group_box->setFont(group_box_font);
    tab1_group_box->setFixedSize(480, 250);

    QHBoxLayout *rise_mummy_layout = new QHBoxLayout();
    rise_mummy_layout->addWidget(rise_mummy_check_box);
    rise_mummy_layout->setSpacing(0);
    rise_mummy_layout->setContentsMargins(20, 0, 0, 0);

    QVBoxLayout *group_layout = new QVBoxLayout();
    group_layout->addWidget(auto_rise_button);
    group_layout->addWidget(not_auto_rise_button);
    group_layout->addLayout(rise_mummy_layout);
    group_layout->addWidget(game_check_box);
    group_layout->addWidget(g3_check_box);
    group_layout->addWidget(p2p_check_box);
    group_layout->addWidget(mummy_check_box);
    group_layout->setSpacing(0);
    group_layout->setContentsMargins(30, 0, 0, 0);
    tab1_group_box->setLayout(group_layout);

    QHBoxLayout *tab1_layout = new QHBoxLayout();
    tab1_layout->addWidget(tab1_group_box);
    tab1_layout->setSpacing(0);
    tab1_layout->setContentsMargins(0, 0, 0, 0);
    tab1->setLayout(tab1_layout);
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

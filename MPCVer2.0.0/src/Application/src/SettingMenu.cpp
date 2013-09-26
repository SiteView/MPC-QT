#include "SettingMenu.h"

SettingMenu::SettingMenu(QWidget *parent) :
    QMenu(parent)
{
    this->createActions();
    this->translateActions();
}
void SettingMenu::createActions()
{

    //创建菜单项
    action_setting = new QAction(this);
    action_new_character = new QAction(this);
    action_check_update = new QAction(this);
    action_change_company = new QAction(this);
    action_help_online = new QAction(this);
    action_platform_help = new QAction(this);
    action_login_home = new QAction(this);
    action_protect = new QAction(this);
    action_about_us = new QAction(this);
    //添加菜单项
    this->addAction(action_setting);
    this->addAction(action_new_character);
    this->addAction(action_check_update);
    this->addAction(action_change_company);
    this->addSeparator();
    this->addAction(action_help_online);
    this->addAction(action_platform_help);
    this->addAction(action_login_home);
    this->addAction(action_protect);
    this->addAction(action_about_us);
    action_about_us->setIcon(QIcon(":/images/mainMenu/about.png"));
    action_help_online->setIcon(QIcon(":/images/mainMenu/help.png"));
    action_check_update->setIcon(QIcon(":/images/mainMenu/update.png"));
    action_setting->setIcon(QIcon(":/images/mainMenu/setting.png"));
    //设置信号连接
    QObject::connect(action_setting, SIGNAL(triggered()), this, SIGNAL(showSettingDialog()));
    QObject::connect(action_new_character, SIGNAL(triggered()), this, SIGNAL(showNewCharacter()));
    QObject::connect(action_about_us, SIGNAL(triggered()), this, SIGNAL(showAboutUs()));

}

void SettingMenu::translateActions()
{
    action_setting->setText(tr("setting"));
    action_new_character->setText(tr("new character"));
    action_check_update->setText(tr("check update"));
    action_change_company->setText(tr("change company"));
    action_help_online->setText(tr("help online"));
    action_platform_help->setText(tr("platform help"));
    action_login_home->setText(tr("login home"));
    action_protect->setText(tr("protect"));
    action_about_us->setText(tr("about us"));
}
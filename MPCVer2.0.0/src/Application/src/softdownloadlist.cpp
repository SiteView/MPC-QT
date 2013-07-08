#include "softdownloadlist.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QFont>
SoftDownloadList::SoftDownloadList(QWidget *parent) :
    QWidget(parent)
{
    icon       =new QLabel(this);
    softname   =new QLabel(this);
    softdetail =new QLabel(this);
    progress   =new QLabel(this);
    speed      =new QLabel(this);
    comment    =new QLabel(this);
    download   =new QPushButton(this);
    QFont font;
    font.setBold(true);
    softname->setFont(font);
    softdetail->setFont(font);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(2);
    layout->addWidget(icon);
    layout->addWidget(softname);
    layout->addWidget(softdetail);
    layout->addWidget(progress);
    layout->addWidget(speed);
    layout->addWidget(comment);
    layout->addWidget(download);
    this->setLayout(layout);
    //layout end

    retranslateUi();

}
void SoftDownloadList::retranslateUi()
{
    progress->setPixmap(QPixmap(":/images/title.png"));

    softname->setText("youkuodgsdsgdgs");
    download->setText("downloadgd");
}

#include "start.h"
#include "allapplicationdialog.h"
#include <QPushButton>
start::start(QWidget *parent) :
    QWidget(parent)
{
    QPushButton * start = new QPushButton("start");
    connect(start, SIGNAL(clicked()), this, SLOT(OnBtnDownloadClicked()));
    start->show();
}

void start::OnBtnDownloadClicked()
{
    AllApplicationDialog allapplication;
    allapplication.show();

}


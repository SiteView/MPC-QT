#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QIcon>
#include "allapplicationdialog.h"
#include "initialize.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    

    //InitializeInfo* test = new InitializeInfo();
    //QList <TaskInformation *> updateTaskInformationList = test->GetUpdateTaskInformation();
    //QList <TaskInformation *> newTaskInformationList = test->GetNewTaskInformation();
    //QList <TaskInformation *> normalTaskInformationList = test->GetNormalTaskInformation();


    //AllApplicationDialog *hello1 = new AllApplicationDialog(&updateTaskInformationList);
    //AllApplicationDialog *hello2= new AllApplicationDialog(&newTaskInformationList);
    //AllApplicationDialog *hello3 = new AllApplicationDialog(&normalTaskInformationList);

    //hello3->show();
    //hello2->show();
    //hello1->show();

    return a.exec();
}

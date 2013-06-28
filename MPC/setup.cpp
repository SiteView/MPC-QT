#include "setup.h"
#include <QFile>
#include <QString>
#include <QProcess>
#include <QDebug>
#include <QDir>
Setup::Setup(const QString& sFileName, const QString& sCmd, const int iTaskId, QObject *parent) :
    QObject(parent)
{
    m_sFileName = sFileName;
    m_sCmd = sCmd;
    m_iTaskId = iTaskId;
}

void Setup::vExecute()
{
    QFile l_fTemp(m_sFileName);

    //qDebug() << m_sFileName;
    //下载失败则直接返回
    if(10 > l_fTemp.size())
    {
        qDebug() <<m_sFileName << "Empty file!";
        //emit sig_vSetupfinish();
        return;
    }

    //QString l_sReplace = m_sFileName.left(m_sFileName.size()-3);
    QString l_sReplace = m_sFileName;

    //if(QFile::exists(l_sReplace))
    //{
    //    QFile::remove(l_sReplace);
    //}
    //如果是EXE文件则需要进行安装
    QProcess *silentExecute = new QProcess();
    QStringList l_slCmd;
    //l_slCmd << m_sCmd;
    //silentExecute->start(l_sReplace, QStringList(m_sCmd));
    //silentExecute->execute("CMD");

    //aaron 0401
    //silentExecute->start("cmd", QStringList()<<l_sReplace);

    //l_sReplace = path +"/" + l_sReplace;
    //QString a = "/";
    //QString b = "\\";

    l_sReplace.replace('/', '\\');
    QString cmd = "CMD" + l_sReplace;
    qDebug() << l_sReplace << m_sCmd;
    silentExecute->start("CMD",QStringList()<<"/c"<<l_sReplace << m_sCmd);

    //silentExecute->start("CMD", QStringList() << "/c" << "C:\\Program Files\\KuGou\\KGMusic\\uninst.exe");

    //silentExecute->start("ipconfig");

    //qDebug() << l_sReplace;

    if (!silentExecute->waitForStarted())
    {
        qDebug() << "Start setup failed";
        emit sig_vSetupfinish(m_iTaskId);
        return ;
    }


    silentExecute->closeWriteChannel();

    QByteArray procOutput;

    while (false == silentExecute->waitForFinished())
    {
        ;
    }

    procOutput = silentExecute->readAll();

    //qDebug() << procOutput.data() ;

    delete silentExecute;
    //通知安装已经完成
    emit sig_vSetupfinish(m_iTaskId);
}

void Setup::vExecuteUninstall()
{
    //QFile l_fTemp(m_sFileName);

    //qDebug() << m_sFileName;
    //下载失败则直接返回
    //if(10 > l_fTemp.size())
    //{
    //    qDebug() <<m_sFileName << "Empty file!";
    //    //emit sig_vSetupfinish();
    //    return;
    //}

    //QString l_sReplace = m_sFileName.left(m_sFileName.size()-3);
    QString l_sReplace = m_sFileName;

    QProcess *silentExecute = new QProcess();
    QStringList l_slCmd;

    //l_sReplace.replace('/', '\\');
    qDebug() << l_sReplace << m_sCmd;

    //l_sReplace.replace('"', '!');
    //l_sReplace.replace('\\', '\\\');
    //
    l_sReplace.remove('\"');

    qDebug() << l_sReplace;
    //silentExecute->start(l_sReplace);
    silentExecute->start("CMD",QStringList()<<"/c"<<l_sReplace);
    //silentExecute->start("CMD", QStringList() << "/c" << "C:\\Program Files\\KuGou\\KGMusic\\uninst.exe");

    //silentExecute->start("ipconfig");

    //qDebug() << l_sReplace;

    if (!silentExecute->waitForStarted())
    {
        qDebug() << "Start setup failed";
        //emit sig_vSetupfinish(m_iTaskId);
        return ;
    }

    silentExecute->closeWriteChannel();

    QByteArray procOutput;

    while (false == silentExecute->waitForFinished())
    {
        ;
    }

    procOutput = silentExecute->readAll();
    qDebug() << procOutput.data()<<"------" ;

    delete silentExecute;

}



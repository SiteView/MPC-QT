#include <QtGui>
#include <QtNetwork>
#include <QProcess>
#include "httphandle.h"
#include "informationanalyze.h"
//#include "ui_authenticationdialog.h"
#define SERVERINI "d:/server.ini"
#define LOCALINI "d:/local.ini"
#define UPDATEINI "d:/update.ini"

HttpHandle::HttpHandle(QObject *parent)
#ifdef Q_WS_MAEMO_5
    : QObject(parent)
#else
    : QObject(parent)
#endif
{

#ifndef QT_NO_OPENSSL
    //urlLineEdit = new QLineEdit("https://qt.nokia.com/");
#else
    //urlLineEdit = new QLineEdit("http://qt.nokia.com/");
#endif

#ifndef Q_WS_MAEMO_5
    progressDialog = new QProgressDialog(NULL);
#endif


    connect(&m_namManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(vSlotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
#ifndef QT_NO_OPENSSL
    connect(&m_namManager, SIGNAL(vSslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(vSslErrors(QNetworkReply*,QList<QSslError>)));
#endif
#ifndef Q_WS_MAEMO_5
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(vCancelDownload()));
#endif

}

//异步下载
 void HttpHandle::vStartRequest(QUrl url)
 {
     m_pnrReply = m_namManager.get(QNetworkRequest(url));
     qDebug() << "download start";
     connect(m_pnrReply, SIGNAL(finished()),
             this, SLOT(vHttpFinished()));
     connect(m_pnrReply, SIGNAL(readyRead()),
             this, SLOT(vHttpReadyRead()));
     connect(m_pnrReply, SIGNAL(downloadProgress(qint64,qint64)),
             this, SLOT(vUpdateDataReadProgress(qint64,qint64)));
 }

 //对下载文件进行处理
void HttpHandle::vExecute(const QString& fileName,const QStringList& parameters)
{

    //如果是INI文件，则直接返回
    if("ini" == fileName.right(3))
    {
        //通知下载完成
        emit sig_vSetupfinish();
        return;
    }

    //如果是EXE文件则需要进行安装
    QProcess *silentExecute = new QProcess();

    silentExecute->start(fileName, parameters);
    qDebug() << fileName;

    if (!silentExecute->waitForStarted())
    {
        qDebug() << "start failed";
        return ;
    }

    silentExecute->closeWriteChannel();


    QByteArray procOutput;

    while (false == silentExecute->waitForFinished())
    {
        ;
    }

    procOutput = silentExecute->readAll();

    qDebug() << procOutput.data() ;

    qDebug() << "setup finished";

    //通知安装已经完成
    emit sig_vSetupfinish();
    //delete silentExecute;
}


//下载准备
 void HttpHandle::vDownloadFile(QUrl url,QString directory, bool replace)
 {


     QFileInfo fileInfo(url.path());
     QString fileName = fileInfo.fileName();
     if (fileName.isEmpty())
         fileName = "index.html";

     fileName = directory.append(fileName);

     if (!replace && QFile::exists(fileName)) {
         if (QMessageBox::question(NULL, tr("HTTP"),
                                   tr("There already exists a file called %1 in "
                                      "the current directory. Overwrite?").arg(fileName),
                                   QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
             == QMessageBox::No)
             return;
         QFile::remove(fileName);
     }

     m_pfFileDownloaded = new QFile(fileName);
     if (!m_pfFileDownloaded->open(QIODevice::WriteOnly)) {
         QMessageBox::information(NULL, tr("HTTP"),
                                  tr("Unable to save the file %1: %2.")
                                  .arg(fileName).arg(m_pfFileDownloaded->errorString()));
         delete m_pfFileDownloaded;
         m_pfFileDownloaded = 0;
         return;
     }

 #ifndef Q_WS_MAEMO_5
     progressDialog->setWindowTitle(tr("HTTP"));
     progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));
 #endif


     // schedule the request
     m_bHttpRequestAborted = false;
     vStartRequest(url);
 }

 //取消安装
 void HttpHandle::vCancelDownload()
 {
     //statusLabel->setText(tr("Download canceled."));
     m_bHttpRequestAborted = true;
     m_pnrReply->abort();
     //downloadButton->setEnabled(true);
 }

 //下载结束处理
 void HttpHandle::vHttpFinished()
 {
     qDebug() << "download finished!";
     if (m_bHttpRequestAborted) {
         if (m_pfFileDownloaded) {
             m_pfFileDownloaded->close();
             m_pfFileDownloaded->remove();
             delete m_pfFileDownloaded;
             m_pfFileDownloaded = 0;
         }
         m_pnrReply->deleteLater();
 #ifndef Q_WS_MAEMO_5
         progressDialog->hide();
 #endif
         return;
     }

 #ifndef Q_WS_MAEMO_5
     progressDialog->hide();
 #endif
     m_pfFileDownloaded->flush();
     m_pfFileDownloaded->close();

     QVariant redirectionTarget = m_pnrReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
     if (m_pnrReply->error()) {
         m_pfFileDownloaded->remove();
         QMessageBox::information(NULL, tr("HTTP"),
                                  tr("Download failed: %1.")
                                  .arg(m_pnrReply->errorString()));
         //downloadButton->setEnabled(true);
     } else if (!redirectionTarget.isNull()) {
         QUrl newUrl = m_uUrl.resolved(redirectionTarget.toUrl());
         if (QMessageBox::question(NULL, tr("HTTP"),
                                   tr("Redirect to %1 ?").arg(newUrl.toString()),
                                   QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
             m_uUrl = newUrl;
             m_pnrReply->deleteLater();
             m_pfFileDownloaded->open(QIODevice::WriteOnly);
             m_pfFileDownloaded->resize(0);
             vStartRequest(m_uUrl);
             return;
         }
     } else {
         //QString fileName = QFileInfo(QUrl(urlLineEdit->text()).path()).fileName();
         //statusLabel->setText(tr("Downloaded %1 to %2.").arg(fileName).arg(QDir::currentPath()));
         //downloadButton->setEnabled(true);
     }

     m_pnrReply->deleteLater();
     m_pnrReply = 0;

     QString executeFileName =  m_pfFileDownloaded->fileName();
     delete m_pfFileDownloaded;
     m_pfFileDownloaded = 0;

     //如果是EXE则执行静默安装 （NSIS安装包使用 "/S"）
     vExecute(executeFileName, QStringList("/S"));
 }

 void HttpHandle::vHttpReadyRead()
 {
     // this slot gets called every time the QNetworkReply has new data.
     // We read all of its new data and write it into the file.
     // That way we use less RAM than when reading it at the finished()
     // signal of the QNetworkReply
     qDebug() << "read all!";
     if (m_pfFileDownloaded)
         m_pfFileDownloaded->write(m_pnrReply->readAll());
 }

 //进度条
 void HttpHandle::vUpdateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
 {
     qDebug() << "progress";
     if (m_bHttpRequestAborted)
         return;

 #ifndef Q_WS_MAEMO_5
     progressDialog->setMaximum(totalBytes);
     progressDialog->setValue(bytesRead);
 #else
     Q_UNUSED(bytesRead);
     Q_UNUSED(totalBytes);
 #endif
 }




 void HttpHandle::vSlotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
 {
     /*
     QDialog dlg;
     Ui::Dialog ui;
     ui.setupUi(&dlg);
     dlg.adjustSize();
     ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(url.host()));

     // Did the URL have information? Fill the UI
     // This is only relevant if the URL-supplied credentials were wrong
     ui.userEdit->setText(url.userName());
     ui.passwordEdit->setText(url.password());

     if (dlg.exec() == QDialog::Accepted) {
         authenticator->setUser(ui.userEdit->text());
         authenticator->setPassword(ui.passwordEdit->text());
     }
     */
 }

 #ifndef QT_NO_OPENSSL
 void HttpHandle::vSslErrors(QNetworkReply*,const QList<QSslError> &errors)
 {
     QString errorString;
     foreach (const QSslError &error, errors) {
         if (!errorString.isEmpty())
             errorString += ", ";
         errorString += error.errorString();
     }

     if (QMessageBox::warning(NULL, tr("HTTP"),
                              tr("One or more SSL errors has occurred: %1").arg(errorString),
                              QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
         m_pnrReply->ignoreSslErrors();
     }
 }
 #endif


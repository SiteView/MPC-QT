/*负责处理下载有关事项*/
#ifndef HTTPWINDOW_H
 #define HTTPWINDOW_H

 #ifdef Q_WS_MAEMO_5
 #include <QWidget>
 #else
 #include <QDialog>
 #endif
 #include <QNetworkAccessManager>
 #include <QUrl>

 class QDialogButtonBox;
 class QFile;
 class QLabel;
 class QLineEdit;
 class QProgressDialog;
 class QPushButton;
 class QSslError;
 class QAuthenticator;
 class QNetworkReply;
 class InformationAnalyze;

 #ifdef Q_WS_MAEMO_5
 class HttpHandle : public QObject
 #else
 class HttpHandle : public QObject
 #endif
 {
     Q_OBJECT

 public:
     HttpHandle(QObject *parent = 0);

     void vStartRequest(QUrl m_uUrl);
     void vExecute(const QString & fileName, const QStringList& cmd);
 signals:
     void sig_vSetupfinish();
 public slots:
      void vDownloadFile(QUrl m_uUrl , QString directory = "", bool replace = true);
 private slots:
     void vCancelDownload();
     void vHttpFinished();
     void vHttpReadyRead();
     void vUpdateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
     void vSlotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
 #ifndef QT_NO_OPENSSL
     void vSslErrors(QNetworkReply*,const QList<QSslError> &errors);
 #endif

 private:
 #ifndef Q_WS_MAEMO_5
     QProgressDialog *progressDialog;
 #endif

     QUrl m_uUrl;
     QNetworkAccessManager m_namManager;
     QNetworkReply *m_pnrReply;
     QFile *m_pfFileDownloaded;
     bool m_bHttpRequestAborted;
 };

 #endif

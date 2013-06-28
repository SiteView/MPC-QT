#ifndef SETUP_H
#define SETUP_H

#include <QObject>

class Setup : public QObject
{
    Q_OBJECT
public:
    //explicit Setup(QObject *parent = 0);
    Setup(const QString& sFileName, const QString& sCmd, const int iTaskid, QObject *parent = 0);

signals:
    void sig_vSetupfinish(int iTaskId);
private:
    QString m_sFileName;
    QString m_sCmd;
    int m_iTaskId;
    
signals:
    
public slots:
    void vExecute();
    void vExecuteUninstall();
    
};

#endif // SETUP_H

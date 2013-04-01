/*负责处理配置文件有关事项*/
#ifndef INFOMATIONANALYZE_H
#define INFOMATIONANALYZE_H
#include <QString>
#include "aes.h"
#define KEY "siteview20130326"
//class QStringList;
class InformationAnalyze
{
public:
    InformationAnalyze();
    InformationAnalyze(const QString& qserverini, const QString& qlocalini, const QString& qupdateini ):m_sServerini(qserverini),
        m_sLocalini(qlocalini),m_sUpdateini(qupdateini)
    {
        unsigned char key[] = "siteview20130326";
        //m_aesTool = new AES(key);
    }

    void vCompare();
    bool bDuplicateFile(const QString &source, const QString &target);
    void vInitLocalInfomation();
    void vGetUpdateInformation(QStringList &updateInformation);
    void vRefresh();
    void vSetServerInformation(const QString& sFileName,const QStringList &serverInformation);
    //void getUpdateInformation(const QString &updateini);
private:
    QString m_sCurrentPath;
    QString m_sServerini;
    QString m_sLocalini;
    QString m_sUpdateini;
    //AES * m_aesTool;

};

#endif // INFOMATIONANALYZE_H

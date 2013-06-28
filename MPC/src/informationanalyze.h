/*负责处理配置文件有关事项*/
#ifndef INFOMATIONANALYZE_H
#define INFOMATIONANALYZE_H
#include <QString>
#include "aesclass.h"
#define KEY "siteview0313"
//class QStringList;
class InformationAnalyze
{
public:
    InformationAnalyze();
    InformationAnalyze(const QString& qserverini, const QString& qlocalini, const QString& qupdateini ):m_sServerini(qserverini),
        m_sLocalini(qlocalini),m_sUpdateini(qupdateini){ m_aesTool = new AesClass(16, "siteview0313");    }

    void vCompare();
    bool bDuplicateFile(const QString &source, const QString &target);
    void vInitLocalInfomation();
    void vGetUpdateInformation(QStringList &updateInformation);
    void vRefresh();
    void vSetServerInformation(const QString& sFileName,const QStringList &serverInformation);
    //void getUpdateInformation(const QString &updateini);
private:
    QString m_sServerini;
    QString m_sLocalini;
    QString m_sUpdateini;
    AesClass * m_aesTool;

};

#endif // INFOMATIONANALYZE_H

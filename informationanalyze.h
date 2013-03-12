/*负责处理配置文件有关事项*/
#ifndef INFOMATIONANALYZE_H
#define INFOMATIONANALYZE_H
#include <QString>
//class QStringList;
class InformationAnalyze
{
public:
    InformationAnalyze(const QString& qserverini, const QString& qlocalini, const QString& qupdateini ):m_sServerini(qserverini),
        m_sLocalini(qlocalini),m_sUpdateini(qupdateini){}

    void vCompare();
    bool bDuplicateFile(const QString &source, const QString &target);
    void vInitLocalInfomation();
    void vGetUpdateInformation(QStringList &updateInformation);
    void vRefresh();
    //void getUpdateInformation(const QString &updateini);
private:
    QString m_sServerini;
    QString m_sLocalini;
    QString m_sUpdateini;
};

#endif // INFOMATIONANALYZE_H


/*
********************************************************************************************
Discription:  ��־�ļ����.

Written By:		��־��
Date:			2013-07-03
Version:		1.0
Mail:			zhouklansman@gmail.com

Modified by:
Modified Date:
Version:
Discription:
********************************************************************************************
*/

#include <QFile>
#include <QDateTime>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString txt;
	switch (type)
	{
		//������Ϣ��ʾ
	case QtDebugMsg:
		txt = QString("Debug: %1").arg(msg);
		break;
		//һ���warning��ʾ
	case QtWarningMsg:
		txt = QString("Warning: %1").arg(msg);
		break;
		//���ش�����ʾ
	case QtCriticalMsg:
		txt = QString("Critical: %1").arg(msg);
		break;
		//����������ʾ
	case QtFatalMsg:
		txt = QString("Fatal: %1").arg(msg);
		abort();
	}
	
	QFile outFile(QString(qApp->applicationDirPath() +  "/debuglog.txt" ));
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QDateTime dd  = QDateTime::currentDateTime();
	QTextStream ts(&outFile);
	ts << dd.toString("yyyyMMdd hh:mm:ss.zzz") <<" "<< txt <<"  file " <<context.file <<"  "<<context.line << endl;
	//�Ժ�Ϳ�������������ֱ�Ӵ���־���ļ��У�������־Ҳ�����ʱ����Ϣ
	//qDebug("This is a debug message at thisisqt.com");
	//qWarning("This is a warning message  at thisisqt.com");
	//qCritical("This is a critical message  at thisisqt.com");
	//qFatal("This is a fatal message at thisisqt.com");

}
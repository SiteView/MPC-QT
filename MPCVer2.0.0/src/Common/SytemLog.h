
/*
********************************************************************************************
Discription:  日志文件输出.

Written By:		周志光
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
		//调试信息提示
	case QtDebugMsg:
		txt = QString("Debug: %1").arg(msg);
		break;
		//一般的warning提示
	case QtWarningMsg:
		txt = QString("Warning: %1").arg(msg);
		break;
		//严重错误提示
	case QtCriticalMsg:
		txt = QString("Critical: %1").arg(msg);
		break;
		//致命错误提示
	case QtFatalMsg:
		txt = QString("Fatal: %1").arg(msg);
		abort();
	}
	
	QFile outFile(QString(qApp->applicationDirPath() +  "/debuglog.txt" ));
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QDateTime dd  = QDateTime::currentDateTime();
	QTextStream ts(&outFile);
	ts << dd.toString("yyyyMMdd hh:mm:ss.zzz") <<" "<< txt <<"  file " <<context.file <<"  "<<context.line << endl;
	//以后就可以像下面这样直接打日志到文件中，而且日志也会包含时间信息
	//qDebug("This is a debug message at thisisqt.com");
	//qWarning("This is a warning message  at thisisqt.com");
	//qCritical("This is a critical message  at thisisqt.com");
	//qFatal("This is a fatal message at thisisqt.com");

}
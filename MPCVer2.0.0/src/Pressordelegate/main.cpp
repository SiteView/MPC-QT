/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
  main.cpp

  A simple example that shows how a view can use a custom delegate to edit
  data obtained from a model.
*/

#include "delegate.h"

#include <QApplication>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTableView>
#include <QTableWidget>

#include"downloadmanager.h"
#include "TestModelDelegate.h"

#include "../Common/SqliteDb.h"

// 全局数据库对象
QSqlDatabase CSQLiteDb::m_db;

extern  downloadmanager* g_objec; 
//! [0]
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	downloadmanager downloader(&app);
	g_objec = &downloader;
	downloader.start();

	if (!CSQLiteDb::ConnectionDB(QString( "./localedb.db" ) ) )
		//if (!CSQLiteDb::ConnectionDB(QString( "" ) ) )
	{
		QString err = QString("database open faile :%1").arg(CSQLiteDb::getDB()->lastError().text());
		qCritical(err.toLocal8Bit().data());
		CSQLiteDb::DisConnectionDB();
		return 0;
	}	


	DatabaseModel *model=new DatabaseModel();
	model->setQuery("select DisplayIcon,DisplayName,DisplayVersion,EstimatedSize,SetupTime from LocalAppInfor");

    TestTableView tableView;
    tableView.setModel(model);

	TestModelDelegate pushbuttondelegate;
	pushbuttondelegate.setModel((QStandardItemModel *)model);
//    tableView.setItemDelegateForColumn(1,&delegate);
//	tableView.setItemDelegateForColumn(0,&pushbuttondelegate);

	tableView.setItemDelegate(&pushbuttondelegate);
//	tableView.setItemDelegateForColumn(0,;
    tableView.horizontalHeader()->setStretchLastSection(true);

	QObject::connect(&downloader,SIGNAL(sendValue(double)),model,SLOT(setValue(double)));


    tableView.setWindowTitle(QObject::tr("Delegate"));
    tableView.show();
    return app.exec();
}
//! [3]

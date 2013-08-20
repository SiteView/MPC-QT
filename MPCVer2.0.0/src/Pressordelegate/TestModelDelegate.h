#pragma once

#include <QStyledItemDelegate>
#include <QTableView>
#include <QDebug>
#include <QIcon>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QModelIndex>
#include <QSqlRecord>
#include <QApplication>
#include <QString>
#include <QVariant>
#include <iostream>
#include <QAbstractTableModel>
#include <QStandardItemModel>
#include "../Common/SqliteDb.h"

class QStyleOptionButton;



class TestModelDelegate: public QStyledItemDelegate
{
	Q_OBJECT;
	public:
		TestModelDelegate(QObject* parent = 0);
		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
		{
			return NULL; // 重载函数使用户数据不可编辑；
		}
		void setModel(QStandardItemModel  * modle)
		{
			m_model = modle;
		}
		void setView(QTableView  * view)
		{
		}
		~TestModelDelegate();

		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual bool event(QEvent * e);
	public slots:
		void hoverIndex(const QModelIndex& index);
		void depressIndex(const QModelIndex& index);
		void release();
	private:
		void initStyleOption(QStyleOptionButton* out, const QStyleOptionViewItem& in, const QIcon& icon, const QModelIndex& index) const;
		const QIcon m_runIcon;
		const QIcon m_editIcon;
		QPersistentModelIndex m_hoverIndex;
		QPersistentModelIndex m_pressedIndex;
		QStandardItemModel  * m_model;
};

class TestTableView :public QTableView
{
	Q_OBJECT;
public:
	TestTableView(QObject* parent = 0)
	{
		connect(this,SIGNAL(pressed(QModelIndex)),this,SLOT(PressIndex(QModelIndex)));
	};
	~TestTableView()
	{

	};
	public slots:
	void PressIndex(const QModelIndex& index)
	{
		if (index.column() == 5 ||index.column() == 6 )
		{
			model()->setData(index,double(model()->data(index).toDouble()+0.5));
		}
	};
};

class DatabaseModel:public QAbstractTableModel
{
public:
	DatabaseModel(QObject *parent=0);
	void setQuery(const QString& sql);
	int rowCount(const QModelIndex & parent) const;
	int columnCount(const QModelIndex & parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
	QVariant headerData(int section,Qt::Orientation orientation,int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
private:
	QSqlQuery *query;
	QSqlRecord record;
	CSQLiteDb m_SQLiteDb;
	int size ;
	//QStandardItemModel* m_porssedata;
	QList<double> pressList;

};


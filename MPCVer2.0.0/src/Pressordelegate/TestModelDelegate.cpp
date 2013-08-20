#include "TestModelDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <QPainter>
#include "../Common/SqliteDb.h"



TestModelDelegate::TestModelDelegate(QObject* parent)
: QStyledItemDelegate(parent)
, m_runIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowDown))
, m_editIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon))
{
	m_model = NULL;
}

void TestModelDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	if(index.isValid())
	{
		if(index.column() == 6)
		{
			QStyleOptionButton opt;
			initStyleOption(&opt, option, m_runIcon, index);
			QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
		}
		else if(index.column() == 5)
		{
			QStyleOptionProgressBar progressBarOption;
			progressBarOption.state = QStyle::State_Enabled;
			progressBarOption.direction = QApplication::layoutDirection();
			progressBarOption.rect = option.rect;
			progressBarOption.fontMetrics = QApplication::fontMetrics();
			progressBarOption.minimum = 0;
			progressBarOption.maximum = 100;
			progressBarOption.textAlignment = Qt::AlignCenter;
			progressBarOption.textVisible = true;

			double progress = 0;
			if (m_model)
			{
				progress = m_model->data(index).toDouble();
			}
			progressBarOption.progress = progress < 0 ? 0 : progress;
			progressBarOption.text = QString("%1 %").arg(progress, 0, 'f', 1);
			QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
			}
	}
}

void TestModelDelegate::release()
{
	m_pressedIndex = QModelIndex();
}

void TestModelDelegate::depressIndex(const QModelIndex& index)
{
	m_pressedIndex = index;
}

void TestModelDelegate::hoverIndex(const QModelIndex& index)
{
	release();
	m_hoverIndex = index;
}

void TestModelDelegate::initStyleOption(QStyleOptionButton* out, const QStyleOptionViewItem& in, const QIcon& icon, const QModelIndex& index) const
{
	out->icon = icon;
	out->iconSize = QSize(24, 24);
	out->rect = in.rect;
	out->state |= in.state & QStyle::State_Enabled;

	if(index == m_pressedIndex && in.state & QStyle::State_MouseOver)
	{
		out->state |= QStyle::State_Sunken;
	}
	if(index == m_hoverIndex && in.state & QStyle::State_MouseOver)
	{
		out->state |= QStyle::State_MouseOver;
	}
}

TestModelDelegate::~TestModelDelegate()
{
}

bool TestModelDelegate::event(QEvent * e)
{
	return QObject::event(e);
}


DatabaseModel::DatabaseModel(QObject *parent):QAbstractTableModel(parent)
{
	query = new QSqlQuery(*m_SQLiteDb.getDB());
	m_crow = 0;
}

void DatabaseModel::setQuery(const QString& sql)
{
	query->prepare(sql);
	size = 0;
	if (!query->exec())
	{
		QString err = QString("database open faile :%1").arg(CSQLiteDb::getDB()->lastError().text());
		qCritical(err.toLocal8Bit().data());
	}
	while ( query->next() )
	{
		size++;
		pressList.push_back(0.0);
	}
	record=query->record();
}

int DatabaseModel::rowCount(const QModelIndex & parent) const
{
	return size;
}

int DatabaseModel::columnCount(const QModelIndex & parent) const
{
	return record.count()+2;
}

bool DatabaseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	m_crow = index.row();
	if (index.column() == 5)
	{
		pressList.replace(index.row(),value.toDouble());
		QVector<int> vel;
		vel.push_back(role);
		emit (dataChanged(index,index,vel));
		return	true;
	}

}
QVariant DatabaseModel::data(const QModelIndex &index, int role) const
{
	
	if(!index.isValid())
		return QVariant();
	if(role==Qt::TextAlignmentRole)
	{
		return int(Qt::AlignRight|Qt::AlignVCenter);
	}
	else if(role==Qt::DisplayRole)
	{
		int row=index.row();
		QSqlQuery query_copy(*query);
		if(query_copy.seek(row)==false)
		{
			return 0.0;
		}
		else
		{
			QSqlRecord record=query_copy.record();
			int col=index.column();
			if(col<record.count())
				return record.value(col).toString();
			else
			{
				static bool bload = false;
				if ( col == 5 )
				{
					return pressList.at(index.row());;
				}
			}
		}
	}
	return QVariant();
}

QVariant DatabaseModel::headerData(int section,Qt::Orientation orientation,int role) const
{
	if(role!=Qt::DisplayRole)
		return QVariant();
	else if(orientation==Qt::Horizontal)
		return record.fieldName(section);
	else 
		return section;
}



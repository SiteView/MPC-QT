#include "TestModelDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <QPainter>

TestModelDelegate::TestModelDelegate(QObject* parent)
: QStyledItemDelegate(parent)
, m_runIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay))
, m_editIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon))
{
//	m_editButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
}

void TestModelDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	if(index.isValid())
	{
		if(index.column() == 0)
		{
			QStyleOptionButton opt;
			initStyleOption(&opt, option, m_runIcon, index);
			QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
		}
		else if(index.column() == 2)
		{
			QStyleOptionButton opt;
			initStyleOption(&opt, option, m_editIcon, index);
			QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
		}
		else if(index.column() == 1)
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

			// Set the progress and text values of the style option.
			//int progress = qobject_cast<MainWindow *>(parent())->clientForRow(index.row())->progress();
			int progress = 30;
			progressBarOption.progress = progress < 0 ? 0 : progress;
			progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

			// Draw the progress bar onto the view.
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
	out->iconSize = QSize(16, 16);
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

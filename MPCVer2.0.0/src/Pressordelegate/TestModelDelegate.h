#pragma once

#include <QStyledItemDelegate>

#include <QIcon>

class QStyleOptionButton;

class TestModelDelegate: public QStyledItemDelegate
{
	Q_OBJECT;
	public:
		TestModelDelegate(QObject* parent = 0);
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
};
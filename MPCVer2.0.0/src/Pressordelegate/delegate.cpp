#include "delegate.h"

#include <QProgressBar>

#include "downloadmanager.h"

//extern QProgressBar* downloadmanager::bar;
downloadmanager* g_objec; 
ProgressbarDelegate::ProgressbarDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *ProgressbarDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
	QProgressBar *bar = new QProgressBar(parent);
	connect(g_objec, SIGNAL(sendValue(int)),bar,  SLOT(setValue(int)));
	return bar;
}

void ProgressbarDelegate::updateEditorGeometry(QWidget *bar,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    bar->setGeometry(option.rect);
}


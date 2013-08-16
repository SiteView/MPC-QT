
#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
//#include "downloadmanager.h"

//! [0]
class ProgressbarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ProgressbarDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
//! [0]

#endif

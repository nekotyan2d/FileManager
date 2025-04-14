#ifndef FILEMODELDELEGATE_H
#define FILEMODELDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class FileModelDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    FileModelDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // FILEMODELDELEGATE_H

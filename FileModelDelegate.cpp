#include "FileModelDelegate.h"

FileItemDelegate::FileItemDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

void FileItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    painter->save();

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QString name = index.data(Qt::DisplayRole).toString();
    QVariant sizeVar = index.data(Qt::UserRole);
    QString size = index.data(Qt::UserRole + 3).toString();
    QString type = index.data(Qt::UserRole + 1).toString();
    QString lastModified = index.data(Qt::UserRole + 4).toString();

    int padding = 5;
    int iconSize = 32;
    QRect iconRect(option.rect.x() + padding, option.rect.y() + padding,
        iconSize, iconSize);
    QRect textRect(option.rect.x() + iconSize + 2 * padding,
        option.rect.y() + padding,
        option.rect.width() - iconSize - 3 * padding,
        option.rect.height() - 2 * padding);

    icon.paint(painter, iconRect, Qt::AlignCenter);

    painter->setPen(option.state & QStyle::State_Selected ?
        option.palette.highlightedText().color() :
        option.palette.text().color());

    QFont nameFont = painter->font();
    nameFont.setBold(true);
    painter->setFont(nameFont);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop, name);

    QFont normalFont = painter->font();
    normalFont.setBold(false);
    painter->setFont(normalFont);
    
    painter->drawText(textRect.adjusted(0, 20, 0, 0),
        Qt::AlignLeft | Qt::AlignTop,
        size + (size.isEmpty() ? "" : " - ") + type + " | " + lastModified);

    painter->restore();
}

QSize FileItemDelegate::sizeHint(const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(200, 50);
}
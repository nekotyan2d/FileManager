#include "DirTreeDelegate.h"

DirTreeDelegate::DirTreeDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {
}

void DirTreeDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const  {
    painter->save();

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QRect rect = option.rect;
    rect.setX(0);
    rect.setWidth(option.widget->width() - 6);

    // цвет фона
    QColor background;
    if (opt.state & QStyle::State_Selected) {
        background = QColor("#3e3e3e");
    }
    else if (opt.state & QStyle::State_MouseOver) {
        background = QColor("#4b4b4b");
    }
    else {
        background = opt.palette.base().color();
    }

    painter->fillRect(rect, background);

    // стрелка
    int branchWidth = 20;
    QRect branchRect(option.rect.x(), option.rect.y(), branchWidth, option.rect.height());

    if (index.model()->hasChildren(index)) {
        QPoint center(branchRect.center().x(), branchRect.center().y());
        int size = 8;
        QIcon icon;
        if (opt.state & QStyle::State_Open) {
            //вниз
            icon = QIcon(":/FileManager/Resources/arrow-down.svg");
        } else {
            // вправо
            icon = QIcon(":/FileManager/Resources/arrow-right.svg");
        }
        painter->drawImage(center.x() - size / 2, center.y() - size / 2, icon.pixmap(size, size).toImage());
    }

    int padding = 8;
    int iconSize = 16;
    QRect iconRect(option.rect.x() + branchWidth + padding, option.rect.y(), iconSize, option.rect.height());
    QRect textRect(option.rect.x() + branchWidth + iconSize + padding + 4, option.rect.y(), option.rect.width() - (branchWidth + iconSize + padding + 4), option.rect.height());

    opt.icon.paint(painter, iconRect, Qt::AlignVCenter);

    if (opt.state & QStyle::State_Selected) {
        painter->setPen(opt.palette.highlightedText().color());
    }
    else {
        painter->setPen(opt.palette.text().color());
    }

    painter->drawText(textRect, Qt::AlignVCenter, index.data().toString());

    painter->restore();
}
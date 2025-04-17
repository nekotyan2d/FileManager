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

    // --- Определяем цвет фона ---
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

    // Фон всей строки
    painter->fillRect(rect, background);

    // --- Кастомная стрелка-ветвь ---
    int branchWidth = 20;
    QRect branchRect(option.rect.x(), option.rect.y(), branchWidth, option.rect.height());

    if (index.model()->hasChildren(index)) {
        QPoint center(branchRect.center().x(), branchRect.center().y());
        int size = 8;
        QPolygon arrow;
        if (opt.state & QStyle::State_Open) {
            // ▼ (вниз)
            arrow << QPoint(center.x() - size/2, center.y() - size/4)
                  << QPoint(center.x() + size/2, center.y() - size/4)
                  << QPoint(center.x(), center.y() + size/2);
        } else {
            // ▶ (вправо)
            arrow << QPoint(center.x() - size/4, center.y() - size/2)
                  << QPoint(center.x() - size/4, center.y() + size/2)
                  << QPoint(center.x() + size/2, center.y());
        }
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setBrush(QColor("#fff"));
        painter->setPen(Qt::NoPen);
        painter->drawPolygon(arrow);
    }

    // --- Остальная отрисовка ---
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
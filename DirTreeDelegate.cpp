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
	rect.setWidth(option.widget->width());

    if (opt.state & QStyle::State_Selected) {
        painter->fillRect(rect, QColor("#3e3e3e"));
    }
    else if (opt.state & QStyle::State_MouseOver) {
        painter->fillRect(rect, QColor("#4b4b4b"));
    }
    else {
        painter->fillRect(rect, opt.palette.base().color());
    }

    QStyleOption branchOption = opt;
    if (index.model()->hasChildren(index)) {
        branchOption.state |= QStyle::State_Children;
    }
    if (opt.state & QStyle::State_Open) {
        branchOption.state |= QStyle::State_Open;
    }

    if (index.model()->rowCount(index.parent()) > index.row() + 1) {
        branchOption.state |= QStyle::State_Sibling;
    }

    QRect branchRect = rect;
    int indentation = 20;
    branchRect.setWidth(indentation);
    branchOption.rect = branchRect;

    const QWidget* widget = opt.widget;
    QStyle* style = widget ? widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_IndicatorBranch, &branchOption, painter, widget);

    int padding = 8;
    int iconSize = 16;
    QRect iconRect(option.rect.x() + padding, option.rect.y(), iconSize, option.rect.height());
    QRect textRect(option.rect.x() + iconSize + padding + 4, option.rect.y(), option.rect.width() - (iconSize + padding + 4), option.rect.height());

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
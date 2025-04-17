#include "ExtendedTreeView.h"

void ExtendedTreeView::mousePressEvent(QMouseEvent* event) {
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        QTreeView::mousePressEvent(event);
        return;
    }
    // определяем область стрелки (20px слева)
    QRect rect = visualRect(index);
    QRect arrowRect(rect.left(), rect.top(), 20, rect.height());
    if (arrowRect.contains(event->pos()) && model()->hasChildren(index)) {
        if (isExpanded(index))
            collapse(index);
        else
            expand(index);
        // не передаём дальше, чтобы не было двойного действия
        return;
    }
    QTreeView::mousePressEvent(event);
}

void ExtendedTreeView::mouseDoubleClickEvent(QMouseEvent* event) {
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        QTreeView::mouseDoubleClickEvent(event);
        return;
    }
    QRect rect = visualRect(index);
    QRect arrowRect(rect.left(), rect.top(), 20, rect.height());
    if (arrowRect.contains(event->pos()) && model()->hasChildren(index)) {
        // игнорируем двойной клик по стрелке
        return;
    }
    QTreeView::mouseDoubleClickEvent(event);
}

void ExtendedTreeView::scrollTo(const QModelIndex &index, ScrollHint hint) {
    if (!autoScroll) return; // не прокручиваем, если отключен автоскролл
    QTreeView::scrollTo(index, hint);
}
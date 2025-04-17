#pragma once
#include <QTreeView>
#include <QMouseEvent>

class ExtendedTreeView : public QTreeView {
    Q_OBJECT
public:
    using QTreeView::QTreeView;

    bool autoScroll = true;

    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};
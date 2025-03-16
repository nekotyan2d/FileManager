#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>

class DirTreeDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	DirTreeDelegate(QObject* parent = nullptr);

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


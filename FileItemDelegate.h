#pragma once

#include <QStyledItemDelegate>
#include <QPainter>

class FileItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FileItemDelegate(QObject* parent = nullptr);

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


#pragma once

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>
#include <QFileIconProvider>

class FileModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	FileModel(QObject* parent = nullptr);
	~FileModel();

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override { return 1; }

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void setPath(const QString& path);
	QString currentPath() const { return currentDir.absolutePath(); };

private:
	QDir currentDir;
	QFileInfoList fileList;


	void refreshModel();
	QString sizeToString(qint64 size) const;
};


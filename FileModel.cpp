#include "FileModel.h"
#include <QDebug>

FileModel::FileModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    setPath(QDir::homePath());
}

FileModel::~FileModel()
{
}

void FileModel::setPath(const QString& path)
{
    beginResetModel();
    currentDir.setPath(path);
    refreshModel();
    endResetModel();
}

void FileModel::refreshModel()
{
    fileList = currentDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot,
        QDir::Name | QDir::DirsFirst);
}

QModelIndex FileModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent) || parent.isValid())
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex FileModel::parent(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int FileModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return fileList.count();
}

QVariant FileModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const QFileInfo& fileInfo = fileList.at(index.row());
    QFileIconProvider iconProvider;

    switch (role) {
        case Qt::DisplayRole:
            return fileInfo.fileName();
        case Qt::DecorationRole:
            return iconProvider.icon(fileInfo);
        case Qt::UserRole: // размер
            return fileInfo.isDir() ? QVariant() : fileInfo.size();
        case Qt::UserRole + 1: // тип
            return fileInfo.isDir() ? "Папка" : fileInfo.completeSuffix();
        case Qt::UserRole + 2: // путь
            return fileInfo.absoluteFilePath();
        case Qt::UserRole + 3:
			return fileInfo.isDir() ? QVariant() : sizeToString(fileInfo.size());
        case Qt::UserRole + 4:
			return fileInfo.lastModified().toString("dd.MM.yyyy hh:mm");
        default:
            return QVariant();
    }
}

QString FileModel::sizeToString(qint64 size) const {
	const quint64 KB = 1024;
	const quint64 MB = KB * 1024;
	const quint64 GB = MB * 1024;
	const quint64 TB = GB * 1024;

    if (size < KB) {
		return QString("%1 Б").arg(size);
	}
    else if (size < MB) {
		return QString("%1 КБ").arg(size / KB);
	}
    else if (size < GB) {
		return QString("%1 МБ").arg(size / MB);
	}
    else if (size < TB) {
		return QString("%1 ГБ").arg(size / GB);
    }
    else {
		return QString("%1 ТБ").arg(size / TB);
    }
}

bool FileModel::deleteFile(const QModelIndex& index) {
	if (!index.isValid()) {
		return false;
	}

	QFileInfo fileInfo = fileList.at(index.row());
    bool success = false;

	beginRemoveRows(QModelIndex(), index.row(), index.row());

	if (fileInfo.isDir()) {
		QDir dir(fileInfo.absoluteFilePath());
		success = dir.removeRecursively();
	}
	else {
		success = QFile::remove(fileInfo.absoluteFilePath());
	}

    if (success) {
        fileList.removeAt(index.row());
    }
    
	endRemoveRows();

	return success;
}
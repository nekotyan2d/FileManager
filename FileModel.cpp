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
    emit pathChanged(path);
    refreshModel();
    endResetModel();
}

void FileModel::refreshModel()
{
    fileList.clear();
    for (const auto& file : currentDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot,
                                                     QDir::Name | QDir::DirsFirst)) {
        File f;
        f.info = file;
        f.icon = QFileIconProvider().icon(file);
        fileList.push_back(f);
    }
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
    return fileList.size();
}

QVariant FileModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const File& file = fileList.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return file.info.fileName();
    case Qt::DecorationRole:
        return file.icon;
    case Qt::UserRole: // размер
        return file.info.isDir() ? QVariant() : file.info.size();
    case Qt::UserRole + 1: // тип
        return file.info.isDir() ? "Папка" : file.info.completeSuffix();
    case Qt::UserRole + 2: // путь
        return file.info.absoluteFilePath();
    case Qt::UserRole + 3:
        return file.info.isDir() ? QVariant() : sizeToString(file.info.size());
    case Qt::UserRole + 4:
        return file.info.lastModified().toString("dd.MM.yyyy hh:mm");
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

    if (QMessageBox::question(nullptr, "Удаление", "Вы уверены, что хотите удалить файл?", QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::No) {
        return false;
    }

    File file = fileList.at(index.row());
    bool success = false;

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    if (file.info.isDir()) {
        QDir dir(file.info.absoluteFilePath());
        success = dir.removeRecursively();
    }
    else {
        success = QFile::remove(file.info.absoluteFilePath());
    }

    if (success) {
        fileList.erase(fileList.begin() + index.row());
    }

    endRemoveRows();

    return success;
}

bool FileModel::moveFile(const QModelIndex& index, const QString& newPath) {
    if (!index.isValid()) {
        return false;
    }

    File file = fileList.at(index.row());
    bool success = false;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    QString newFilePath = newPath + "/" + file.info.fileName();
    if (file.info.isDir()) {
        QDir dir(file.info.absoluteFilePath());
        success = dir.rename(file.info.absoluteFilePath(), newFilePath);
    }
    else {
        success = QFile::rename(file.info.absoluteFilePath(), newFilePath);
    }

    if (success) {
        fileList.erase(fileList.begin() + index.row());
    }

    endRemoveRows();

    if (QMessageBox::question(nullptr, "Перемещение", "Перейти к файлу?", QMessageBox::Open | QMessageBox::Cancel) == QMessageBox::Open) {
        setPath(newPath);
    }

    return success;
}

bool FileModel::copyFile(const QModelIndex& index, const QString& path) {
    if (!index.isValid()) {
        return false;
    }

    File file = fileList.at(index.row());
    bool success = false;

    QString newFilePath = path + "/" + file.info.fileName();
    if (file.info.isDir()) {
        // доделать
    }
    else {
        success = QFile::copy(file.info.absoluteFilePath(), newFilePath);
    }

    if (QMessageBox::question(nullptr, "Копирование", "Перейти к файлу?", QMessageBox::Open | QMessageBox::Cancel) == QMessageBox::Open) {
        setPath(path);
    }

    return success;
}

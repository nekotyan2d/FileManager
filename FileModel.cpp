#include "FileModel.h"

#include <QDebug>

FileModel::FileModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    setPath(QDir::homePath());
    qDebug() << "test!";
}

FileModel::~FileModel()
{
}

void FileModel::setPath(const QString &path)
{
    beginResetModel();
    currentDir.setPath(path);

    // вызываем сигнал об изменении пути
    emit pathChanged(path);

    // включаем загрузку
    emit loading(true);

    auto future = QtConcurrent::run([=]()
                                    {
        QDir dir(path);
        // загружаем список файлов с сортировкой по имени и папкам
        QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries, QDir::DirsFirst | QDir::Name);
        return list; })
                      .then([=](const QFileInfoList &list)
                            {
        fileList.clear();
        for (const auto file : list) {
            File f;
            f.info = file;

            // плейсхолдер для иконки
            f.icon = QIcon(":/FileManager/Resources/icon.png");
            fileList.push_back(f);
        }
        for(auto &file : fileList){
            // загружаем иконку в отдельном потоке
            auto future = QtConcurrent::run([=](){
                return iconProvider.icon(file.info);
            });
            file.icon = future.result();
        }
        endResetModel();
        emit loading(false); });
}

QModelIndex FileModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || parent.isValid())
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex FileModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return fileList.size();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const File &file = fileList.at(index.row());

    switch (role)
    {
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
    case Qt::UserRole + 3: // отформатированный размер
        return file.info.isDir() ? QVariant() : sizeToString(file.info.size());
    case Qt::UserRole + 4: // последнее изменение
        return file.info.lastModified().toString("dd.MM.yyyy hh:mm");
    default:
        return QVariant();
    }
}

QString FileModel::sizeToString(qint64 size) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;

    if (size < KB)
    {
        return QString("%1 Б").arg(size);
    }
    else if (size < MB)
    {
        return QString("%1 КБ").arg(size / KB);
    }
    else if (size < GB)
    {
        return QString("%1 МБ").arg(size / MB);
    }
    else if (size < TB)
    {
        return QString("%1 ГБ").arg(size / GB);
    }
    else
    {
        return QString("%1 ТБ").arg(size / TB);
    }
}

bool FileModel::deleteFile(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return false;
    }
    // спрашиваем пользователя
    if (QMessageBox::question(nullptr, "Удаление", "Вы уверены, что хотите удалить файл?", QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Cancel)
    {
        return false;
    }

    File file = fileList.at(index.row());
    bool success = false;

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    // проверяем, является ли файл директорией или файлом, в случае директории удаляем рекурсивно файлы в ней
    if (file.info.isDir())
    {
        QDir dir(file.info.absoluteFilePath());
        success = dir.removeRecursively();
    }
    else
    {
        success = QFile::remove(file.info.absoluteFilePath());
    }

    if (success)
    {
        fileList.erase(fileList.begin() + index.row());
    }

    endRemoveRows();

    return success;
}

bool FileModel::moveFile(const QModelIndex &index, const QString &newPath)
{
    if (!index.isValid())
    {
        return false;
    }

    File file = fileList.at(index.row());
    bool success = false;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    QString newFilePath = newPath + "/" + file.info.fileName();
    if (file.info.isDir())
    {
        QDir dir(file.info.absoluteFilePath());
        success = dir.rename(file.info.absoluteFilePath(), newFilePath);
    }
    else
    {
        success = QFile::rename(file.info.absoluteFilePath(), newFilePath);
    }

    if (success)
    {
        fileList.erase(fileList.begin() + index.row());
    }

    endRemoveRows();

    if (QMessageBox::question(nullptr, "Перемещение", "Перейти к файлу?", QMessageBox::Open | QMessageBox::Cancel) == QMessageBox::Open)
    {
        setPath(newPath);
    }

    return success;
}

bool FileModel::copyDirectory(QString source, QString destination, bool overwrite)
{
    QDir sourceDir(source);
    QDir destinationDir(destination);

    if (!sourceDir.exists())
    {
        return false;
    }

    if (destinationDir.exists() && !overwrite)
    {
        return false;
    }
    else if (destinationDir.exists() && overwrite)
    {
        // очищаем директорию назначения
        destinationDir.removeRecursively();
    }

    QDir().mkpath(destination);

    for (QString directoryName : sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString destinationPath = destination + "/" + directoryName;
        copyDirectory(source + "/" + directoryName, destinationPath, overwrite);
    }

    for (QString fileName : sourceDir.entryList(QDir::Files))
    {
        QFile::copy(source + "/" + fileName, destination + "/" + fileName);
    }

    return true;
}

bool FileModel::copyFile(const QModelIndex &index, const QString &path)
{
    if (!index.isValid())
    {
        return false;
    }

    File file = fileList.at(index.row());
    bool success = false;
    bool overwrite = false;

    if (QFile::exists(path + "/" + file.info.fileName()))
    {
        // файл существует, спрашиваем пользователя
        overwrite = QMessageBox::question(nullptr, "Копирование", "Файл существует. Перезаписать?", QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
    }

    if (file.info.isDir())
    {
        success = copyDirectory(file.info.absoluteFilePath(), path + "/" + file.info.fileName(), overwrite);
    }
    else
    {
        if (overwrite)
        {
            // удаляем существующий файл
            QFile::remove(path + "/" + file.info.fileName());
        }
        success = QFile::copy(file.info.absoluteFilePath(), path + "/" + file.info.fileName());
    }

    if (QMessageBox::question(nullptr, "Копирование", "Перейти к файлу?", QMessageBox::Open | QMessageBox::Cancel) == QMessageBox::Open)
    {
        setPath(path);
    }

    return success;
}

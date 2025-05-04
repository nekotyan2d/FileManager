#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>
#include <QFileIconProvider>
#include <QMessageBox>
#include <vector>
#include <QThread>
#include <QFuture>
#include <QtConcurrent>

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

    bool deleteFile(const QModelIndex& index);
    bool moveFile(const QModelIndex& index, const QString& newPath);
    bool copyFile(const QModelIndex& index, const QString& path);

    struct File {
        QFileInfo info;
        QIcon icon;
    };
signals:
    void pathChanged(const QString& newPath);
    void loading(bool state);
private:
    QDir currentDir;
    std::vector<File> fileList;

    QString sizeToString(qint64 size) const;
    bool copyDirectory(QString source, QString destination, bool overwrite = false);
    QFileIconProvider iconProvider;
};

#endif // FILEMODEL_H

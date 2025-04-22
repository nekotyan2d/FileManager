#pragma once
#include <QObject>
#include <QFileInfoList>
#include <QDir>

class FileModelWorker : public QObject {
    Q_OBJECT
public:
    explicit FileModelWorker(const QString& path, QObject* parent = nullptr)
        : QObject(parent), m_path(path) {}

public slots:
    void process();

signals:
    void finished(const QFileInfoList& list);

private:
    QString m_path;
};
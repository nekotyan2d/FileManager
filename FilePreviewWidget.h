#pragma once

#include <QWidget>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QImageReader>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QIcon>

namespace Ui {
class FilePreviewWidget;
}

class FilePreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit FilePreviewWidget(QWidget* parent = nullptr);
    ~FilePreviewWidget();
    
    void clear();
public slots:
    void previewFile(const QString& path, bool isDir);
private:
    Ui::FilePreviewWidget* ui;
};
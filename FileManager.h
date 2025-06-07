#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "ui_FileManager.h"
#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include "FileModel.h"
#include "DirTreeModel.h"
#include "FileModelDelegate.h"
#include "DirTreeDelegate.h"
#include <QSplitter>
#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QTreeView>
#include "FilePreviewWidget.h"
#include <QStatusBar>
#include <QDesktopServices>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class FileManager;
}
QT_END_NAMESPACE

class FileManager : public QMainWindow
{
    Q_OBJECT

public:
    FileManager(QWidget *parent = nullptr);
    ~FileManager();

    void pathChanged(const QString &newPath);

protected:
    Ui::FileManager *ui;
    FilePreviewWidget *previewWidget;
    FileModel *fileModel;
    DirTreeModel *treeModel;
private:
    bool loaded = false;

    void expandToIndex(QTreeView *tree, const QModelIndex &index)
    {
        QModelIndex parent = index;
        while (parent.isValid())
        {
            tree->expand(parent);
            parent = parent.parent();
        }
    }

    void expandSideTreeToPath(QString path);
    void enableActions(bool enable);
    void initSideBar();
protected slots:
    void on_pathLineEdit_returnPressed();

    void on_listView_doubleClicked(const QModelIndex &index);
    void on_listView_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void on_listView_customContextMenuRequested(const QPoint &pos);

    void on_treeView_clicked(const QModelIndex &index);

    void on_deleteButton_clicked();
    void on_moveButton_clicked();
    void on_copyButton_clicked();
    void on_refreshButton_clicked();

    void keyPressEvent(QKeyEvent *event) override;
};
#endif // FILEMANAGER_H

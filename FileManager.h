#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include "ui_FileManager.h"
#include "FileModel.h"
#include "DirTreeModel.h"
#include "FileItemDelegate.h"
#include "DirTreeDelegate.h"
#include <QSplitter>

class FileManager : public QMainWindow
{
    Q_OBJECT

public:
    FileManager(QWidget *parent = nullptr);
    ~FileManager();

private:
    Ui::FileManagerClass ui;
	FileModel* fileModel;
	DirTreeModel* treeModel;

    void expandToIndex(QTreeView* tree, const QModelIndex& index) {
        QModelIndex parent = index;
        while (parent.isValid()) {
            tree->expand(parent);
            parent = parent.parent();
        }
    }

    void expandSideTreeToPath(QString path);
	void enableActions(bool enable);
private slots:
	void on_pathLineEdit_returnPressed();

	void on_listView_doubleClicked(const QModelIndex& index);
	void on_listView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

	void on_treeView_doubleClicked(const QModelIndex& index);

	void on_deleteButton_clicked();

	void keyPressEvent(QKeyEvent* event) override;
};

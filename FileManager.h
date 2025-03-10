#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include "ui_FileManager.h"
#include "FileModel.h"
#include "FileItemDelegate.h"

class FileManager : public QMainWindow
{
    Q_OBJECT

public:
    FileManager(QWidget *parent = nullptr);
    ~FileManager();

private:
    Ui::FileManagerClass ui;
	FileModel* fileModel;
private slots:
	void on_pathLineEdit_returnPressed();

	void on_listView_doubleClicked(const QModelIndex& index);
	void on_listView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

	void on_deleteButton_clicked();

	void keyPressEvent(QKeyEvent* event) override;
};

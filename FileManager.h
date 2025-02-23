#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QKeyEvent>
#include "ui_FileManager.h"

class FileManager : public QMainWindow
{
    Q_OBJECT

public:
    FileManager(QWidget *parent = nullptr);
    ~FileManager();

private:
    Ui::FileManagerClass ui;
	QFileSystemModel* fileModel;
	void loadFiles(QString path);
private slots:
	void on_pathLineEdit_returnPressed();

	void on_tableView_doubleClicked(const QModelIndex& index);
	void on_tableView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

	void on_deleteButton_clicked();

	void keyPressEvent(QKeyEvent* event) override;
};

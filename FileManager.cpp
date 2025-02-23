#include "FileManager.h"

FileManager::FileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());
    ui.tableView->setModel(fileModel);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->setShowGrid(false);
    
    ui.tableView->setStyleSheet(
        "QTableView {"
        "    border: 0px;"
        "}"
        "QTableView::item {"
        "    border: 0px;"
        "    outline: 0;"
        "}"
        "QTableView::item:hover {"
        "    background: transparent;"
        "}"
        "QTableView::item:selected {"
        "    background: transparent;"
        "}"
        "QHeaderView::section {"
        "    border: 0px;"
        "    padding: 4px;"
        "}"
    );

    ui.tableView->setRootIndex(fileModel->index(QDir::homePath()));

	ui.pathLineEdit->setText(QDir::homePath());

	ui.deleteButton->setEnabled(false);
	ui.copyButton->setEnabled(false);

	connect(ui.tableView, &QTableView::doubleClicked, this, &FileManager::on_tableView_doubleClicked);
	connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileManager::on_tableView_selectionChanged);
}

FileManager::~FileManager()
{}

void FileManager::on_pathLineEdit_returnPressed()
{
	loadFiles(ui.pathLineEdit->text());
}

void FileManager::on_tableView_doubleClicked(const QModelIndex& index) {
    if (!index.isValid()) {
        ui.tableView->clearSelection();
		return;
    }
    if (fileModel->isDir(index)) {
        QString path = fileModel->filePath(index);
        ui.pathLineEdit->setText(path);
        loadFiles(path);
    }
    else {

    }
}

void FileManager::loadFiles(QString path) {
	if (QDir(path).exists())
	{
		ui.tableView->setRootIndex(fileModel->index(path));
	}
	else
	{
		QMessageBox::warning(this, "Error", "Invalid path");
	}
}

void FileManager::on_tableView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
	bool hasSelection = !selected.isEmpty();
	ui.deleteButton->setEnabled(hasSelection);
	ui.copyButton->setEnabled(hasSelection);
}

void FileManager::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
		ui.tableView->clearSelection();
    }
    else {
		QMainWindow::keyPressEvent(event);
    }
}

void FileManager::on_deleteButton_clicked() {
    QItemSelectionModel* selectionModel = ui.tableView->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedRows();

    if (selectedIndexes.isEmpty()) return;

    QStringList filePaths;
    for (const QModelIndex& index : selectedIndexes) {
        filePaths << fileModel->filePath(index);
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Удаление", "Вы уверены, что хотите удалить " + QString::number(filePaths.size()) + " элементов?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool allDeleted = true;
        for (const QModelIndex& index : selectedIndexes) {
            if (!fileModel->remove(index)) {
                allDeleted = false;
            }
        }
        if (allDeleted) {
            QMessageBox::information(this, "Удаление", "Все файлы успешно удалены.");
        }
        else {
            QMessageBox::warning(this, "Удаление", "Не удалось удалить некоторые файлы.");
        }
    }
}
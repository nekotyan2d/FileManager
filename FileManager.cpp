#include "FileManager.h"

FileManager::FileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	fileModel = new FileModel(this);

    ui.listView->setModel(fileModel);
    ui.listView->setItemDelegate(new FileItemDelegate(ui.listView));
    ui.listView->setViewMode(QListView::ListMode);
    ui.listView->setResizeMode(QListView::Adjust);

    connect(ui.listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileManager::on_listView_selectionChanged);

	ui.pathLineEdit->setText(fileModel->currentPath());

	ui.deleteButton->setEnabled(false);
	ui.copyButton->setEnabled(false);
}

FileManager::~FileManager()
{}

void FileManager::on_pathLineEdit_returnPressed()
{
    fileModel->setPath(ui.pathLineEdit->text());
}

void FileManager::on_listView_doubleClicked(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }

    QString fullPath = index.data(Qt::UserRole + 2).toString();
    QFileInfo fileInfo(fullPath);

    if (fileInfo.isDir()) {
        fileModel->setPath(fullPath);
        ui.pathLineEdit->setText(fullPath);
    }
}

void FileManager::on_listView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
	bool hasSelection = !selected.isEmpty();
	ui.deleteButton->setEnabled(hasSelection);
	ui.copyButton->setEnabled(hasSelection);
}

void FileManager::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
		ui.listView->clearSelection();
    }
    else {
		QMainWindow::keyPressEvent(event);
    }
}

void FileManager::on_deleteButton_clicked() {
    /*QItemSelectionModel* selectionModel = ui.listView->selectionModel();
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
    }*/
}
#include "FileManager.h"
#include <QDebug>

FileManager::FileManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	fileModel = new FileModel(this);

    ui.listView->setModel(fileModel);
    ui.listView->setItemDelegate(new FileItemDelegate(ui.listView));
    ui.listView->setViewMode(QListView::ListMode);
    ui.listView->setResizeMode(QListView::Adjust);

	treeModel = new DirTreeModel(nullptr);
    treeModel->setRootToDrives();

	ui.treeView->setModel(treeModel);
	ui.treeView->header()->hide();

    QModelIndex targetIndex = treeModel->expandToPath(fileModel->currentPath());

    if (targetIndex.isValid()) {
        expandToIndex(ui.treeView, targetIndex);
        ui.treeView->scrollTo(targetIndex);
        ui.treeView->setCurrentIndex(targetIndex);
    }
    else {
        qDebug() << "Не удалось найти путь:" << fileModel->currentPath();
    }

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
    expandSideTreeToPath(fileModel->currentPath());
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
		expandSideTreeToPath(fullPath);
    }
}

void FileManager::on_treeView_doubleClicked(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }

	QString fullPath = index.data(Qt::UserRole).toString();
	fileModel->setPath(fullPath);
	ui.pathLineEdit->setText(fullPath);
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
	bool result = fileModel->deleteFile(ui.listView->currentIndex());
    if (result) {
		QMessageBox::information(this, "Удаление", "Файл успешно удален");
    }
    else {
		QMessageBox::warning(this, "Удаление", "Не удалось удалить файл");
    }
}

void FileManager::expandSideTreeToPath(QString path) {
	QModelIndex targetIndex = treeModel->expandToPath(path);

	if (targetIndex.isValid()) {
		expandToIndex(ui.treeView, targetIndex);
		ui.treeView->scrollTo(targetIndex);
		ui.treeView->setCurrentIndex(targetIndex);
	}
	else {
		qDebug() << "Не удалось найти путь:" << path;
	}
}
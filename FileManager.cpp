#include "FileManager.h"
#include "ui_FileManager.h"

FileManager::FileManager(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FileManager)
{
    ui->setupUi(this);

    ui->mainSplitter->setSizes({ 300, 7000, 250});
    ui->mainSplitter->widget(0)->setMinimumWidth(300);
    ui->mainSplitter->widget(2)->setMinimumWidth(250);

    fileModel = new FileModel(this);
    connect(fileModel, &FileModel::pathChanged, this, &FileManager::pathChanged);

    ui->listView->setModel(fileModel);
    ui->listView->setItemDelegate(new FileModelDelegate(ui->listView));
    ui->listView->setViewMode(QListView::ListMode);
    ui->listView->setResizeMode(QListView::Adjust);

    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->listView->setUniformItemSizes(true);
    ui->treeView->setUniformRowHeights(true);

    treeModel = new DirTreeModel(nullptr);
    treeModel->setRootToDrives();

    initSideBar();

    connect(treeModel, &DirTreeModel::dataChanged, this, &FileManager::initSideBar);
    connect(ui->treeView, &QTreeView::expanded, treeModel, &DirTreeModel::loadNode);

    ui->treeView->setModel(treeModel);
    ui->treeView->header()->hide();
    ui->treeView->setIndentation(8);
    ui->treeView->setItemDelegate(new DirTreeDelegate(ui->treeView));

    connect(ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileManager::on_listView_selectionChanged);

    ui->pathLineEdit->setText(fileModel->currentPath());

    ui->deleteButton->setEnabled(false);
    ui->copyButton->setEnabled(false);
    ui->moveButton->setEnabled(false);
}

FileManager::~FileManager()
{
    delete ui;
}

void FileManager::on_pathLineEdit_returnPressed()
{
    fileModel->setPath(ui->pathLineEdit->text());
    enableActions(false);
}

void FileManager::on_listView_doubleClicked(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }

    QString fullPath = index.data(Qt::UserRole + 2).toString();
    QFileInfo fileInfo(fullPath);

    if (fileInfo.isDir()) {
        fileModel->setPath(fullPath);
        enableActions(false);
    }
}

void FileManager::on_treeView_doubleClicked(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }

    QString fullPath = index.data(Qt::UserRole).toString();
    fileModel->setPath(fullPath);
    ui->pathLineEdit->setText(fullPath);
}

void FileManager::on_listView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
    bool hasSelection = !selected.isEmpty();
    enableActions(hasSelection);
}

void FileManager::on_listView_customContextMenuRequested(const QPoint& pos) {
    QModelIndex index = ui->listView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    QMenu contextMenu(this);
    QAction* openAction = contextMenu.addAction("Открыть");
    QAction* moveAction = contextMenu.addAction("Переместить");
    QAction* deleteAction = contextMenu.addAction("Удалить");
    openAction->setIcon(QIcon(":/FileManager/Resources/open.svg"));
    moveAction->setIcon(QIcon(":/FileManager/Resources/move.svg"));
    deleteAction->setIcon(QIcon(":/FileManager/Resources/delete.svg"));

    QAction* selectedItem = contextMenu.exec(ui->listView->mapToGlobal(pos));
    if (selectedItem == openAction) {
        on_listView_doubleClicked(index);
    }
    else if (selectedItem == moveAction) {
        on_moveButton_clicked();
    }
    else if (selectedItem == deleteAction) {
        on_deleteButton_clicked();
    }
}

void FileManager::enableActions(bool enable) {
    ui->deleteButton->setEnabled(enable);
    ui->copyButton->setEnabled(enable);
    ui->moveButton->setEnabled(enable);
}

void FileManager::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        ui->listView->clearSelection();
    }
    else {
        QMainWindow::keyPressEvent(event);
    }
}

void FileManager::on_deleteButton_clicked() {
    bool result = fileModel->deleteFile(ui->listView->currentIndex());
    if (result) {
        QMessageBox::information(this, "Удаление", "Файл успешно удален");
    }
    else {
        QMessageBox::warning(this, "Удаление", "Не удалось удалить файл");
    }
}

void FileManager::on_moveButton_clicked() {
    QString targetPath = QFileDialog::getExistingDirectory(this, "Переместить в", fileModel->currentPath(), QFileDialog::ShowDirsOnly);
    if (targetPath.isEmpty()) {
        return;
    }

    bool result = fileModel->moveFile(ui->listView->currentIndex(), targetPath);
    if (!result) {
        QMessageBox::warning(this, "Перемещение", "Не удалось переместить файл");
    }
}

void FileManager::on_copyButton_clicked() {
    QString targetPath = QFileDialog::getExistingDirectory(this, "Копировать в", fileModel->currentPath(), QFileDialog::ShowDirsOnly);
    if (targetPath.isEmpty()) {
        return;
    }

    bool result = fileModel->copyFile(ui->listView->currentIndex(), targetPath);
    if (!result) {
        QMessageBox::warning(this, "Копирование", "Не удалось скопировать файл");
    }
}

void FileManager::on_refreshButton_clicked() {
    fileModel->setPath(fileModel->currentPath());
}

void FileManager::pathChanged(const QString& newPath) {
    ui->pathLineEdit->setText(newPath);
    expandSideTreeToPath(fileModel->currentPath());
}



void FileManager::initSideBar() {
    QModelIndex targetIndex = treeModel->expandToPath(fileModel->currentPath());
    if (targetIndex.isValid()) {
        expandToIndex(ui->treeView, targetIndex);
        ui->treeView->scrollTo(targetIndex);
        ui->treeView->setCurrentIndex(targetIndex);
    }
    else {
        qDebug() << "Не удалось найти путь:" << fileModel->currentPath();
    }
}

void FileManager::expandSideTreeToPath(QString path) {
    QModelIndex targetIndex = treeModel->expandToPath(path);

    if (targetIndex.isValid()) {
        expandToIndex(ui->treeView, targetIndex);
        ui->treeView->scrollTo(targetIndex);
        ui->treeView->setCurrentIndex(targetIndex);
    }
    else {
        qDebug() << "Не удалось найти путь:" << path;
    }
}

#include "FileManager.h"
#include "ChooseDirWindow.h"

FileManager::FileManager(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FileManager)
{
    ui->setupUi(this);

    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->setSizeGripEnabled(false);
    ui->contentLayout->addWidget(statusBar);

    ui->mainSplitter->setSizes({300, 7000, 250});
    ui->mainSplitter->widget(0)->setMinimumWidth(300);
    ui->mainSplitter->widget(2)->setMinimumWidth(250);

    fileModel = new FileModel(this);
    connect(fileModel, &FileModel::pathChanged, this, &FileManager::pathChanged);
    connect(fileModel, &FileModel::loading, this, [=](bool state)
            {
        if (state) {
            statusBar->showMessage("Загрузка...");
        } else {
            statusBar->showMessage(QString("Элементов: %1").arg(fileModel->rowCount()));
        } });

    ui->listView->setModel(fileModel);
    ui->listView->setItemDelegate(new FileModelDelegate(ui->listView));
    ui->listView->setViewMode(QListView::ListMode);
    ui->listView->setResizeMode(QListView::Adjust);

    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->listView->setUniformItemSizes(true);
    ui->treeView->setUniformRowHeights(true);
    ui->treeView->autoScroll = false;

    treeModel = new DirTreeModel(nullptr);
    treeModel->setRootToDrives();

    initSideBar();
    loaded = false;

    connect(treeModel, &DirTreeModel::dataChanged, this, &FileManager::initSideBar);
    connect(ui->treeView, &QTreeView::expanded, treeModel, &DirTreeModel::loadNode);
    connect(ui->treeView, &QTreeView::expanded, this, [this](const QModelIndex &index)
            {
        ui->treeView->setCurrentIndex(index);
        int childCount = ui->treeView->model()->rowCount(index);
        if (childCount > 0) {
            QModelIndex lastChild = ui->treeView->model()->index(childCount - 1, 0, index);

            ui->treeView->autoScroll = false;
            ui->treeView->scrollTo(index, QAbstractItemView::PositionAtTop);
            ui->treeView->scrollTo(lastChild, QAbstractItemView::PositionAtBottom);
            ui->treeView->autoScroll = true;
        } else {
            ui->treeView->autoScroll = false;
            ui->treeView->scrollTo(index, QAbstractItemView::PositionAtTop);
            ui->treeView->autoScroll = true;
        } });

    connect(ui->treeView->model(), &QAbstractItemModel::rowsInserted, this,
            [this](const QModelIndex &parent, int first, int last)
            {
                if (ui->treeView->isExpanded(parent))
                {
                    QModelIndex lastChild = ui->treeView->model()->index(last, 0, parent);
                    ui->treeView->autoScroll = false;
                    ui->treeView->scrollTo(parent, QAbstractItemView::PositionAtTop);
                    ui->treeView->scrollTo(lastChild, QAbstractItemView::PositionAtBottom);
                    ui->treeView->autoScroll = true;
                }
            });

    ui->treeView->setModel(treeModel);
    ui->treeView->header()->hide();
    ui->treeView->setIndentation(8);
    ui->treeView->setItemDelegate(new DirTreeDelegate(ui->treeView));

    connect(ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileManager::on_listView_selectionChanged);

    ui->pathLineEdit->setText(fileModel->currentPath());

    ui->deleteButton->setEnabled(false);
    ui->copyButton->setEnabled(false);
    ui->moveButton->setEnabled(false);

    previewWidget = new FilePreviewWidget(this);
    ui->previewLayout->addWidget(previewWidget);
}

FileManager::~FileManager()
{
    // delete ui;
}

void FileManager::on_pathLineEdit_returnPressed()
{
    fileModel->setPath(ui->pathLineEdit->text());
    enableActions(false);
}

void FileManager::on_listView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    QString fullPath = index.data(Qt::UserRole + 2).toString();
    QFileInfo fileInfo(fullPath);

    if (fileInfo.isDir())
    {
        fileModel->setPath(fullPath);
        enableActions(false);
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
    }
}

void FileManager::on_treeView_clicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    QString fullPath = index.data(Qt::UserRole).toString();
    fileModel->setPath(fullPath);
    ui->pathLineEdit->setText(fullPath);
}

void FileManager::on_listView_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    bool hasSelection = !selected.isEmpty();
    enableActions(hasSelection);

    if (hasSelection)
    {
        QModelIndex index = selected.indexes().first();
        QString path = index.data(Qt::UserRole + 2).toString();
        bool isDir = index.data(Qt::UserRole + 1).toString() == "Папка";
        if (previewWidget)
            previewWidget->previewFile(path, isDir);
    }
    else
    {
        if (previewWidget)
            previewWidget->clear();
    }
}

void FileManager::on_listView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->listView->indexAt(pos);
    if (!index.isValid())
    {
        return;
    }

    QMenu contextMenu(this);
    QAction *openAction = contextMenu.addAction("Открыть");
    QAction *copyAction = contextMenu.addAction("Копировать");
    QAction *moveAction = contextMenu.addAction("Переместить");
    QAction *deleteAction = contextMenu.addAction("Удалить");
    openAction->setIcon(QIcon(":/FileManager/Resources/open.svg"));
    copyAction->setIcon(QIcon(":/FileManager/Resources/copy.svg"));
    moveAction->setIcon(QIcon(":/FileManager/Resources/move.svg"));
    deleteAction->setIcon(QIcon(":/FileManager/Resources/delete.svg"));

    QAction *selectedItem = contextMenu.exec(ui->listView->mapToGlobal(pos));
    if (selectedItem == openAction)
    {
        on_listView_doubleClicked(index);
    }
    else if (selectedItem == moveAction)
    {
        on_moveButton_clicked();
    }
    else if (selectedItem == deleteAction)
    {
        on_deleteButton_clicked();
    }
}

void FileManager::enableActions(bool enable)
{
    if (ui->deleteButton == nullptr || ui->copyButton == nullptr || ui->moveButton == nullptr)
        return;
    ui->deleteButton->setEnabled(enable);
    ui->copyButton->setEnabled(enable);
    ui->moveButton->setEnabled(enable);
}

void FileManager::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        ui->listView->clearSelection();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

void FileManager::on_deleteButton_clicked()
{
    bool result = fileModel->deleteFile(ui->listView->currentIndex());
    if (result)
    {
        QMessageBox::information(this, "Удаление", "Файл успешно удален");
    }
    else
    {
        QMessageBox::warning(this, "Удаление", "Не удалось удалить файл");
    }
}

void FileManager::on_moveButton_clicked()
{
    QString targetPath = ChooseDirWindow::chooseDirPath(this, fileModel->currentPath(), "Переместить в");
    if (targetPath.isEmpty())
    {
        return;
    }

    bool result = fileModel->moveFile(ui->listView->currentIndex(), targetPath);
    if (!result)
    {
        QMessageBox::warning(this, "Перемещение", "Не удалось переместить файл");
    }
}

void FileManager::on_copyButton_clicked()
{
    QString targetPath = ChooseDirWindow::chooseDirPath(this, fileModel->currentPath(), "Копировать в");
    if (targetPath.isEmpty())
    {
        return;
    }

    bool result = fileModel->copyFile(ui->listView->currentIndex(), targetPath);
    if (!result)
    {
        QMessageBox::warning(this, "Копирование", "Не удалось скопировать файл");
    }
}

void FileManager::on_refreshButton_clicked()
{
    fileModel->setPath(fileModel->currentPath());
}

void FileManager::pathChanged(const QString &newPath)
{
    ui->pathLineEdit->setText(newPath);
    expandSideTreeToPath(fileModel->currentPath());
    if (previewWidget)
        previewWidget->clear();
}

void FileManager::initSideBar()
{
    if (loaded)
        return;

    loaded = true;
    QModelIndex targetIndex = treeModel->expandToPath(fileModel->currentPath());
    if (targetIndex.isValid())
    {
        expandToIndex(ui->treeView, targetIndex);
        ui->treeView->scrollTo(targetIndex);
        ui->treeView->setCurrentIndex(targetIndex);
    }
    else
    {
        qDebug() << "Не удалось найти путь:" << fileModel->currentPath();
    }
}

void FileManager::expandSideTreeToPath(QString path)
{
    QModelIndex targetIndex = treeModel->expandToPath(path);

    if (targetIndex.isValid())
    {
        expandToIndex(ui->treeView, targetIndex);
        ui->treeView->scrollTo(targetIndex);
        ui->treeView->setCurrentIndex(targetIndex);
    }
    else
    {
        qDebug() << "Не удалось найти путь:" << path;
    }
}

#include "DirTreeModel.h"

DirTreeModel::DirTreeModel(QObject* parent)
    : QAbstractItemModel(parent) {
    setRootToDrives();
}

DirTreeModel::~DirTreeModel() {
    for (Worker* worker : activeWorkers.values()) {
        if (worker->thread()) {
            worker->thread()->quit();
            worker->thread()->wait();
        }
        delete worker;
    }
    activeWorkers.clear();
    delete root;
}

void DirTreeModel::setRootToDrives() {
    beginResetModel();
    delete root;
    root = new Node("");
    QFileInfoList drives = QDir::drives();
	QFileIconProvider iconProvider;
    for (const QFileInfo& drive : drives) {
        Node* driveNode = new Node(drive.absoluteFilePath(), root);
		driveNode->icon = iconProvider.icon(drive);
		driveNode->hasChildren = 1;
        root->children.append(driveNode);
        nodeCache[driveNode->path] = driveNode;
    }
    root->populated = true;
    endResetModel();
}

void DirTreeModel::populateNode(Node* node) {
    if (node->populated || node->isLoading) return;

    node->isLoading = true;

    QModelIndex nodeIndex = indexForNode(node);
    if (nodeIndex.isValid()) {
        emit dataChanged(nodeIndex, nodeIndex, QVector<int>{Qt::DisplayRole});
    }

    Worker* worker = new Worker(node->path);
    QThread* thread = new QThread(this);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &Worker::process);
    connect(worker, &Worker::finished, this, &DirTreeModel::onPopulateFinished);
    connect(worker, &QObject::destroyed, this, &DirTreeModel::onWorkerDestroyed);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    activeWorkers[node] = worker;

    thread->start();
}

void DirTreeModel::onPopulateFinished(const QFileInfoList& dirList) {
    Worker* worker = qobject_cast<Worker*>(sender());
    if (!worker) {
        return;
    }

    Node* node = activeWorkers.key(worker);
    if (!node) {
        worker->deleteLater();
        return;
    }

	QFileIconProvider iconProvider;
	QModelIndex parentIndex = indexForNode(node);
    node->dirList = dirList;

    if (parentIndex.isValid() && dirList.size() > 0) {
        beginInsertRows(parentIndex, 0, dirList.size() - 1);

        for (const QFileInfo& info : dirList) {
            Node* child = new Node(info.absoluteFilePath(), node);
			child->icon = iconProvider.icon(info);
            child->hasChildren = -1;
            node->children.append(child);
            nodeCache[child->path] = child;
        }

        node->isLoading = false;
        node->populated = true;
        endInsertRows();

        emit dataChanged(parentIndex, parentIndex, QVector<int>{Qt::DisplayRole});
    }
    else {
        node->isLoading = false;
        node->populated = true;
        
        if (parentIndex.isValid()) {
            emit dataChanged(parentIndex, parentIndex, QVector<int>{Qt::DisplayRole});
        }
    }
    

    activeWorkers.remove(node);
    worker->deleteLater();
}

void DirTreeModel::onWorkerDestroyed(QObject* worker) {
    Node* node = activeWorkers.key(static_cast<Worker*>(worker));
	if (node) activeWorkers.remove(node);
}

void DirTreeModel::loadNode(const QModelIndex& index) {
    Node* node = nodeFromIndex(index);
	if (!node->populated && !node->isLoading) {
		populateNode(node);
	}
}

QModelIndex DirTreeModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    Node* parentNode = nodeFromIndex(parent);
    if (!parentNode->populated) {
        return QModelIndex();
    }

    if (row >= parentNode->children.count()) {
        return QModelIndex();
    }

    return createIndex(row, column, parentNode->children[row]);
}

QModelIndex DirTreeModel::parent(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    Node* childNode = static_cast<Node*>(index.internalPointer());
    Node* parentNode = childNode->parent;

    if (!parentNode || parentNode == root) {
        return QModelIndex();
    }

    Node* grandParentNode = parentNode->parent;
    int row = grandParentNode->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int DirTreeModel::rowCount(const QModelIndex& parent) const {
    Node* parentNode = nodeFromIndex(parent);
    if (!parentNode->populated) {
        return 0;
    }
    return parentNode->children.count();
}

int DirTreeModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant DirTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    Node* node = static_cast<Node*>(index.internalPointer());
    QFileInfo dirInfo(node->path);
    QFileIconProvider iconProvider;

    switch (role) {
    case Qt::DisplayRole:
        if (node->isLoading) {
            return "Loading...";
        }
        return dirInfo.fileName().isEmpty() ? node->path : dirInfo.fileName();
    case Qt::DecorationRole:
        return node->icon;
    case Qt::UserRole:
		return node->path;
    default:
        return QVariant();
    }
}

bool DirTreeModel::hasChildren(const QModelIndex& parent) const {
    Node* node = nodeFromIndex(parent);
	if (node->hasChildren == -1) {
		QDir dir(node->path);
        dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		node->hasChildren = dir.count() > 0 ? 1 : 0;
	}
    return node->hasChildren == 1;
}

DirTreeModel::Node* DirTreeModel::nodeFromIndex(const QModelIndex& index) const {
    if (!index.isValid()) {
        return root;
    }
    return static_cast<Node*>(index.internalPointer());
}

QModelIndex DirTreeModel::indexForNode(Node* node) const {
    if (node == root) {
        return QModelIndex();
    }
    Node* parentNode = node->parent;
    int row = parentNode->children.indexOf(node);
    return createIndex(row, 0, node);
}

QModelIndex DirTreeModel::expandToPath(const QString& path) {
    if (path.isEmpty()) {
        return QModelIndex();
    }

    QString cleanPath = QDir::cleanPath(path);

    QModelIndex currentIndex = QModelIndex();
    QString drivePath;
    for (int row = 0; row < rowCount(QModelIndex()); ++row) {
        QModelIndex driveIndex = index(row, 0, QModelIndex());
        drivePath = driveIndex.data(Qt::UserRole).toString();
        if (cleanPath.startsWith(drivePath, Qt::CaseInsensitive)) {
            currentIndex = driveIndex;
            loadNode(currentIndex);
            break;
        }
    }

    if (!currentIndex.isValid()) {
        return QModelIndex();
    }

    QString remainingPath = cleanPath.mid(drivePath.length());
    if (remainingPath.startsWith('/')) {
        remainingPath = remainingPath.mid(1);
    }
    QStringList segments = remainingPath.split('/', Qt::SkipEmptyParts);

    for (const QString& segment : segments) {
        Node* currentNode = nodeFromIndex(currentIndex);
        if (!currentNode->populated && !currentNode->isLoading) {
            loadNode(currentIndex);

            while (currentNode->isLoading) {
                QThread::msleep(10);
                QCoreApplication::processEvents();
            }
        }

        bool found = false;
        for (int row = 0; row < rowCount(currentIndex); ++row) {
            QModelIndex childIndex = index(row, 0, currentIndex);
            QString childName = childIndex.data().toString();

            if (childName.compare(segment, Qt::CaseInsensitive) == 0) {
                currentIndex = childIndex;
                found = true;
                break;
            }
        }

        if (!found) {
            break;
        }
    }

    return currentIndex;
}
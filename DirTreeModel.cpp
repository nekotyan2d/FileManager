#include "DirTreeModel.h"

DirTreeModel::DirTreeModel(QObject* parent)
    : QAbstractItemModel(parent) {
    setRootToDrives();
}

void DirTreeModel::setRootToDrives() {
    beginResetModel();
    delete root;
    root = new Node("");
    QFileInfoList drives = QDir::drives();
    for (const QFileInfo& drive : drives) {
        Node* driveNode = new Node(drive.absoluteFilePath(), root);
        root->children.append(driveNode);
        nodeCache[driveNode->path] = driveNode;
    }
    root->populated = true;
    endResetModel();
}

void DirTreeModel::populateNode(Node* node) const {
    if (node->populated) {
        return;
    }

    QDir dir(node->path);
    node->dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    node->children.clear();
    for (const QFileInfo& info : node->dirList) {
        Node* child = new Node(info.absoluteFilePath(), node);
        node->children.append(child);
        nodeCache[child->path] = child;
    }
    node->populated = true;
}

void DirTreeModel::preFetchChildren(const QModelIndex& parent) const {
    Node* parentNode = nodeFromIndex(parent);
    if (!parentNode->populated) {
        populateNode(parentNode);
    }
    // предзагрузка на 1 уровень ниже
    for (Node* child : parentNode->children) {
        if (!child->populated && hasChildren(index(parentNode->children.indexOf(child), 0, parent))) {
            populateNode(child);
        }
    }
}

QModelIndex DirTreeModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    Node* parentNode = nodeFromIndex(parent);
    if (!parentNode->populated) {
        populateNode(parentNode);
        preFetchChildren(parent);
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
        populateNode(parentNode);
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
        return dirInfo.fileName().isEmpty() ? node->path : dirInfo.fileName();
    case Qt::DecorationRole:
        return iconProvider.icon(dirInfo);
    case Qt::UserRole:
		return node->path;
    default:
        return QVariant();
    }
}

bool DirTreeModel::hasChildren(const QModelIndex& parent) const {
    Node* node = nodeFromIndex(parent);
    QDir dir(node->path);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    return dir.entryList().count() > 0;
}

DirTreeModel::Node* DirTreeModel::nodeFromIndex(const QModelIndex& index) const {
    if (!index.isValid()) {
        return root;
    }
    return static_cast<Node*>(index.internalPointer());
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
        drivePath = driveIndex.data().toString();
        if (cleanPath.startsWith(drivePath, Qt::CaseInsensitive)) {
            currentIndex = driveIndex;
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
        if (!currentNode->populated) {
            populateNode(currentNode);
            preFetchChildren(currentIndex);
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
            return currentIndex;
        }
    }

    return currentIndex;
}
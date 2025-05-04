#ifndef DIRTREEMODEL_H
#define DIRTREEMODEL_H

#include <QAbstractItemModel>
#include <QDir>
#include <QFileInfoList>
#include <QFileIconProvider>
#include <QThread>
#include <QObject>
#include <QCoreApplication>
#include <QtConcurrent>
#include <QFuture>

class DirTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DirTreeModel(QObject* parent = nullptr);
    ~DirTreeModel() override;

    void setRootToDrives();
    QModelIndex expandToPath(const QString& path);
    void loadNode(const QModelIndex& index);

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override { return 1; };
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
private:
    struct Node {
        QString path;
        QFileInfoList dirList;
        Node* parent;
        QList<Node*> children;
        QIcon icon;

        bool populated = false;
        bool isLoading = false;

        int hasChildren = -1; // -1 - неизвестно, 0 - нет, 1 - да

        int fileCount;

        Node(const QString& path, Node* parent = nullptr) : path(path), parent(parent) {}
        ~Node() { qDeleteAll(children); }
    };

    Node* root = nullptr;

    mutable QHash<QString, Node*> nodeCache;

    void populateNode(Node* node);
    Node* nodeFromIndex(const QModelIndex& index) const;
    QModelIndex indexForNode(Node* node) const;

    QFileIconProvider iconProvider;
private slots:
    void onPopulateFinished(Node* node, const QFileInfoList& dirList);
};

#endif // DIRTREEMODEL_H

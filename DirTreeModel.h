#pragma once

#include <QAbstractItemModel>
#include <QDir>
#include <QFileInfoList>
#include <QFileIconProvider>

class DirTreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit DirTreeModel(QObject* parent = nullptr);
	~DirTreeModel() override = default;

	void setRootToDrives();
	QModelIndex expandToPath(const QString& path);

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
private:
	struct Node {
		QString path;
		QFileInfoList dirList;
		Node* parent;
		QList<Node*> children;
		bool populated = false;
		bool isDrive = false;

		Node(const QString& path, Node* parent = nullptr) : path(path), parent(parent) {}
		~Node() { qDeleteAll(children); }
	};

	Node* root = nullptr;

	mutable QHash<QString, Node*> nodeCache;

	void populateNode(Node* node) const;
	Node* nodeFromIndex(const QModelIndex& index) const;
	void preFetchChildren(const QModelIndex& parent) const;
};


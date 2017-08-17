#ifndef SNIPPETTREE_H
#define SNIPPETTREE_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QString>
#include <QList>
#include <QDialog>

class Snippet;
class QAction;
class QItemSelection;

enum SnippetTreeItemType {
	ROOT,
	CATEGORY,
	SUBCATEGORY,
	SNIPPET
};

class SnippetTreeItem {
public:
	SnippetTreeItem (QString name, SnippetTreeItemType type, SnippetTreeItem* parent = 0);
	virtual ~SnippetTreeItem ();

	void setData (Snippet* data);
	Snippet* getData ();

	QString getName() const { return m_name; }
	void setName(const QString &name) { m_name = name; }

	SnippetTreeItem* child (int row) { return m_children[row]; }
	int childCount () { return m_children.length (); }
	int columnCount () { return 1; }
	SnippetTreeItem* parentItem () { return m_parent; }

	void addChild (SnippetTreeItem* child);
	void removeChild (int row);

	int row () const;

	SnippetTreeItemType getType () const  { return m_type; }

	QVariant data (int /*column*/) { return QVariant (m_name); }
private:
	QString m_name;
	SnippetTreeItemType m_type;
	Snippet* m_data;
	SnippetTreeItem* m_parent;
	QList<SnippetTreeItem*> m_children;
};


class SnippetModel: public QAbstractItemModel {
public:
	SnippetModel (QObject* parent);

	~SnippetModel () { delete m_rootItem; }

	QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex ()) const;
	QModelIndex parent(const QModelIndex & index) const;

	int rowCount(const QModelIndex & parent = QModelIndex ()) const;
	int	columnCount (const QModelIndex & parent = QModelIndex ()) const;
	QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags (const QModelIndex &index) const;
private:
	SnippetTreeItem* m_rootItem;
};

class SnippetTree : public QTreeView {
	Q_OBJECT
public:
	explicit SnippetTree (QWidget* parent = 0);
protected:
	void contextMenuEvent (QContextMenuEvent * e);
	void selectionChanged (const QItemSelection & selected, const QItemSelection & deselected);
signals:
	void selection (SnippetTreeItem*);
public slots:
	void addCategory ();
private:
	void createActions();

	SnippetModel* m_model;
	QAction* m_addCategory;
};

class CategoryDialog : public QDialog {
	Q_OBJECT
public:
	CategoryDialog (QWidget* parent);

};

#endif // SNIPPETTREE_H

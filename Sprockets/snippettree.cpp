#include "snippettree.h"
#include "snippetdb.h"
#include <QHeaderView>
#include <QDebug>
#include <QContextMenuEvent>
#include <QAction>
#include <QAbstractItemView>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QItemSelection>

QModelIndex SnippetModel::index (int row, int column, const QModelIndex& parent) const {
	if (!hasIndex (row, column, parent)) {
		return QModelIndex ();
	}

	SnippetTreeItem* parentItem;

	if (!parent.isValid ()) {
		parentItem = m_rootItem;
	} else {
		parentItem = static_cast<SnippetTreeItem*>(parent.internalPointer());
	}

	SnippetTreeItem* childItem = parentItem->child (row);

	if (childItem) {
		return createIndex (row, column, childItem);
	} else {
		return QModelIndex();
	}
}

QModelIndex SnippetModel::parent (const QModelIndex& index) const {
	if (!index.isValid ()) {
		return QModelIndex ();
	}

	SnippetTreeItem* childItem = static_cast<SnippetTreeItem*> (index.internalPointer ());
	SnippetTreeItem* parentItem = childItem->parentItem ();

	if (parentItem == m_rootItem) {
		return QModelIndex ();
	}

	return createIndex (parentItem->row (), 0, parentItem);
}

int SnippetModel::rowCount (const QModelIndex& parent) const {
	SnippetTreeItem* parentItem;

	if (parent.column () > 0) {
		return 0;
	}

	if (!parent.isValid ()) {
		parentItem = m_rootItem;
	} else {
		parentItem = static_cast<SnippetTreeItem*> (parent.internalPointer ());
	}

	return parentItem->childCount ();
}

int SnippetModel::columnCount (const QModelIndex& parent) const {
	if (parent.isValid ()) {
		return static_cast<SnippetTreeItem*> (parent.internalPointer ())->columnCount ();
	} else {
		return m_rootItem->columnCount ();
	}
}

QVariant SnippetModel::data (const QModelIndex &index, int role) const {

	if (!index.isValid ()) {
		return QVariant();
	}

	if (role != Qt::DisplayRole) {
		return QVariant ();
	}

	SnippetTreeItem *item = static_cast<SnippetTreeItem*> (index.internalPointer ());


	return item->data (0);
}

Qt::ItemFlags SnippetModel::flags (const QModelIndex &index) const {
	if (!index.isValid ()) {
		return 0;
	}

	return QAbstractItemModel::flags (index);
}


SnippetTreeItem::SnippetTreeItem (QString name, SnippetTreeItemType type, SnippetTreeItem* parent) : m_name (name), m_type (type), m_parent (parent) {

}

SnippetTreeItem::~SnippetTreeItem () {
	for (int i = 0; i < m_children.length (); ++i) {
		delete m_children[i];
	}
}

void SnippetTreeItem::setData (Snippet* data) {
	m_data = data;
}

void SnippetTreeItem::addChild(SnippetTreeItem *child) {
	int i = 0;
	for (; i < m_children.length (); ++i) {
		if (m_children[i]->getName () <= child->getName ()) break;
	}
	m_children.insert (i, child);
}

void SnippetTreeItem::removeChild (int row) {
	m_children.removeAt (row);
}

int SnippetTreeItem::row() const {
	int index = 0;

	if (m_parent) {
		m_parent->m_children.indexOf (const_cast<SnippetTreeItem*> (this));
	}

	return index;
}


Snippet* SnippetTreeItem::getData () {
	return m_data;
}



SnippetModel::SnippetModel (QObject* parent) : QAbstractItemModel (parent) {
	m_rootItem = new SnippetTreeItem ("Snippets", SnippetTreeItemType::ROOT);


	SnippetTreeItem* jsFeatures = new SnippetTreeItem (tr ("Language"), SnippetTreeItemType::CATEGORY, m_rootItem);

	m_rootItem->addChild (jsFeatures);
}

SnippetTree::SnippetTree (QWidget *parent) : QTreeView (parent) {
	m_model = new SnippetModel (this);
	setModel (m_model);
	header ()->hide ();

	setSelectionMode (QAbstractItemView::SingleSelection);
	createActions ();
}

void SnippetTree::createActions () {
	m_addCategory = new QAction ("Add Category", this);

	connect(m_addCategory, SIGNAL(triggered()), this, SLOT(addCategory ()));
}

void SnippetTree::addCategory() {
	QModelIndexList list = selectedIndexes ();
	SnippetTreeItem* item = static_cast<SnippetTreeItem*>(list[0].internalPointer ());
	qDebug () << item->getName ();
}

void SnippetTree::contextMenuEvent (QContextMenuEvent* e) {
	qDebug () << "context menu event";
	QMenu contextMenu (tr("Snippets"), this);

	QModelIndexList list = selectedIndexes ();
	SnippetTreeItem* item = static_cast<SnippetTreeItem*>(list[0].internalPointer ());

	if (item->getType () == SnippetTreeItemType::SNIPPET) {

	} else {
		contextMenu.addAction (m_addCategory);
	}

	contextMenu.exec (e->globalPos ());
}

void SnippetTree::selectionChanged (const QItemSelection &selected, const QItemSelection &deselected) {
	QModelIndexList list = selected.indexes ();
	SnippetTreeItem* item = static_cast<SnippetTreeItem*>(list[0].internalPointer ());

	emit selection (item);
}


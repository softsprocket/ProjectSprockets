#include "filetreedock.h"

#include <QDir>
#include <QStringList>
#include <QModelIndex>
#include <QCoreApplication>
#include <QDebug>

TreeContextMenu::TreeContextMenu (QWidget *parent) : QMenu (parent) {

}

void TreeContextMenu::createMenu (QFileInfo fileInfo) {
	m_fileInfo = fileInfo;

	createActions ();
}

void TreeContextMenu::createActions () {
	m_cutAction = new QAction (tr("Cut"), this);
	m_cutAction->setShortcut (QKeySequence::Cut);
	connect(m_cutAction, SIGNAL(triggered()), this, SLOT(cut()));
	addAction (m_cutAction);

	m_copyAction = new QAction (tr("Copy"), this);
	m_copyAction->setShortcut (QKeySequence::Copy);
	connect(m_copyAction, SIGNAL(triggered()), this, SLOT(copy()));
	addAction (m_copyAction);


	m_pasteAction = new QAction (tr("Paste"), this);
	m_pasteAction->setShortcut (QKeySequence::Paste);
	connect(m_pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
	addAction (m_pasteAction);


	m_deleteAction = new QAction (tr("Delete"), this);
	m_deleteAction->setShortcut (QKeySequence::Delete);
	connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(del()));
	addAction (m_deleteAction);


	if (m_fileInfo.isFile ()) {
		m_openFileAction = new QAction (tr("Open"), this);
		m_openFileAction->setShortcut (QKeySequence::Open);
		connect(m_openFileAction, SIGNAL(triggered()), this, SLOT(emitOpenFile()));
		addAction (m_openFileAction);

	}
}

void TreeContextMenu::cut () {

}

void TreeContextMenu::copy () {

}

void TreeContextMenu::paste () {

}

void TreeContextMenu::del () {

}

void TreeContextMenu::emitOpenFile () {
	emit openFile ();
}


FileTreeDock::FileTreeDock (const QString &title, QWidget *parent, Qt::WindowFlags flags) : QDockWidget (title, parent, flags) {
	setProperties ();
}

FileTreeDock::FileTreeDock (QWidget *parent, Qt::WindowFlags flags) : QDockWidget (parent, flags) {
	setProperties ();
}


void FileTreeDock::setProperties () {
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
}

void FileTreeDock::createTree () {
	m_treeView = new QTreeView;
	connect (m_treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitOpenFile(QModelIndex)));
	m_fileModel = new QFileSystemModel;
	m_treeView->setModel (m_fileModel);
	m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect (m_treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
	setWidget (m_treeView);
}

void FileTreeDock::onCustomContextMenu (const QPoint &point) {
	m_currentSelectedIndex = m_treeView->indexAt (point);

	if (m_currentSelectedIndex.isValid ()) {
		TreeContextMenu contextMenu (this);
		connect (&contextMenu, SIGNAL(openFile()), this, SLOT(emitOpenFile()));
		contextMenu.createMenu (m_fileModel->fileInfo (m_currentSelectedIndex));
		contextMenu.exec (m_treeView->mapToGlobal(point));
	}
}

void FileTreeDock::emitOpenFile () {
	emit openFile (m_fileModel->fileInfo (m_currentSelectedIndex));
}


void FileTreeDock::emitOpenFile (QModelIndex index) {
	emit openFile (m_fileModel->fileInfo (index));
}
void FileTreeDock::setRootDir (QString dir) {
	createTree ();
	m_fileModel->setRootPath (dir);
	QModelIndex idx = m_fileModel->index (dir);
	m_treeView->setRootIndex (idx);
}


ToolBoxTreeDock::ToolBoxTreeDock (const QString &title, QWidget *parent, Qt::WindowFlags flags) : QDockWidget (title, parent, flags) {
	setProperties ();
}

ToolBoxTreeDock::ToolBoxTreeDock (QWidget *parent, Qt::WindowFlags flags) : QDockWidget (parent, flags) {
	setProperties ();
}


void ToolBoxTreeDock::setProperties () {
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	m_toolBox = new QToolBox;
	setWidget (m_toolBox);
}


void ToolBoxTreeDock::addRootDir (QString dir) {
	m_treeView = new QTreeView;
	connect (m_treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitOpenFile(QModelIndex)));
	m_fileModel = new QFileSystemModel;
	m_fileModel->setRootPath (dir);
	m_treeView->setModel (m_fileModel);
	QModelIndex idx = m_fileModel->index (dir);
	m_treeView->setRootIndex (idx);
	m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect (m_treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	QStringList lst = dir.split (QDir::separator ());

	m_toolBox->setCurrentIndex (m_toolBox->addItem (m_treeView, lst.back ()));

}

void ToolBoxTreeDock::onCustomContextMenu (const QPoint &point) {
	m_currentSelectedIndex = m_treeView->indexAt (point);

	if (m_currentSelectedIndex.isValid ()) {
		TreeContextMenu contextMenu (this);
		connect (&contextMenu, SIGNAL(openFile()), this, SLOT(emitOpenFile()));
		contextMenu.createMenu (m_fileModel->fileInfo (m_currentSelectedIndex));
		contextMenu.exec (m_treeView->mapToGlobal(point));
	}
}

void ToolBoxTreeDock::emitOpenFile () {
	emit openFile (m_fileModel->fileInfo (m_currentSelectedIndex));
}

void ToolBoxTreeDock::emitOpenFile (QModelIndex index) {
	emit openFile (m_fileModel->fileInfo (index));
}

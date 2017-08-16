#ifndef FILETREEDOCK_H
#define FILETREEDOCK_H

#include <QDockWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QList>
#include <QToolBox>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QModelIndex>

class TreeContextMenu : public QMenu {
	Q_OBJECT
private slots:
	void cut ();
	void copy ();
	void paste ();
	void del ();
	void emitOpenFile ();
signals:
	void openFile ();
private:
	QAction* m_openFileAction;
	QAction* m_cutAction;
	QAction* m_copyAction;
	QAction* m_pasteAction;
	QAction* m_deleteAction;

	QFileInfo m_fileInfo;

	void createActions ();
public:
	TreeContextMenu (QWidget* parent = 0);
	void createMenu (QFileInfo fileInfo);

};

class FileTreeDock : public QDockWidget {
	Q_OBJECT
private slots:
	void onCustomContextMenu (const QPoint &point);
	void emitOpenFile ();
	void emitOpenFile (QModelIndex index);
signals:
	void openFile (QFileInfo fileInfo);
private:
	QFileSystemModel* m_fileModel;
	QModelIndex m_currentSelectedIndex;

	QTreeView* m_treeView;
	void setProperties ();
	void createTree ();

public:
	FileTreeDock (const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);
	FileTreeDock (QWidget * parent = 0, Qt::WindowFlags flags = 0);

	void setRootDir (QString dir);

};

class ToolBoxTreeDock : public QDockWidget {
	Q_OBJECT
private slots:
	void onCustomContextMenu (const QPoint &point);
	void emitOpenFile ();
	void emitOpenFile (QModelIndex index);
signals:
	void openFile (QFileInfo fileInfo);
private:
	QFileSystemModel* m_fileModel;
	QModelIndex m_currentSelectedIndex;

	QTreeView* m_treeView;
	QToolBox* m_toolBox;
	void setProperties ();
public:

	ToolBoxTreeDock (const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);
	ToolBoxTreeDock (QWidget * parent = 0, Qt::WindowFlags flags = 0);

	void addRootDir (QString dir);
};

#endif // FILETREEDOCK_H

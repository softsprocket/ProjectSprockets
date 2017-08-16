#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QJsonDocument>
#include <QList>
#include <QTreeWidgetItem>
#include <QVariant>
#include <QDir>
#include <QMetaType>
#include <QTabWidget>
#include <QFileInfo>
#include <QDockWidget>

#include "filetreedock.h"
#include "scratchpad.h"
#include "snippetwidget.h"
#include "tooltabs.h"

class ProjectWizard;
class QTreeWidget;

Q_DECLARE_METATYPE(QDir)

namespace Ui {
class MainWindow;
}

class LibSprockets;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow (QWidget *parent = 0);
	~MainWindow();

	void closeEvent(QCloseEvent *event);
public slots:
	void openFile (QFileInfo fileInfo);
	void closeFile (int);
	void toolTabWindowClosed ();
private slots:
	void newProjectWizard ();
	void newFile ();
	void openProject ();
	void textEdited ();
	void saveFile ();
	void saveFileAs ();
	void dockedLibrariesWindowVisibilityChanged (bool visible);
	void dockedProjectWindowVisibilityChanged (bool visible);
	void toggleLibrariesDockVisibility ();
	void toggleProjectDockVisibility ();
	void toggleToolTabsVisibility ();
private:

	void writeSettings ();
	void readSettings ();
	void createActions ();
	void createMenus ();
	void saveFileAt (int index);
	void enableSaveFileActions (bool enable);

	QMenu* m_fileMenu;
	QMenu* m_windowMenu;
	QMenu* m_newMenu;
	QMenu* m_openMenu;

	QAction* m_newProjectAction;
	QAction* m_newFileAction;
	QAction* m_openProjectAction;
	QAction* m_saveAction;
	QAction* m_saveAsAction;
	QAction* m_saveAllAction;
	QAction* m_toggleProjectDock;
	QAction* m_toggleLibrariesDock;
	QAction* m_toggleToolTabs;

	ProjectWizard* m_projectWizard;
	QJsonDocument m_projectDocument;

	QTabWidget* m_fileTabs;
	FileTreeDock* m_projectTree;
	ToolBoxTreeDock* m_libraryTrees;
	ToolTabs* m_toolTabsWindow;
	QTabWidget* m_toolTabs;
	Scratchpad* m_scratchpad;
	SnippetWidget* m_snippetWidget;

	LibSprockets* m_libSprockets;

	void newProjectAccept ();
	void buildProjectTreeModel ();
	void setupEditor (QFileInfo info);
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "projectwizard.h"
#include "codeeditor.h"

#include <QKeySequence>
#include <QMenuBar>
#include <QVBoxLayout>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFileIconProvider>
#include <QDockWidget>
#include <QTreeWidget>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QScreen>
#include <QSignalBlocker>
#include <libsprockets.h>
#include <QSettings>

#include "jshighlighter.h"


MainWindow::MainWindow (QWidget *parent) :
	QMainWindow(parent) {

	resize (1400, 800);

	createActions ();
	createMenus ();

	m_projectTree = new FileTreeDock ("Project", this);
	connect (m_projectTree, SIGNAL(openFile(QFileInfo)), this, SLOT(openFile(QFileInfo)));
	connect (m_projectTree, SIGNAL(visibilityChanged(bool)), this, SLOT(dockedProjectWindowVisibilityChanged(bool)));
	addDockWidget(Qt::LeftDockWidgetArea, m_projectTree);


	m_libraryTrees = new ToolBoxTreeDock ("Libraries", this);
	connect (m_libraryTrees, SIGNAL(openFile(QFileInfo)), this, SLOT(openFile(QFileInfo)));
	connect (m_libraryTrees, SIGNAL(visibilityChanged(bool)), this, SLOT(dockedLibrariesWindowVisibilityChanged(bool)));
	addDockWidget(Qt::RightDockWidgetArea, m_libraryTrees);

	m_toolTabsWindow = new ToolTabs ();
	m_toolTabs = new QTabWidget (m_toolTabsWindow);

	m_scratchpad = new Scratchpad (m_toolTabs);
	m_toolTabs->addTab (m_scratchpad, "Scratchpad");

	m_snippetWidget = new SnippetWidget (m_toolTabs);
	m_toolTabs->addTab (m_snippetWidget, "Snippet Tree");

	QVBoxLayout *toolsLayout = new QVBoxLayout;
	m_toolTabsWindow->setLayout (toolsLayout);
	connect (m_toolTabsWindow, SIGNAL (close()), this, SLOT (toolTabWindowClosed()));

	toolsLayout->addWidget (m_toolTabs);

	m_fileTabs = new QTabWidget;
	m_fileTabs->setTabsClosable (true);
	connect (m_fileTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));

	setCentralWidget (m_fileTabs);

	m_libSprockets = new LibSprockets ();

	m_scratchpad->setEngine (m_libSprockets->getEngine ());
	m_scratchpad->setEditorTabs (m_fileTabs);
	m_scratchpad->createJSScratchPad ();
	m_scratchpad->setSnippetTree (m_snippetWidget->snippetTree ());

	readSettings ();
}

MainWindow::~MainWindow () {
	delete m_toolTabsWindow;
}

void MainWindow::toolTabWindowClosed () {
	m_toggleToolTabs->setText ("Show Tool Tabs");
}

void MainWindow::dockedProjectWindowVisibilityChanged (bool /*visible*/) {
	toggleProjectDockVisibility ();
}

void MainWindow::dockedLibrariesWindowVisibilityChanged (bool /*visible*/) {
	toggleLibrariesDockVisibility ();
}

void MainWindow::toggleLibrariesDockVisibility () {
	QString txt = m_toggleLibrariesDock->text ();
	QStringList lst = txt.split (" ");
	if (lst[0] == "Show") {
		m_toggleLibrariesDock->setText ("Hide Libraries");
		const QSignalBlocker blocker(m_libraryTrees);
		m_libraryTrees->setVisible (true);
	} else {
		m_toggleLibrariesDock->setText ("Show Libraries");
		const QSignalBlocker blocker(m_libraryTrees);
		m_libraryTrees->setVisible (false);
	}
}

void MainWindow::toggleProjectDockVisibility () {
	QString txt = m_toggleProjectDock->text ();
	QStringList lst = txt.split (" ");
	if (lst[0] == "Show") {
		m_toggleProjectDock->setText ("Hide Project");
		const QSignalBlocker blocker(m_projectTree);
		m_projectTree->setVisible (true);
	} else {
		m_toggleProjectDock->setText ("Show Project");
		const QSignalBlocker blocker(m_projectTree);
		m_projectTree->setVisible (false);
	}

}

void MainWindow::toggleToolTabsVisibility () {
	QString txt = m_toggleToolTabs->text ();
	QStringList lst = txt.split (" ");
	if (lst[0] == "Show") {
		m_toggleToolTabs->setText ("Hide Tool Tabs");
		const QSignalBlocker blocker(m_toolTabs);
		m_toolTabsWindow->setVisible (true);
	} else {
		m_toggleToolTabs->setText ("Show Tool Tabs");
		const QSignalBlocker blocker(m_toolTabs);
		m_toolTabsWindow->setVisible (false);
	}

}

void MainWindow::createActions () {
	m_newProjectAction = new QAction (tr("&Project"), this);
	m_newProjectAction->setShortcut (QKeySequence(tr("Ctrl+N+P")));
	m_newProjectAction->setStatusTip (tr("Open the New Project wizard"));
	connect(m_newProjectAction, SIGNAL(triggered()), this, SLOT(newProjectWizard ()));

	m_newFileAction = new QAction (tr("&File"), this);
	m_newFileAction->setShortcut (QKeySequence::New);
	m_newFileAction->setStatusTip (tr("Open a New File for editing"));
	connect(m_newFileAction, SIGNAL(triggered()), this, SLOT(newFile ()));

	m_openProjectAction = new QAction (tr("&Project"), this);
	m_openProjectAction->setShortcut (QKeySequence(tr("Ctrl+O+P")));
	m_openProjectAction->setStatusTip (tr("Open Project"));
	connect(m_openProjectAction, SIGNAL(triggered()), this, SLOT(openProject ()));

	m_saveAction = new QAction (tr("&Save"), this);
	m_saveAction->setShortcut (QKeySequence::Save);
	m_saveAction->setStatusTip (tr("Save current file"));
	connect (m_saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	m_saveAsAction = new QAction (tr("Save &As"), this);
	m_saveAsAction->setShortcut (QKeySequence::SaveAs);
	m_saveAsAction->setStatusTip (tr("Save current file as"));
	connect (m_saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

	m_saveAllAction = new QAction (tr("Save A&ll"), this);
	m_saveAllAction->setStatusTip (tr("Save all files"));

	enableSaveFileActions (false);

	m_toggleProjectDock = new QAction (tr("Hide Project"), this);
	connect (m_toggleProjectDock, SIGNAL(triggered()), this, SLOT(toggleProjectDockVisibility()));
	m_toggleLibrariesDock = new QAction (tr("Hide Libraries"), this);
	connect (m_toggleLibrariesDock, SIGNAL(triggered()), this, SLOT(toggleLibrariesDockVisibility()));
	m_toggleToolTabs = new QAction (tr("Show Tool Tabs"), this);
	connect (m_toggleToolTabs, SIGNAL(triggered()), this, SLOT(toggleToolTabsVisibility()));
}

void MainWindow::enableSaveFileActions (bool enable) {
	m_saveAction->setEnabled (enable);
	m_saveAsAction->setEnabled (enable);
	m_saveAllAction->setEnabled (enable);
}

void MainWindow::createMenus () {
	m_fileMenu = menuBar ()->addMenu (tr("&File"));
	m_newMenu = m_fileMenu->addMenu (tr("&New"));
	m_newMenu->addAction (m_newProjectAction);
	m_newMenu->addAction (m_newFileAction);
	m_openMenu = m_fileMenu->addMenu (tr("&Open"));
	m_openMenu->addAction (m_openProjectAction);

	m_fileMenu->addAction (m_saveAction);
	m_fileMenu->addAction (m_saveAsAction);
	m_fileMenu->addAction (m_saveAllAction);

	m_windowMenu = menuBar ()->addMenu (tr("&Window"));
	m_windowMenu->addAction (m_toggleProjectDock);
	m_windowMenu->addAction (m_toggleLibrariesDock);
	m_windowMenu->addAction (m_toggleToolTabs);
}

void MainWindow::newProjectWizard () {
	m_projectWizard = new ProjectWizard (this);
	connect (m_projectWizard, &ProjectWizard::accept, this, &MainWindow::newProjectAccept);

	m_projectWizard->show ();
}

void MainWindow::newFile () {
	setupEditor (QFile ());
}

void MainWindow::newProjectAccept () {
	QJsonObject json;

	json["ProjectName"] = m_projectWizard->field ("ProjectName").toString ();

	json["ProjectLocation"] = m_projectWizard->field ("ProjectLocation").toString ();

	QStringList libs = m_projectWizard->m_setupPage->getLibPaths ();
	QJsonArray libpath;

	for (int i = 0; i < libs.count (); ++i) {
		QJsonValue s (libs.at (i));
		libpath.append (s);
	}

	json["LibPath"] = libpath;
	m_projectDocument.setObject (json);

	QByteArray bdata = m_projectDocument.toBinaryData ();
	QString fname = json["ProjectLocation"].toString () + "/.sprockets.prj";
	QFile doc (fname);
	doc.open (QIODevice::WriteOnly);
	doc.write (bdata);
	doc.close ();

	buildProjectTreeModel();

	m_projectWizard->close ();
}

void MainWindow::buildProjectTreeModel () {

	QString projectName = m_projectDocument.object ()["ProjectName"].toString ();
	m_projectTree->setWidget (new QLabel (projectName));
	QString projectLocation = m_projectDocument.object ()["ProjectLocation"].toString ();
	m_projectTree->setRootDir (projectLocation);

	QJsonArray projectLibraries = m_projectDocument.object ()["LibPath"].toArray ();
	for (int i = 0; i < projectLibraries.count (); ++i) {
		QString lib = projectLibraries.at (i).toString ();

		m_libraryTrees->addRootDir (lib);
	}

	m_projectTree->setVisible (true);
	m_libraryTrees->setVisible (true);
}

void MainWindow::openProject () {
	QDir home = QDir::home ();

	QString selection = QFileDialog::getExistingDirectory (this, tr("Open Directory"), home.absolutePath (), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	QDir dir (selection);
	if (!dir.exists ()) {
		qWarning ("Cannot find the selected directory");
		return;
	}

	if (!dir.exists (".sprockets.prj")) {
		qWarning ("Project doesn't exist");
		return;
	}

	QString fname = dir.filePath (".sprockets.prj");
	QFile f (fname);
	f.open (QIODevice::ReadOnly);
	QByteArray buf = f.readAll ();
	f.close ();

	m_projectDocument = QJsonDocument::fromBinaryData (buf);
	buildProjectTreeModel();
}

void MainWindow::setupEditor (QFileInfo info) {
	CodeEditor* editor = new CodeEditor;

	QString fileName = info.fileName () == "" ? "new" : info.fileName ();

	int index = m_fileTabs->addTab (editor, fileName);
	m_fileTabs->setCurrentIndex (index);


	QString filePath = info.filePath ();


	if (filePath != "") {
		QFile file (info.filePath ());
		if (file.open (QFile::ReadOnly | QFile::Text)) {
			QString txt = file.readAll();
			editor->setPlainText (txt);
			file.close ();
		}
	}

	m_fileTabs->tabBar ()->setTabData (index, filePath);
	enableSaveFileActions (true);

	connect (editor, SIGNAL(textChanged()), this, SLOT(textEdited()));
}

void MainWindow::textEdited () {
	int index = m_fileTabs->currentIndex ();
	CodeEditor* textEdit = dynamic_cast<CodeEditor*> (m_fileTabs->widget (index));
	textEdit->setWindowModified (true);

	QString txt = m_fileTabs->tabText (index);
	if (txt.right (1) != "*") {
		txt.append ("*");
		m_fileTabs->setTabText (index, txt);
	}
}

void MainWindow::closeFile (int index) {
	CodeEditor* textEdit = dynamic_cast<CodeEditor*> (m_fileTabs->widget (index));
	if (textEdit->isWindowModified ()) {
		QMessageBox msgBox;
		msgBox.setText ("The document has been modified.");
		msgBox.setInformativeText ("Save your changes?");
		msgBox.setStandardButtons (QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton (QMessageBox::Save);
		int ret = msgBox.exec ();

		if (ret == QMessageBox::Discard) {
			m_fileTabs->removeTab (index);
		} else if (ret == QMessageBox::Save) {
			saveFileAt (index);
		}
	} else {
		m_fileTabs->removeTab (index);
	}

	if (m_fileTabs->currentIndex () == -1) {
		enableSaveFileActions (false);
	}
}

void MainWindow::saveFile () {
	int index = m_fileTabs->currentIndex ();

	QString path = m_fileTabs->tabBar ()->tabData (index).toString ();

	if (path != "") {
		saveFileAt (index);
	} else {
		saveFileAs ();
	}
}

void MainWindow::saveFileAt (int index) {
	CodeEditor* textEdit = dynamic_cast<CodeEditor*> (m_fileTabs->widget (index));
	if (textEdit->isWindowModified ()) {
		QString path = m_fileTabs->tabBar ()->tabData (index).toString ();

		QFile file (path);
		if (file.open (QFile::WriteOnly | QFile::Text)) {
			QTextStream os (&file);
			os << textEdit->toPlainText ();
			file.close ();
			textEdit->setWindowModified (false);
			QString txt = m_fileTabs->tabText (index);
			txt.chop (1);
			m_fileTabs->setTabText (index, txt);
		}
	}
}

void MainWindow::saveFileAs () {
	int index = m_fileTabs->currentIndex ();
	QString path = m_fileTabs->tabBar ()->tabData (index).toString ();
	QString projectLocation = m_projectDocument.object ()["ProjectLocation"].toString ();
	QDir home = QDir::home ();

	QString fileName = QFileDialog::getSaveFileName (this, tr("Save File"), path == "" ? (projectLocation == "" ? home.absolutePath () : projectLocation) : path);

	m_fileTabs->tabBar ()->setTabData (index, fileName);
	QStringList sl = fileName.split (QDir::separator ());
	m_fileTabs->setTabText (index, sl.last () + "*");

	saveFileAt (index);

}

void MainWindow::openFile (QFileInfo fileInfo) {
	if (fileInfo.isFile () && fileInfo.isReadable ()) {
		setupEditor (fileInfo);
	}
}

void MainWindow::writeSettings () {
	QSettings settings ("SoftSprocket", "Sprockets");

	settings.beginGroup ("mainWindow");
	settings.setValue ("geometry", saveGeometry());
	settings.setValue ("state", saveState());
	settings.endGroup ();
}

void MainWindow::readSettings ()
{
	QSettings settings ("Softsprocket", "Sprockets");

	settings.beginGroup ("mainWindow");
	restoreGeometry(settings.value ("geometry").toByteArray());
	restoreState(settings.value ("state").toByteArray());
	settings.endGroup ();
}
void MainWindow::closeEvent (QCloseEvent *event) {
//	if (maybeSave ()) {
		writeSettings ();
		event->accept ();
//	} else {
//		event->ignore ();
//	}
}

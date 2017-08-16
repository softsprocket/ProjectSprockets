#include "scratchpad.h"

#include "snippetdialog.h"

#include <QSplitter>
#include <QBoxLayout>
#include <QStyle>
#include <libsprockets.h>
#include <QDebug>
#include <QJSValue>
#include <QJSEngine>
#include <QInputDialog>
#include <QMenu>

Scratchpad::Scratchpad (QWidget *parent) : QWidget (parent)  {
	QBoxLayout* layout = new QBoxLayout (QBoxLayout::Direction::TopToBottom);
	setLayout (layout);

	m_toolbar = new QToolBar (this);
	QIcon run (style ()->standardIcon (QStyle::SP_MediaPlay));

	m_runAction = new QAction (run, "Run", this);
	m_toolbar->addAction (m_runAction);


	QIcon clear (style ()->standardIcon (QStyle::SP_LineEditClearButton));
	m_clearAction = new QAction (clear, "Clear", this);
	m_toolbar->addAction (m_clearAction);

	QIcon addTo (style ()->standardIcon (QStyle::SP_ArrowUp));
	m_addToSnippetsAction = new QAction (addTo, "Add To Snippets", this);
	m_toolbar->addAction (m_addToSnippetsAction);

	QIcon addSelectedTo (style ()->standardIcon (QStyle::SP_DialogApplyButton));
	m_addSelectedToSnippetsAction = new QAction (addSelectedTo, "Add Selected To Snippets", this);
	m_toolbar->addAction (m_addSelectedToSnippetsAction);

	connect (m_runAction, SIGNAL(triggered()), this, SLOT(runCode()));
	connect (m_clearAction, SIGNAL(triggered()), this, SLOT(clearOutput()));
	connect (m_addToSnippetsAction, SIGNAL(triggered()), this, SLOT(addToSnippets()));
	connect (m_addSelectedToSnippetsAction, SIGNAL(triggered()), this, SLOT(addSelectedToSnippets()));

	layout->addWidget (m_toolbar);
	QSplitter* splitter = new QSplitter (Qt::Orientation::Vertical, this);

	m_codeArea = new QPlainTextEdit (this);
	QFont font;
	font.setFamily ("Courier");
	font.setFixedPitch (true);
	font.setPointSize (12);

	m_codeArea->setFont (font);
	m_highlighter = new JSHighlighter (m_codeArea->document ());

	splitter->addWidget (m_codeArea);

	m_outputArea = new QPlainTextEdit (this);
	m_outputArea->setFont (font);
	m_outputArea->setReadOnly (true);
	splitter->addWidget (m_outputArea);

	splitter->setStretchFactor (0, 3);
	splitter->setStretchFactor (1, 1);
	layout->addWidget (splitter);

	m_codeArea->setContextMenuPolicy (Qt::CustomContextMenu);

	connect(m_codeArea, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(showScratchPadContextMenu (const QPoint &)));

}

void Scratchpad::setEngine (QJSEngine *engine) {
	m_engine = engine;
}

void Scratchpad::createJSScratchPad () {
	m_jsScratchPad = new JSScratchPad (m_outputArea, m_editors, m_engine, this);
	QJSValue sp = m_engine->newQObject (m_jsScratchPad);
	m_engine->globalObject ().setProperty ("ScratchPad", sp);
}

void Scratchpad::setEditorTabs(QTabWidget *editors) {
	m_editors = editors;
}

void Scratchpad::runCode () {
	QString code = m_codeArea->toPlainText ();
	QJSValue val = m_engine->evaluate (code);

	m_outputArea->appendPlainText (val.toString ());
}

void Scratchpad::clearOutput () {
	m_outputArea->setPlainText ("");
}

void Scratchpad::addToSnippets () {
	SnippetDialog* dialog = new SnippetDialog (m_snippetTree, this);
	dialog->exec ();
}

void Scratchpad::addSelectedToSnippets () {

}

void Scratchpad::showScratchPadContextMenu (const QPoint &point) {
	QMenu contextMenu (tr("ScratchPad"), this);
	contextMenu.addAction (m_runAction);
	contextMenu.addAction (m_clearAction);
	contextMenu.addAction (m_addToSnippetsAction);
	contextMenu.addAction (m_addSelectedToSnippetsAction);

	contextMenu.exec (mapToGlobal (point));
}

JSScratchPad::JSScratchPad (QPlainTextEdit *outputArea, QTabWidget *editors, QJSEngine* engine, QObject *parent)
	:  QObject (parent), m_outputArea (outputArea), m_editors (editors), m_engine (engine) {

}

void JSScratchPad::out (QJSValue val) {
	m_outputArea->appendPlainText (val.toString ());
}

QString JSScratchPad::in (QString prompt) {
	return QInputDialog::getText (nullptr, tr("ScratchPad.in"), tr(prompt.toStdString ().c_str ()), QLineEdit::Normal);
}

QJSValue JSScratchPad::getEditorByTabText (QString fileName) {
	int numEds = m_editors->count ();
	QWidget* widget = nullptr;
	QString tabtext = "";

	for (int i = 0; i < m_editors->count (); ++i) {
		QString label = m_editors->tabText (i);
		if (label == fileName) {
			widget = m_editors->widget (i);
			tabtext = m_editors->tabText (i);
			break;
		}
	}

	return m_engine->newQObject (new JSEditor (static_cast<CodeEditor*> (widget), tabtext, this));
}

QJSValue JSScratchPad::getEditorByIndex (int num) {
	int numEds = m_editors->count ();
	QWidget* widget = nullptr;
	QString tabtext = "";

	if (num < numEds) {
			widget = m_editors->widget (num);
			tabtext = m_editors->tabText (num);
	}

	return m_engine->newQObject (new JSEditor (static_cast<CodeEditor*> (widget), tabtext, this));
}

QString JSEditor::getText () {
	return m_editor->toPlainText ();
}

QString JSEditor::getTabText () {
	return m_tabtext;
}

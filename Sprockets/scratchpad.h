#ifndef SCRATCHPAD_H
#define SCRATCHPAD_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QObject>
#include <QJSValue>
#include <QTabWidget>
#include <QString>

#include "jshighlighter.h"
#include "codeeditor.h"

class QJSEngine;
class JSScratchPad;
class SnippetTree;

class Scratchpad : public QWidget {
	Q_OBJECT
public:
	Scratchpad (QWidget* parent = 0);
	void setEngine (QJSEngine* engine);
	void setEditorTabs (QTabWidget* editors);
	void setSnippetTree (SnippetTree* tree) { m_snippetTree = tree; }
	void createJSScratchPad();

private:
	QPlainTextEdit* m_codeArea;
	QPlainTextEdit* m_outputArea;
	JSHighlighter* m_highlighter;
	QToolBar* m_toolbar;

	QAction* m_runAction;
	QAction* m_clearAction;
	QAction* m_addToSnippetsAction;
	QAction* m_addSelectedToSnippetsAction;

	QJSEngine* m_engine;
	JSScratchPad* m_jsScratchPad;
	QTabWidget* m_editors;
	SnippetTree* m_snippetTree;
private slots:
	void runCode ();
	void clearOutput ();
	void addToSnippets ();
	void addSelectedToSnippets ();
	void showScratchPadContextMenu (const QPoint & point);
};

class JSEditor : public QObject {
	Q_OBJECT
public:
	JSEditor (CodeEditor* editor, QString tabtext, QObject* parent = 0) : QObject (parent), m_editor (editor), m_tabtext (tabtext) {}
	Q_INVOKABLE QString getText ();
	Q_INVOKABLE QString getTabText ();
private:
	CodeEditor* m_editor;
	QString m_tabtext;
};


class JSScratchPad : public QObject {
	Q_OBJECT
public:
	JSScratchPad (QPlainTextEdit* outputArea, QTabWidget* editors, QJSEngine* engine, QObject* parent = 0);
	Q_INVOKABLE void out (QJSValue val);
	Q_INVOKABLE QString in (QString prompt = "Input:");
	Q_INVOKABLE QJSValue getEditorByTabText (QString fileName);
	Q_INVOKABLE QJSValue getEditorByIndex (int num);
private:
	QPlainTextEdit* m_outputArea;
	QTabWidget* m_editors;
	QJSEngine* m_engine;
};

#endif // SCRATCHPAD_H

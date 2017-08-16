#ifndef SNIPPETWIDGET_H
#define SNIPPETWIDGET_H

#include <QObject>
#include <QWidget>

class CodeEditor;
class SnippetTree;

class SnippetWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SnippetWidget(QWidget *parent = 0);

	SnippetTree* snippetTree () { return m_snippetTree; }
private:
	CodeEditor* m_editor;
	SnippetTree* m_snippetTree;
signals:

public slots:
};

#endif // SNIPPETWIDGET_H

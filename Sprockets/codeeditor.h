#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

class JSHighlighter;
class EditorLineNumbers;

class CodeEditor : public QPlainTextEdit {
	Q_OBJECT
public:
	CodeEditor (QWidget* parent = 0);

	void lineNumberAreaPaintEvent (QPaintEvent *event);
	int lineNumberAreaWidth ();
private:

	JSHighlighter* m_highlighter;
	EditorLineNumbers* m_lineNumbers;

protected:
	void resizeEvent (QResizeEvent *event) override;

private slots:
	void updateLineNumberAreaWidth (int newBlockCount);
	void highlightCurrentLine ();
	void updateLineNumberArea (const QRect& rect, int dy);
};

#endif // CODEEDITOR_H

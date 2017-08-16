#include "editorlinenumbers.h"

EditorLineNumbers::EditorLineNumbers(CodeEditor *editor) : QWidget (editor) {
	m_editor = editor;
}


QSize EditorLineNumbers::sizeHint () const {
	return QSize (m_editor->lineNumberAreaWidth (), 0);
}


void EditorLineNumbers::paintEvent (QPaintEvent *event) {
	m_editor->lineNumberAreaPaintEvent (event);
}

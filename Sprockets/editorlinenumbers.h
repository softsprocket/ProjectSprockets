#ifndef EDITORLINENUMBERS_H
#define EDITORLINENUMBERS_H

#include <QWidget>
#include "codeeditor.h"

class EditorLineNumbers : public QWidget {
	Q_OBJECT
public:
	EditorLineNumbers (CodeEditor* editor);

	QSize sizeHint () const override;

protected:
	void paintEvent (QPaintEvent *event) override;
private:
	CodeEditor* m_editor;

signals:

public slots:
};

#endif // EDITORLINENUMBERS_H

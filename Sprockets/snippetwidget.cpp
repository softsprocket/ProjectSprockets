#include "snippetwidget.h"
#include "codeeditor.h"
#include "snippettree.h"

#include <QSplitter>
#include <QBoxLayout>

SnippetWidget::SnippetWidget(QWidget* parent) : QWidget(parent) {
	QBoxLayout* layout = new QBoxLayout (QBoxLayout::Direction::TopToBottom);
	setLayout (layout);


	QSplitter* splitter = new QSplitter (Qt::Orientation::Horizontal, this);

	layout->addWidget (splitter);


	m_editor = new CodeEditor (this);
	m_snippetTree = new SnippetTree (this);

	splitter->addWidget (m_snippetTree);
	splitter->addWidget (m_editor);
}

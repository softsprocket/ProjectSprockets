#include "snippetdialog.h"

#include "snippettree.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

SnippetDialog::SnippetDialog (SnippetTree *snippetTree, QWidget *parent) : QDialog (parent), m_snippetTree (snippetTree) {
	QVBoxLayout* layout = new QVBoxLayout (this);
	setLayout (layout);

	layout->addWidget (m_snippetTree);

	QHBoxLayout* group = new QHBoxLayout (this);
	m_ok = new QPushButton ("OK", this);
	m_cancel = new QPushButton ("Cancel", this);
	group->addWidget (m_ok);
	group->addWidget (m_cancel);

	layout->addLayout(group);
}

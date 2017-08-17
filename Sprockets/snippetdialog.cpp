#include "snippetdialog.h"

#include "snippettree.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

SnippetDialog::SnippetDialog (SnippetTree *snippetTree, QWidget *parent) : QDialog (parent), m_snippetTree (snippetTree) {
	QVBoxLayout* layout = new QVBoxLayout (this);
	setLayout (layout);

	layout->addWidget (m_snippetTree);

	QHBoxLayout* tgroup = new QHBoxLayout (this);
	m_category = new QLineEdit (this);
	m_subcategory = new QLineEdit (this);
	QLabel* cat = new QLabel (tr("Category"), this);
	tgroup->addWidget (cat);
	tgroup->addWidget (m_category);

	QLabel* sub = new QLabel (tr("Subcategory"), this);
	tgroup->addWidget (sub);
	tgroup->addWidget (m_subcategory);

	layout->addLayout(tgroup);


	QHBoxLayout* bgroup = new QHBoxLayout (this);
	m_ok = new QPushButton (tr("OK"), this);
	m_cancel = new QPushButton (tr("Cancel"), this);
	bgroup->addWidget (m_ok);
	bgroup->addWidget (m_cancel);

	layout->addLayout(bgroup);

	setWindowTitle ("Add to ...");

	connect (m_snippetTree, SIGNAL(selection(SnippetTreeItem*)), this, SLOT(populateSelection(SnippetTreeItem*)));
}

void SnippetDialog::populateSelection (SnippetTreeItem *item) {
	SnippetTreeItemType type = item->getType ();

	if (type == SnippetTreeItemType::CATEGORY) {
		m_category->setText (item->getName ());
	} else {
		m_subcategory->setText (item->getName ());
	}
}

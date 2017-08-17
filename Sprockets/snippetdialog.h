#ifndef SNIPPETDIALOG_H
#define SNIPPETDIALOG_H

#include <QDialog>
#include <QObject>

class SnippetTree;
class QPushButton;
class QLineEdit;
class QEvent;
class SnippetTreeItem;

class SnippetDialog : public QDialog {
	Q_OBJECT
private:
	SnippetTree* m_snippetTree;
	QPushButton* m_ok;
	QPushButton* m_cancel;
	QLineEdit* m_category;
	QLineEdit* m_subcategory;

private slots:
	void populateSelection (SnippetTreeItem* item);
public:
        SnippetDialog (SnippetTree* snippetTree, QWidget* parent);
};

#endif // SNIPPETDIALOG_H

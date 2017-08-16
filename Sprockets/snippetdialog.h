#ifndef SNIPPETDIALOG_H
#define SNIPPETDIALOG_H

#include <QDialog>

class SnippetTree;
class QPushButton;

class SnippetDialog : public QDialog {
private:
	SnippetTree* m_snippetTree;
	QPushButton* m_ok;
	QPushButton* m_cancel;
public:
        SnippetDialog (SnippetTree* snippetTree, QWidget* parent);
};

#endif // SNIPPETDIALOG_H

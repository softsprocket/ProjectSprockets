#ifndef JSHIGHLIGHTER_H
#define JSHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QVector>

class JSHighlighter : public QSyntaxHighlighter {
public:
	JSHighlighter (QTextDocument *parent = 0);
protected:
	void highlightBlock (const QString &text) override;

private:
	struct HighlightingRule {
		QRegularExpression pattern;
		QTextCharFormat format;
	};

	QVector<HighlightingRule> m_highlightingRules;

	QRegularExpression m_commentStartExpression;
	QRegularExpression m_commentEndExpression;

	QTextCharFormat m_keywordFormat;
	QTextCharFormat m_singleLineCommentFormat;
	QTextCharFormat m_multiLineCommentFormat;
	QTextCharFormat m_singleQuotationFormat;
	QTextCharFormat m_doubleQuotationFormat;
};

#endif // JSHIGHLIGHTER_H

#include "jshighlighter.h"

JSHighlighter::JSHighlighter (QTextDocument* parent) : QSyntaxHighlighter (parent) {
	HighlightingRule rule;

	m_keywordFormat.setForeground (Qt::darkBlue);
	m_keywordFormat.setFontWeight (QFont::Bold);

	QStringList keywordPatterns;
	keywordPatterns  << "\\bbreak\\b"
		 << "\\bcase\\b"
		 << "\\bcatch\\b"
		 << "\\bclass\\b"
		 << "\\bconst\\b"
		 << "\\bcontinue\\b"
		 << "\\bdebugger\\b"
		 << "\\bdefault\\b"
		 << "\\bdelete\\b"
		 << "\\bdo\\b"
		 << "\\belse\\b"
		 << "\\bexport\\b"
		 << "\\bextends\\b"
		 << "\\bfinally\\b"
		 << "\\bfor\\b"
		 << "\\bfunction\\b"
		 << "\\bif\\b"
		 << "\\bimport\\b"
		 << "\\bin\\b"
		 << "\\binstanceof\\b"
		 << "\\bnew\\b"
		 << "\\breturn\\b"
		 << "\\bsuper\\b"
		 << "\\bswitch\\b"
		 << "\\bthis\\b"
		 << "\\bthrow\\b"
		 << "\\btry\\b"
		 << "\\btypeof\\b"
		 << "\\bvar\\b"
		 << "\\bvoid\\b"
		 << "\\bwhile\\b"
		 << "\\bwith\\b"
		 << "\\byield\\b";

	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegularExpression (pattern);
		rule.format = m_keywordFormat;
		m_highlightingRules.append (rule);
	}

	m_doubleQuotationFormat.setForeground (Qt::darkGreen);
	rule.pattern = QRegularExpression ("\".*\"");
	rule.format = m_doubleQuotationFormat;
	m_highlightingRules.append (rule);

	m_singleQuotationFormat.setForeground (Qt::darkGreen);
	rule.pattern = QRegularExpression ("'.*'");
	rule.format = m_singleQuotationFormat;
	m_highlightingRules.append (rule);

	m_singleLineCommentFormat.setForeground (Qt::red);
	rule.pattern = QRegularExpression ("//[^\n]*");
	rule.format = m_singleLineCommentFormat;
	m_highlightingRules.append (rule);

	m_multiLineCommentFormat.setForeground (Qt::red);

	m_commentStartExpression = QRegularExpression ("/\\*");
	m_commentEndExpression = QRegularExpression ("\\*/");
}

void JSHighlighter::highlightBlock (const QString &text) {

	foreach (const HighlightingRule &rule, m_highlightingRules) {
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch (text);

		while (matchIterator.hasNext ()) {
			QRegularExpressionMatch match = matchIterator.next ();
			setFormat (match.capturedStart (), match.capturedLength (), rule.format);
		}
	}

	setCurrentBlockState (0);

	int startIndex = 0;
	if (previousBlockState() != 1) {
		startIndex = text.indexOf (m_commentStartExpression);
	}

	while (startIndex >= 0) {
		QRegularExpressionMatch match = m_commentEndExpression.match (text, startIndex);

		int endIndex = match.capturedStart ();
		int commentLength = 0;

		if (endIndex == -1) {
			setCurrentBlockState (1);
			commentLength = text.length () - startIndex;
		} else {
			commentLength = endIndex - startIndex + match.capturedLength ();
		}

		setFormat (startIndex, commentLength, m_multiLineCommentFormat);
		startIndex = text.indexOf (m_commentStartExpression, startIndex + commentLength);
	}


}

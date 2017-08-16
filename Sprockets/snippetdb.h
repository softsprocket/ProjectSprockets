#ifndef SNIPPETDB_H
#define SNIPPETDB_H

#include <QDataStream>
#include <QString>

class Snippet {
public:
	Snippet (QString name, QString category, QString subcategory = "");
	Snippet (const Snippet & other);
	Snippet () {}
	~Snippet () {}

	void insertToStream (QDataStream & stream);
	void extractFromStream (QDataStream & stream);

	void setName (QString name) { m_name = name; }
	void setCategory (QString category) { m_category = category; }
	void setSubCategory (QString subcategory) { m_subcategory = subcategory; }
	void setCode (QString code) { m_code = code; }
	QString getCode () const { return m_code; }
	QString getCategory () const { return m_category; }
	QString getSubCategory () const { return m_subcategory; }
	QString getName () const { return m_name; }

private:
	QString m_name;
	QString m_category;
	QString m_subcategory;
	QString m_code;
};

Q_DECLARE_METATYPE (Snippet)

class SnippetDB {
public:
	SnippetDB (QString path);
private:
	QString m_path;
	QDataStream m_dataStream;
};

#endif // SNIPPETDB_H

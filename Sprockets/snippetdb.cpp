#include "snippetdb.h"

SnippetDB::SnippetDB (QString path) {

}

Snippet::Snippet (QString name, QString category, QString subcategory)
    : m_name (name), m_category (category), m_subcategory (subcategory) {

}

Snippet::Snippet (const Snippet & other)
		: m_name (other.getName ()), m_category (other.getCategory ()),
		m_subcategory (other.getCategory ()), m_code (other.getSubCategory ()) {
}

void Snippet::insertToStream (QDataStream &stream) {
	stream << m_name << m_category << m_subcategory;
	stream << (m_code.isEmpty () ? "" : m_code);
}

void Snippet::extractFromStream (QDataStream &stream) {
	stream >> m_name;
	stream >> m_category;
	stream >> m_subcategory;
	stream >> m_code;
}


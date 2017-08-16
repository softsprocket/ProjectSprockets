#include "libsprockets.h"

#include <QJSEngine>
#include <QDir>
#include <QFile>
#include <QIODevice>

LibSprockets::LibSprockets () {
	m_engine = new QJSEngine;
	setFSObjects ();
	setOSObject ();
}

LibSprockets::~LibSprockets () {
	delete m_engine;
}

QJSEngine *LibSprockets::getEngine () {
	return m_engine;
}

void LibSprockets::setFSObjects () {
	QJSValue factoryObj = m_engine->newQObject (new FSFactory (m_engine));
	m_engine->globalObject ().setProperty ("_fsFactory", factoryObj);
	m_engine->evaluate (
		"function Dir (path) {"
		"    return _fsFactory.createDirInstance (path)"
		"}"
	);

	m_engine->evaluate (
		"function File (path) {"
		"    return _fsFactory.createFileInstance (path)"
		"}"
				);
}

void LibSprockets::setOSObject () {
	QJSValue osObj = m_engine->newQObject (new OS ());
	m_engine->globalObject ().setProperty ("OS", osObj);
}


QJSValue FSFactory::createDirInstance (QString path) {
	return m_engine->newQObject(new Dir (path));
}


QJSValue FSFactory::createFileInstance (QString path) {
	return m_engine->newQObject(new File (path));
}


Dir::Dir (QString path) : QObject (), m_path (path), m_dir (path) {

}

QString Dir::absolutePath () const {
	return m_dir.absolutePath ();
}

QString Dir::canonicalPath () const {
	return m_dir.canonicalPath ();
}

QStringList Dir::entryList () {
	return m_dir.entryList ();
}

bool Dir::cd (QString& path) {
	return m_dir.cd (path);
}

void Dir::refresh () {
	return m_dir.refresh ();
}

bool Dir::exists () {
	return m_dir.exists ();
}

bool Dir::exists (QString& path) {
	return m_dir.exists (path);
}


File::File (QString path) : QObject (), m_path (path), m_file (path) {

}

bool File::exists() {
	return m_file.exists ();
}

bool File::copy (const QString &newName) {
	return m_file.copy (newName);
}

bool File::open () {
	return m_file.open (QIODevice::ReadWrite);
}

QString File::readAll () {
	QByteArray buf = m_file.readAll ();

	return QString (buf.toStdString ().c_str ());
}

QString OS::cwd () {
	return QDir::current ().absolutePath ();
}

QChar OS::seperator () {
	return QDir::separator ();
}

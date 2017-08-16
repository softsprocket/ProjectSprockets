#ifndef LIBSPROCKETS_H
#define LIBSPROCKETS_H

#include "libsprockets_global.h"

#ifndef QOBJECT_HEADER_INCLUDED_
#define QOBJECT_HEADER_INCLUDED_
#include <QObject>
#endif // QOBJECT_HEADER_INCLUDED_

#ifndef QJSVALUE_HEADER_INCLUDED_
#define QJSVALUE_HEADER_INCLUDED_
#include <QJSValue>
#endif // QJSVALUE_HEADER_INCLUDED_

#ifndef QDIR_HEADER_INCLUDED_
#define QDIR_HEADER_INCLUDED_
#include <QDir>
#endif // QDIR_HEADER_INCLUDED_

#ifndef QFILE_HEADER_INCLUDED_
#define QFILE_HEADER_INCLUDED_
#include <QFile>
#endif // QFILE_HEADER_INCLUDED_

#ifndef QSTRING_HEADER_INCLUDED_
#define QSTRING_HEADER_INCLUDED_
#include <QString>
#endif // QSTRING_HEADER_INCLUDED_

#ifndef QSTRINGLIST_HEADER_INCLUDED_
#define QSTRINGLIST_HEADER_INCLUDED_
#include <QStringList>
#endif // QSTRINGLIST_HEADER_INCLUDED_

class QJSEngine;

class LIBSPROCKETSSHARED_EXPORT LibSprockets {
private:
	QJSEngine* m_engine;

	void setFSObjects ();
	void setOSObject ();

public:
	LibSprockets ();
	~LibSprockets ();

	QJSEngine* getEngine ();
};


class FSFactory : public QObject {
	Q_OBJECT
public:
	FSFactory(QJSEngine* engine) : m_engine (engine) {}
	Q_INVOKABLE QJSValue createDirInstance (QString path);
	Q_INVOKABLE QJSValue createFileInstance (QString path);
private:
	QJSEngine* m_engine;
};

class Dir : public QObject {
	Q_OBJECT
public:
	Dir (QString path);
	Q_INVOKABLE QString	absolutePath () const;
	Q_INVOKABLE QString	canonicalPath () const;
	Q_INVOKABLE QStringList entryList ();


	Q_INVOKABLE bool cd (QString &path);
	Q_INVOKABLE void refresh ();
	Q_INVOKABLE bool exists ();
	Q_INVOKABLE bool exists (QString& path);
private:
	QString m_path;
	QDir m_dir;
};


class File : public QObject {
	Q_OBJECT
public:
	File (QString path);
	Q_INVOKABLE bool exists ();
	Q_INVOKABLE bool copy (const QString& newName);
	Q_INVOKABLE bool open ();
	Q_INVOKABLE QString readAll ();
private:
	QString m_path;
	QFile m_file;
};

class OS : public QObject {
	Q_OBJECT
public:
	OS () : QObject () {}

	Q_INVOKABLE QString cwd ();
	Q_INVOKABLE QChar seperator ();
};

#endif // LIBSPROCKETS_H

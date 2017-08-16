#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QStringList>

#include "mainwindow.h"

class IntroPage : public QWizardPage {
	Q_OBJECT
public:
	IntroPage (QWidget* parent = 0);
};

class SetupPage : public QWizardPage {
	Q_OBJECT
public:
	SetupPage (QWidget* parent = 0);

	QStringList getLibPaths ();
private:
	QLineEdit* m_projectNameLineEdit;
	QLineEdit* m_projectLocation;
	QLineEdit* m_libraryLocation;
	QListWidget* m_libraryLocations;

private slots:
	void openLocationDialog ();
	void openLibraryLocationDialog ();
	void addLibraryLocation ();
	void showContextMenu (const QPoint&);
	void removeItem ();
};

class ProjectWizard : public QWizard {
	Q_OBJECT
public:
	ProjectWizard (QWidget* parent = 0);

	QJsonDocument getDocument ();
signals:
	void accept ();
private:
	SetupPage* m_setupPage;

	friend class MainWindow;

};

#endif // PROJECTWIZARD_H


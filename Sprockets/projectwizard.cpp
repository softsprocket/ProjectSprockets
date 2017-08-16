#include "projectwizard.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialog>
#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QListWidgetItem>
/*#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
*/
IntroPage::IntroPage (QWidget* parent) : QWizardPage (parent) {
	setTitle (tr("Introduction"));

	QLabel* label = new QLabel (tr("This wizard will generate a JSON Configuration file for Sprockets Javascript Project."));

	label->setWordWrap (true);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget (label);
	setLayout (layout);

}

SetupPage::SetupPage (QWidget* parent) : QWizardPage (parent) {
	setTitle (tr("Project Setup"));

	m_projectNameLineEdit = new QLineEdit;

	QFormLayout* layout = new QFormLayout;
	layout->addRow ("Project Name", m_projectNameLineEdit);

	m_projectLocation = new QLineEdit;
	QPushButton* locationFileSelector = new QPushButton ("...");
	connect (locationFileSelector, SIGNAL(clicked()), SLOT(openLocationDialog()));
	QHBoxLayout* loc = new QHBoxLayout;
	loc->addWidget (m_projectLocation);
	loc->addWidget (locationFileSelector);
	layout->addRow ("Project Location", loc);

	m_libraryLocation = new QLineEdit;
	QPushButton* libraryLocationFileSelector = new QPushButton ("...");
	connect (libraryLocationFileSelector, SIGNAL(clicked()), SLOT(openLibraryLocationDialog()));
	QHBoxLayout* libloc = new QHBoxLayout;
	libloc->addWidget (m_libraryLocation);
	libloc->addWidget (libraryLocationFileSelector);
	QPushButton* addLocationButton = new QPushButton ("Add");
	connect (addLocationButton, SIGNAL(clicked()), SLOT(addLibraryLocation()));
	libloc->addWidget (addLocationButton);
	layout->addRow ("Library Paths", libloc);

	m_libraryLocations = new QListWidget;
	m_libraryLocations->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_libraryLocations, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

	layout->addRow (m_libraryLocations);

	setLayout (layout);

	registerField ("ProjectName*", m_projectNameLineEdit);
	registerField ("ProjectLocation*", m_projectLocation);
	registerField ("LibraryLocations", m_libraryLocations);

}

void SetupPage::showContextMenu (const QPoint &pos)
{
	QPoint globalPos = m_libraryLocations->mapToGlobal (pos);

	QMenu menu;
	menu.addAction ("Remove",  this, SLOT(removeItem()));

	menu.exec (globalPos);
}

void SetupPage::removeItem ()
{
	for (int i = 0; i < m_libraryLocations->selectedItems ().size (); ++i) {
		QListWidgetItem *item = m_libraryLocations->takeItem (m_libraryLocations->currentRow ());
		delete item;
	}
}

void SetupPage::addLibraryLocation () {
	QString val = m_libraryLocation->text ();
	if (val.trimmed () == "") { return; }

	new QListWidgetItem (val, m_libraryLocations);
}

void SetupPage::openLibraryLocationDialog () {
	QString selection = QFileDialog::getExistingDirectory (this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	QDir dir (selection);
	if (!dir.exists ()) {
		qWarning ("Cannot find the example directory");
	}

	m_libraryLocation->setText (selection);
}

void SetupPage::openLocationDialog () {
	QString selection = QFileDialog::getExistingDirectory (this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	QDir dir (selection);
	if (!dir.exists ()) {
		qWarning ("Cannot find the example directory");
	}

	m_projectLocation->setText (selection);
}

QStringList SetupPage::getLibPaths () {
	int c = m_libraryLocations->count ();
	QStringList rv;
	for (int i = 0; i < c; ++i) {
		rv.append (m_libraryLocations->item (i)->text ());
	}

	return rv;
}

ProjectWizard::ProjectWizard (QWidget* parent) : QWizard (parent) {
	addPage (new IntroPage);
	m_setupPage = new SetupPage;
	addPage (m_setupPage);

	setWindowTitle (tr("New Project"));
}
/*
void ProjectWizard::accept () {
	QJsonObject json;

	json["ProjectName"] = field ("ProjectName").toString ();
	json["ProjectLocation"] = field ("ProjectLocation").toString ();

	QStringList libs = m_setupPage->getLibPaths ();
	QJsonArray libpath;

	for (int i = 0; i < libs.count (); ++i) {
		QJsonValue s (libs.at (i));
		libpath.append (s);
	}

	json["LibPath"] = libpath;
	m_document.setObject (json);


	QDialog::accept ();
}
*/

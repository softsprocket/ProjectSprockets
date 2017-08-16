#include "tooltabs.h"

#include <QDebug>
#include <QResizeEvent>
#include <QMoveEvent>

ToolTabs::ToolTabs () : QWidget (nullptr) {}

void ToolTabs::resizeEvent(QResizeEvent * event) {
	qDebug () << event->size ();
	qDebug () << event->oldSize ();
}

void ToolTabs::moveEvent(QMoveEvent * event) {
	qDebug () << event->pos ();
	qDebug () << event->oldPos ();
}

void ToolTabs::closeEvent (QCloseEvent *event) {
	emit close ();
}

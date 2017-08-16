#ifndef TOOLTABS_H
#define TOOLTABS_H

#include <QWidget>
#include <QObject>


class ToolTabs : public QWidget {
	Q_OBJECT
protected:
	void resizeEvent(QResizeEvent * event);
	void moveEvent(QMoveEvent * event);
	void closeEvent (QCloseEvent * event);
public:
	ToolTabs ();
signals:
	void close ();
};

#endif // TOOLTABS_H

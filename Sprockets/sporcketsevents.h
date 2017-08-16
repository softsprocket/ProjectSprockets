#ifndef SPORCKETSEVENTS_H
#define SPORCKETSEVENTS_H

#include <QEvent>

#define OPEN_FILE_TYPE 2000

class OpenFileEvent : public QEvent {
public:
	static const QEvent::Type type = static_cast<QEvent::Type> (QEvent::registerEventType ());

	OpenFileEvent (QString path);
};

#endif // SPORCKETSEVENTS_H

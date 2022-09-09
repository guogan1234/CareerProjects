#ifndef COMMUNICATETHREAD_H
#define COMMUNICATETHREAD_H

#include <QObject>
#include <QThread>

class CommunicateThread : public QThread
{
        Q_OBJECT
    public:
        explicit CommunicateThread(QObject *parent = 0);

        void run();
    signals:

    public slots:
};

#endif // COMMUNICATETHREAD_H

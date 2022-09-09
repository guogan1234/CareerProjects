#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QHash>
#include <QMutex>
#include "workthread.h"

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = nullptr);

    int addThread();
    int delThread();
signals:

public slots:

private:
    QHash<QString,WorkThread*> * threadHash;
    QMutex* mutex;

    int takeAddThread();
    int takeDelThread();
};

#endif // THREADMANAGER_H

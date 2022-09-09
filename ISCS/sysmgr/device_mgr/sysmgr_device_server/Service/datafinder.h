#ifndef DATAFINDER_H
#define DATAFINDER_H

#include <QObject>
#include <QMutex>
#include <QReadWriteLock>
#include <QHash>
#include <QMap>
#include "Data/devinforequest.h"
#include "Data/fulldevinfo.h"

class DataFinder : public QObject
{
        Q_OBJECT
    public:
        explicit DataFinder(QObject *parent = 0);

        void setRawInfoList(QList<QByteArray> * list,QMutex* mutex);
        void setRawInfoHash(QHash<QString,QByteArray> * hash);
        void setNetInfoMap(QMap<QString,NetInfo*> * map,QReadWriteLock* lock);
        FullDevInfo* getMatchedData(DevInfoRequest* request);
    signals:

    public slots:

    private:
        QList<QByteArray> * rawInfoList;
        QMutex* rawListMetux;
        QHash<QString,QByteArray> * rawInfoHash;
        QMap<QString,NetInfo*> * netInfoMap;
        QReadWriteLock* netMapLock;
};

#endif // DATAFINDER_H

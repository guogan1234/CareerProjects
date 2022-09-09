#ifndef PROSYSTEMMANAGER_H
#define PROSYSTEMMANAGER_H

#include <QObject>
#include "prosystemdata.h"

class ProSystemManager : public QObject
{
        Q_OBJECT
    public:
        explicit ProSystemManager(QObject *parent = 0);

        QList<ProSystem*> * loadDataByCFDB(QString sql);
    signals:

    public slots:

    private:
        ProSystemData* data;

        void init();
        void initParam();
};

#endif // PROSYSTEMMANAGER_H

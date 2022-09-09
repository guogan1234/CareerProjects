#ifndef DEVMANAGER_H
#define DEVMANAGER_H

#include <QObject>
#include "devdata.h"

class DevManager : public QObject
{
        Q_OBJECT
    public:
        explicit DevManager(QObject *parent = 0);

        QList<Dev*> * loadDataByCFDB(QString sql);
    signals:

    public slots:

    private:
        DevData* data;

        void init();
        void initParam();
};

#endif // DEVMANAGER_H

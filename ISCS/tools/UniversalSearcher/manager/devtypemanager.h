#ifndef DEVTYPEMANAGER_H
#define DEVTYPEMANAGER_H

#include <QObject>
#include "devtypedata.h"

class DevTypeManager : public QObject
{
        Q_OBJECT
    public:
        explicit DevTypeManager(QObject *parent = 0);

        QList<DevType*> * loadDataByCFDB(QString sql);
    signals:

    public slots:

    private:
        DevTypeData* data;

        void init();
        void initParam();
};

#endif // DEVTYPEMANAGER_H

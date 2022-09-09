#ifndef DEVINFOMANAGER_H
#define DEVINFOMANAGER_H

#include <QObject>
#include "devinfodataferry.h"

class DevInfoManager : public QObject
{
        Q_OBJECT
    public:
        explicit DevInfoManager(QObject *parent = 0);

        int addDevInfo(DevInfo* info);
        DevInfo* getDevInfo();
    signals:

    public slots:

    private:
        void init();
        void initParam();

        DevInfoDataFerry* ferry;

        int takeAddDevInfo(DevInfo *info);
        DevInfo* takeGetDevInfo();
};

#endif // DEVINFOMANAGER_H

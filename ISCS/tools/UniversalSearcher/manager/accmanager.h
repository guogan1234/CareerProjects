#ifndef ACCMANAGER_H
#define ACCMANAGER_H

#include <QObject>
#include "accdata.h"

class AccManager : public QObject
{
        Q_OBJECT
    public:
        explicit AccManager(QObject *parent = 0);

        QList<Acc*> * loadDataByCFDB(QString sql);
        char** getRecordHeaderByCFDB();
        int getRecordHeaderSizeByCFDB();
    signals:

    public slots:

    private:
        AccData* data;

        void init();
        void initParam();
};

#endif // ACCMANAGER_H

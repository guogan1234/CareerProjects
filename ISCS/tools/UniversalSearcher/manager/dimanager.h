#ifndef DIMANAGER_H
#define DIMANAGER_H

#include <QObject>
#include "didata.h"

class DiManager : public QObject
{
        Q_OBJECT
    public:
        explicit DiManager(QObject *parent = 0);

        QList<Di*> * loadDataByCFDB(QString sql);
        char** getRecordHeaderByCFDB();
        int getRecordHeaderSizeByCFDB();
    signals:

    public slots:

    private:
        DiData* data;

        void init();
        void initParam();
};

#endif // DIMANAGER_H

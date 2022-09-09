#ifndef AIMANAGER_H
#define AIMANAGER_H

#include <QObject>
#include "aidata.h"

class AiManager : public QObject
{
        Q_OBJECT
    public:
        explicit AiManager(QObject *parent = 0);

        QList<Ai*> * loadDataByCFDB(QString sql);
        char** getRecordHeaderByCFDB();
        int getRecordHeaderSizeByCFDB();
    signals:

    public slots:

    private:
        AiData* data;

        void init();
        void initParam();
};

#endif // AIMANAGER_H

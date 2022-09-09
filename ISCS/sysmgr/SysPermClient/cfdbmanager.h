#ifndef CFDBMANAGER_H
#define CFDBMANAGER_H

#include <QObject>

class CFDBManager : public QObject
{
        Q_OBJECT
    public:
        explicit CFDBManager(QObject *parent = 0);

        int startCFDBModule();
        int endCFDBModule();

    signals:

    public slots:
};

#endif // CFDBMANAGER_H

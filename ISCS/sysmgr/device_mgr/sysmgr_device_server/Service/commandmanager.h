#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QReadWriteLock>

class CommandManager : public QObject
{
        Q_OBJECT
    public:
        explicit CommandManager(QObject *parent = 0);

        QByteArray getCommand();
        int addCommand(QByteArray bArray);
    signals:

    public slots:

    private:
        QList<QByteArray> * commandList;
        QReadWriteLock* lock;

        void init();
        void initParam();
};

#endif // COMMANDMANAGER_H

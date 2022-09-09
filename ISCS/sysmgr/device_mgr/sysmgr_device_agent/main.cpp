#include <QCoreApplication>
#include "servers/appserverworkerimpl.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    AppServerWorker* appWorker = new AppServerWorkerImpl();
    appWorker->start();

    return a.exec();
}

#include <QCoreApplication>
#include "infoserverworker.h"
#include "infoserverworkerimpl.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    InfoServerWorker* appWorker = new InfoServerWorkerImpl();
    appWorker->start();

    return a.exec();
}

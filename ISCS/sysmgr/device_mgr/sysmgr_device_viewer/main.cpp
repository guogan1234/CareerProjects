#include "widget.h"
#include <QApplication>
#include "Servers/confserverworkerimpl.h"
#include "Servers/uiserverworkerimpl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfServerWorker* confServer = new ConfServerWorkerImpl();
    //获取本地域及域所管理的节点信息
    QHash<int,DomainNodes*> * startConf = confServer->getStartConf();
    //获取本界面工具要连接的服务器连接信息
    QList<ConnClient*> * connServer = confServer->getConnServer();

    UIServerWorker* UIServer = new UIServerWorkerImpl();
    UIServer->start(startConf,connServer);

//    CommunicateServerWorker* communicateServer = new CommunicateServerWorkerImpl();
//    communicateServer->setDevInfo(UIServer->recvDevInfo);
//    communicateServer->start();

//    Widget w(arg_1);
//    w.showMaximized();
//    w.showFullScreen();

    return a.exec();
}

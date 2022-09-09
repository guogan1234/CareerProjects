#include "networkerimpl.h"

NetWorkerImpl::NetWorkerImpl(QObject *parent) : QObject(parent)
{
    init();
}

int NetWorkerImpl::collectNetInfo(QList<ConnClient *> *config, qint64 timestamp)
{
    return takeCollectNetInfo(config,timestamp);
}

void NetWorkerImpl::setConfig(QList<ConnClient *> *config)
{
    takeSetConfig(config);
}

void NetWorkerImpl::setManager(NetInfoManager *manager)
{
    takeSetManager(manager);
}

void NetWorkerImpl::dataTimeoutSlot(qint64 timestamp)
{
    takeCollectNetInfo(connConfigs,timestamp);
}

void NetWorkerImpl::init()
{
    initParam();
}

void NetWorkerImpl::initParam()
{
#ifdef Q_OS_WIN
    process = new QProcess();
#endif
}

void NetWorkerImpl::takeSetConfig(QList<ConnClient *> *config)
{
    connConfigs = config;
}

void NetWorkerImpl::takeSetManager(NetInfoManager *manager)
{
    netInfoManager = manager;
}

int NetWorkerImpl::takeCollectNetInfo(QList<ConnClient *> *config, qint64 timestamp)
{
    qDebug("takeCollectNetInfo...\n");
    if(!config || !netInfoManager) return -1;
    foreach (ConnClient* temp, *config) {
        QList<QString> * ips = temp->ips;
//        bool bConn = false;

        NetInfo* data = new NetInfo();
        data->name = temp->name;
        data->netConn = new QList<NetConn*>();
        foreach (QString ip, *ips) {
            QByteArray ba = ip.toUtf8();
            if(!ba.isNull()){
                char* connIp = ba.data();
                int ret = -1;
#ifdef Q_OS_LINUX
                ret = isConnectedLinux(connIp);
#elif defined(Q_OS_WIN)
                ret = isConnectedWin(connIp);
#endif
                NetConn* netConn = new NetConn();
                netConn->ip = ip;
                netConn->bConn = false;
                if(0 == ret){
//                    bConn = true;
//                    break;
                    netConn->bConn = true;
                }
                data->netConn->append(netConn);
            }
        }

//        NetInfo* data = new NetInfo();
//        data->name = temp->name;
//        data->bConn = bConn;

        netInfoManager->addNetInfo(timestamp,data);
    }
    return 0;
}

#ifdef Q_OS_LINUX
//linux下使用system系统调用测试网络通断状态
int NetWorkerImpl::isConnectedLinux(const char *ip)
{
    if(NULL == ip) return -1;

    char buf[80] = {0};
    strcpy(buf,"ping -c 2 ");
    strcat(buf,ip);
    int ret = system(buf);

    if (-1 == ret)
    {
        qDebug("system error!");
        return ret;
    }
    else
    {
        qDebug("exit value = [0x%x]\n", ret);

        if (WIFEXITED(ret))
        {
            if (0 == WEXITSTATUS(ret))
            {
                qDebug("run shell script successfully.\n");
                return 0;
            }
            else
            {
                qDebug("run shell script fail, script exit code: %d\n", WEXITSTATUS(ret));
                return -1;
            }
        }
        else
        {
            qDebug("exit status = [%d]\n", WEXITSTATUS(ret));
            return -1;
        }
    }
    return -1;
}
#endif

#ifdef Q_OS_WIN
//windows下使用QProcess测试网络通断状态
int NetWorkerImpl::isConnectedWin(const char *ip)
{
    if(!ip) return -1;

    if(!process) return -1;
    qDebug("isConnectedWin...");
    char buf[80] = {0};
    strcpy(buf,"ping -n 2 ");
    strcat(buf,ip);

    process->start(buf);
    bool bFinished = process->waitForFinished(-1);
    if(!bFinished){
        return -1;
    }else {
        if((process->exitStatus() == QProcess::NormalExit) && (process->exitCode() == 0)){
            return 0;
        }
        return -1;
    }

    return -1;
}
#endif

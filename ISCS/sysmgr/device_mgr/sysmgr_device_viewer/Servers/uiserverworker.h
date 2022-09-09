#ifndef UISERVERWORKER_H
#define UISERVERWORKER_H

#include <QObject>
#include "Data/sysconfout.h"
#include "nodemanager.h"
#include "ui/nodestreemodel.h"
#include "ui/processtablemodel.h"
#include "ui/cputablemodel.h"
#include "ui/memorytablemodel.h"
#include "ui/disktablemodel.h"
#include "ui/nettablemodel.h"
#include "widget.h"
#include "Servers/communicateserverworkerimpl.h"

class UIServerWorker : public QObject
{
        Q_OBJECT
    public:
        explicit UIServerWorker(QObject *parent = 0);

        virtual void start(QHash<int,DomainNodes*> * confInfo,QList<ConnClient*> * connServer) = 0;
    signals:
        void uiTreeItemClick(QStandardItem* item);
    public slots:

    protected:
        NodeManager* confManager;
        Widget* mainUI;
        NodesTreeModel* uiTreeModel;
        ProcessTableModel* processModel;
        CpuTableModel* cpuModel;
        MemoryTableModel* memoryModel;
        DiskTableModel* diskModel;
        NetTableModel* netModel;

        DevInfo* recvDevInfo;
        FullDevInfo* fullDevInfo;
        CommunicateServerWorkerImpl* communicateServer;

    private:
        virtual void setConfManager(QHash<int,DomainNodes*> * confInfo) = 0;
        virtual void showUI(QList<ConnClient*> * connInfo) = 0;
        virtual void startCommunicate(QList<ConnClient*> * connServer) = 0;
};

#endif // UISERVERWORKER_H

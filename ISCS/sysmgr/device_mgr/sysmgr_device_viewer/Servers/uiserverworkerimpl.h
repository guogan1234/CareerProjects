#ifndef UISERVERWORKERIMPL_H
#define UISERVERWORKERIMPL_H

#include <QObject>
#include "uiserverworker.h"
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

using namespace QtCharts;

class UIServerWorkerImpl : public UIServerWorker
{
        Q_OBJECT
    public:
        explicit UIServerWorkerImpl(QObject *parent = 0);

        void start(QHash<int,DomainNodes*> * startConf,QList<ConnClient*> * connServer);
    signals:

    public slots:
        void gettedDevInfoSlot();
    private:
        void init();
        void initParam();
        void initCharts();
        void initMemoryChart();
        void initCpuTotalChart();
        void initCpuCharts(int size);
        void clearCpuCharts();

        void setConfManager(QHash<int,DomainNodes*> * startConf);
        void showUI(QList<ConnClient*> * connInfo);
        void startCommunicate(QList<ConnClient*> * connServer);
        void showDevNodes();

        void updateProcessModel(QList<Process *> *list);
        void updateCpuModel(QList<Cpu*> * list);
        void updateMemoryModel(Memory* memory);
        void updateDiskModel(QList<Disk*> * list);
        void updateNetModel(NetInfo* net);

        void updateMemoryChart(qint64 time,Memory* memory);
        void updateCpuTotalChart(qint64 time,Cpu* cpu);
        void updateCpuChart(qint64 time,Cpu* cpu,int index);
        void updateCpuCharts(qint64 time,QList<Cpu*> * cpuList);

        QLineSeries* memorySeries;
        QChart* memoryChart;

        QLineSeries* cpuTotalSeries;
        QChart* cpuTotalChart;
        QList<QLineSeries*> * cpuSeries;
        QList<QChart*> * cpuCharts;

        void fillCpuSeries(QList<QLineSeries*> * series);

};

#endif // UISERVERWORKERIMPL_H

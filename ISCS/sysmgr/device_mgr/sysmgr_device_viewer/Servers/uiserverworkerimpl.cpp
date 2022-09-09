#include "uiserverworkerimpl.h"
#include <QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QCoreApplication>
#include <QTime>

UIServerWorkerImpl::UIServerWorkerImpl(QObject *parent) : UIServerWorker(parent)
{
    init();
}

void UIServerWorkerImpl::start(QHash<int, DomainNodes *> *startConf, QList<ConnClient *> *connServer)
{
    setConfManager(startConf);
    showUI(connServer);
    startCommunicate(connServer);
}

void UIServerWorkerImpl::gettedDevInfoSlot()
{
    QList<Process*> * processList = NULL;
    QList<Cpu*> * cpuList = NULL;
    Memory* memory = NULL;
    QList<Disk*> * diskList = NULL;
    if(fullDevInfo){
//        processList = recvDevInfo->procList;
//        updateProcessModel(processList);
        DevInfo* devInfo = fullDevInfo->devInfo;
        if(devInfo){
            qint64 time = devInfo->timestamp;
//            QDateTime date = QDateTime::fromMSecsSinceEpoch(time);

            processList = devInfo->procList;
            updateProcessModel(processList);

            cpuList = devInfo->cpuList;
            updateCpuModel(cpuList);
            updateCpuCharts(time,cpuList);

            memory = devInfo->memory;
            updateMemoryModel(memory);
            updateMemoryChart(time,memory);

            diskList = devInfo->diskList;
            updateDiskModel(diskList);
        }
        NetInfo* netInfo = fullDevInfo->netInfo;
        if(netInfo){
            updateNetModel(netInfo);
        }
    }

//    QCoreApplication::processEvents(QEventLoop::AllEvents,500);
}

void UIServerWorkerImpl::init()
{
    initParam();
    initCharts();
}

void UIServerWorkerImpl::initParam()
{
    confManager = new NodeManager(this);
    mainUI = new Widget();
//    connect(mainUI,SIGNAL(treeItemClick(QStandardItem*)),this,SIGNAL(uiTreeItemClick(QStandardItem*)));
    uiTreeModel = new NodesTreeModel();
    processModel = new ProcessTableModel();
    cpuModel = new CpuTableModel();
    memoryModel = new MemoryTableModel();
    diskModel = new DiskTableModel();
    netModel = new NetTableModel();
    recvDevInfo = new DevInfo();
    fullDevInfo = new FullDevInfo();
    communicateServer = new CommunicateServerWorkerImpl();
    communicateServer->setDevInfo(recvDevInfo);
    communicateServer->setFullDevInfo(fullDevInfo);
    connect(communicateServer,SIGNAL(gettedDevInfoSignal()),this,SLOT(gettedDevInfoSlot()));

    cpuSeries = NULL;
    cpuCharts = NULL;
}

void UIServerWorkerImpl::initCharts()
{
    initMemoryChart();
    initCpuTotalChart();
}

void UIServerWorkerImpl::initMemoryChart()
{
    memorySeries = new QLineSeries(this);
    memorySeries->setPointsVisible(true);
    memorySeries->setPointLabelsFormat("(@yPoint)");
//    memorySeries->setPointLabelsVisible(true);
    memoryChart = new QChart();
    memoryChart->addSeries(memorySeries);
    memoryChart->setTitle("内存占用情况");
    memoryChart->legend()->hide();

    QDateTimeAxis* timeAxis = new QDateTimeAxis(this);
    timeAxis->setFormat("yyyy-MM-dd hh:mm:ss");
    timeAxis->setRange(QDateTime::currentDateTime().addSecs(-30),QDateTime::currentDateTime().addSecs(30));
    QValueAxis* valueAxis = new QValueAxis(this);
    valueAxis->setRange(0,100);

    memoryChart->setAxisX(timeAxis,memorySeries);
    memoryChart->setAxisY(valueAxis,memorySeries);

    mainUI->setMemoryChart(memoryChart);
}

void UIServerWorkerImpl::initCpuTotalChart()
{
    cpuTotalSeries = new QLineSeries(this);
    cpuTotalSeries->setPointsVisible(true);

    cpuTotalChart = new QChart();
    cpuTotalChart->addSeries(cpuTotalSeries);
    cpuTotalChart->setTitle("CPU总使用情况");
    cpuTotalChart->legend()->hide();

    QDateTimeAxis* timeAxis = new QDateTimeAxis(this);
    timeAxis->setFormat("yyyy-MM-dd hh:mm:ss");
    timeAxis->setRange(QDateTime::currentDateTime().addSecs(-30),QDateTime::currentDateTime().addSecs(30));
    QValueAxis* valueAxis = new QValueAxis(this);
    valueAxis->setRange(0,100);

    cpuTotalChart->setAxisX(timeAxis,cpuTotalSeries);
    cpuTotalChart->setAxisY(valueAxis,cpuTotalSeries);

    mainUI->setCpuTotalChart(cpuTotalChart);
}

void UIServerWorkerImpl::initCpuCharts(int size)
{
    cpuSeries = new QList<QLineSeries*>();
    cpuCharts = new QList<QChart*>();
    for(int i = 0;i < size;i++){
        QLineSeries* tempSeries = new QLineSeries();
        tempSeries->setPointsVisible(true);

        QChart* tempChart = new QChart();
        tempChart->addSeries(tempSeries);
        tempChart->legend()->hide();

        QDateTimeAxis* timeAxis = new QDateTimeAxis(this);
        timeAxis->setFormat("yyyy-MM-dd hh:mm:ss");
        timeAxis->setLabelsVisible(false);
        timeAxis->setRange(QDateTime::currentDateTime().addSecs(-5),QDateTime::currentDateTime().addSecs(5));
        QValueAxis* valueAxis = new QValueAxis(this);
        valueAxis->setRange(0,100);

        tempChart->setAxisX(timeAxis,tempSeries);
        tempChart->setAxisY(valueAxis,tempSeries);

        cpuSeries->append(tempSeries);
        cpuCharts->append(tempChart);

        mainUI->setCpuChart(tempChart,i);
    }
}

void UIServerWorkerImpl::clearCpuCharts()
{
    if(!cpuSeries || !cpuCharts) return;
    foreach (QLineSeries* series, *cpuSeries) {
        series->clear();
        delete series;
    }
    delete cpuSeries;
    foreach (QChart* chart, *cpuCharts) {
        delete chart;
    }
    delete cpuCharts;
}

void UIServerWorkerImpl::setConfManager(QHash<int, DomainNodes *> *startConf)
{
    confManager->setDomainNodes(startConf);
    uiTreeModel->setDevNodes(startConf);
}

void UIServerWorkerImpl::showUI(QList<ConnClient *> *connInfo)
{
    uiTreeModel->start();
    mainUI->setTreeModel(uiTreeModel);
    mainUI->setProcessModel(processModel);
    mainUI->setCpuModel(cpuModel);
    mainUI->setMemoryModel(memoryModel);
    mainUI->setDiskModel(diskModel);
    mainUI->setNetModel(netModel);

    mainUI->setConnectInfo(connInfo);

    mainUI->showMaximized();
}

void UIServerWorkerImpl::startCommunicate(QList<ConnClient *> *connServer)
{
    communicateServer->start(connServer);
    connect(mainUI,SIGNAL(treeItemClick(QStandardItem*)),communicateServer,SIGNAL(devInfoRequestSig(QStandardItem*)));
    connect(mainUI,SIGNAL(updateCollectPeriod(int)),communicateServer,SIGNAL(updateCollectPeriod(int)));
}

void UIServerWorkerImpl::updateProcessModel(QList<Process*> * list)
{
    processModel->setModelData(list);
    processModel->start();
}

void UIServerWorkerImpl::updateCpuModel(QList<Cpu *> *list)
{
    cpuModel->setModelData(list);
    cpuModel->start();
}

void UIServerWorkerImpl::updateMemoryModel(Memory *memory)
{
    memoryModel->setModelData(memory);
    memoryModel->start();
}

void UIServerWorkerImpl::updateDiskModel(QList<Disk *> *list)
{
    diskModel->setModelData(list);
    diskModel->start();
}

void UIServerWorkerImpl::updateNetModel(NetInfo *net)
{
    netModel->setModelData(net);
    netModel->start();
}

void UIServerWorkerImpl::updateMemoryChart(qint64 time, Memory *memory)
{
    if(!memory) return;
//    int elapse = 0;
    double rate = (memory->used*100.0)/memory->total;
    memorySeries->append(time,rate);
    QDateTimeAxis* axis = static_cast<QDateTimeAxis*>(memoryChart->axisX());
    if(time >= axis->max().toMSecsSinceEpoch()){
        qDebug("scroll memory chart!\n");
        memoryChart->axisX()->setRange(QDateTime::currentDateTime().addSecs(-30),QDateTime::currentDateTime().addSecs(30));
//        memoryChart->scroll(10,0);
    }
//    qDebug()<<"memory-111-"<<elapse<<axis->max();

    QVector<QPointF> list = memorySeries->pointsVector();
    qDebug("memorySeries - %d",list.size());
    if(list.size() >= 100){
        qDebug("remove memorySeries - %d",list.size());
        memorySeries->removePoints(0,50);
    }
//    memoryChart->axisX()->setRange(QDateTime::currentDateTime().addSecs(-60),QDateTime::currentDateTime());
}

void UIServerWorkerImpl::updateCpuTotalChart(qint64 time, Cpu *cpu)
{
    if(!cpu) return;
    cpuTotalSeries->append(time,cpu->usedRate);
    QDateTimeAxis* axis = static_cast<QDateTimeAxis*>(memoryChart->axisX());
    if(time >= axis->max().toMSecsSinceEpoch()){
        QDateTime startDate = QDateTime::fromMSecsSinceEpoch(time);
        cpuTotalChart->axisX()->setRange(startDate.addSecs(-20),startDate.addSecs(40));
//        cpuTotalChart->axisX()->setRange(QDateTime::currentDateTime().addSecs(-30),QDateTime::currentDateTime().addSecs(30));
//        cpuTotalChart->scroll(10,0);
    }
    QVector<QPointF> list = cpuTotalSeries->pointsVector();
    qDebug("cpuTotalSeries - %d",list.size());
//    if(list.size() >= 2){
//        QPointF p1 = list.at(list.size() - 1);
//        QPointF p2 = list.at(list.size() - 2);
//        QDateTime pTime1 = QDateTime::fromMSecsSinceEpoch(p1.x());
//        QDateTime pTime2 = QDateTime::fromMSecsSinceEpoch(p2.x());
////        qDebug()<<"pt time1 - "<<pTime1;
////        qDebug()<<"pt time2 - "<<pTime2;
//    }
    if(list.size() >= 100){
        qDebug("remove cpuTotalSeries - %d",list.size());
        cpuTotalSeries->removePoints(0,50);
    }
//    cpuTotalChart->axisX()->setRange(QDateTime::currentDateTime().addSecs(-60),QDateTime::currentDateTime());
}

void UIServerWorkerImpl::updateCpuChart(qint64 time, Cpu *cpu, int index)
{
    if(!cpu) return;
    QLineSeries* tempSeries = cpuSeries->value(index);
    QChart* tempChart = cpuCharts->value(index);
    if(tempSeries && tempChart){
        tempSeries->append(time,cpu->usedRate);
        QDateTimeAxis* axis = static_cast<QDateTimeAxis*>(memoryChart->axisX());
        if(time >= axis->max().toMSecsSinceEpoch()){
            QDateTime startDate = QDateTime::fromMSecsSinceEpoch(time);
            tempChart->axisX()->setRange(startDate.addSecs(-5),startDate.addSecs(5));
//            tempChart->axisX()->setRange(QDateTime::currentDateTime().addSecs(-5),QDateTime::currentDateTime().addSecs(5));
//            tempChart->scroll(10,0);
        }
        QVector<QPointF> list = tempSeries->pointsVector();
        if(list.size() >= 20){
            qDebug("remove tempSeries - %d",list.size());
            tempSeries->removePoints(0,10);
        }
//        tempChart->axisX()->setRange(QDateTime::currentDateTime().addSecs(-10),QDateTime::currentDateTime());
    }
}

void UIServerWorkerImpl::updateCpuCharts(qint64 time, QList<Cpu *> *cpuList)
{
    if(!cpuList) return;
    int listSize = cpuList->size();
    if(mainUI->bItemSelect){
        cpuTotalSeries->clear();
        clearCpuCharts();
        mainUI->createCpuChartViews(listSize - 1);
//        clearCpuCharts();//chart关联到view，删除需注意
        initCpuCharts(listSize - 1);
    }
    updateCpuTotalChart(time,cpuList->value(0));

    for(int i = 1;i < listSize;i++){
        updateCpuChart(time,cpuList->value(i),i - 1);
    }

    mainUI->bItemSelect = false;
//    QCoreApplication::processEvents(QEventLoop::AllEvents,500);
}

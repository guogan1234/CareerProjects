#include "widget.h"
#include "ui_widget.h"
#include <string.h>
#include <QHBoxLayout>
#include <QThread>
#include "config/configworker.h"
#include <QDebug>

//#include "viewdata.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    Init();
}

Widget::Widget(char *flag, QWidget *parent):
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug("flag - %s\n",flag);
    locationFlag = flag;

    Init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setTreeModel(NodesTreeModel *model)
{
    treeModel = model;

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setModel(treeModel);
}

void Widget::setProcessModel(ProcessTableModel *model)
{
    processModel = model;

    ui->tb_process->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_process->setModel(processModel);
}

void Widget::setCpuModel(CpuTableModel *model)
{
    ui->tb_cpu->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_cpu->setModel(model);
}

void Widget::setMemoryModel(MemoryTableModel *model)
{
    ui->tb_memory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_memory->setModel(model);
}

void Widget::setDiskModel(DiskTableModel *model)
{
    ui->tb_disk->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_disk->setModel(model);
}

void Widget::setNetModel(NetTableModel *model)
{
    ui->tb_net->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_net->setModel(model);
}

void Widget::setMemoryChart(QChart *chart)
{
    ui->graphicsView->setChart(chart);
}

void Widget::setCpuTotalChart(QChart *chart)
{
    ui->cpuTotalView->setChart(chart);
}

void Widget::setCpuChart(QChart *chart, int index)
{
    QChartView* view = cpuChartViews->value(index);
    if(!view) return;
    view->setChart(chart);
}

void Widget::createCpuChartViews(int size)
{
    QLayout* cpuChartsLayout = ui->cpuChartsWidget->layout();
    if(!cpuChartsLayout) return;
    //clear
    if(cpuChartViews){
        foreach (QChartView* temp, *cpuChartViews) {
            delete temp;
        }
        cpuChartViews->clear();
    }

    //create
    cpuChartViews = new QList<QChartView*>();
    if(size < 8){
        ui->cpuChartsWidget->setMinimumSize(0,0);
        ui->cpuChartsWidget->resize(1200,225);
    }else {
        ui->cpuChartsWidget->setMinimumSize(2400,225);
    }
    for(int i = 0;i < size;i++){
        QChartView* temp = new QChartView();
        temp->setMaximumSize(300,225);
        cpuChartViews->append(temp);

        cpuChartsLayout->addWidget(temp);
    }
    QBoxLayout* boxLayout = static_cast<QBoxLayout*>(cpuChartsLayout);
    boxLayout->addStretch(5);
}

void Widget::setConnectInfo(QList<ConnClient *> *info)
{
    connInfo = info;
}

int Widget::Init()
{
    InitParams();
    InitStyle();
    InitConnects();
    return 0;
}

int Widget::InitParams()
{
    bItemSelect = false;

    cpuChartViews = NULL;

    configManager = new ConfigManager(this);
    configValues = NULL;
    return 0;
}

int Widget::InitStyle()
{
//    QWidget* temp = ui->scrollArea->takeWidget();
//    delete temp;
    ui->scrollArea->takeWidget();
    ui->scrollArea->setWidget(ui->cpuChartsWidget);
    return 0;
}

void Widget::InitConnects()
{
    connect(ui->collectPeriod_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));
    connect(ui->sustainTime_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));
    connect(ui->cpuLimit_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));
    connect(ui->memoryLimit_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));
    connect(ui->diskLimit_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));
    connect(ui->sourceId_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));
    connect(ui->alarmType_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));
    connect(ui->alarmLevel_LE,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(configChangedSlot(int,int)));

    connect(ui->collectPeriod_LE,SIGNAL(textEdited(QString)),this,SLOT(configTextChangedSlot(QString)));
}

void Widget::loadNodeConfig()
{
    deleteNodeOldConfig();
    configValues = configManager->loadConf();
    showNodeConfig(configValues);
}

void Widget::showNodeConfig(QList<KeyValue *> *config)
{
    if(!config) return;
    foreach (KeyValue* temp, *config) {
        char* key = temp->key;
//        qDebug("showNodeConfig key - %s\n",key);
        if(!strcmp(key,"collect_period")){
            ui->collectPeriod_LE->setText(QString(temp->value));
        }
        if(!strcmp(key,"sustain_time")){
            ui->sustainTime_LE->setText(QString(temp->value));
        }
        if(!strcmp(key,"cpu_limit")){
            ui->cpuLimit_LE->setText(QString(temp->value));
        }
        if(!strcmp(key,"mem_limit")){
            ui->memoryLimit_LE->setText(QString(temp->value));
        }
        if(!strcmp(key,"disk_limit")){
            ui->diskLimit_LE->setText(QString(temp->value));
        }
        if(!strcmp(key,"source_id")){
            ui->sourceId_LE->setText(QString(temp->value));
        }
        if(!strcmp(key,"alarm_type")){
            ui->alarmType_LE->setText(QString(temp->value));
        }
        if(!strcmp(key,"is_del_on_ack")){
            int bValue;
            sscanf(temp->value,"%d",&bValue);
            ui->isDelAlarm_CB->setCurrentIndex(bValue);
        }
        if(!strcmp(key,"is_replace_alarm")){
            int bValue;
            sscanf(temp->value,"%d",&bValue);
            ui->isReplaceAlarm_LE->setCurrentIndex(bValue);
        }
        if(!strcmp(key,"alarm_level")){
            ui->alarmLevel_LE->setText(QString(temp->value));
        }
    }
    ui->handOutStatusLabel->setText(" ");
}

void Widget::acquireNodeNewConfig()
{
    if(!configValues) return;
    QString collectPeriod = ui->collectPeriod_LE->text();
    char* collect_period = qstringCastCharPointer(collectPeriod);

    QString sustainTime = ui->sustainTime_LE->text();
    char* sustain_time = qstringCastCharPointer(sustainTime);

    QString cpuLimit = ui->cpuLimit_LE->text();
    char* cpu_limit = qstringCastCharPointer(cpuLimit);

    QString memoryLimit = ui->memoryLimit_LE->text();
    char* memory_limit = qstringCastCharPointer(memoryLimit);

    QString diskLimit = ui->diskLimit_LE->text();
    char* disk_limit = qstringCastCharPointer(diskLimit);

    QString sourceId = ui->sourceId_LE->text();
    char* source_id = qstringCastCharPointer(sourceId);

    QString alarmType = ui->alarmType_LE->text();
    char* alarm_type = qstringCastCharPointer(alarmType);

    int isDelAlarm = ui->isDelAlarm_CB->currentIndex();
    char* is_del_alarm = (char*)malloc(5);
    sprintf(is_del_alarm,"%d",isDelAlarm);

    int isReplaceAlarm = ui->isReplaceAlarm_LE->currentIndex();
    char* is_replace_alarm = (char*)malloc(5);
    sprintf(is_replace_alarm,"%d",isReplaceAlarm);

    QString alarmLevel = ui->alarmLevel_LE->text();
    char* alarm_level = qstringCastCharPointer(alarmLevel);

    foreach (KeyValue* temp, *configValues) {
        if(!strcmp(temp->key,"collect_period")){
            temp->value = collect_period;
        }
        if(!strcmp(temp->key,"sustain_time")){
            temp->value = sustain_time;
        }
        if(!strcmp(temp->key,"cpu_limit")){
            temp->value = cpu_limit;
        }
        if(!strcmp(temp->key,"mem_limit")){
            temp->value = memory_limit;
        }
        if(!strcmp(temp->key,"disk_limit")){
            temp->value = disk_limit;
        }
        if(!strcmp(temp->key,"source_id")){
            temp->value = source_id;
        }
        if(!strcmp(temp->key,"alarm_type")){
            temp->value = alarm_type;
        }
        if(!strcmp(temp->key,"is_del_on_ack")){
            temp->value = is_del_alarm;
        }
        if(!strcmp(temp->key,"is_replace_alarm")){
            temp->value = is_replace_alarm;
        }
        if(!strcmp(temp->key,"alarm_level")){
            temp->value = alarm_level;
        }
    }
}

void Widget::deleteNodeOldConfig()
{
    if(!configValues) return;
    foreach (KeyValue* temp, *configValues) {
        free(temp->key);
        free(temp->value);
        delete temp;
    }
    delete configValues;
    configValues = NULL;
}

char *Widget::qstringCastCharPointer(QString &qstring)
{
    char* ret = NULL;
    QByteArray temp = qstring.toLocal8Bit();
    ret = strdup(temp.data());
    return ret;
}

void Widget::on_treeView_doubleClicked(const QModelIndex &index)
{
    bItemSelect = true;
    loadNodeConfig();

    qDebug("[load node alarm config ok!]");
    QStandardItem* item = NULL;
    QAbstractItemModel* abstractModel = ui->treeView->model();
    QStandardItemModel* standardModel = static_cast<QStandardItemModel*>(abstractModel);
    if(standardModel){
        item = standardModel->itemFromIndex(index);
        emit treeItemClick(item);
    }
}

void Widget::on_configHandOut_clicked()
{
    acquireNodeNewConfig();
    foreach (KeyValue* temp, *configValues) {
        qDebug("[handout]key - %s,value - %s",temp->key,temp->value);
    }
    configManager->saveConf(configValues);
    ConfigWorker* configWorker = new ConfigWorker();
    configWorker->setConfig(configValues);
    configWorker->setConnectInfo(connInfo);
    QThread* thread = new QThread();
    connect(thread,SIGNAL(started()),configWorker,SLOT(handOutConfigSlot()));
    thread->start();

    QString newCollectPeriod = ui->collectPeriod_LE->text();
    int collectPeriod = newCollectPeriod.toInt();
    emit updateCollectPeriod(collectPeriod);
    ui->handOutStatusLabel->setText("下发成功！");
}

void Widget::configChangedSlot(int old, int now)
{
//    qDebug("old - %d,now - %d",old,now);
//    ui->handOutStatusLabel->setText(" ");
}

void Widget::configTextChangedSlot(QString text)
{
    qDebug()<<"###"<<text;
    ui->handOutStatusLabel->setText(" ");
}

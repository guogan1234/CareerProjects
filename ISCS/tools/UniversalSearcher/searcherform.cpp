#include "searcherform.h"
#include "ui_searcherform.h"

#include "model/stationactivemodel.h"
#include "model/domainactivemodel.h"
#include "model/prosystemactivemodel.h"
#include "model/devtypeactivemodel.h"
#include "model/deviceactivemodel.h"
#include "model/accactivemodel.h"
#include "model/aiactivemodel.h"
#include "model/diactivemodel.h"

#include "ui/activecombobox.h"
#include <QDebug>

//test
#include "confcmn.h"

SearcherForm::SearcherForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearcherForm)
{
    ui->setupUi(this);

    init();
}

SearcherForm::~SearcherForm()
{
    qDebug("SearcherForm::~SearcherForm start!");
    uiModel->endCFDBService();
    delete ui;
    qDebug("SearcherForm::~SearcherForm end!");
}

void SearcherForm::test()
{
//    StationData* data = new StationData();
    //    data->loadDataByCFDB("select * from conf.station_tbl where domain_id=99");
}

QString SearcherForm::getCurrentPoint()
{
    return currentPointValue;
}

char *SearcherForm::getCurrentPointChar()
{
    return currentPointValue.toLocal8Bit().data();
}

void SearcherForm::init()
{
    initModule();
    initParam();
    initStyle();
    initConnect();
}

void SearcherForm::initParam()
{
    domainModel = new DomainActiveModel(this);
    ui->CB_domain->setModel(domainModel);
//    domainModel->initShow();
    DomainActiveModel* domainActiveModel = static_cast<DomainActiveModel*>(domainModel);
    domainActiveModel->initShow();

    stationModel = new StationActiveModel(this);
    ui->CB_station->setModel(stationModel);

    proSystemModel = new ProSystemActiveModel(this);
    ui->CB_system->setModel(proSystemModel);
//    proSystemModel->initShow();
    ProSystemActiveModel* proSystemActiveModel = static_cast<ProSystemActiveModel*>(proSystemModel);
    proSystemActiveModel->initShow();

    devTypeModel = new DevTypeActiveModel(this);
    ui->CB_devType->setModel(devTypeModel);

    deviceModel = new DeviceActiveModel(this);
    ui->TV_device->setModel(deviceModel);

    accModel = new AccActiveModel(this);
    aiModel = new AiActiveModel(this);
    diModel = new DiActiveModel(this);

    pointHeaderModel = new QStandardItemModel(this);
    pointHeaderModel->setSortRole(Qt::UserRole);
    ui->TV_pointHeader->setModel(pointHeaderModel);
}

void SearcherForm::initConnect()
{
    connect(ui->CB_station,SIGNAL(enterSignal()),this,SLOT(stationEnterSlot()));
    connect(ui->CB_devType,SIGNAL(enterSignal()),this,SLOT(devTypeEnterSlot()));
    connect(ui->TV_device,SIGNAL(enterSignal()),this,SLOT(deviceEnterSLot()));
    connect(ui->TV_point,SIGNAL(enterSignal()),this,SLOT(pointEnterSlot()));
    connect(ui->TV_pointHeader,SIGNAL(enterSignal()),this,SLOT(pointTypeEnterSlot()));
    connect(ui->TV_pointHeader,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(pointTypeDClickSLot(QModelIndex)));
    connect(ui->TV_pointHeader,SIGNAL(pressed(QModelIndex)),this,SLOT(ptHeaderViewPressSlot(QModelIndex)));
}

void SearcherForm::initModule()
{
    uiModel = new UIModel();
    uiModel->startCFDBService();
}

void SearcherForm::initStyle()
{
    ui->TV_device->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TV_device->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TV_device->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TV_device->horizontalHeader()->setStretchLastSection(true);
    ui->TV_device->horizontalHeader()->setHidden(true);

    ui->TV_point->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TV_point->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TV_point->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TV_point->horizontalHeader()->setStretchLastSection(true);
    ui->TV_point->horizontalHeader()->hide();

    ui->TV_pointHeader->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TV_pointHeader->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TV_pointHeader->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TV_pointHeader->horizontalHeader()->setStretchLastSection(true);
    ui->TV_pointHeader->horizontalHeader()->hide();
}

int SearcherForm::fillModelWithHeaders(QStandardItemModel *model, char **columns, int size)
{
    if(model && columns){
        for(int i = 0;i < size; ++i){
            QStandardItem* item = new QStandardItem(columns[i]);
            item->setData("2",Qt::UserRole);
            if(!strcmp("acc_value",columns[i]) || !strcmp("ai_value",columns[i]) || !strcmp("di_value",columns[i])){
                item->setData("0",Qt::UserRole);
                qDebug()<<"0 -- "<<columns[i];
            }
            if(!strcmp("acc_status",columns[i]) || !strcmp("ai_status",columns[i]) || !strcmp("di_status",columns[i])){
                item->setData("1",Qt::UserRole);
                qDebug()<<"1 -- "<<columns[i];
            }
            model->appendRow(item);
        }
        return 0;
    }else {
        return -1;
    }
}

void SearcherForm::stationEnterSlot()
{
    qDebug()<<"SearcherForm::stationEnterSlot";
    QVariant varDomainId = ui->CB_domain->currentData();
    qDebug()<<"varDomainId - "<<varDomainId;
    QVariantMap* map = new QVariantMap();
    map->insert("domainId",varDomainId);
//    QString sql = stationModel->conditionsToSQL(map);
//    stationModel->fillModelDatas(sql);
    StationActiveModel* stationActiveModel = static_cast<StationActiveModel*>(stationModel);
    QString sql = stationActiveModel->conditionsToSQL(map);
    stationActiveModel->fillModelDatas(sql);
}

void SearcherForm::devTypeEnterSlot()
{
    qDebug()<<"SearcherForm::devTypeEnterSlot";
    QVariant varProSystemId = ui->CB_system->currentData();
    qDebug()<<"varProSystemId - "<<varProSystemId;
    QVariantMap* map = new QVariantMap();
    map->insert("systemId",varProSystemId);
//    QString sql = devTypeModel->conditionsToSQL(map);
//    devTypeModel->fillModelDatas(sql);
    DevTypeActiveModel* devTypeActiveModel = static_cast<DevTypeActiveModel*>(devTypeModel);
    QString sql = devTypeActiveModel->conditionsToSQL(map);
    devTypeActiveModel->fillModelDatas(sql);
}

void SearcherForm::deviceEnterSLot()
{
    qDebug()<<"SearcherForm::deviceEnterSLot";
    QVariant varDomainId = ui->CB_domain->currentData();
    qDebug()<<"varDomainId - "<<varDomainId;
    QVariant varStationId = ui->CB_station->currentData();
    QVariant varProSystemId = ui->CB_system->currentData();
    QVariant varDevTypeLabel = ui->CB_devType->currentData();
    QVariantMap* map = new QVariantMap();
    map->insert("domainId",varDomainId);
    map->insert("stationId",varStationId);
    map->insert("proSystemId",varProSystemId);
    map->insert("devTypeLabel",varDevTypeLabel);
//    QString sql = deviceModel->conditionsToSQL(map);
//    deviceModel->fillModelDatas(sql);
    DeviceActiveModel* deviceActiveModel = static_cast<DeviceActiveModel*>(deviceModel);
    QString sql = deviceActiveModel->conditionsToSQL(map);
    deviceActiveModel->fillModelDatas(sql);
}

void SearcherForm::pointEnterSlot()
{
    qDebug()<<"SearcherForm::pointEnterSlot";
    QModelIndex modelIndex = ui->TV_device->currentIndex();
    QAbstractItemModel* absModel = ui->TV_device->model();
    QVariant varDevLabel = absModel->data(modelIndex);
    qDebug()<<"varDevLabel - "<<varDevLabel;
    QVariantMap* map = new QVariantMap();
    map->insert("devLabel",QVariant(varDevLabel));
    int index = ui->CB_pointType->currentIndex();
    QString sql;
    if(0 == index){//di
//        sql = diModel->conditionsToSQL(map);
//        diModel->fillModelDatas(sql);
//        ui->TV_point->setModel(diModel);
        DiActiveModel* diActiveModel = static_cast<DiActiveModel*>(diModel);
        sql = diActiveModel->conditionsToSQL(map);
        diActiveModel->fillModelDatas(sql);
        ui->TV_point->setModel(diActiveModel);
    }
    if(1 == index){//ai
//        sql = aiModel->conditionsToSQL(map);
//        aiModel->fillModelDatas(sql);
//        ui->TV_point->setModel(aiModel);
        AiActiveModel* aiActiveModel = static_cast<AiActiveModel*>(aiModel);
        sql = aiActiveModel->conditionsToSQL(map);
        aiActiveModel->fillModelDatas(sql);
        ui->TV_point->setModel(aiActiveModel);
    }
    if(2 == index){//acc
//        sql = accModel->conditionsToSQL(map);
//        accModel->fillModelDatas(sql);
//        ui->TV_point->setModel(accModel);
        AccActiveModel* accActiveModel = static_cast<AccActiveModel*>(accModel);
        sql = accActiveModel->conditionsToSQL(map);
        accActiveModel->fillModelDatas(sql);
        ui->TV_point->setModel(accActiveModel);
    }
}

void SearcherForm::pointTypeEnterSlot()
{
    pointHeaderModel->clear();
    int index = ui->CB_pointType->currentIndex();
    char** columns = NULL;
    int colSize = -1;
    if(0 == index){//di
//        columns = diModel->getRecordHeaderByCFDB();
//        colSize = diModel->getRecordHeaderSizeByCFDB();
        DiActiveModel* diActiveModel = static_cast<DiActiveModel*>(diModel);
        columns = diActiveModel->getRecordHeaderByCFDB();
        colSize = diActiveModel->getRecordHeaderSizeByCFDB();
    }
    if(1 == index){//ai
//        columns = aiModel->getRecordHeaderByCFDB();
//        colSize = aiModel->getRecordHeaderSizeByCFDB();
        AiActiveModel* aiActiveModel = static_cast<AiActiveModel*>(aiModel);
        columns = aiActiveModel->getRecordHeaderByCFDB();
        colSize = aiActiveModel->getRecordHeaderSizeByCFDB();
    }
    if(2 == index){//acc
//        columns = accModel->getRecordHeaderByCFDB();
//        colSize = accModel->getRecordHeaderSizeByCFDB();
        AccActiveModel* accActiveModel = static_cast<AccActiveModel*>(accModel);
        columns = accActiveModel->getRecordHeaderByCFDB();
        colSize = accActiveModel->getRecordHeaderSizeByCFDB();
    }
    int ret = fillModelWithHeaders(pointHeaderModel,columns,colSize);
    if(-1 == ret){
        qDebug()<<"fill pointHeaders failed!";
    }
    pointHeaderModel->sort(0);
}

void SearcherForm::pointTypeDClickSLot(QModelIndex index)
{
    qDebug()<<"SearcherForm::pointTypeDClickSLot";
    QModelIndex modelIndex = ui->TV_point->currentIndex();
    QAbstractItemModel* absModel = ui->TV_point->model();
    QVariant varPointLabel = absModel->data(modelIndex);
    qDebug()<<"modelIndex - "<<modelIndex;
    qDebug()<<"varPointLabel - "<<varPointLabel;
    QString pointLabel;
    if(varPointLabel.canConvert<QString>()){
        pointLabel = varPointLabel.toString();
    }
    if(pointLabel.isEmpty()){
        qDebug()<<"no select point!";
    }
    QAbstractItemModel* pointHeaderModel = ui->TV_pointHeader->model();
    QVariant varPointHeader = pointHeaderModel->data(index);
    QString pointHeader;
    if(varPointHeader.canConvert<QString>()){
        pointHeader = varPointHeader.toString();
    }
    currentPointValue = pointLabel + "." + pointHeader;
    qDebug()<<"currentPointValue - "<<currentPointValue;
//    emit finishedSearchSignal();
    //test
    QString ret = getCurrentPoint();
    char* retChar = getCurrentPointChar();
    qDebug()<<"ret - "<<ret;
    qDebug("retChar - %s",retChar);
}

void SearcherForm::ptHeaderViewPressSlot(const QModelIndex &index)
{
    qDebug()<<"SearcherForm::ptHeaderViewPressSlot";
    QModelIndex modelIndex = ui->TV_point->currentIndex();
    QAbstractItemModel* absModel = ui->TV_point->model();
    QVariant varPointLabel = absModel->data(modelIndex);
    qDebug()<<"modelIndex - "<<modelIndex;
    qDebug()<<"varPointLabel - "<<varPointLabel;
    QString pointLabel;
    if(varPointLabel.canConvert<QString>()){
        pointLabel = varPointLabel.toString();
    }
    if(pointLabel.isEmpty()){
        qDebug()<<"no select point!";
    }

    QVariant varPointHeader = pointHeaderModel->data(index);
    QString pointHeader;
    if(varPointHeader.canConvert<QString>()){
        pointHeader = varPointHeader.toString();
    }
    currentPointValue = pointLabel + "." + pointHeader;
    qDebug()<<"currentPointValue - "<<currentPointValue;

    ui->TV_pointHeader->setCurrentPtValue(currentPointValue);
}

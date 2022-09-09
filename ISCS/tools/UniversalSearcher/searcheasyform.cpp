#include "searcheasyform.h"
#include "ui_searcheasyform.h"

#include "model/domainactivemodel.h"
#include "model/stationactivemodel.h"
#include "model/prosystemactivemodel.h"
#include "model/devtypeactivemodel.h"
#include "model/deviceactivemodel.h"
#include "model/accactivemodel.h"
#include "model/aiactivemodel.h"
#include "model/diactivemodel.h"
#include "worker/pointmimedata.h"
#include <QDebug>

searchEasyForm::searchEasyForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchEasyForm)
{
    ui->setupUi(this);

    init();
}

searchEasyForm::~searchEasyForm()
{
    uiModel->endCFDBService();
    delete ui;
}

QString searchEasyForm::getPointSelected()
{
    return selectPoint;
}

void searchEasyForm::init()
{
    initModule();
    initParam();
    initStyle();

    bShow = true;
}

void searchEasyForm::initParam()
{
    bShow = false;

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

void searchEasyForm::initModule()
{
    uiModel = new UIModel(this);
    uiModel->startCFDBService();
}

void searchEasyForm::initStyle()
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

void searchEasyForm::changePointViewData(int index)
{
    qDebug()<<"searchEasyForm::pointEnterSlot";
    QModelIndex modelIndex = ui->TV_device->currentIndex();
    QAbstractItemModel* absModel = ui->TV_device->model();
    QVariant varDevLabel = absModel->data(modelIndex,Qt::UserRole);
    qDebug()<<"varDevLabel - "<<varDevLabel;
    QVariantMap* map = new QVariantMap();
    map->insert("devLabel",QVariant(varDevLabel));
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

void searchEasyForm::changePointHeaderData(int index)
{
    pointHeaderModel->clear();
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

int searchEasyForm::fillModelWithHeaders(QStandardItemModel *model, char **columns, int size)
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

QString searchEasyForm::getPointSelectedByIndex(const QModelIndex &index)
{
    QModelIndex modelIndex = ui->TV_point->currentIndex();
    QAbstractItemModel* absModel = ui->TV_point->model();
    QVariant varPointLabel = absModel->data(modelIndex,Qt::UserRole);
    qDebug()<<"modelIndex - "<<modelIndex;
    qDebug()<<"varPointLabel - "<<varPointLabel;
    QString currentPointValue;
    QString pointLabel;
    if(varPointLabel.canConvert<QString>()){
        pointLabel = varPointLabel.toString();
    }
    if(pointLabel.isEmpty()){
        qDebug()<<"no select point!";
        currentPointValue = "请先选择设备点！";
        ui->TV_pointHeader->setCurrentPtValue(currentPointValue);
        return currentPointValue;
    }

    QVariant varPointHeader = pointHeaderModel->data(index);
    QString pointHeader;
    if(varPointHeader.canConvert<QString>()){
        pointHeader = varPointHeader.toString();
    }
    currentPointValue = pointLabel + "." + pointHeader;
    qDebug()<<"currentPointValue - "<<currentPointValue;

    ui->TV_pointHeader->setCurrentPtValue(currentPointValue);
    return currentPointValue;
}

void searchEasyForm::formMimeData(const QModelIndex &index)
{
    if(index.isValid()){
        int row = index.row();
        int typeIndex = ui->CB_point->currentIndex();
        formMimeDataByCondition(row,typeIndex);
    }
}

void searchEasyForm::formMimeDataByCondition(int index, int typeIndex)
{
    MimePoint* pointData = new MimePoint();
    pointData->accPoint = NULL;
    pointData->aiPoint = NULL;
    pointData->diPoint = NULL;
    if(0 == typeIndex){//di
        DiActiveModel* diActiveModel = static_cast<DiActiveModel*>(diModel);
        pointData->diPoint = diActiveModel->getModelDataByIndex(index);
    }
    if(1 == typeIndex){//ai
        AiActiveModel* aiActiveModel = static_cast<AiActiveModel*>(aiModel);
        pointData->aiPoint = aiActiveModel->getModelDataByIndex(index);
    }
    if(2 == typeIndex){//acc
        AccActiveModel* accActiveModel = static_cast<AccActiveModel*>(accModel);
        pointData->accPoint = accActiveModel->getModelDataByIndex(index);
    }
    ui->TV_pointHeader->setMimeData(pointData);
}

void searchEasyForm::on_TV_pointHeader_pressed(const QModelIndex &index)
{
    qDebug()<<"SearcherForm::ptHeaderViewPressSlot";
    QString currentPointValue;
    currentPointValue = getPointSelectedByIndex(index);
    emit pointSelected(currentPointValue);

    //使用拖拽功能传递点对象信息
    formMimeData(index);
}

void searchEasyForm::on_CB_domain_activated(int index)
{
    qDebug()<<"searchEasyForm::on_CB_domain_activated - "<<index;
    QVariant varDomainId = ui->CB_domain->itemData(index);
    qDebug()<<"varDomainId - "<<varDomainId;
    QVariantMap* map = new QVariantMap();
    map->insert("domainId",varDomainId);
//    QString sql = stationModel->conditionsToSQL(map);
//    stationModel->fillModelDatas(sql);
    StationActiveModel* stationActiveModel = static_cast<StationActiveModel*>(stationModel);
    QString sql = stationActiveModel->conditionsToSQL(map);
    stationActiveModel->fillModelDatas(sql);
}

void searchEasyForm::on_CB_system_activated(int index)
{
    qDebug()<<"searchEasyForm::on_CB_system_activated - "<<index;
    QVariant varProSystemId = ui->CB_system->itemData(index);
    qDebug()<<"varProSystemId - "<<varProSystemId;
    QVariantMap* map = new QVariantMap();
    map->insert("systemId",varProSystemId);
//    QString sql = devTypeModel->conditionsToSQL(map);
//    devTypeModel->fillModelDatas(sql);
    DevTypeActiveModel* devTypeActiveModel = static_cast<DevTypeActiveModel*>(devTypeModel);
    QString sql = devTypeActiveModel->conditionsToSQL(map);
    devTypeActiveModel->fillModelDatas(sql);
}

void searchEasyForm::on_CB_devType_activated(int index)
{
    qDebug()<<"searchEasyForm::on_CB_devType_activated - "<<index;
    QVariant varDomainId = ui->CB_domain->currentData();
    qDebug()<<"varDomainId - "<<varDomainId;
    QVariant varStationId = ui->CB_station->currentData();
    QVariant varProSystemId = ui->CB_system->currentData();
    QVariant varDevTypeLabel = ui->CB_devType->itemData(index);
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

void searchEasyForm::on_CB_point_activated(int index)
{
    qDebug()<<"searchEasyForm::on_CB_point_activated - "<<index;
    changePointViewData(index);
    changePointHeaderData(index);
}

void searchEasyForm::on_TV_pointHeader_doubleClicked(const QModelIndex &index)
{
    qDebug()<<"SearcherForm::TV_pointHeader_doubleClick";
    QString currentPointValue;
    currentPointValue = getPointSelectedByIndex(index);
    emit pointDoubleSelected(currentPointValue);
}

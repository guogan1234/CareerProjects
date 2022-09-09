#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "nodemanager.h"
#include "ui/nodestreemodel.h"
#include "ui/processtablemodel.h"
#include "ui/cputablemodel.h"
#include "ui/memorytablemodel.h"
#include "ui/disktablemodel.h"
#include "ui/nettablemodel.h"
#include "ConfHandle/configmanager.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
using namespace QtCharts;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    Widget(char* flag,QWidget* parent = 0);
    ~Widget();

    void setStartConfig(QHash<int,DomainNodes*> * config);
    void setTreeModel(NodesTreeModel* model);
    void setProcessModel(ProcessTableModel* model);
    void setCpuModel(CpuTableModel* model);
    void setMemoryModel(MemoryTableModel* model);
    void setDiskModel(DiskTableModel* model);
    void setNetModel(NetTableModel* model);

    void setMemoryChart(QChart* chart);
    void setCpuTotalChart(QChart* chart);
    void setCpuChart(QChart* chart,int index);
    void createCpuChartViews(int size);

    void setConnectInfo(QList<ConnClient*> * info);
    bool bItemSelect;
private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_configHandOut_clicked();

    void configChangedSlot(int old,int now);
    void configTextChangedSlot(QString text);
signals:
    void treeItemClick(QStandardItem* item);
    void updateCollectPeriod(int msec);
private:
    Ui::Widget *ui;

    char* locationFlag;

    int Init();
    int InitParams();
    int InitStyle();
    void InitConnects();

    //
    NodeManager* nodeManager;
    NodesTreeModel* treeModel;
    ProcessTableModel* processModel;

    QList<QChartView*> * cpuChartViews;

    QList<ConnClient *> *connInfo;
    QList<KeyValue*> * configValues;
    ConfigManager* configManager;
    void loadNodeConfig();
    void showNodeConfig(QList<KeyValue*> * config);
    void acquireNodeNewConfig();
    void deleteNodeOldConfig();

    char* qstringCastCharPointer(QString& qstring);
};

#endif // WIDGET_H

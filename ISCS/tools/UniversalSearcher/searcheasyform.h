#ifndef SEARCHEASYFORM_H
#define SEARCHEASYFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include "universalsearcher_global.h"
#include "model/uimodel.h"

//使用searchEasyForm类，SearcherForm类为早期版本使用

namespace Ui {
class searchEasyForm;
}

class UNIVERSALSEARCHERSHARED_EXPORT searchEasyForm : public QWidget
{
    Q_OBJECT

public:
    explicit searchEasyForm(QWidget *parent = 0);
    ~searchEasyForm();

    QString getPointSelected();

signals:
    void pointSelected(QString);
    void pointDoubleSelected(QString);
private slots:
    void on_TV_pointHeader_pressed(const QModelIndex &index);

    void on_CB_domain_activated(int index);

    void on_CB_system_activated(int index);

    void on_CB_devType_activated(int index);

    void on_CB_point_activated(int index);

    void on_TV_pointHeader_doubleClicked(const QModelIndex &index);

    private:
    Ui::searchEasyForm *ui;

    UIModel* uiModel;
    QStandardItemModel* domainModel;
    QStandardItemModel* stationModel;
    QStandardItemModel* proSystemModel;
    QStandardItemModel* devTypeModel;
    QStandardItemModel* deviceModel;
    QStandardItemModel* accModel;
    QStandardItemModel* aiModel;
    QStandardItemModel* diModel;
    QStandardItemModel* pointHeaderModel;
    bool bShow;

    void init();
    void initParam();
    void initModule();
    void initStyle();

    void changePointViewData(int index);
    void changePointHeaderData(int index);
    int fillModelWithHeaders(QStandardItemModel* model,char** columns,int size);

    QString getPointSelectedByIndex(const QModelIndex& index);
    QString selectPoint;

    void formMimeData(const QModelIndex& index);
    void formMimeDataByCondition(int index,int typeIndex);
};

#endif // SEARCHEASYFORM_H

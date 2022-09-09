#ifndef SEARCHERFORM_H
#define SEARCHERFORM_H

#include <QWidget>
#include <QModelIndex>
#include "universalsearcher_global.h"
#include "model/uimodel.h"
//#include "model/stationactivemodel.h"
//#include "model/domainactivemodel.h"
//#include "model/prosystemactivemodel.h"
//#include "model/devtypeactivemodel.h"
//#include "model/deviceactivemodel.h"
//#include "model/accactivemodel.h"
//#include "model/aiactivemodel.h"
//#include "model/diactivemodel.h"
#include <QStandardItemModel>

namespace Ui {
class SearcherForm;
}

class UNIVERSALSEARCHERSHARED_EXPORT SearcherForm : public QWidget
{
        Q_OBJECT

    public:
        explicit SearcherForm(QWidget *parent = 0);
        ~SearcherForm();

        void test();
        QString getCurrentPoint();
        char* getCurrentPointChar();
    private:
        Ui::SearcherForm *ui;

        void init();
        void initParam();
        void initConnect();
        void initModule();
        void initStyle();

        UIModel* uiModel;
//        DomainActiveModel* domainModel;
//        StationActiveModel* stationModel;
//        ProSystemActiveModel* proSystemModel;
//        DevTypeActiveModel* devTypeModel;
//        DeviceActiveModel* deviceModel;
//        AccActiveModel* accModel;
//        AiActiveModel* aiModel;
//        DiActiveModel* diModel;
//        QStandardItemModel* pointHeaderModel;

        QStandardItemModel* domainModel;
        QStandardItemModel* stationModel;
        QStandardItemModel* proSystemModel;
        QStandardItemModel* devTypeModel;
        QStandardItemModel* deviceModel;
        QStandardItemModel* accModel;
        QStandardItemModel* aiModel;
        QStandardItemModel* diModel;
        QStandardItemModel* pointHeaderModel;

        int fillModelWithHeaders(QStandardItemModel* model,char** columns,int size);

        QString currentPointValue;
signals:
        void finishedSearchSignal();
public slots:
        void stationEnterSlot();
        void devTypeEnterSlot();
        void deviceEnterSLot();
        void pointEnterSlot();
        void pointTypeEnterSlot();

        void pointTypeDClickSLot(QModelIndex index);

        void ptHeaderViewPressSlot(const QModelIndex& index);
};

#endif // SEARCHERFORM_H

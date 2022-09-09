#ifndef AIACTIVEMODEL_H
#define AIACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/aimanager.h"

class AiActiveModel : public ActiveListModelBase
{
    Q_OBJECT
public:
    explicit AiActiveModel(QObject *parent = nullptr);

    QString conditionsToSQL(QVariantMap *listValues);
    void fillModelDatas(QString sql);

    char** getRecordHeaderByCFDB();
    int getRecordHeaderSizeByCFDB();

    Ai* getModelDataByIndex(int index);
signals:

public slots:

private:
    AiManager* manager;

    void init();
    void initParam();

    QList<Ai*> * getModelDatas(QString sql);
    void fillModel(QList<Ai*> * data);

    QList<Ai*> * modelData;
};

#endif // AIACTIVEMODEL_H

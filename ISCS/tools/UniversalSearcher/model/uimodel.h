#ifndef UIMODEL_H
#define UIMODEL_H

#include <QObject>

class UIModel : public QObject
{
    Q_OBJECT
public:
    explicit UIModel(QObject *parent = nullptr);

    int startCFDBService();
    int endCFDBService();
signals:

public slots:
};

#endif // UIMODEL_H

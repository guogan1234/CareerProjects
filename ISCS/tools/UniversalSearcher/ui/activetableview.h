#ifndef ACTIVETABLEVIEW_H
#define ACTIVETABLEVIEW_H

#include <QObject>
#include <QTableView>
#include <QEvent>
#include <QMouseEvent>
#include "worker/pointmimedata.h"

class ActiveTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ActiveTableView(QWidget *parent = nullptr);

    bool event(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void setCurrentPtValue(QString ptValue);
    void setMimeData(MimePoint* data);
signals:
    void enterSignal();
public slots:

private:
    QPointF startPt;

    QString currentPointValue;
    MimePoint* pointData;
};

#endif // ACTIVETABLEVIEW_H

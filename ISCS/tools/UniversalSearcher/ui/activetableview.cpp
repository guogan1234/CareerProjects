#include "activetableview.h"

#include <QApplication>
#include <QMimeData>
#include <QDrag>

ActiveTableView::ActiveTableView(QWidget *parent) : QTableView(parent)
{

}

bool ActiveTableView::event(QEvent *event)
{
    if(event->type() == QEvent::Enter){
        qDebug("ActiveTableView begin to emit enterSignal().");
        emit enterSignal();
    }
    return QTableView::event(event);
}

void ActiveTableView::mousePressEvent(QMouseEvent *event)
{
    startPt = event->screenPos();
    return QTableView::mousePressEvent(event);
}

void ActiveTableView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF nowPt = event->screenPos();
    QLineF line(startPt,nowPt);
    if(line.length() >= QApplication::startDragDistance()/2){
        QDrag* drag = new QDrag(this);
//        //传递点标签信息
//        QMimeData* data = new QMimeData();
//        data->setText(currentPointValue);
        //传递点对象信息
        PointMimeData* data = new PointMimeData();
        data->setText(currentPointValue);
        data->setPointData(pointData);

        drag->setMimeData(data);
        drag->exec();
    }
    return QTableView::mouseMoveEvent(event);
}

void ActiveTableView::setCurrentPtValue(QString ptValue)
{
    currentPointValue = ptValue;
}

void ActiveTableView::setMimeData(MimePoint *data)
{
    pointData = data;
}

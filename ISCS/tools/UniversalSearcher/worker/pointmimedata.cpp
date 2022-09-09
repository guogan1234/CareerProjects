#include "pointmimedata.h"

PointMimeData::PointMimeData() : QMimeData()
{
    pointData = NULL;
}

void PointMimeData::setPointData(MimePoint *mimePoint)
{
    pointData = mimePoint;
}

MimePoint *PointMimeData::getPointData() const
{
    return pointData;
}

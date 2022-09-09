#ifndef POINTMIMEDATA_H
#define POINTMIMEDATA_H

#include <QMimeData>
#include "data/mimepoint.h"

class PointMimeData : public QMimeData
{
        Q_OBJECT
    public:
        explicit PointMimeData();

        void setPointData(MimePoint* mimePoint);
        MimePoint* getPointData() const;
    private:
        MimePoint* pointData;

    signals:

    public slots:
};

#endif // POINTMIMEDATA_H

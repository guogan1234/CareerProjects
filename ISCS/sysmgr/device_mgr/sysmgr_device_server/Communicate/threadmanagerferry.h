#ifndef THREADMANAGERFERRY_H
#define THREADMANAGERFERRY_H

#include <QObject>

class ThreadManagerFerry : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManagerFerry(QObject *parent = nullptr);

signals:

public slots:
};

#endif // THREADMANAGERFERRY_H
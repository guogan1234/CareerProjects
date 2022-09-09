#ifndef ACTIVECOMBOBOX_H
#define ACTIVECOMBOBOX_H

#include <QComboBox>
#include <QEvent>

class ActiveComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ActiveComboBox(QWidget *parent = nullptr);

    bool event(QEvent *event);
signals:
    void enterSignal();
public slots:
};

#endif // ACTIVECOMBOBOX_H

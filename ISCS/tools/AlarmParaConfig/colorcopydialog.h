#ifndef COLORCOPYDIALOG_H
#define COLORCOPYDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "configstruct.h"


namespace Ui {
class ColorCopyDialog;
}

class ColorCopyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorCopyDialog(QMap<int,AlarmColor> &map,QWidget *parent = 0);
    ~ColorCopyDialog();
private:
    void initView();
    void initComboboxLevel();
    void initListWidgetDestination(QString source);

    AlarmColor* getSourceColor();
    AlarmColor* getDestinationColor(QString text);
    QList<AlarmColor*> *getDestinationColorList(QList<QString> list);
private slots:
    void Slot_ChangedCombobox(int index);
    void Slot_ClickedButton(bool status);

private:
    Ui::ColorCopyDialog *ui;
    QMap<int,AlarmColor> &m_qmap_alarmcolor;
    QMap<int,AlarmColor> m_qmap_alarmcolor_current;
};

#endif // COLORCOPYDIALOG_H

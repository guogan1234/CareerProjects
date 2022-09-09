#include "colorcopydialog.h"
#include "ui_colorcopydialog.h"
#include <QMessageBox>
#include <QDebug>

ColorCopyDialog::ColorCopyDialog(QMap<int, AlarmColor> &map, QWidget *parent) : QDialog(parent),
    ui(new Ui::ColorCopyDialog),m_qmap_alarmcolor(map)
{
    ui->setupUi(this);
    this->m_qmap_alarmcolor_current.operator =(this->m_qmap_alarmcolor);
    initView();
    initComboboxLevel();
    connect(ui->m_qcombobox_source,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_ChangedCombobox(int)));
    connect(ui->m_qpushbutton_enter,SIGNAL(clicked(bool)),this,SLOT(Slot_ClickedButton(bool)));
    connect(ui->m_qpushbutton_cancel,SIGNAL(clicked(bool)),this,SLOT(Slot_ClickedButton(bool)));
}

ColorCopyDialog::~ColorCopyDialog()
{
    delete ui;
}

void ColorCopyDialog::initView()
{
    ui->m_qlistwidget_destination->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->m_qlistwidget_destination->setSelectionMode(QAbstractItemView::MultiSelection);
}

void ColorCopyDialog::initComboboxLevel()
{
    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_alarmcolor_current.begin();
    int i = 0;
    while(this->m_qmap_alarmcolor_current.end() != iterator_color)
    {
        ui->m_qcombobox_source->insertItem(i,iterator_color.value().m_qstr_alarm_name);
        ++iterator_color;
    }
    ui->m_qcombobox_source->setCurrentIndex(-1);
    ui->m_qlistwidget_destination->clear();
    return;
}

void ColorCopyDialog::initListWidgetDestination(QString source)
{
    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_alarmcolor_current.begin();
    int i = 0;
    while(this->m_qmap_alarmcolor_current.end() != iterator_color)
    {
        if(!source.operator ==(iterator_color.value().m_qstr_alarm_name.toLatin1()))
            ui->m_qlistwidget_destination->addItem(iterator_color.value().m_qstr_alarm_name);
        ++iterator_color;
    }
    return;
}

AlarmColor *ColorCopyDialog::getSourceColor()
{
    QString qstr_levelname = ui->m_qcombobox_source->currentText();
    if(qstr_levelname.isEmpty())
        return NULL;
    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_alarmcolor_current.begin();
    while(this->m_qmap_alarmcolor_current.end() != iterator_color)
    {
        if(qstr_levelname.operator ==(iterator_color.value().m_qstr_alarm_name.toLatin1()))
            return &this->m_qmap_alarmcolor_current[iterator_color.key()];
        ++iterator_color;
    }
    return NULL;
}

AlarmColor *ColorCopyDialog::getDestinationColor(QString text)
{
    if(text.isEmpty())
        return NULL;
    QMap<int,AlarmColor>::iterator iterator_color = this->m_qmap_alarmcolor.begin();
    while(this->m_qmap_alarmcolor.end() != iterator_color)
    {
        if(text.operator ==(iterator_color.value().m_qstr_alarm_name.toLatin1()))
            return &this->m_qmap_alarmcolor[iterator_color.key()];
        ++iterator_color;
    }
    return NULL;
}

QList<AlarmColor *> *ColorCopyDialog::getDestinationColorList(QList<QString> list)
{

}

void ColorCopyDialog::Slot_ChangedCombobox(int index)
{
    QString qstr_sendername = sender()->objectName();
    if(ui->m_qcombobox_source->objectName().operator ==(qstr_sendername.toLatin1()))
    {
        ui->m_qlistwidget_destination->clear();
        initListWidgetDestination(ui->m_qcombobox_source->currentText());
    }
    return;
}

void ColorCopyDialog::Slot_ClickedButton(bool status)
{
    QString qstr_sendername = sender()->objectName();
    if(ui->m_qpushbutton_enter->objectName().operator ==(qstr_sendername.toLatin1()))
    {
        if(ui->m_qcombobox_source->currentText().isEmpty())
            return;
        QList<QListWidgetItem*> qlist_items = ui->m_qlistwidget_destination->selectedItems();
        if(qlist_items.isEmpty())
            return;
        AlarmColor* palarmcolor_temp = NULL;
        QList<QListWidgetItem*>::iterator iterator_items = qlist_items.begin();
        while(qlist_items.end() != iterator_items)
        {
            palarmcolor_temp = getDestinationColor((*iterator_items)->text());
            if(palarmcolor_temp)
            {
                palarmcolor_temp->m_qstr_alarm_text.operator =(getSourceColor()->m_qstr_alarm_text);
                palarmcolor_temp->m_qstr_alarm_back.operator =(getSourceColor()->m_qstr_alarm_back);
                palarmcolor_temp->m_qstr_alarm_flash_text.operator =(getSourceColor()->m_qstr_alarm_flash_text);
                palarmcolor_temp->m_qstr_alarm_flash_back.operator =(getSourceColor()->m_qstr_alarm_flash_back);
                palarmcolor_temp->m_qstr_recover_text.operator =(getSourceColor()->m_qstr_recover_text);
                palarmcolor_temp->m_qstr_recover_back.operator =(getSourceColor()->m_qstr_recover_back);
                palarmcolor_temp->m_qstr_recover_flash_text.operator =(getSourceColor()->m_qstr_recover_flash_text);
                palarmcolor_temp->m_qstr_recover_flash_back.operator =(getSourceColor()->m_qstr_recover_flash_back);
            }
            ++iterator_items;
        }
        QMessageBox::information(this,"复制信息","复制完成！");
        ui->m_qlistwidget_destination->clearSelection();
    }
    else if(ui->m_qpushbutton_cancel->objectName().operator ==(qstr_sendername.toLatin1()))
    {
        this->close();
    }
    return;

}

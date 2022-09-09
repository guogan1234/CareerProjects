#include "activecombobox.h"

ActiveComboBox::ActiveComboBox(QWidget *parent) : QComboBox(parent)
{

}

bool ActiveComboBox::event(QEvent *event)
{
    if(event->type() == QEvent::Enter){
        qDebug("ActiveComboBox begin to emit enterSignal().");
        emit enterSignal();
    }
    return QComboBox::event(event);
}

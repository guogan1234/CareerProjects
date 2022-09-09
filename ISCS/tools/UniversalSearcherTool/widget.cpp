#include "widget.h"
#include "ui_widget.h"

#include <QVBoxLayout>
#include "searcheasyform.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    QVBoxLayout* mainLay = new QVBoxLayout();
    searchEasyForm* form = new searchEasyForm();
    mainLay->addWidget(form,9);
    mainLay->setContentsMargins(0,0,0,0);
    mainLay->setSpacing(0);

    setLayout(mainLay);
    resize(400,620);
}

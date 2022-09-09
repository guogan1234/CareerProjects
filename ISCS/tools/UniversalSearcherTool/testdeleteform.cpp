#include "testdeleteform.h"
#include "ui_testdeleteform.h"

#include "searcheasyform.h"
#include <QThread>

TestDeleteForm::TestDeleteForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestDeleteForm)
{
    ui->setupUi(this);
}

TestDeleteForm::~TestDeleteForm()
{
    delete ui;
}

void TestDeleteForm::on_pushButton_clicked()
{
    form = new searchEasyForm();
    form->show();
}

void TestDeleteForm::on_pushButton_2_clicked()
{
    delete form;
}

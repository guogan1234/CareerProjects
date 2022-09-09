#ifndef TESTDELETEFORM_H
#define TESTDELETEFORM_H

#include <QWidget>
#include "searcheasyform.h"

namespace Ui {
class TestDeleteForm;
}

class TestDeleteForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestDeleteForm(QWidget *parent = 0);
    ~TestDeleteForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TestDeleteForm *ui;

    searchEasyForm* form;
};

#endif // TESTDELETEFORM_H

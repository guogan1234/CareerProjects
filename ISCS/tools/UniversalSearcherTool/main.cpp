#include "widget.h"
#include <QApplication>
#include "testdeleteform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    TestDeleteForm form;
    form.show();

    return a.exec();
}

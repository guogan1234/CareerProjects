/********************************************************************************
** Form generated from reading UI file 'searcherform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHERFORM_H
#define UI_SEARCHERFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "activecombobox.h"
#include "activetableview.h"

QT_BEGIN_NAMESPACE

class Ui_SearcherForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *RB_cfdb;
    QRadioButton *RB_tsdb;
    QRadioButton *RB_rtdb;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *CB_domain;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    ActiveComboBox *CB_station;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *CB_system;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_7;
    QWidget *deviceW;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    ActiveComboBox *CB_devType;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *LE_device;
    ActiveTableView *TV_device;
    QWidget *pointW;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QComboBox *CB_pointType;
    QSpacerItem *horizontalSpacer;
    ActiveTableView *TV_point;
    QWidget *valueW;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_2;
    ActiveTableView *TV_pointHeader;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_11;
    QWidget *widget;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_9;
    QLineEdit *lineEdit_2;
    QTableView *TV_table;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_11;
    QLineEdit *lineEdit_3;
    QTableView *TV_record;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_5;
    QTableView *TV_recordHeader;

    void setupUi(QWidget *SearcherForm)
    {
        if (SearcherForm->objectName().isEmpty())
            SearcherForm->setObjectName(QStringLiteral("SearcherForm"));
        SearcherForm->resize(367, 573);
        verticalLayout_3 = new QVBoxLayout(SearcherForm);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        groupBox = new QGroupBox(SearcherForm);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        RB_cfdb = new QRadioButton(groupBox);
        RB_cfdb->setObjectName(QStringLiteral("RB_cfdb"));

        verticalLayout->addWidget(RB_cfdb);

        RB_tsdb = new QRadioButton(groupBox);
        RB_tsdb->setObjectName(QStringLiteral("RB_tsdb"));

        verticalLayout->addWidget(RB_tsdb);

        RB_rtdb = new QRadioButton(groupBox);
        RB_rtdb->setObjectName(QStringLiteral("RB_rtdb"));

        verticalLayout->addWidget(RB_rtdb);


        horizontalLayout_4->addWidget(groupBox);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(SearcherForm);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        CB_domain = new QComboBox(SearcherForm);
        CB_domain->setObjectName(QStringLiteral("CB_domain"));

        horizontalLayout->addWidget(CB_domain);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);

        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(SearcherForm);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        CB_station = new ActiveComboBox(SearcherForm);
        CB_station->setObjectName(QStringLiteral("CB_station"));

        horizontalLayout_2->addWidget(CB_station);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 3);

        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(SearcherForm);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        CB_system = new QComboBox(SearcherForm);
        CB_system->setObjectName(QStringLiteral("CB_system"));

        horizontalLayout_3->addWidget(CB_system);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 3);

        verticalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout_2);

        horizontalLayout_4->setStretch(1, 5);

        verticalLayout_3->addLayout(horizontalLayout_4);

        tabWidget = new QTabWidget(SearcherForm);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_7 = new QVBoxLayout(tab);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        deviceW = new QWidget(tab);
        deviceW->setObjectName(QStringLiteral("deviceW"));
        verticalLayout_4 = new QVBoxLayout(deviceW);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_4 = new QLabel(deviceW);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        CB_devType = new ActiveComboBox(deviceW);
        CB_devType->setObjectName(QStringLiteral("CB_devType"));

        horizontalLayout_5->addWidget(CB_devType);


        horizontalLayout_7->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(deviceW);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        LE_device = new QLineEdit(deviceW);
        LE_device->setObjectName(QStringLiteral("LE_device"));
        LE_device->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_6->addWidget(LE_device);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 3);

        horizontalLayout_7->addLayout(horizontalLayout_6);


        verticalLayout_4->addLayout(horizontalLayout_7);

        TV_device = new ActiveTableView(deviceW);
        TV_device->setObjectName(QStringLiteral("TV_device"));

        verticalLayout_4->addWidget(TV_device);


        verticalLayout_7->addWidget(deviceW);

        pointW = new QWidget(tab);
        pointW->setObjectName(QStringLiteral("pointW"));
        verticalLayout_5 = new QVBoxLayout(pointW);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_6 = new QLabel(pointW);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_8->addWidget(label_6);

        CB_pointType = new QComboBox(pointW);
        CB_pointType->setObjectName(QStringLiteral("CB_pointType"));

        horizontalLayout_8->addWidget(CB_pointType);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        verticalLayout_5->addLayout(horizontalLayout_8);

        TV_point = new ActiveTableView(pointW);
        TV_point->setObjectName(QStringLiteral("TV_point"));

        verticalLayout_5->addWidget(TV_point);


        verticalLayout_7->addWidget(pointW);

        valueW = new QWidget(tab);
        valueW->setObjectName(QStringLiteral("valueW"));
        verticalLayout_6 = new QVBoxLayout(valueW);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(valueW);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_2);


        verticalLayout_6->addLayout(horizontalLayout_9);

        TV_pointHeader = new ActiveTableView(valueW);
        TV_pointHeader->setObjectName(QStringLiteral("TV_pointHeader"));

        verticalLayout_6->addWidget(TV_pointHeader);


        verticalLayout_7->addWidget(valueW);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_11 = new QVBoxLayout(tab_2);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(tab_2);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_8 = new QVBoxLayout(widget);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(widget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_10->addWidget(label_8);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_3);

        label_9 = new QLabel(widget);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_10->addWidget(label_9);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_10->addWidget(lineEdit_2);


        verticalLayout_8->addLayout(horizontalLayout_10);

        TV_table = new QTableView(widget);
        TV_table->setObjectName(QStringLiteral("TV_table"));

        verticalLayout_8->addWidget(TV_table);


        verticalLayout_11->addWidget(widget);

        widget_2 = new QWidget(tab_2);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_9 = new QVBoxLayout(widget_2);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_10 = new QLabel(widget_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_11->addWidget(label_10);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_4);

        label_11 = new QLabel(widget_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_11->addWidget(label_11);

        lineEdit_3 = new QLineEdit(widget_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_11->addWidget(lineEdit_3);


        verticalLayout_9->addLayout(horizontalLayout_11);

        TV_record = new QTableView(widget_2);
        TV_record->setObjectName(QStringLiteral("TV_record"));

        verticalLayout_9->addWidget(TV_record);


        verticalLayout_11->addWidget(widget_2);

        widget_3 = new QWidget(tab_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        verticalLayout_10 = new QVBoxLayout(widget_3);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_12 = new QLabel(widget_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_12->addWidget(label_12);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_5);


        verticalLayout_10->addLayout(horizontalLayout_12);

        TV_recordHeader = new QTableView(widget_3);
        TV_recordHeader->setObjectName(QStringLiteral("TV_recordHeader"));

        verticalLayout_10->addWidget(TV_recordHeader);


        verticalLayout_11->addWidget(widget_3);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);


        retranslateUi(SearcherForm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SearcherForm);
    } // setupUi

    void retranslateUi(QWidget *SearcherForm)
    {
        SearcherForm->setWindowTitle(QApplication::translate("SearcherForm", "Form", Q_NULLPTR));
        groupBox->setTitle(QString());
        RB_cfdb->setText(QApplication::translate("SearcherForm", "\351\205\215\347\275\256\345\272\223", Q_NULLPTR));
        RB_tsdb->setText(QApplication::translate("SearcherForm", "\345\216\206\345\217\262\345\272\223", Q_NULLPTR));
        RB_rtdb->setText(QApplication::translate("SearcherForm", "\345\256\236\346\227\266\345\272\223", Q_NULLPTR));
        label->setText(QApplication::translate("SearcherForm", "\345\237\237\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("SearcherForm", "\350\275\246\347\253\231\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("SearcherForm", "\344\270\223\344\270\232\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("SearcherForm", "\350\256\276\345\244\207\347\261\273\345\236\213\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("SearcherForm", "\350\277\207\346\273\244\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("SearcherForm", "\347\202\271\347\261\273\345\236\213\357\274\232", Q_NULLPTR));
        CB_pointType->clear();
        CB_pointType->insertItems(0, QStringList()
         << QApplication::translate("SearcherForm", "\347\264\257\347\247\257\351\207\217(acc)", Q_NULLPTR)
         << QApplication::translate("SearcherForm", "\346\250\241\346\213\237\351\207\217(ai)", Q_NULLPTR)
         << QApplication::translate("SearcherForm", "\347\246\273\346\225\243\351\207\217(di)", Q_NULLPTR)
        );
        label_7->setText(QApplication::translate("SearcherForm", "\345\261\236\346\200\247\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SearcherForm", "\350\256\276\345\244\207", Q_NULLPTR));
        label_8->setText(QApplication::translate("SearcherForm", "\350\241\250\345\220\215\357\274\232", Q_NULLPTR));
        label_9->setText(QApplication::translate("SearcherForm", "\350\277\207\346\273\244\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("SearcherForm", "\347\202\271\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("SearcherForm", "\350\277\207\346\273\244\357\274\232", Q_NULLPTR));
        label_12->setText(QApplication::translate("SearcherForm", "\345\261\236\346\200\247\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SearcherForm", "\350\241\250", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SearcherForm: public Ui_SearcherForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHERFORM_H

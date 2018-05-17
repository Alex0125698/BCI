/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "GraphWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QWidget *wtitle;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_5;
    QFrame *line;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    GraphWidgetWrapper *plot_time;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_8;
    QToolButton *btn_connect;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_4;
    QProgressBar *progress_battery;
    QVBoxLayout *verticalLayout_2;
    GraphWidgetWrapper *plot_freq;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_2;
    QWidget *plot_channels;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setStyleSheet(QLatin1String("QWidget#centralwidget{\n"
"background-color: rgb(200, 200, 200);\n"
"}"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        wtitle = new QWidget(centralwidget);
        wtitle->setObjectName(QStringLiteral("wtitle"));
        wtitle->setStyleSheet(QLatin1String("QWidget#wtitle {background-color: qlineargradient(spread:pad, x1:0.546, y1:0, x2:0.539909, y2:0.489, stop:0 rgba(192, 143, 101, 255), stop:1 rgba(255, 255, 255, 255));}\n"
"\n"
"\n"
""));
        horizontalLayout_7 = new QHBoxLayout(wtitle);
        horizontalLayout_7->setSpacing(2);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(3, 3, 3, 3);
        label = new QLabel(wtitle);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(100, 100));
        QFont font;
        font.setFamily(QStringLiteral("Tekton Pro Ext"));
        label->setFont(font);
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/images/tmp2.png")));
        label->setScaledContents(false);

        horizontalLayout_7->addWidget(label);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        label_2 = new QLabel(wtitle);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(16777215, 100));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial Rounded MT Bold"));
        label_2->setFont(font1);

        verticalLayout_3->addWidget(label_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        line_2 = new QFrame(wtitle);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(wtitle);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        horizontalLayout_4->addWidget(label_3);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        line = new QFrame(wtitle);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShadow(QFrame::Raised);
        line->setFrameShape(QFrame::VLine);

        horizontalLayout_4->addWidget(line);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        label_6 = new QLabel(wtitle);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font1);

        horizontalLayout_4->addWidget(label_6);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        horizontalLayout_4->setStretch(1, 1);
        horizontalLayout_4->setStretch(3, 1);
        horizontalLayout_4->setStretch(5, 12);

        verticalLayout_3->addLayout(horizontalLayout_4);


        horizontalLayout_7->addLayout(verticalLayout_3);


        verticalLayout_4->addWidget(wtitle);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        plot_time = new GraphWidgetWrapper(centralwidget);
        plot_time->setObjectName(QStringLiteral("plot_time"));
        plot_time->setMinimumSize(QSize(350, 200));

        verticalLayout->addWidget(plot_time);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setStyleSheet(QStringLiteral("QWidget#widget_2{background-color: rgb(255, 255, 255);}"));
        horizontalLayout_8 = new QHBoxLayout(widget_2);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        btn_connect = new QToolButton(widget_2);
        btn_connect->setObjectName(QStringLiteral("btn_connect"));
        btn_connect->setMinimumSize(QSize(100, 0));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/arrow_green.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_connect->setIcon(icon);
        btn_connect->setIconSize(QSize(32, 32));
        btn_connect->setCheckable(true);
        btn_connect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn_connect->setAutoRaise(false);

        horizontalLayout_8->addWidget(btn_connect);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        horizontalLayout_2->addWidget(widget_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(10, 0));
        widget_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        horizontalLayout_9 = new QHBoxLayout(widget_3);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_4 = new QLabel(widget_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_9->addWidget(label_4);

        progress_battery = new QProgressBar(widget_3);
        progress_battery->setObjectName(QStringLiteral("progress_battery"));
        progress_battery->setMinimumSize(QSize(280, 0));
        progress_battery->setValue(0);
        progress_battery->setTextVisible(true);

        horizontalLayout_9->addWidget(progress_battery);


        horizontalLayout->addWidget(widget_3);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);

        horizontalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        plot_freq = new GraphWidgetWrapper(centralwidget);
        plot_freq->setObjectName(QStringLiteral("plot_freq"));
        plot_freq->setMinimumSize(QSize(350, 200));

        verticalLayout_2->addWidget(plot_freq);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        horizontalLayout_6 = new QHBoxLayout(widget);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        QFont font2;
        font2.setFamily(QStringLiteral("Cooper Black"));
        label_5->setFont(font2);

        horizontalLayout_6->addWidget(label_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        horizontalLayout_3->addWidget(widget);


        verticalLayout_2->addLayout(horizontalLayout_3);

        plot_channels = new QWidget(centralwidget);
        plot_channels->setObjectName(QStringLiteral("plot_channels"));
        plot_channels->setMinimumSize(QSize(350, 100));
        plot_channels->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        verticalLayout_2->addWidget(plot_channels);

        verticalLayout_2->setStretch(0, 1);

        horizontalLayout_5->addLayout(verticalLayout_2);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 1);

        verticalLayout_4->addLayout(horizontalLayout_5);

        verticalLayout_4->setStretch(0, 1);
        verticalLayout_4->setStretch(1, 10);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt; color:#3b4771;\">Brain-Computer Interface</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#78562f;\">Version 1.0</span></p></body></html>", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#78562f;\">by A.S Woodcock &amp; A. Vasilyev</span></p></body></html>", nullptr));
        btn_connect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Battery:</span></p></body></html>", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; color:#3a435f;\">Contact Quality:</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

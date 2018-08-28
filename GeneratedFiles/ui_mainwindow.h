/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <view.h>
#include "game.h"
#include "graphwidget.h"
#include "pixplotter.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *tabWidget;
    QWidget *tab_home;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_top;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QToolButton *btn_uoa;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QFrame *line_2;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_7;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox_2;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QLineEdit *line_out_file;
    QToolButton *btn_out_file;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *btn_save_options;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *btn_save_start;
    QToolButton *btn_save_stop;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_7;
    QWidget *tab_data;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QComboBox *box_source;
    QToolButton *btn_connect;
    QSpacerItem *verticalSpacer_5;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_4;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_8;
    QProgressBar *progressBar;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_10;
    QWidget *tab_time;
    QVBoxLayout *verticalLayout_9;
    GraphWidget *plot_time;
    QWidget *tab_freq;
    QVBoxLayout *verticalLayout_10;
    GraphWidget *plot_freq;
    QHBoxLayout *horizontalLayout_9;
    StateGroup *group_freqs;
    QWidget *widget_4;
    QWidget *tab_activity;
    QVBoxLayout *verticalLayout_11;
    PixPlotter *openGLWidget;
    QWidget *tab_stft;
    QVBoxLayout *verticalLayout_12;
    Game *openGLWidget_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(863, 577);
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setStyleSheet(QLatin1String("QWidget#centralwidget{\n"
"background-color: rgb(200, 200, 200);\n"
"}"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 6);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_home = new QWidget();
        tab_home->setObjectName(QStringLiteral("tab_home"));
        tab_home->setStyleSheet(QLatin1String("QWidget#tab_home{\n"
"background-image: url(:/images/images/background-dark.png);\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(tab_home);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget_top = new QWidget(tab_home);
        widget_top->setObjectName(QStringLiteral("widget_top"));
        widget_top->setStyleSheet(QLatin1String("QWidget#widget_top{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(widget_top);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btn_uoa = new QToolButton(widget_top);
        btn_uoa->setObjectName(QStringLiteral("btn_uoa"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/images/UoA_Logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_uoa->setIcon(icon);
        btn_uoa->setIconSize(QSize(230, 106));
        btn_uoa->setAutoRaise(true);
        btn_uoa->setArrowType(Qt::NoArrow);

        horizontalLayout->addWidget(btn_uoa);

        label_2 = new QLabel(widget_top);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(16777215, 80));
        QFont font;
        font.setFamily(QStringLiteral("Cooper Std Black"));
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout->addWidget(label_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        line = new QFrame(widget_top);
        line->setObjectName(QStringLiteral("line"));
        line->setStyleSheet(QLatin1String("QFrame#line\n"
"{\n"
"color: rgb(139, 149, 157)\n"
"}"));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        verticalLayout_2->addWidget(line);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_3 = new QLabel(widget_top);
        label_3->setObjectName(QStringLiteral("label_3"));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe UI Black"));
        font1.setBold(true);
        font1.setWeight(75);
        label_3->setFont(font1);

        horizontalLayout_2->addWidget(label_3);

        line_2 = new QFrame(widget_top);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setStyleSheet(QLatin1String("QFrame#line_2\n"
"{\n"
"color: rgb(139, 149, 157)\n"
"}"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setFrameShape(QFrame::VLine);

        horizontalLayout_2->addWidget(line_2);

        label_4 = new QLabel(widget_top);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font1);

        horizontalLayout_2->addWidget(label_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(widget_top);

        label_7 = new QLabel(tab_home);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setStyleSheet(QStringLiteral("image: url(:/images/images/Emotiv.png);"));

        verticalLayout_3->addWidget(label_7);

        verticalSpacer = new QSpacerItem(20, 19, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        groupBox = new QGroupBox(tab_home);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(200, 110));
        groupBox->setStyleSheet(QLatin1String("QWidget#groupBox{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));

        horizontalLayout_3->addWidget(groupBox);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        groupBox_2 = new QGroupBox(tab_home);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(200, 110));
        groupBox_2->setStyleSheet(QLatin1String("QWidget#groupBox_2{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));

        horizontalLayout_3->addWidget(groupBox_2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        groupBox_3 = new QGroupBox(tab_home);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(200, 110));
        groupBox_3->setStyleSheet(QLatin1String("QWidget#groupBox_3{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout = new QVBoxLayout(groupBox_3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout->addWidget(label_5);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_5->addWidget(label_6);

        line_out_file = new QLineEdit(groupBox_3);
        line_out_file->setObjectName(QStringLiteral("line_out_file"));

        horizontalLayout_5->addWidget(line_out_file);

        btn_out_file = new QToolButton(groupBox_3);
        btn_out_file->setObjectName(QStringLiteral("btn_out_file"));

        horizontalLayout_5->addWidget(btn_out_file);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        btn_save_options = new QToolButton(groupBox_3);
        btn_save_options->setObjectName(QStringLiteral("btn_save_options"));
        btn_save_options->setMinimumSize(QSize(0, 0));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/Settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_save_options->setIcon(icon1);
        btn_save_options->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(btn_save_options);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        btn_save_start = new QToolButton(groupBox_3);
        btn_save_start->setObjectName(QStringLiteral("btn_save_start"));
        btn_save_start->setMinimumSize(QSize(0, 0));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/icons/Save.JPG"), QSize(), QIcon::Normal, QIcon::Off);
        btn_save_start->setIcon(icon2);
        btn_save_start->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(btn_save_start);

        btn_save_stop = new QToolButton(groupBox_3);
        btn_save_stop->setObjectName(QStringLiteral("btn_save_stop"));
        btn_save_stop->setMinimumSize(QSize(0, 0));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/icons/Stop_32.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_save_stop->setIcon(icon3);
        btn_save_stop->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(btn_save_stop);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_3->addWidget(groupBox_3);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        horizontalLayout_3->setStretch(0, 9);
        horizontalLayout_3->setStretch(1, 1);
        horizontalLayout_3->setStretch(2, 2);
        horizontalLayout_3->setStretch(3, 1);
        horizontalLayout_3->setStretch(4, 2);
        horizontalLayout_3->setStretch(5, 1);
        horizontalLayout_3->setStretch(6, 9);

        verticalLayout_3->addLayout(horizontalLayout_3);

        verticalLayout_3->setStretch(0, 2);
        verticalLayout_3->setStretch(1, 12);
        verticalLayout_3->setStretch(2, 1);
        verticalLayout_3->setStretch(3, 1);
        tabWidget->addTab(tab_home, QString());
        tab_data = new QWidget();
        tab_data->setObjectName(QStringLiteral("tab_data"));
        horizontalLayout_8 = new QHBoxLayout(tab_data);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        groupBox_4 = new QGroupBox(tab_data);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout_5 = new QVBoxLayout(groupBox_4);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        box_source = new QComboBox(groupBox_4);
        box_source->addItem(QString());
        box_source->addItem(QString());
        box_source->setObjectName(QStringLiteral("box_source"));

        verticalLayout_5->addWidget(box_source);

        btn_connect = new QToolButton(groupBox_4);
        btn_connect->setObjectName(QStringLiteral("btn_connect"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/icons/arrow_green.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_connect->setIcon(icon4);
        btn_connect->setIconSize(QSize(32, 32));
        btn_connect->setCheckable(true);
        btn_connect->setChecked(false);
        btn_connect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        verticalLayout_5->addWidget(btn_connect);


        verticalLayout_8->addWidget(groupBox_4);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_5);

        groupBox_6 = new QGroupBox(tab_data);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        verticalLayout_7 = new QVBoxLayout(groupBox_6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_7->addWidget(label_9);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_9);


        verticalLayout_7->addLayout(horizontalLayout_7);

        verticalSpacer_4 = new QSpacerItem(20, 77, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_4);


        verticalLayout_8->addWidget(groupBox_6);

        groupBox_5 = new QGroupBox(tab_data);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        verticalLayout_6 = new QVBoxLayout(groupBox_5);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_6->addWidget(label_8);

        progressBar = new QProgressBar(groupBox_5);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        horizontalLayout_6->addWidget(progressBar);


        verticalLayout_6->addLayout(horizontalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 75, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_3);


        verticalLayout_8->addWidget(groupBox_5);


        horizontalLayout_8->addLayout(verticalLayout_8);

        horizontalSpacer_10 = new QSpacerItem(552, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_10);

        horizontalLayout_8->setStretch(0, 1);
        horizontalLayout_8->setStretch(1, 2);
        tabWidget->addTab(tab_data, QString());
        tab_time = new QWidget();
        tab_time->setObjectName(QStringLiteral("tab_time"));
        verticalLayout_9 = new QVBoxLayout(tab_time);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        plot_time = new GraphWidget(tab_time);
        plot_time->setObjectName(QStringLiteral("plot_time"));

        verticalLayout_9->addWidget(plot_time);

        tabWidget->addTab(tab_time, QString());
        tab_freq = new QWidget();
        tab_freq->setObjectName(QStringLiteral("tab_freq"));
        verticalLayout_10 = new QVBoxLayout(tab_freq);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        plot_freq = new GraphWidget(tab_freq);
        plot_freq->setObjectName(QStringLiteral("plot_freq"));

        verticalLayout_10->addWidget(plot_freq);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        group_freqs = new StateGroup(tab_freq);
        group_freqs->setObjectName(QStringLiteral("group_freqs"));

        horizontalLayout_9->addWidget(group_freqs);

        widget_4 = new QWidget(tab_freq);
        widget_4->setObjectName(QStringLiteral("widget_4"));

        horizontalLayout_9->addWidget(widget_4);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 1);

        verticalLayout_10->addLayout(horizontalLayout_9);

        verticalLayout_10->setStretch(0, 3);
        verticalLayout_10->setStretch(1, 1);
        tabWidget->addTab(tab_freq, QString());
        tab_activity = new QWidget();
        tab_activity->setObjectName(QStringLiteral("tab_activity"));
        verticalLayout_11 = new QVBoxLayout(tab_activity);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        openGLWidget = new PixPlotter(tab_activity);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        verticalLayout_11->addWidget(openGLWidget);

        tabWidget->addTab(tab_activity, QString());
        tab_stft = new QWidget();
        tab_stft->setObjectName(QStringLiteral("tab_stft"));
        verticalLayout_12 = new QVBoxLayout(tab_stft);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        openGLWidget_2 = new Game(tab_stft);
        openGLWidget_2->setObjectName(QStringLiteral("openGLWidget_2"));

        verticalLayout_12->addWidget(openGLWidget_2);

        tabWidget->addTab(tab_stft, QString());

        verticalLayout_4->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_uoa->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt; color:#d9e9f5;\">BCI Controller</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt; color:#a2b1c3;\">Version 2.0.0</span></p></body></html>", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt; color:#a2b1c3;\">by A.S. Woodcock &amp; A. Vasilyev</span></p></body></html>", nullptr));
        label_7->setText(QString());
        groupBox->setTitle(QString());
        groupBox_2->setTitle(QString());
        groupBox_3->setTitle(QString());
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#ccdbe7;\">Save Options</span></p></body></html>", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ffffff;\">Name:</span></p></body></html>", nullptr));
        btn_out_file->setText(QApplication::translate("MainWindow", "...", nullptr));
        btn_save_options->setText(QString());
        btn_save_start->setText(QApplication::translate("MainWindow", "...", nullptr));
        btn_save_stop->setText(QApplication::translate("MainWindow", "...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_home), QApplication::translate("MainWindow", "Home", nullptr));
        groupBox_4->setTitle(QString());
        box_source->setItemText(0, QApplication::translate("MainWindow", "Emotiv", nullptr));
        box_source->setItemText(1, QApplication::translate("MainWindow", "Offline", nullptr));

        btn_connect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        groupBox_6->setTitle(QString());
        label_9->setText(QApplication::translate("MainWindow", "Contact Quality:", nullptr));
        groupBox_5->setTitle(QString());
        label_8->setText(QApplication::translate("MainWindow", "Battery:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_data), QApplication::translate("MainWindow", "Data Aquisition", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_time), QApplication::translate("MainWindow", "Time Analysis", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_freq), QApplication::translate("MainWindow", "Freq Analysis", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_activity), QApplication::translate("MainWindow", "Brain Activity", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_stft), QApplication::translate("MainWindow", "STFT Plot", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

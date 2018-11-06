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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "balltest.h"
#include "brainmap.h"
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
    QVBoxLayout *verticalLayout_19;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_15;
    QComboBox *box_source;
    QHBoxLayout *horizontalLayout_19;
    QSpacerItem *horizontalSpacer_13;
    QToolButton *btn_connect;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_20;
    QWidget *widget_offline;
    QVBoxLayout *verticalLayout_21;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_17;
    QLineEdit *line_data_in;
    QToolButton *btn_data_in;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_18;
    QSpinBox *box_freq;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_27;
    QLabel *lbl_com_port;
    QComboBox *box_com_port;
    QHBoxLayout *horizontalLayout_24;
    QSpacerItem *horizontalSpacer_17;
    QToolButton *btn_debug_window;
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
    QVBoxLayout *verticalLayout_22;
    QHBoxLayout *horizontalLayout_23;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_9;
    QWidget *widget;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_19;
    QSpacerItem *horizontalSpacer_10;
    QToolButton *toolButton_3;
    QOpenGLWidget *openGLWidget_4;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_21;
    QSpacerItem *horizontalSpacer_16;
    QToolButton *toolButton_4;
    QOpenGLWidget *openGLWidget_5;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_20;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_20;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer_15;
    QWidget *tab_time;
    QVBoxLayout *verticalLayout_9;
    GraphWidget *plot_time;
    QWidget *tab_freq;
    QVBoxLayout *verticalLayout_10;
    GraphWidget *plot_freq;
    QWidget *tab_stft;
    QVBoxLayout *verticalLayout_18;
    PixPlotter *openGLWidget_2;
    QToolButton *btn_stft_larger;
    QGroupBox *groupBox_stft_controls;
    QVBoxLayout *verticalLayout_17;
    QVBoxLayout *verticalLayout_23;
    QHBoxLayout *horizontalLayout_29;
    QLabel *label;
    QSpacerItem *horizontalSpacer_19;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_30;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_10;
    QSpinBox *box_wnd_size;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QSpinBox *box_wnd_overlap;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_22;
    QSpinBox *box_time_span;
    QSpacerItem *horizontalSpacer_12;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout_31;
    QLabel *label_25;
    QSlider *slider_max_freq;
    QHBoxLayout *horizontalLayout_32;
    QLabel *label_26;
    QSlider *slider_brightness;
    QHBoxLayout *horizontalLayout_33;
    QLabel *label_27;
    QSlider *slider_dark_boost;
    QHBoxLayout *horizontalLayout_26;
    QLabel *label_24;
    QSlider *slider_hard_limit;
    QSpacerItem *horizontalSpacer_11;
    QVBoxLayout *verticalLayout_15;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_14;
    QSlider *slider_blur_size;
    QHBoxLayout *horizontalLayout_15;
    QLabel *lab;
    QSlider *slider_blur_amount;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QSlider *slider_sharpen_size;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_13;
    QSlider *slider_sharpen_amount;
    QSpacerItem *horizontalSpacer_8;
    QVBoxLayout *verticalLayout_16;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_16;
    QComboBox *box_wnd_type;
    QHBoxLayout *horizontalLayout_28;
    QSpacerItem *horizontalSpacer_18;
    QToolButton *btn_stft_enabled;
    QWidget *tab_activity;
    QVBoxLayout *verticalLayout_11;
    BrainMap *openGLWidget;
    QWidget *tab_test;
    QVBoxLayout *verticalLayout_13;
    BallTest *openGLWidget_3;
    QWidget *tab_translation;
    QVBoxLayout *verticalLayout_28;
    QSplitter *splitter;
    GraphWidget *plot_translation_left;
    GraphWidget *plot_translation_right;
    QToolButton *btn_tr_larger;
    QGroupBox *groupBox_translation;
    QVBoxLayout *verticalLayout_27;
    QVBoxLayout *verticalLayout_25;
    QHBoxLayout *horizontalLayout_36;
    QLabel *label_29;
    QSpacerItem *horizontalSpacer_21;
    QFrame *line_4;
    QHBoxLayout *horizontalLayout_40;
    QVBoxLayout *verticalLayout_24;
    QHBoxLayout *horizontalLayout_38;
    QLabel *label_30;
    QSpinBox *box_tr_centre_freq;
    QLabel *label_32;
    QSpinBox *box_tr_span;
    QLabel *label_33;
    QSpinBox *box_tr_spreading;
    QSpacerItem *horizontalSpacer_20;
    QCheckBox *box_tr_flip;
    QHBoxLayout *horizontalLayout_35;
    QLabel *label_34;
    QSlider *slider_tr_sensitivity;
    QHBoxLayout *horizontalLayout_43;
    QLabel *label_42;
    QSlider *slider_tr_smoothing;
    QVBoxLayout *verticalLayout_26;
    QHBoxLayout *horizontalLayout_37;
    QLabel *label_31;
    QSlider *slider_tr_gain;
    QLabel *lbl_tr_gain;
    QToolButton *btn_tr_auto_gain;
    QHBoxLayout *horizontalLayout_39;
    QLabel *label_35;
    QSlider *slider_tr_offset;
    QLabel *lbl_tr_off;
    QToolButton *btn_tr_auto_offset;
    QHBoxLayout *horizontalLayout_41;
    QLabel *label_36;
    QLabel *label_37;
    QSlider *slider_tr_off_speed;
    QLabel *label_38;
    QSlider *slider_tr_off_limit;
    QHBoxLayout *horizontalLayout_42;
    QLabel *label_39;
    QLabel *label_40;
    QSlider *slider_tr_mag_speed;
    QLabel *label_41;
    QSlider *slider_tr_offset_5;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(861, 577);
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        MainWindow->setStyleSheet(QLatin1String("QMainWindow#MainWindow{\n"
"background-color: rgb(220,220,220);\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setStyleSheet(QLatin1String("QWidget#centralwidget{\n"
"background-color: rgb(20,20,30);\n"
"}"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setStyleSheet(QLatin1String("QWidget#tab_stft{\n"
"background-image: url(:/images/images/background-dark.png);\n"
"}"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setMovable(true);
        tabWidget->setTabBarAutoHide(false);
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
        verticalLayout_19 = new QVBoxLayout(groupBox);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_18->addWidget(label_15);

        box_source = new QComboBox(groupBox);
        box_source->addItem(QString());
        box_source->addItem(QString());
        box_source->addItem(QString());
        box_source->setObjectName(QStringLiteral("box_source"));

        horizontalLayout_18->addWidget(box_source);


        verticalLayout_19->addLayout(horizontalLayout_18);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_13);

        btn_connect = new QToolButton(groupBox);
        btn_connect->setObjectName(QStringLiteral("btn_connect"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/arrow_green.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_connect->setIcon(icon1);
        btn_connect->setIconSize(QSize(32, 32));
        btn_connect->setCheckable(true);
        btn_connect->setChecked(false);
        btn_connect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout_19->addWidget(btn_connect);


        verticalLayout_19->addLayout(horizontalLayout_19);


        horizontalLayout_3->addWidget(groupBox);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        groupBox_2 = new QGroupBox(tab_home);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(200, 110));
        groupBox_2->setStyleSheet(QLatin1String("QWidget#groupBox_2{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout_20 = new QVBoxLayout(groupBox_2);
        verticalLayout_20->setSpacing(4);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        verticalLayout_20->setContentsMargins(9, 6, 9, 6);
        widget_offline = new QWidget(groupBox_2);
        widget_offline->setObjectName(QStringLiteral("widget_offline"));
        verticalLayout_21 = new QVBoxLayout(widget_offline);
        verticalLayout_21->setSpacing(4);
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        verticalLayout_21->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        label_17 = new QLabel(widget_offline);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_21->addWidget(label_17);

        line_data_in = new QLineEdit(widget_offline);
        line_data_in->setObjectName(QStringLiteral("line_data_in"));

        horizontalLayout_21->addWidget(line_data_in);

        btn_data_in = new QToolButton(widget_offline);
        btn_data_in->setObjectName(QStringLiteral("btn_data_in"));

        horizontalLayout_21->addWidget(btn_data_in);


        verticalLayout_21->addLayout(horizontalLayout_21);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        label_18 = new QLabel(widget_offline);
        label_18->setObjectName(QStringLiteral("label_18"));

        horizontalLayout_17->addWidget(label_18);

        box_freq = new QSpinBox(widget_offline);
        box_freq->setObjectName(QStringLiteral("box_freq"));
        box_freq->setMinimum(1);
        box_freq->setMaximum(2048);
        box_freq->setValue(250);

        horizontalLayout_17->addWidget(box_freq);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_14);


        verticalLayout_21->addLayout(horizontalLayout_17);


        verticalLayout_20->addWidget(widget_offline);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        lbl_com_port = new QLabel(groupBox_2);
        lbl_com_port->setObjectName(QStringLiteral("lbl_com_port"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbl_com_port->sizePolicy().hasHeightForWidth());
        lbl_com_port->setSizePolicy(sizePolicy);

        horizontalLayout_27->addWidget(lbl_com_port);

        box_com_port = new QComboBox(groupBox_2);
        box_com_port->setObjectName(QStringLiteral("box_com_port"));

        horizontalLayout_27->addWidget(box_com_port);

        horizontalLayout_27->setStretch(1, 1);

        verticalLayout_20->addLayout(horizontalLayout_27);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_24->addItem(horizontalSpacer_17);

        btn_debug_window = new QToolButton(groupBox_2);
        btn_debug_window->setObjectName(QStringLiteral("btn_debug_window"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/icons/debug.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_debug_window->setIcon(icon2);
        btn_debug_window->setCheckable(true);
        btn_debug_window->setChecked(true);
        btn_debug_window->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn_debug_window->setAutoRaise(false);

        horizontalLayout_24->addWidget(btn_debug_window);


        verticalLayout_20->addLayout(horizontalLayout_24);


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
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/icons/Settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_save_options->setIcon(icon3);
        btn_save_options->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(btn_save_options);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        btn_save_start = new QToolButton(groupBox_3);
        btn_save_start->setObjectName(QStringLiteral("btn_save_start"));
        btn_save_start->setMinimumSize(QSize(0, 0));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/icons/Save.JPG"), QSize(), QIcon::Normal, QIcon::Off);
        btn_save_start->setIcon(icon4);
        btn_save_start->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(btn_save_start);

        btn_save_stop = new QToolButton(groupBox_3);
        btn_save_stop->setObjectName(QStringLiteral("btn_save_stop"));
        btn_save_stop->setEnabled(false);
        btn_save_stop->setMinimumSize(QSize(0, 0));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/icons/Stop_32.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_save_stop->setIcon(icon5);
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
        tab_data->setStyleSheet(QLatin1String("QWidget#tab_data{\n"
"background-image: url(:/images/images/background-dark.png);\n"
"}"));
        verticalLayout_22 = new QVBoxLayout(tab_data);
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        groupBox_5 = new QGroupBox(tab_data);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setStyleSheet(QLatin1String("QWidget#groupBox_5{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout_5 = new QVBoxLayout(groupBox_5);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_9 = new QLabel(groupBox_5);
        label_9->setObjectName(QStringLiteral("label_9"));
        QFont font2;
        font2.setFamily(QStringLiteral("Cooper Std Black"));
        font2.setBold(true);
        font2.setItalic(false);
        font2.setWeight(75);
        label_9->setFont(font2);

        horizontalLayout_6->addWidget(label_9);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_9);


        verticalLayout_5->addLayout(horizontalLayout_6);

        widget = new QWidget(groupBox_5);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 100));

        verticalLayout_5->addWidget(widget);

        verticalLayout_5->setStretch(1, 1);

        horizontalLayout_23->addWidget(groupBox_5);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        groupBox_6 = new QGroupBox(tab_data);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setStyleSheet(QLatin1String("QWidget#groupBox_6{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout_6 = new QVBoxLayout(groupBox_6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_19 = new QLabel(groupBox_6);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setFont(font2);

        horizontalLayout_7->addWidget(label_19);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_10);

        toolButton_3 = new QToolButton(groupBox_6);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(toolButton_3->sizePolicy().hasHeightForWidth());
        toolButton_3->setSizePolicy(sizePolicy1);
        toolButton_3->setIcon(icon1);
        toolButton_3->setCheckable(true);
        toolButton_3->setChecked(true);
        toolButton_3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolButton_3->setAutoRaise(false);

        horizontalLayout_7->addWidget(toolButton_3);


        verticalLayout_6->addLayout(horizontalLayout_7);

        openGLWidget_4 = new QOpenGLWidget(groupBox_6);
        openGLWidget_4->setObjectName(QStringLiteral("openGLWidget_4"));
        openGLWidget_4->setMinimumSize(QSize(0, 100));

        verticalLayout_6->addWidget(openGLWidget_4);

        verticalLayout_6->setStretch(1, 1);

        verticalLayout_8->addWidget(groupBox_6);

        groupBox_7 = new QGroupBox(tab_data);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setStyleSheet(QLatin1String("QWidget#groupBox_7{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout_7 = new QVBoxLayout(groupBox_7);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        label_21 = new QLabel(groupBox_7);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setFont(font2);

        horizontalLayout_22->addWidget(label_21);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_22->addItem(horizontalSpacer_16);

        toolButton_4 = new QToolButton(groupBox_7);
        toolButton_4->setObjectName(QStringLiteral("toolButton_4"));
        sizePolicy1.setHeightForWidth(toolButton_4->sizePolicy().hasHeightForWidth());
        toolButton_4->setSizePolicy(sizePolicy1);
        toolButton_4->setIcon(icon1);
        toolButton_4->setCheckable(true);
        toolButton_4->setChecked(true);
        toolButton_4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolButton_4->setAutoRaise(false);

        horizontalLayout_22->addWidget(toolButton_4);


        verticalLayout_7->addLayout(horizontalLayout_22);

        openGLWidget_5 = new QOpenGLWidget(groupBox_7);
        openGLWidget_5->setObjectName(QStringLiteral("openGLWidget_5"));
        openGLWidget_5->setMinimumSize(QSize(0, 100));

        verticalLayout_7->addWidget(openGLWidget_5);

        verticalLayout_7->setStretch(1, 1);

        verticalLayout_8->addWidget(groupBox_7);


        horizontalLayout_23->addLayout(verticalLayout_8);


        verticalLayout_22->addLayout(horizontalLayout_23);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_22->addItem(verticalSpacer_3);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_20 = new QLabel(tab_data);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setFont(font2);

        horizontalLayout_8->addWidget(label_20);

        progressBar = new QProgressBar(tab_data);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(90);
        progressBar->setTextVisible(false);

        horizontalLayout_8->addWidget(progressBar);


        horizontalLayout_20->addLayout(horizontalLayout_8);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_15);


        verticalLayout_22->addLayout(horizontalLayout_20);

        verticalLayout_22->setStretch(0, 10);
        verticalLayout_22->setStretch(1, 1);
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

        verticalLayout_10->setStretch(0, 3);
        tabWidget->addTab(tab_freq, QString());
        tab_stft = new QWidget();
        tab_stft->setObjectName(QStringLiteral("tab_stft"));
        verticalLayout_18 = new QVBoxLayout(tab_stft);
        verticalLayout_18->setSpacing(0);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        openGLWidget_2 = new PixPlotter(tab_stft);
        openGLWidget_2->setObjectName(QStringLiteral("openGLWidget_2"));

        verticalLayout_18->addWidget(openGLWidget_2);

        btn_stft_larger = new QToolButton(tab_stft);
        btn_stft_larger->setObjectName(QStringLiteral("btn_stft_larger"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btn_stft_larger->sizePolicy().hasHeightForWidth());
        btn_stft_larger->setSizePolicy(sizePolicy2);
        btn_stft_larger->setMaximumSize(QSize(16777215, 14));
        btn_stft_larger->setCheckable(true);
        btn_stft_larger->setChecked(false);
        btn_stft_larger->setAutoRaise(true);
        btn_stft_larger->setArrowType(Qt::DownArrow);

        verticalLayout_18->addWidget(btn_stft_larger);

        groupBox_stft_controls = new QGroupBox(tab_stft);
        groupBox_stft_controls->setObjectName(QStringLiteral("groupBox_stft_controls"));
        groupBox_stft_controls->setStyleSheet(QLatin1String("QWidget#groupBox_stft_controls{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout_17 = new QVBoxLayout(groupBox_stft_controls);
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(-1, 6, -1, 6);
        verticalLayout_23 = new QVBoxLayout();
        verticalLayout_23->setSpacing(0);
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setSpacing(0);
        horizontalLayout_29->setObjectName(QStringLiteral("horizontalLayout_29"));
        label = new QLabel(groupBox_stft_controls);
        label->setObjectName(QStringLiteral("label"));
        QFont font3;
        font3.setFamily(QStringLiteral("Cooper Std Black"));
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setWeight(75);
        label->setFont(font3);

        horizontalLayout_29->addWidget(label);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_29->addItem(horizontalSpacer_19);


        verticalLayout_23->addLayout(horizontalLayout_29);

        line_3 = new QFrame(groupBox_stft_controls);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setStyleSheet(QLatin1String("QFrame#line_3\n"
"{\n"
"color: rgb(139, 149, 157)\n"
"}"));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setFrameShape(QFrame::HLine);

        verticalLayout_23->addWidget(line_3);


        verticalLayout_17->addLayout(verticalLayout_23);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setObjectName(QStringLiteral("horizontalLayout_30"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_10 = new QLabel(groupBox_stft_controls);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setMinimumSize(QSize(110, 0));
        QFont font4;
        font4.setFamily(QStringLiteral("Levenim MT"));
        label_10->setFont(font4);

        horizontalLayout_10->addWidget(label_10);

        box_wnd_size = new QSpinBox(groupBox_stft_controls);
        box_wnd_size->setObjectName(QStringLiteral("box_wnd_size"));
        box_wnd_size->setMaximum(4096);
        box_wnd_size->setValue(128);

        horizontalLayout_10->addWidget(box_wnd_size);


        verticalLayout_12->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_11 = new QLabel(groupBox_stft_controls);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setMinimumSize(QSize(110, 0));
        label_11->setFont(font4);

        horizontalLayout_11->addWidget(label_11);

        box_wnd_overlap = new QSpinBox(groupBox_stft_controls);
        box_wnd_overlap->setObjectName(QStringLiteral("box_wnd_overlap"));
        box_wnd_overlap->setMaximum(4096);
        box_wnd_overlap->setValue(64);

        horizontalLayout_11->addWidget(box_wnd_overlap);


        verticalLayout_12->addLayout(horizontalLayout_11);

        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        label_22 = new QLabel(groupBox_stft_controls);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setMinimumSize(QSize(110, 0));
        label_22->setFont(font4);

        horizontalLayout_25->addWidget(label_22);

        box_time_span = new QSpinBox(groupBox_stft_controls);
        box_time_span->setObjectName(QStringLiteral("box_time_span"));
        box_time_span->setMinimum(6);
        box_time_span->setMaximum(300);
        box_time_span->setValue(20);

        horizontalLayout_25->addWidget(box_time_span);


        verticalLayout_12->addLayout(horizontalLayout_25);


        horizontalLayout_30->addLayout(verticalLayout_12);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_30->addItem(horizontalSpacer_12);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setObjectName(QStringLiteral("horizontalLayout_31"));
        label_25 = new QLabel(groupBox_stft_controls);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setMinimumSize(QSize(66, 0));
        label_25->setFont(font4);

        horizontalLayout_31->addWidget(label_25);

        slider_max_freq = new QSlider(groupBox_stft_controls);
        slider_max_freq->setObjectName(QStringLiteral("slider_max_freq"));
        slider_max_freq->setMinimumSize(QSize(100, 0));
        slider_max_freq->setMaximum(1000);
        slider_max_freq->setValue(500);
        slider_max_freq->setOrientation(Qt::Horizontal);

        horizontalLayout_31->addWidget(slider_max_freq);


        verticalLayout_14->addLayout(horizontalLayout_31);

        horizontalLayout_32 = new QHBoxLayout();
        horizontalLayout_32->setObjectName(QStringLiteral("horizontalLayout_32"));
        label_26 = new QLabel(groupBox_stft_controls);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setMinimumSize(QSize(66, 0));
        label_26->setFont(font4);

        horizontalLayout_32->addWidget(label_26);

        slider_brightness = new QSlider(groupBox_stft_controls);
        slider_brightness->setObjectName(QStringLiteral("slider_brightness"));
        slider_brightness->setMinimumSize(QSize(100, 0));
        slider_brightness->setMaximum(1000);
        slider_brightness->setValue(500);
        slider_brightness->setOrientation(Qt::Horizontal);

        horizontalLayout_32->addWidget(slider_brightness);


        verticalLayout_14->addLayout(horizontalLayout_32);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setObjectName(QStringLiteral("horizontalLayout_33"));
        label_27 = new QLabel(groupBox_stft_controls);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setMinimumSize(QSize(66, 0));
        label_27->setFont(font4);

        horizontalLayout_33->addWidget(label_27);

        slider_dark_boost = new QSlider(groupBox_stft_controls);
        slider_dark_boost->setObjectName(QStringLiteral("slider_dark_boost"));
        slider_dark_boost->setMinimumSize(QSize(100, 0));
        slider_dark_boost->setMaximum(1000);
        slider_dark_boost->setValue(0);
        slider_dark_boost->setOrientation(Qt::Horizontal);

        horizontalLayout_33->addWidget(slider_dark_boost);


        verticalLayout_14->addLayout(horizontalLayout_33);

        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        label_24 = new QLabel(groupBox_stft_controls);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setMinimumSize(QSize(66, 0));
        label_24->setFont(font4);

        horizontalLayout_26->addWidget(label_24);

        slider_hard_limit = new QSlider(groupBox_stft_controls);
        slider_hard_limit->setObjectName(QStringLiteral("slider_hard_limit"));
        slider_hard_limit->setMinimumSize(QSize(100, 0));
        slider_hard_limit->setMaximum(1000);
        slider_hard_limit->setValue(0);
        slider_hard_limit->setOrientation(Qt::Horizontal);

        horizontalLayout_26->addWidget(slider_hard_limit);


        verticalLayout_14->addLayout(horizontalLayout_26);


        horizontalLayout_30->addLayout(verticalLayout_14);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_30->addItem(horizontalSpacer_11);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(3);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_14 = new QLabel(groupBox_stft_controls);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setMinimumSize(QSize(100, 0));
        label_14->setFont(font4);

        horizontalLayout_14->addWidget(label_14);

        slider_blur_size = new QSlider(groupBox_stft_controls);
        slider_blur_size->setObjectName(QStringLiteral("slider_blur_size"));
        slider_blur_size->setMinimumSize(QSize(100, 0));
        slider_blur_size->setMaximum(1000);
        slider_blur_size->setValue(500);
        slider_blur_size->setOrientation(Qt::Horizontal);

        horizontalLayout_14->addWidget(slider_blur_size);


        verticalLayout_15->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        lab = new QLabel(groupBox_stft_controls);
        lab->setObjectName(QStringLiteral("lab"));
        lab->setMinimumSize(QSize(100, 0));
        lab->setFont(font4);

        horizontalLayout_15->addWidget(lab);

        slider_blur_amount = new QSlider(groupBox_stft_controls);
        slider_blur_amount->setObjectName(QStringLiteral("slider_blur_amount"));
        slider_blur_amount->setMinimumSize(QSize(100, 0));
        slider_blur_amount->setMaximum(1000);
        slider_blur_amount->setValue(0);
        slider_blur_amount->setOrientation(Qt::Horizontal);

        horizontalLayout_15->addWidget(slider_blur_amount);


        verticalLayout_15->addLayout(horizontalLayout_15);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_12 = new QLabel(groupBox_stft_controls);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setMinimumSize(QSize(100, 0));
        label_12->setFont(font4);

        horizontalLayout_12->addWidget(label_12);

        slider_sharpen_size = new QSlider(groupBox_stft_controls);
        slider_sharpen_size->setObjectName(QStringLiteral("slider_sharpen_size"));
        slider_sharpen_size->setMinimumSize(QSize(100, 0));
        slider_sharpen_size->setMaximum(1000);
        slider_sharpen_size->setValue(500);
        slider_sharpen_size->setOrientation(Qt::Horizontal);

        horizontalLayout_12->addWidget(slider_sharpen_size);


        verticalLayout_15->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_13 = new QLabel(groupBox_stft_controls);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setMinimumSize(QSize(100, 0));
        label_13->setFont(font4);

        horizontalLayout_13->addWidget(label_13);

        slider_sharpen_amount = new QSlider(groupBox_stft_controls);
        slider_sharpen_amount->setObjectName(QStringLiteral("slider_sharpen_amount"));
        slider_sharpen_amount->setMinimumSize(QSize(100, 0));
        slider_sharpen_amount->setMaximum(1000);
        slider_sharpen_amount->setValue(0);
        slider_sharpen_amount->setOrientation(Qt::Horizontal);

        horizontalLayout_13->addWidget(slider_sharpen_amount);


        verticalLayout_15->addLayout(horizontalLayout_13);


        horizontalLayout_30->addLayout(verticalLayout_15);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_30->addItem(horizontalSpacer_8);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        label_16 = new QLabel(groupBox_stft_controls);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setFont(font4);

        horizontalLayout_16->addWidget(label_16);

        box_wnd_type = new QComboBox(groupBox_stft_controls);
        box_wnd_type->addItem(QString());
        box_wnd_type->addItem(QString());
        box_wnd_type->setObjectName(QStringLiteral("box_wnd_type"));

        horizontalLayout_16->addWidget(box_wnd_type);


        verticalLayout_16->addLayout(horizontalLayout_16);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setObjectName(QStringLiteral("horizontalLayout_28"));
        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_28->addItem(horizontalSpacer_18);

        btn_stft_enabled = new QToolButton(groupBox_stft_controls);
        btn_stft_enabled->setObjectName(QStringLiteral("btn_stft_enabled"));
        sizePolicy2.setHeightForWidth(btn_stft_enabled->sizePolicy().hasHeightForWidth());
        btn_stft_enabled->setSizePolicy(sizePolicy2);
        btn_stft_enabled->setIcon(icon1);
        btn_stft_enabled->setCheckable(true);
        btn_stft_enabled->setChecked(true);
        btn_stft_enabled->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn_stft_enabled->setAutoRaise(false);

        horizontalLayout_28->addWidget(btn_stft_enabled);


        verticalLayout_16->addLayout(horizontalLayout_28);


        horizontalLayout_30->addLayout(verticalLayout_16);

        horizontalLayout_30->setStretch(0, 1);
        horizontalLayout_30->setStretch(1, 1);
        horizontalLayout_30->setStretch(2, 4);
        horizontalLayout_30->setStretch(3, 1);
        horizontalLayout_30->setStretch(4, 4);
        horizontalLayout_30->setStretch(5, 1);
        horizontalLayout_30->setStretch(6, 1);

        verticalLayout_17->addLayout(horizontalLayout_30);


        verticalLayout_18->addWidget(groupBox_stft_controls);

        verticalLayout_18->setStretch(0, 1);
        tabWidget->addTab(tab_stft, icon3, QString());
        tab_activity = new QWidget();
        tab_activity->setObjectName(QStringLiteral("tab_activity"));
        verticalLayout_11 = new QVBoxLayout(tab_activity);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        openGLWidget = new BrainMap(tab_activity);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        verticalLayout_11->addWidget(openGLWidget);

        tabWidget->addTab(tab_activity, QString());
        tab_test = new QWidget();
        tab_test->setObjectName(QStringLiteral("tab_test"));
        verticalLayout_13 = new QVBoxLayout(tab_test);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        openGLWidget_3 = new BallTest(tab_test);
        openGLWidget_3->setObjectName(QStringLiteral("openGLWidget_3"));

        verticalLayout_13->addWidget(openGLWidget_3);

        tabWidget->addTab(tab_test, QString());
        tab_translation = new QWidget();
        tab_translation->setObjectName(QStringLiteral("tab_translation"));
        tab_translation->setStyleSheet(QLatin1String("QWidget#tab_translation{\n"
"background-image: url(:/images/images/background-dark.png);\n"
"}"));
        verticalLayout_28 = new QVBoxLayout(tab_translation);
        verticalLayout_28->setSpacing(0);
        verticalLayout_28->setObjectName(QStringLiteral("verticalLayout_28"));
        verticalLayout_28->setContentsMargins(4, 4, 4, 4);
        splitter = new QSplitter(tab_translation);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setMinimumSize(QSize(400, 120));
        splitter->setLineWidth(1);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(true);
        splitter->setHandleWidth(4);
        plot_translation_left = new GraphWidget(splitter);
        plot_translation_left->setObjectName(QStringLiteral("plot_translation_left"));
        plot_translation_left->setMinimumSize(QSize(360, 240));
        splitter->addWidget(plot_translation_left);
        plot_translation_right = new GraphWidget(splitter);
        plot_translation_right->setObjectName(QStringLiteral("plot_translation_right"));
        plot_translation_right->setMinimumSize(QSize(360, 240));
        splitter->addWidget(plot_translation_right);

        verticalLayout_28->addWidget(splitter);

        btn_tr_larger = new QToolButton(tab_translation);
        btn_tr_larger->setObjectName(QStringLiteral("btn_tr_larger"));
        sizePolicy2.setHeightForWidth(btn_tr_larger->sizePolicy().hasHeightForWidth());
        btn_tr_larger->setSizePolicy(sizePolicy2);
        btn_tr_larger->setMaximumSize(QSize(16777215, 14));
        btn_tr_larger->setCheckable(true);
        btn_tr_larger->setChecked(false);
        btn_tr_larger->setAutoRaise(true);
        btn_tr_larger->setArrowType(Qt::DownArrow);

        verticalLayout_28->addWidget(btn_tr_larger);

        groupBox_translation = new QGroupBox(tab_translation);
        groupBox_translation->setObjectName(QStringLiteral("groupBox_translation"));
        groupBox_translation->setStyleSheet(QLatin1String("QWidget#groupBox_translation{\n"
"background-color: rgba(150,150,170, 130);\n"
"}"));
        verticalLayout_27 = new QVBoxLayout(groupBox_translation);
        verticalLayout_27->setObjectName(QStringLiteral("verticalLayout_27"));
        verticalLayout_25 = new QVBoxLayout();
        verticalLayout_25->setSpacing(0);
        verticalLayout_25->setObjectName(QStringLiteral("verticalLayout_25"));
        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(0);
        horizontalLayout_36->setObjectName(QStringLiteral("horizontalLayout_36"));
        label_29 = new QLabel(groupBox_translation);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setFont(font3);

        horizontalLayout_36->addWidget(label_29);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_36->addItem(horizontalSpacer_21);


        verticalLayout_25->addLayout(horizontalLayout_36);

        line_4 = new QFrame(groupBox_translation);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setStyleSheet(QLatin1String("QFrame#line_4\n"
"{\n"
"color: rgb(139, 149, 157)\n"
"}"));
        line_4->setFrameShadow(QFrame::Plain);
        line_4->setFrameShape(QFrame::HLine);

        verticalLayout_25->addWidget(line_4);


        verticalLayout_27->addLayout(verticalLayout_25);

        horizontalLayout_40 = new QHBoxLayout();
        horizontalLayout_40->setObjectName(QStringLiteral("horizontalLayout_40"));
        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setObjectName(QStringLiteral("verticalLayout_24"));
        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setObjectName(QStringLiteral("horizontalLayout_38"));
        label_30 = new QLabel(groupBox_translation);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setMinimumSize(QSize(50, 0));
        label_30->setFont(font4);

        horizontalLayout_38->addWidget(label_30);

        box_tr_centre_freq = new QSpinBox(groupBox_translation);
        box_tr_centre_freq->setObjectName(QStringLiteral("box_tr_centre_freq"));
        box_tr_centre_freq->setMinimumSize(QSize(26, 0));
        box_tr_centre_freq->setMaximum(1024);
        box_tr_centre_freq->setValue(10);

        horizontalLayout_38->addWidget(box_tr_centre_freq);

        label_32 = new QLabel(groupBox_translation);
        label_32->setObjectName(QStringLiteral("label_32"));
        label_32->setMinimumSize(QSize(0, 0));
        label_32->setFont(font4);

        horizontalLayout_38->addWidget(label_32);

        box_tr_span = new QSpinBox(groupBox_translation);
        box_tr_span->setObjectName(QStringLiteral("box_tr_span"));
        box_tr_span->setMaximum(10);
        box_tr_span->setValue(2);

        horizontalLayout_38->addWidget(box_tr_span);

        label_33 = new QLabel(groupBox_translation);
        label_33->setObjectName(QStringLiteral("label_33"));
        label_33->setMinimumSize(QSize(50, 0));
        label_33->setFont(font4);

        horizontalLayout_38->addWidget(label_33);

        box_tr_spreading = new QSpinBox(groupBox_translation);
        box_tr_spreading->setObjectName(QStringLiteral("box_tr_spreading"));
        box_tr_spreading->setMaximum(10);
        box_tr_spreading->setValue(2);

        horizontalLayout_38->addWidget(box_tr_spreading);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_38->addItem(horizontalSpacer_20);

        box_tr_flip = new QCheckBox(groupBox_translation);
        box_tr_flip->setObjectName(QStringLiteral("box_tr_flip"));
        box_tr_flip->setChecked(true);
        box_tr_flip->setTristate(false);

        horizontalLayout_38->addWidget(box_tr_flip);


        verticalLayout_24->addLayout(horizontalLayout_38);

        horizontalLayout_35 = new QHBoxLayout();
        horizontalLayout_35->setObjectName(QStringLiteral("horizontalLayout_35"));
        label_34 = new QLabel(groupBox_translation);
        label_34->setObjectName(QStringLiteral("label_34"));
        label_34->setMinimumSize(QSize(50, 0));
        label_34->setFont(font4);

        horizontalLayout_35->addWidget(label_34);

        slider_tr_sensitivity = new QSlider(groupBox_translation);
        slider_tr_sensitivity->setObjectName(QStringLiteral("slider_tr_sensitivity"));
        slider_tr_sensitivity->setMinimumSize(QSize(100, 0));
        slider_tr_sensitivity->setMaximum(1000);
        slider_tr_sensitivity->setValue(500);
        slider_tr_sensitivity->setOrientation(Qt::Horizontal);

        horizontalLayout_35->addWidget(slider_tr_sensitivity);


        verticalLayout_24->addLayout(horizontalLayout_35);

        horizontalLayout_43 = new QHBoxLayout();
        horizontalLayout_43->setObjectName(QStringLiteral("horizontalLayout_43"));
        label_42 = new QLabel(groupBox_translation);
        label_42->setObjectName(QStringLiteral("label_42"));
        label_42->setMinimumSize(QSize(50, 0));
        label_42->setFont(font4);

        horizontalLayout_43->addWidget(label_42);

        slider_tr_smoothing = new QSlider(groupBox_translation);
        slider_tr_smoothing->setObjectName(QStringLiteral("slider_tr_smoothing"));
        slider_tr_smoothing->setMinimumSize(QSize(100, 0));
        slider_tr_smoothing->setMaximum(1000);
        slider_tr_smoothing->setValue(500);
        slider_tr_smoothing->setOrientation(Qt::Horizontal);

        horizontalLayout_43->addWidget(slider_tr_smoothing);


        verticalLayout_24->addLayout(horizontalLayout_43);


        horizontalLayout_40->addLayout(verticalLayout_24);

        verticalLayout_26 = new QVBoxLayout();
        verticalLayout_26->setObjectName(QStringLiteral("verticalLayout_26"));
        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setObjectName(QStringLiteral("horizontalLayout_37"));
        label_31 = new QLabel(groupBox_translation);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setMinimumSize(QSize(0, 0));
        label_31->setFont(font4);

        horizontalLayout_37->addWidget(label_31);

        slider_tr_gain = new QSlider(groupBox_translation);
        slider_tr_gain->setObjectName(QStringLiteral("slider_tr_gain"));
        slider_tr_gain->setMinimumSize(QSize(100, 0));
        slider_tr_gain->setMaximum(1000);
        slider_tr_gain->setValue(500);
        slider_tr_gain->setOrientation(Qt::Horizontal);

        horizontalLayout_37->addWidget(slider_tr_gain);

        lbl_tr_gain = new QLabel(groupBox_translation);
        lbl_tr_gain->setObjectName(QStringLiteral("lbl_tr_gain"));
        lbl_tr_gain->setMinimumSize(QSize(36, 0));
        lbl_tr_gain->setFont(font4);
        lbl_tr_gain->setAlignment(Qt::AlignCenter);

        horizontalLayout_37->addWidget(lbl_tr_gain);

        btn_tr_auto_gain = new QToolButton(groupBox_translation);
        btn_tr_auto_gain->setObjectName(QStringLiteral("btn_tr_auto_gain"));
        btn_tr_auto_gain->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn_tr_auto_gain->setArrowType(Qt::NoArrow);

        horizontalLayout_37->addWidget(btn_tr_auto_gain);


        verticalLayout_26->addLayout(horizontalLayout_37);

        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setObjectName(QStringLiteral("horizontalLayout_39"));
        label_35 = new QLabel(groupBox_translation);
        label_35->setObjectName(QStringLiteral("label_35"));
        label_35->setMinimumSize(QSize(40, 0));
        label_35->setFont(font4);

        horizontalLayout_39->addWidget(label_35);

        slider_tr_offset = new QSlider(groupBox_translation);
        slider_tr_offset->setObjectName(QStringLiteral("slider_tr_offset"));
        slider_tr_offset->setMinimumSize(QSize(100, 0));
        slider_tr_offset->setMaximum(1000);
        slider_tr_offset->setValue(500);
        slider_tr_offset->setOrientation(Qt::Horizontal);

        horizontalLayout_39->addWidget(slider_tr_offset);

        lbl_tr_off = new QLabel(groupBox_translation);
        lbl_tr_off->setObjectName(QStringLiteral("lbl_tr_off"));
        lbl_tr_off->setMinimumSize(QSize(36, 0));
        lbl_tr_off->setFont(font4);
        lbl_tr_off->setAlignment(Qt::AlignCenter);

        horizontalLayout_39->addWidget(lbl_tr_off);

        btn_tr_auto_offset = new QToolButton(groupBox_translation);
        btn_tr_auto_offset->setObjectName(QStringLiteral("btn_tr_auto_offset"));

        horizontalLayout_39->addWidget(btn_tr_auto_offset);


        verticalLayout_26->addLayout(horizontalLayout_39);

        horizontalLayout_41 = new QHBoxLayout();
        horizontalLayout_41->setObjectName(QStringLiteral("horizontalLayout_41"));
        label_36 = new QLabel(groupBox_translation);
        label_36->setObjectName(QStringLiteral("label_36"));
        label_36->setMinimumSize(QSize(66, 0));
        label_36->setFont(font4);

        horizontalLayout_41->addWidget(label_36);

        label_37 = new QLabel(groupBox_translation);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setMinimumSize(QSize(40, 0));
        label_37->setFont(font4);

        horizontalLayout_41->addWidget(label_37);

        slider_tr_off_speed = new QSlider(groupBox_translation);
        slider_tr_off_speed->setObjectName(QStringLiteral("slider_tr_off_speed"));
        slider_tr_off_speed->setMinimumSize(QSize(100, 0));
        slider_tr_off_speed->setMaximum(1000);
        slider_tr_off_speed->setValue(500);
        slider_tr_off_speed->setOrientation(Qt::Horizontal);

        horizontalLayout_41->addWidget(slider_tr_off_speed);

        label_38 = new QLabel(groupBox_translation);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setMinimumSize(QSize(30, 0));
        label_38->setFont(font4);

        horizontalLayout_41->addWidget(label_38);

        slider_tr_off_limit = new QSlider(groupBox_translation);
        slider_tr_off_limit->setObjectName(QStringLiteral("slider_tr_off_limit"));
        slider_tr_off_limit->setMinimumSize(QSize(100, 0));
        slider_tr_off_limit->setMaximum(1000);
        slider_tr_off_limit->setValue(500);
        slider_tr_off_limit->setOrientation(Qt::Horizontal);

        horizontalLayout_41->addWidget(slider_tr_off_limit);


        verticalLayout_26->addLayout(horizontalLayout_41);

        horizontalLayout_42 = new QHBoxLayout();
        horizontalLayout_42->setObjectName(QStringLiteral("horizontalLayout_42"));
        label_39 = new QLabel(groupBox_translation);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setMinimumSize(QSize(66, 0));
        label_39->setFont(font4);

        horizontalLayout_42->addWidget(label_39);

        label_40 = new QLabel(groupBox_translation);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setMinimumSize(QSize(40, 0));
        label_40->setFont(font4);

        horizontalLayout_42->addWidget(label_40);

        slider_tr_mag_speed = new QSlider(groupBox_translation);
        slider_tr_mag_speed->setObjectName(QStringLiteral("slider_tr_mag_speed"));
        slider_tr_mag_speed->setMinimumSize(QSize(100, 0));
        slider_tr_mag_speed->setMaximum(1000);
        slider_tr_mag_speed->setValue(500);
        slider_tr_mag_speed->setOrientation(Qt::Horizontal);

        horizontalLayout_42->addWidget(slider_tr_mag_speed);

        label_41 = new QLabel(groupBox_translation);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setMinimumSize(QSize(30, 0));
        label_41->setFont(font4);

        horizontalLayout_42->addWidget(label_41);

        slider_tr_offset_5 = new QSlider(groupBox_translation);
        slider_tr_offset_5->setObjectName(QStringLiteral("slider_tr_offset_5"));
        slider_tr_offset_5->setMinimumSize(QSize(100, 0));
        slider_tr_offset_5->setMaximum(1000);
        slider_tr_offset_5->setValue(500);
        slider_tr_offset_5->setOrientation(Qt::Horizontal);

        horizontalLayout_42->addWidget(slider_tr_offset_5);


        verticalLayout_26->addLayout(horizontalLayout_42);


        horizontalLayout_40->addLayout(verticalLayout_26);


        verticalLayout_27->addLayout(horizontalLayout_40);


        verticalLayout_28->addWidget(groupBox_translation);

        verticalLayout_28->setStretch(0, 1);
        tabWidget->addTab(tab_translation, QString());

        verticalLayout_4->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_uoa->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt; color:#d9e9f5;\">BCI Controller</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt; color:#a2b1c3;\">Version 3.0.0</span></p></body></html>", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt; color:#a2b1c3;\">by A.S. Woodcock &amp; A. Vasilyev</span></p></body></html>", nullptr));
        label_7->setText(QString());
        groupBox->setTitle(QString());
        label_15->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Source:</span></p></body></html>", nullptr));
        box_source->setItemText(0, QApplication::translate("MainWindow", "Offline", nullptr));
        box_source->setItemText(1, QApplication::translate("MainWindow", "CytonDaisy", nullptr));
        box_source->setItemText(2, QApplication::translate("MainWindow", "Emotiv", nullptr));

        btn_connect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        groupBox_2->setTitle(QString());
        label_17->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Data:</span></p></body></html>", nullptr));
        btn_data_in->setText(QApplication::translate("MainWindow", "...", nullptr));
        label_18->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Frequency:</span></p></body></html>", nullptr));
        lbl_com_port->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Serial Port:</span></p></body></html>", nullptr));
        btn_debug_window->setText(QApplication::translate("MainWindow", "Hide Debug Window", nullptr));
        groupBox_3->setTitle(QString());
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#ccdbe7;\">Save Options</span></p></body></html>", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ffffff;\">Name:</span></p></body></html>", nullptr));
        btn_out_file->setText(QApplication::translate("MainWindow", "...", nullptr));
        btn_save_options->setText(QString());
        btn_save_start->setText(QApplication::translate("MainWindow", "...", nullptr));
        btn_save_stop->setText(QApplication::translate("MainWindow", "...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_home), QApplication::translate("MainWindow", "Home", nullptr));
        groupBox_5->setTitle(QString());
        label_9->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#ccdbe7;\">Electrode Contact Quality</span></p></body></html>", nullptr));
        groupBox_6->setTitle(QString());
        label_19->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#ccdbe7;\">Accelerometer</span></p></body></html>", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButton_3->setToolTip(QApplication::translate("MainWindow", "Enable/Disable STFT Plot (disable to improve perfromance)", nullptr));
#endif // QT_NO_TOOLTIP
        toolButton_3->setText(QApplication::translate("MainWindow", " Enabled", nullptr));
        groupBox_7->setTitle(QString());
        label_21->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#ccdbe7;\">Gyro</span></p></body></html>", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButton_4->setToolTip(QApplication::translate("MainWindow", "Enable/Disable STFT Plot (disable to improve perfromance)", nullptr));
#endif // QT_NO_TOOLTIP
        toolButton_4->setText(QApplication::translate("MainWindow", "Enabled", nullptr));
        label_20->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#ccdbe7;\">Battery Level:</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_data), QApplication::translate("MainWindow", "Data", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_time), QApplication::translate("MainWindow", "Time Analysis", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_freq), QApplication::translate("MainWindow", "Freq Analysis", nullptr));
        btn_stft_larger->setText(QApplication::translate("MainWindow", "...", nullptr));
        groupBox_stft_controls->setTitle(QString());
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:16pt; color:#ccdbe7;\">Controls</span></p></body></html>", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Window Size:</span></p></body></html>", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Overlap:</span></p></body></html>", nullptr));
        label_22->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Time Span (s):</span></p></body></html>", nullptr));
        label_25->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Max Freq:</span></p></body></html>", nullptr));
        label_26->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Brightness:</span></p></body></html>", nullptr));
        label_27->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Dark Boost:</span></p></body></html>", nullptr));
        label_24->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Hard Limit:</span></p></body></html>", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Blur Radius:</span></p></body></html>", nullptr));
        lab->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Blur Amount:</span></p></body></html>", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Sharpen Radius:</span></p></body></html>", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Sharpen Amount:</span></p></body></html>", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Window Type:</span></p></body></html>", nullptr));
        box_wnd_type->setItemText(0, QApplication::translate("MainWindow", "Gaussian", nullptr));
        box_wnd_type->setItemText(1, QApplication::translate("MainWindow", "Rectangular", nullptr));

#ifndef QT_NO_TOOLTIP
        btn_stft_enabled->setToolTip(QApplication::translate("MainWindow", "Enable/Disable STFT Plot (disable to improve perfromance)", nullptr));
#endif // QT_NO_TOOLTIP
        btn_stft_enabled->setText(QApplication::translate("MainWindow", "STFT Enabled", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_stft), QApplication::translate("MainWindow", "STFT Plot", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_activity), QApplication::translate("MainWindow", "Brain Activity", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_test), QApplication::translate("MainWindow", "Ball Test", nullptr));
        btn_tr_larger->setText(QApplication::translate("MainWindow", "...", nullptr));
        groupBox_translation->setTitle(QString());
        label_29->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:16pt; color:#ccdbe7;\">Controls</span></p></body></html>", nullptr));
        label_30->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Centre Freq (Hz): </span></p></body></html>", nullptr));
        label_32->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Span (Hz):</span></p></body></html>", nullptr));
        label_33->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Spreading:</span></p></body></html>", nullptr));
        box_tr_flip->setText(QApplication::translate("MainWindow", "Flip", nullptr));
        label_34->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Sensitivity:</span></p></body></html>", nullptr));
        label_42->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Smoothing:</span></p></body></html>", nullptr));
        label_31->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Gain:</span></p></body></html>", nullptr));
        lbl_tr_gain->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">10</span></p></body></html>", nullptr));
        btn_tr_auto_gain->setText(QApplication::translate("MainWindow", "Auto", nullptr));
        label_35->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">Offset:</span></p></body></html>", nullptr));
        lbl_tr_off->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; color:#ccdbe7;\">10</span></p></body></html>", nullptr));
        btn_tr_auto_offset->setText(QApplication::translate("MainWindow", "Auto", nullptr));
        label_36->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Offset Corr</span></p></body></html>", nullptr));
        label_37->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Speed:</span></p></body></html>", nullptr));
        label_38->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Limit</span></p></body></html>", nullptr));
        label_39->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:9pt; color:#ccdbe7;\">Mag Corr</span></p></body></html>", nullptr));
        label_40->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Speed:</span></p></body></html>", nullptr));
        label_41->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ccdbe7;\">Limit</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_translation), QApplication::translate("MainWindow", "Translation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

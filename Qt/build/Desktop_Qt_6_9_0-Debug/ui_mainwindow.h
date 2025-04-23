/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *toolBar;
    QPushButton *connectButton;
    QPushButton *load_fButton;
    QPushButton *save_fButton;
    QPushButton *exitButton;
    QDial *x_dial;
    QDial *y_dial;
    QDial *dial;
    QDial *dial_2;
    QLabel *label;
    QLabel *label_2;
    QWidget *graphsView;
    QLabel *imu_label;
    QWidget *imu_widget;
    QWidget *motors_widget;
    QLabel *motors_speed_la;
    QWidget *lineS_widget;
    QLabel *lineS_label;
    QWidget *tof_widget;
    QLabel *tof_label;
    QWidget *robotView;
    QLabel *motor_speed_1_Label;
    QLabel *motor_speed_2_Label;
    QLabel *mainrobot;
    QLabel *lineS_Active_1;
    QLabel *lineS_Active_2;
    QLabel *lineS_Active_3;
    QLabel *tof_Green_1;
    QLabel *tof_Green_2;
    QLabel *tof_Green_3;
    QLabel *tof_Green_4;
    QLabel *tof_Red_1;
    QLabel *tof_Red_2;
    QLabel *tof_Red_3;
    QLabel *tof_Red_4;
    QLabel *motor1_arrow;
    QLabel *motor2_arrow;
    QLabel *imu_arrow;
    QPushButton *tof1_button;
    QPushButton *line1_button;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setMinimumSize(QSize(897, 694));
        MainWindow->setMaximumSize(QSize(897, 694));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setMinimumSize(QSize(897, 694));
        centralwidget->setMaximumSize(QSize(897, 16777215));
        toolBar = new QWidget(centralwidget);
        toolBar->setObjectName("toolBar");
        toolBar->setGeometry(QRect(0, 0, 901, 71));
        toolBar->setStyleSheet(QString::fromUtf8("background-color: #696969;  \n"
"border: 2px solid #505050;  \n"
"border-radius: 8px;  "));
        connectButton = new QPushButton(toolBar);
        connectButton->setObjectName("connectButton");
        connectButton->setGeometry(QRect(20, 10, 111, 51));
        connectButton->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        load_fButton = new QPushButton(toolBar);
        load_fButton->setObjectName("load_fButton");
        load_fButton->setGeometry(QRect(140, 10, 88, 25));
        load_fButton->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; \n"
""));
        save_fButton = new QPushButton(toolBar);
        save_fButton->setObjectName("save_fButton");
        save_fButton->setGeometry(QRect(140, 40, 88, 25));
        save_fButton->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        exitButton = new QPushButton(toolBar);
        exitButton->setObjectName("exitButton");
        exitButton->setGeometry(QRect(760, 10, 121, 51));
        exitButton->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        x_dial = new QDial(toolBar);
        x_dial->setObjectName("x_dial");
        x_dial->setGeometry(QRect(600, 30, 41, 41));
        x_dial->setMinimum(-100);
        x_dial->setMaximum(100);
        y_dial = new QDial(toolBar);
        y_dial->setObjectName("y_dial");
        y_dial->setGeometry(QRect(560, 30, 41, 41));
        y_dial->setMinimum(-100);
        y_dial->setMaximum(100);
        dial = new QDial(toolBar);
        dial->setObjectName("dial");
        dial->setGeometry(QRect(399, 23, 41, 51));
        dial->setMinimum(-100);
        dial->setMaximum(100);
        dial_2 = new QDial(toolBar);
        dial_2->setObjectName("dial_2");
        dial_2->setGeometry(QRect(439, 23, 41, 51));
        dial_2->setMinimum(-100);
        dial_2->setMaximum(100);
        label = new QLabel(toolBar);
        label->setObjectName("label");
        label->setGeometry(QRect(410, 10, 71, 21));
        label->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        label_2 = new QLabel(toolBar);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(560, 10, 81, 21));
        label_2->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        graphsView = new QWidget(centralwidget);
        graphsView->setObjectName("graphsView");
        graphsView->setGeometry(QRect(0, 70, 401, 621));
        graphsView->setMaximumSize(QSize(401, 621));
        graphsView->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        graphsView->setStyleSheet(QString::fromUtf8("background-color: #bbbbbb;  \n"
"border: 2px solid #505050;  \n"
"border-radius: 8px;  "));
        imu_label = new QLabel(graphsView);
        imu_label->setObjectName("imu_label");
        imu_label->setGeometry(QRect(10, 10, 381, 17));
        imu_label->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        imu_label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        imu_widget = new QWidget(graphsView);
        imu_widget->setObjectName("imu_widget");
        imu_widget->setGeometry(QRect(30, 30, 341, 131));
        imu_widget->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        motors_widget = new QWidget(graphsView);
        motors_widget->setObjectName("motors_widget");
        motors_widget->setGeometry(QRect(30, 200, 341, 131));
        motors_widget->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        motors_speed_la = new QLabel(graphsView);
        motors_speed_la->setObjectName("motors_speed_la");
        motors_speed_la->setGeometry(QRect(10, 180, 381, 17));
        motors_speed_la->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        motors_speed_la->setAlignment(Qt::AlignmentFlag::AlignCenter);
        lineS_widget = new QWidget(graphsView);
        lineS_widget->setObjectName("lineS_widget");
        lineS_widget->setGeometry(QRect(30, 380, 341, 81));
        lineS_widget->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        lineS_label = new QLabel(graphsView);
        lineS_label->setObjectName("lineS_label");
        lineS_label->setGeometry(QRect(10, 360, 381, 17));
        lineS_label->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        lineS_label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        tof_widget = new QWidget(graphsView);
        tof_widget->setObjectName("tof_widget");
        tof_widget->setGeometry(QRect(30, 510, 341, 81));
        tof_widget->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        tof_label = new QLabel(graphsView);
        tof_label->setObjectName("tof_label");
        tof_label->setGeometry(QRect(10, 490, 381, 17));
        tof_label->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        tof_label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        robotView = new QWidget(centralwidget);
        robotView->setObjectName("robotView");
        robotView->setGeometry(QRect(400, 70, 501, 621));
        robotView->setStyleSheet(QString::fromUtf8("background-color: #555555;\n"
"border: 2px solid #505050;  \n"
"border-radius: 8px;  \n"
""));
        motor_speed_1_Label = new QLabel(robotView);
        motor_speed_1_Label->setObjectName("motor_speed_1_Label");
        motor_speed_1_Label->setGeometry(QRect(140, 490, 61, 31));
        motor_speed_1_Label->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        motor_speed_1_Label->setScaledContents(false);
        motor_speed_1_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        motor_speed_2_Label = new QLabel(robotView);
        motor_speed_2_Label->setObjectName("motor_speed_2_Label");
        motor_speed_2_Label->setGeometry(QRect(330, 490, 61, 31));
        motor_speed_2_Label->setStyleSheet(QString::fromUtf8("background-color: #B0B0B0;\n"
"color: #555555;            \n"
"font-weight: bold;\n"
"border: 2px solid #505050; "));
        motor_speed_2_Label->setScaledContents(false);
        motor_speed_2_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        mainrobot = new QLabel(robotView);
        mainrobot->setObjectName("mainrobot");
        mainrobot->setGeometry(QRect(200, 340, 131, 131));
        mainrobot->setStyleSheet(QString::fromUtf8(""));
        mainrobot->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/robotv1.png")));
        mainrobot->setScaledContents(true);
        mainrobot->setOpenExternalLinks(false);
        lineS_Active_1 = new QLabel(robotView);
        lineS_Active_1->setObjectName("lineS_Active_1");
        lineS_Active_1->setGeometry(QRect(200, 340, 31, 20));
        lineS_Active_1->setStyleSheet(QString::fromUtf8("border:0px;"));
        lineS_Active_1->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/activeSensor.png")));
        lineS_Active_1->setScaledContents(true);
        lineS_Active_2 = new QLabel(robotView);
        lineS_Active_2->setObjectName("lineS_Active_2");
        lineS_Active_2->setGeometry(QRect(300, 340, 31, 20));
        lineS_Active_2->setStyleSheet(QString::fromUtf8("border:0px;"));
        lineS_Active_2->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/activeSensor.png")));
        lineS_Active_2->setScaledContents(true);
        lineS_Active_3 = new QLabel(robotView);
        lineS_Active_3->setObjectName("lineS_Active_3");
        lineS_Active_3->setGeometry(QRect(250, 440, 31, 20));
        lineS_Active_3->setStyleSheet(QString::fromUtf8("border:0px;"));
        lineS_Active_3->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/activeSensor.png")));
        lineS_Active_3->setScaledContents(true);
        tof_Green_1 = new QLabel(robotView);
        tof_Green_1->setObjectName("tof_Green_1");
        tof_Green_1->setGeometry(QRect(40, 340, 151, 51));
        tof_Green_1->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Green_1->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/greenLEFT.png")));
        tof_Green_1->setScaledContents(true);
        tof_Green_2 = new QLabel(robotView);
        tof_Green_2->setObjectName("tof_Green_2");
        tof_Green_2->setGeometry(QRect(170, 160, 71, 171));
        tof_Green_2->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Green_2->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/greenUP.png")));
        tof_Green_2->setScaledContents(true);
        tof_Green_3 = new QLabel(robotView);
        tof_Green_3->setObjectName("tof_Green_3");
        tof_Green_3->setGeometry(QRect(290, 160, 71, 171));
        tof_Green_3->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Green_3->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/greenUpR.png")));
        tof_Green_3->setScaledContents(true);
        tof_Green_4 = new QLabel(robotView);
        tof_Green_4->setObjectName("tof_Green_4");
        tof_Green_4->setGeometry(QRect(340, 340, 141, 51));
        tof_Green_4->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Green_4->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/greenRIGHT.png")));
        tof_Green_4->setScaledContents(true);
        tof_Red_1 = new QLabel(robotView);
        tof_Red_1->setObjectName("tof_Red_1");
        tof_Red_1->setGeometry(QRect(40, 340, 151, 51));
        tof_Red_1->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Red_1->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/redLEFT.png")));
        tof_Red_1->setScaledContents(true);
        tof_Red_2 = new QLabel(robotView);
        tof_Red_2->setObjectName("tof_Red_2");
        tof_Red_2->setGeometry(QRect(170, 160, 71, 171));
        tof_Red_2->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Red_2->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/redUp.png")));
        tof_Red_2->setScaledContents(true);
        tof_Red_3 = new QLabel(robotView);
        tof_Red_3->setObjectName("tof_Red_3");
        tof_Red_3->setGeometry(QRect(290, 160, 71, 171));
        tof_Red_3->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Red_3->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/redUpR.png")));
        tof_Red_3->setScaledContents(true);
        tof_Red_4 = new QLabel(robotView);
        tof_Red_4->setObjectName("tof_Red_4");
        tof_Red_4->setGeometry(QRect(340, 340, 141, 51));
        tof_Red_4->setStyleSheet(QString::fromUtf8("border:0px;"));
        tof_Red_4->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/redRIGHT.png")));
        tof_Red_4->setScaledContents(true);
        motor1_arrow = new QLabel(robotView);
        motor1_arrow->setObjectName("motor1_arrow");
        motor1_arrow->setGeometry(QRect(150, 390, 31, 91));
        motor1_arrow->setStyleSheet(QString::fromUtf8("border:0px;"));
        motor1_arrow->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/arrowUp.png")));
        motor1_arrow->setScaledContents(false);
        motor2_arrow = new QLabel(robotView);
        motor2_arrow->setObjectName("motor2_arrow");
        motor2_arrow->setGeometry(QRect(340, 390, 31, 91));
        motor2_arrow->setStyleSheet(QString::fromUtf8("border:0px;"));
        motor2_arrow->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/arrowUp.png")));
        motor2_arrow->setScaledContents(false);
        imu_arrow = new QLabel(robotView);
        imu_arrow->setObjectName("imu_arrow");
        imu_arrow->setGeometry(QRect(170, 320, 181, 181));
        imu_arrow->setStyleSheet(QString::fromUtf8("background: transparent;\n"
"border: none;"));
        imu_arrow->setPixmap(QPixmap(QString::fromUtf8(":/new/image/img/arrowIMU.png")));
        tof1_button = new QPushButton(centralwidget);
        tof1_button->setObjectName("tof1_button");
        tof1_button->setGeometry(QRect(260, 40, 88, 25));
        line1_button = new QPushButton(centralwidget);
        line1_button->setObjectName("line1_button");
        line1_button->setGeometry(QRect(260, 10, 88, 25));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        QObject::connect(exitButton, &QPushButton::clicked, MainWindow, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "CONNECT", nullptr));
        load_fButton->setText(QCoreApplication::translate("MainWindow", "LOAD FILE", nullptr));
        save_fButton->setText(QCoreApplication::translate("MainWindow", "SAVE FILE", nullptr));
        exitButton->setText(QCoreApplication::translate("MainWindow", "EXIT", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "IMU Ctrl", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "PWM Ctrl", nullptr));
        imu_label->setText(QCoreApplication::translate("MainWindow", "IMU", nullptr));
        motors_speed_la->setText(QCoreApplication::translate("MainWindow", "MOTORS SPEED", nullptr));
        lineS_label->setText(QCoreApplication::translate("MainWindow", "LINE SENSORS", nullptr));
        tof_label->setText(QCoreApplication::translate("MainWindow", "REFLECTIVE SENSORS", nullptr));
        motor_speed_1_Label->setText(QCoreApplication::translate("MainWindow", "10%", nullptr));
        motor_speed_2_Label->setText(QCoreApplication::translate("MainWindow", "10%", nullptr));
        mainrobot->setText(QString());
        lineS_Active_1->setText(QString());
        lineS_Active_2->setText(QString());
        lineS_Active_3->setText(QString());
        tof_Green_1->setText(QString());
        tof_Green_2->setText(QString());
        tof_Green_3->setText(QString());
        tof_Green_4->setText(QString());
        tof_Red_1->setText(QString());
        tof_Red_2->setText(QString());
        tof_Red_3->setText(QString());
        tof_Red_4->setText(QString());
        motor1_arrow->setText(QString());
        motor2_arrow->setText(QString());
        imu_arrow->setText(QString());
        tof1_button->setText(QCoreApplication::translate("MainWindow", "tof1", nullptr));
        line1_button->setText(QCoreApplication::translate("MainWindow", "lin1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

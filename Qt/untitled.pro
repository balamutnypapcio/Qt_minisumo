TARGET = untitled

QT       += core gui
QT       += charts
QT       += network
QT += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chartviewhelper.cpp \
    csvmanager.cpp \
    imuchartmanager.cpp \
    linechartmenager.cpp \
    main.cpp \
    mainwindow.cpp \
    motorchartmanager.cpp \
    proportionalwidget.cpp \
    sensordata.cpp \
    tcpmanager.cpp \
    tofchartmanager.cpp \
    visualizationmanager.cpp

HEADERS += \
    chartviewhelper.h \
    csvmanager.h \
    imuchartmanager.h \
    linechartmenager.h \
    mainwindow.h \
    motorchartmanager.h \
    proportionalwidget.h \
    rotatedlabel.h \
    sensordata.h \
    tcpmanager.h \
    tofchartmanager.h \
    visualizationmanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

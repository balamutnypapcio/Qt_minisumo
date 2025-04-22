QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    csvmanager.cpp \
    imuchartmanager.cpp \
    linechartmenager.cpp \
    main.cpp \
    mainwindow.cpp \
    motorchartmanager.cpp \
    sensordata.cpp \
    tofchartmanager.cpp \
    visualizationmanager.cpp

HEADERS += \
    csvmanager.h \
    imuchartmanager.h \
    linechartmenager.h \
    mainwindow.h \
    motorchartmanager.h \
    sensordata.h \
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

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    file/configurationparser.cpp \
    file/directorycontent.cpp \
    file/filereader.cpp \
    gpio/configgpio.cpp \
    gpio/gpiomenager.cpp \
    guielements/configlistcombobox.cpp \
    main.cpp \
    mainwindow.cpp \
    scenario/buttonoperation.cpp \
    scenario/commentoperation.cpp \
    scenario/loopoperation.cpp \
    scenario/scenario.cpp \
    scenario/sleepoperation.cpp \
    scenario/timeoperation.cpp \
    scenario/userreactionoperation.cpp \
    threads/threads.cpp

HEADERS += \
    file/configurationparser.h \
    file/directorycontent.h \
    file/filereader.h \
    gpio/configgpio.h \
    gpio/gpiomenager.h \
    guielements/configlistcombobox.h \
    mainwindow.h \
    scenario/buttonoperation.h \
    scenario/commentoperation.h \
    scenario/loopoperation.h \
    scenario/operation.h \
    scenario/scenario.h \
    scenario/sleepoperation.h \
    scenario/timeoperation.h \
    scenario/userreactionoperation.h \
    threads/threads.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

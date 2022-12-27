QT       += core gui widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TuringAlphabet.cpp \
    TuringExecuter.cpp \
    TuringSettings.cpp \
    TuringTable.cpp \
    main.cpp \
    mainwindow.cpp \
    stringview.cpp

HEADERS += \
    TuringAlphabet.h \
    TuringExecuter.h \
    TuringSettings.h \
    TuringTable.h \
    mainwindow.h \
    stringview.h

FORMS += \
    mainwindow.ui \
    stringview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Img.qrc

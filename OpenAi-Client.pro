QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += qt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    curl_utils.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    curl_utils.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    OpenAi-Client_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

CURL_INCLUDE = E:/cxiangmu/CogleapProtector/ThirdParty/libcurl/include
CURL_LIB = E:/cxiangmu/CogleapProtector/ThirdParty/libcurl/lib

LIBS += -LE:/chromeDownLoads/curl-7.81.0/lib -lcurldll -lws2_32 -lwldap32
INCLUDEPATH += E:/chromeDownLoads/curl-7.81.0/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    window-icon.qrc

RC_ICONS = winicon.ico

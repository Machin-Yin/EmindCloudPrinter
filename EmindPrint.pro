TEMPLATE = app

QT += qml quick network widgets printsupport dbus core
CONFIG += c++11

LIBS += -lcups

TRANSLATIONS += translations/zh_CN.ts


SOURCES += main.cpp \
    licensefactory.cpp \
    client.cpp \
    cupsbackend.cpp \
    printer.cpp \
    emindprintdbus.cpp \
    printerlistmodel.cpp \
    tcpthread.cpp \
    clientassistant.cpp \
    clienter.cpp

lupdate_only{
SOURCES = *.qml \
          *.js
}

RESOURCES += qml.qrc


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeList.txt \
    org.emindprinter.serveice \
    cups/emindprinter \
    cups/emindprinterbackend \
    cups/emindprinter.ppd \
    debian/emindprinter.postrm \
    debian/source/format \
    debian/compat \
    debian/control \
    debian/emindprinter.install \
    debian/changelog \
    debian/README.Debian \
    debian/README.source \
    img/printer.png \
    translations/zh_CN.qm \
    translations/zh_CN.ts

HEADERS += \
    licensefactory.h \
    client.h \
    cupsbackend.h \
    printer.h \
    emindprintdbus.h \
    printerlistmodel.h \
    tcpthread.h \
    protocol.h \
    debian/emindprinter.postinst \
    debian/rules \
    clientassistant.h \
    clienter.h

TARGET = emindprinter

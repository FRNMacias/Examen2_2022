QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17
QMAKE_CXXFLAGS += -Wa,-mbig-obj
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += -L"C:\Users\macia\OneDrive\Documentos\InformaticaII\MuParser_dll"
LIBS += -lMuParser_dll
INCLUDEPATH += "C:\Users\macia\OneDrive\Documentos\InformaticaII\MuParser_dll"

LIBS += -L"C:\Users\macia\OneDrive\Documentos\InformaticaII\QCustomPlot"
LIBS += -lqcustomplot2 #nombre de la dll
INCLUDEPATH += "C:\Users\macia\OneDrive\Documentos\InformaticaII\QCustomPlot"

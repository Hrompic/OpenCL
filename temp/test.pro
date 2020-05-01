TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lOpenCL
SOURCES += \
        1.c

DISTFILES += \
    test.cl

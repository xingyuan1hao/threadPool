TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.c \
    ThreadPool_op.c

LIBS += \
    -l pthread

HEADERS += \
    ThreadPool_op.h

DISTFILES += \
    readme.md

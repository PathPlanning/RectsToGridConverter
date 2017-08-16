TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tinyxml/tinystr.cpp \
    tinyxml/tinyxml.cpp \
    tinyxml/tinyxmlerror.cpp \
    tinyxml/tinyxmlparser.cpp \
    map.cpp \
    filler.cpp \

HEADERS += \
    parser.h \
    tinyxml/tinystr.h \
    tinyxml/tinyxml.h \
    gl.const.h \
    map.h \
    filler.h \
    point.h \
    bresenham.h

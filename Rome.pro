#----------------------------------------------------------------------------
#   AUTHOR: Shaun Katz
#           University of Cape Town (UCT)
#           Department of Electrical Engineering
#           Software Defined Radio Group (SDRG)
#   DATE:   02/03/2012
#----------------------------------------------------------------------------

QT       += core gui \
            xml

TARGET = Rome
TEMPLATE = app
UI_DIR = ui

SOURCES += \
        src/main.cpp\
        src/mainwindow.cpp \
        src/port.cpp \
        src/component.cpp \
        src/wire.cpp \
        src/archivetreeview.cpp \
        src/graphicsscene.cpp \
        src/parser.cpp \
        src/graphicsview.cpp

HEADERS  += \
        src/mainwindow.h \
        src/port.h \
        src/component.h \
        src/wire.h \
        src/archivetreeview.h \
        src/graphicsscene.h \
        src/constants.h \
        src/parser.h \
        src/graphicsview.h

FORMS    += src/mainwindow.ui

OTHER_FILES += \
    TODO.txt \
    MILESTONES.txt





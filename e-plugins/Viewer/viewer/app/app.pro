TEMPLATE	= app

# CONFIG
QT += opengl
QT += widgets
CONFIG += debug
CONFIG -= release
CONFIG += warn_on 
CONFIG += console
#message(CONFIG: $$CONFIG)

# INPUTS 
INCLUDEPATH += include
INCLUDEPATH += ../glwidget/include
INCLUDEPATH += ../core/include
INCLUDEPATH += ../interfaces
SOURCES	+= *.cpp

# OUTPUTS
TARGET 		= viewer
DESTDIR = ../bin

debug: TARGET = $$join(TARGET,,,d)
debug: DEST = build/debug/
debug: LIBS += -Wl,-rpath $$DESTDIR -L$$DESTDIR -lglwidgetd

release: DEST = build/release
release: LIBS += -Wl,-rpath $$DESTDIR -L$$DESTDIR -lglwidget

OBJECTS_DIR = $$DEST
MOC_DIR = $$DEST
RCC_DIR = $$DEST
UI_DIR = $$DEST

# GLEW

win32:INCLUDEPATH += E:/lib/glew-1.10.0/include/
win32:LIBS += -LE:/lib/glew-1.10.0/lib/Release/Win32
win32:LIBS += -lglew32


#message(Working dir: $$PWD)
#message(Target dir: $$DESTDIR)

TEMPLATE   = lib

# CONFIG
QT     += opengl
QT     += widgets
CONFIG += plugin 
CONFIG += debug
CONFIG -= release
CONFIG += warn_on 


# INPUTS 
INCLUDEPATH += include
INCLUDEPATH += ../core/include
INCLUDEPATH += ../interfaces

HEADERS += include/*.h 
SOURCES	+= src/*.cpp


# OUTPUTS
TARGET     = glwidget
DESTDIR = $$(PWD)/../bin
#message("will install in $$DESTDIR")

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  MOC_DIR = build/debug
  OBJECTS_DIR = build/debug
  RCC_DIR = build/debug
} else {
  MOC_DIR = build/release
  OBJECTS_DIR = build/release
  RCC_DIR = build/release
}


win32:INCLUDEPATH += E:/lib/glew-1.10.0/include/
win32:LIBS += -LE:/lib/glew-1.10.0/lib/Release/Win32
win32:LIBS += -lglew32

# GLEW
macx{
   LIBS +=  -L../bin/  -lcore -install_name $$DESTDIR/libglwidgetd.dylib
} else {
   LIBS += -Wl,--rpath-link=../bin -L../bin  -lGLU -lcore -lGL # Cal a linux, però no a Mac...
}

DEFINES += PLUGINGLWIDGET_LIBRARY   # see Qt docs, "Creating shared libraries"



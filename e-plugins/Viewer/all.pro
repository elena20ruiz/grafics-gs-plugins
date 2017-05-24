TEMPLATE=subdirs
SUBDIRS= viewer plugins
CONFIG += ordered
CONFIG += silent

plugins.depends += viewer/bin/libglwidget.so
plugins.depends += viewer/bin/libglwidgetd.so
plugins.depends += viewer/bin/libcore.so
plugins.depends += viewer/bin/libcored.so


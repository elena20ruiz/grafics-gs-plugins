#Sessió 1: Plugins

## Instalar Viewer

1. Descomprimir zip, nombrarlo como "Viewer"
2. Entrar i hacer qmake-qt5
3. Hacer make
4. setenv LD_LIBRARY_PATH $PWD/viewer/bin
5. viewer/bin/viewerd 

Després en el viewer tornar a fer qmake i make.

## Agregar plugins al viewer

1. model-info

	mkdir plugins/model-info
	cd plugins/model-info
	echo "" > model-info.pro
	echo "" >  model-info.h
	echo "" >  model-info.cpp
	cd ..
	cd ..
	qmake-qt5
	make

2. Afegir SUBDIR
	
	en /plugins/plugins.pro poner:
	SUBDIR += nombre

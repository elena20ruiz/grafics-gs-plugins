#Sessió 1: Plugins

## Instalar Viewer

1. Descomprimir zip, nombrarlo como "Viewer"
2. Entrar i hacer qmake-qt5
3. Hacer make
4. setenv LD_LIBRARY_PATH $PWD/viewer/bin

## Crear nou plugins

Volent que el nostre plugin es digui "my-effect":

	mkdir plugins/my-effect
	echo "" > my-effect.pro
	mkdir include
	mkdir src
	cd include 
	echo "" >  my-effect.h
	cd ..
	cd src
	echo "" >  my-effect.cpp

Després en el viewer tornar a fer qmake i make.

## Agregar plugins al viewer

model-info

	mkdir plugins/model-info
	cd plugins/model-info
	echo "" > model-info.pro
	mkdir include
	mkdir src
	cd include 
	echo "" >  model-info.h
	cd ..
	cd src
	echo "" >  model-info.cpp

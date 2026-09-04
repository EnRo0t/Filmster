# README

## ¿QUÉ ES FILMSTER?

Filmster es un software para realizar listas de películas y poder exportarlas a -de momento- formato markdown.

En el futuro quiero añadir la exportación a pdf.

## OBJETIVOS

Este es un proyecto de aprendizaje para entender las bases de C++.
Este proyecto lo estoy realizando utilizando la IA solamente como
consulta -a modo de buscador rápido- para forzarme a resolver los
diferentes problemas de programación por mi mismo. El objetivo es
aprender y entrenar la mente en la resolución de problemas. 
No hay algoritmo, función o estructura de datos incluidos en este
software que no entienda. 

## INSTALACIÓN

```sh
git clone https://github.com/EnRo0t/Filmster.git && cd Filmster
sudo apt update 
sudo apt install build-essential libgtk-3-dev cmake
mkdir build
cd build
cmake ..
make
```

## API TOKEN Y API KEY

Para poder hacer llamadas a la API de TMDB (The Movie Database) necesitarás el
bearer token y la API key. Regístrate en su página: https://www.themoviedb.org/
para obtenerlos.

Una vez tengas el token cópialo y pégalo en un archivo .txt llamado
apiToken.txt y la key en un archivo llamado apiKey.txt, y añade ambos al
directorio build en la raíz del proyecto.

## EJECUCION

En /build

```sh
./peliculas.app
```

## ROADMAP / TASKLIST

+ ~~Añadir covers de las películas mediante llamadas a la api de tmdb
  utilizando la libreria CPR.~~
+ ~~Añadir genero de la película mediante llamada a la api de tmdb~~
+ ~~Añadir sinopsis de la película medienta llamada a la api de tmdb~~
+ Añadir exportación a PDF
+ Descargar imagen de portada en local





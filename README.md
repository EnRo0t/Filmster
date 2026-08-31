# README

## ¿QUÉ ES FILMSTER?

Filmster es un software para realizar listas de películas y poder exportarlas a -de momento- formato markdown.

En el futuro quiero añadir la exportación a pdf.

## OBJETIVOS

Este es un proyecto de aprendizaje para entender las bases de C++.

## INSTALACIÓN

```sh
sudo apt update && sudo apt install cmake
mkdir build
cd build
cmake ..
make
```

## API TOKEN Y API KEY

Para poder hacer llamadas a la API de TMDB (The Movie Database) necesitaras el
bearer token y la api key. Registrate en su página: https://www.themoviedb.org/ para
obtenerlos.

Una vez tengas el token copialo y pegalo en un archivo .txt llamado
apiToken.txt y la key en un archivo llamado apiKey.txt añadelo al
directorio build en la raíz del proyecto.

## ROADMAP / TASKLIST

+ ~~Añadir covers de las películas mediante llamadas a la api de tmdb
  utilizando la libreria CPR.~~
+ ~~Añadir genero de la película mediante llamada a la api de tmdb~~
+ ~~Añadir sinopsis de la película medienta llamada a la api de tmdb~~
+ Añadir exportación a PDF
+ Descargar imagen de portada en local





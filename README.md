# README

## ¿QUÉ ES FILMSTER?

Filmster es un software para realizar listas de películas y poder exportarlas a
formato markdown.

El proyecto nace de mi obsesión por mantener mi información de manera local y
en formato texto plano, como medida de independencia y resilencia digital. 

## INSTALACIÓN

```sh
git clone https://github.com/EnRo0t/Filmster.git && cd Filmster
sudo apt update && sudo apt install cmake
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

## ROADMAP / TASKLIST

**Versión 0.2**

+ Añadir una TUI mediante la librería FTXUI
+ Refactorización de código 



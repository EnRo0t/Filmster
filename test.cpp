#include <iostream>
#include <fstream> // para manipular archivos
#include <string>
#include <limits> // Necesario para numeric_limits
#include <vector> // Para incluir vectores
#include <nlohmann/json.hpp> // Para trabajar con json (persistencia de datos)
#include <unistd.h>
#include <filesystem>
#include <cpr/cpr.h>
#include <cctype>
#include <map>

using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace std;
using namespace fs;

// Constantes
const string CAMBIOS = "Los cambios se han producido correctamente";
const string AFIRMATIVO[8] = {"Si", "SI", "si", "s","YES", "Yes", "yes", "y"};
const string NEGATIVO[4] = {"No", "NO","no", "n"};



// Función para obtener el bearer token de la API
string obtenerApiToken() {
		ifstream archivo("apiToken.txt");
		if(!archivo.is_open()) {
				return "El archivo no existe";
		}
		string token;
		archivo >> token;
		archivo.close();
		return token;
}
// Función para obtener la API key
string obtenerApiKey() {
		ifstream archivo("apiKey.txt");
		if(!archivo.is_open()) {
				return "El archivo no existe";
		}
		string key;
		archivo >> key;
		archivo.close();
		return key;
}


const string APITOKEN = obtenerApiToken();
const string APIKEY = obtenerApiKey();

void crearDirectorio() {
		// Crear directorio si no existe
		string ruta = "assets";
		
		if(!exists(ruta)) {
				if(create_directory(ruta)) {
						cout << "Se ha creado el directorio " << ruta << " con exito" << "\n";
				}
		}
}

class Pelicula {
		//Visibilidad
		public:
		//Atributos
				string nombre;
				string comentario;
				float puntuacion;
				bool vista;
				// Constructor
				Pelicula(string n, string c, float p, bool v) {
						nombre = n;
						comentario = c;
						puntuacion = p;
						vista = v;
				}
};

vector<Pelicula> peliculas;

// Cargar el vector con los datos del json
vector<Pelicula> cargarPeliculas() {
    vector<Pelicula> peliculas;
	// Abrimos el archivo json
    ifstream archivo("peliculas.json");
    
    if (!archivo.is_open()) {
        return peliculas; // si no existe el archivo, devuelve vector vacío
    }
    // Se crea objeto json
    json j;
	// Cargamos el contenido del archivo al objeto json
    archivo >> j;
	// Cerramos el archivo
    archivo.close();
    
    for (auto& item : j) {
        Pelicula p(item["nombre"], item["comentario"], item["puntuacion"], item["vista"]);
        peliculas.push_back(p);
    }
    
    return peliculas;
}

// Guardar pelicula en un JSON
void guardarPeliculas(vector<Pelicula>& peliculas) {
    // Inicializamos un array de tipo json
    json j = json::array();
    
    for (int i = 0; i < peliculas.size(); i++) {
        j.push_back({
            {"id", i},
            {"nombre", peliculas[i].nombre},
            {"comentario", peliculas[i].comentario},
            {"puntuacion", peliculas[i].puntuacion},
			{"vista", peliculas[i].vista}
        });
    }
    // Abrimos el archivo en modo ios::out (borramos el archivo) 
    ofstream archivo("peliculas.json");
    archivo << j.dump(4); // el 4 es la indentación, para que quede legible
    archivo.close();
}

// Borrar Películas
void borrarPeliculas(vector<Pelicula>& peliculas) {
		int id;
		// Bucle para comprobar que el input del usuario este dentro de rango del vector peliculas
		do {
				cout << "Qué película quieres borrar (Id)" << "\n";
				cin >> id;
				
				if(id < 0 || id >= peliculas.size()) { 
						cout << "El id no es valido" << "\n";
				}
		} while (id < 0 || id >= peliculas.size());

		// Borramos la película del vector
		peliculas.erase(peliculas.begin() + id);
		// Cargamos la funcion guardarPeliculas para resetear el json con los cambios
		guardarPeliculas(peliculas);
		// Avisamos al usuario
		cout << "Película borrada" << "\n";
}



/* ------------------------------------------------------------ */
/*			       PETICIONES A LA API DE TMDB					*/
/*--------------------------------------------------------------*/

// Inicialización del map con los generos
map<int, string> generos;

// Devuelve el path del poster de la película
string tmdbCover(string busqueda) {
		json j;
		string cover;
		cpr::Response r = cpr::Get(
						cpr::Url{"https://api.themoviedb.org/3/search/movie"},
						cpr::Parameters{
								{"query", busqueda},
								{"language", "es"},
								{"api_key", APIKEY}
						}
				);
		if(r.status_code == 200) {
				// Parseamos el string de respuesta a json
				j = json::parse(r.text);
				cover = j["results"][0]["poster_path"];
				return cover;
		} else {
				cout << r.status_code << "\n";
				return "Ha habido un error";
		}
}
// Devuelve el path de la sinopsis
string tmdbSinopsis(string busqueda) {
		json j;
		string sinopsis;
		cpr::Response r = cpr::Get(
						cpr::Url{"https://api.themoviedb.org/3/search/movie"},
						cpr::Parameters{
								{"query", busqueda},
								{"language", "es"},
								{"api_key", APIKEY}
						}
				);
		if(r.status_code == 200) {
				// Parseamos respuesta a json
				j =  json::parse(r.text);
				// Metemos resultado de sinopsis en string
				sinopsis = j["results"][0]["overview"];
				// Devolvemos dicho string
				return sinopsis;
		} else {
				cout << r.status_code << "\n";
				return "Ha habido un error";
		}
}

// Devuelve el id del genero
int tmdbGenreID(string busqueda) {
		json j;
		int genreID;
		cpr::Response r = cpr::Get(
						cpr::Url{"https://api.themoviedb.org/3/search/movie"},
						cpr::Parameters{
								{"query", busqueda},
								{"language", "es"},
								{"api_key", APIKEY}
						}
				);
		if(r.status_code == 200) {
				// Parseamos respuesta a json
				j =  json::parse(r.text);
				// Metemos resultado de genre ids en un int (solo el primer elemento del primer resultado, de ahí esos 0's
				genreID = j["results"][0]["genre_ids"][0].get<int>();
				// Devolvemos dicho string
				return genreID;
		} else {
				cout << r.status_code << "\n";
				return 0;
		}
}

// Devuelve un string con el genero
string tmdbGenre(int id) {
		json j;
		cpr::Response r = cpr::Get(
						cpr::Url{"https://api.themoviedb.org/3/genre/movie/list"},
						cpr::Parameters{
								{"language","es"},
								{"api_key", APIKEY}
						}
				);
		if(r.status_code == 200) {
				// Convertimos respuesta a un string tipo json
				j = json::parse(r.text);
				// Limpiar mapa
				generos.clear();
				// Iteramos sobre el map genres y vamos metiendo los resultados del json
				for(const auto& elemento : j["genres"]) {
						int genre_id = elemento["id"].get<int>();
						string genre_name = elemento["name"].get<string>();

						generos[genre_id] = genre_name;
				}
				// Devolvemos el valor asociado a la clave = id
				cout << generos.at(id);
				return generos.at(id);
		} else {
				// Si falla, devolvemos un string con un aviso
				cout << r.status_code << "\n";
				return "Ha habido un error en la petición a la API";
		}
}

// Sirve para exportar en .md la lista de películas
void exportarPeliculas() {
    const string STAR_1 = "■";
	const string STAR_2 = "■■";
	const string STAR_3 = "■■■";
	const string STAR_4 = "■■■■";
	const string STAR_5 = "■■■■■";

	int contador = 0;
	// Lo abro en modo truncar para resetear el archivo
	ofstream Registro("peliculas.md");

	/* ofstream Registro("peliculas.md", ios::app); ios::app permite que no se borre el contenido previo del archivo */
	for(Pelicula pelicula : peliculas ) {
				float puntuacion =  peliculas.at(contador).puntuacion;
			    string puntuacionStar;
				string check;
				// Comprobar genero
				int gen;
				string genero;
				gen = tmdbGenreID(peliculas.at(contador).nombre);
				genero = tmdbGenre(gen);
				// Comprobar path de la imagen
				string img_path;
				img_path = tmdbCover(peliculas.at(contador).nombre);
				// Comprobar sinopsis
				string sinopsis;
				sinopsis = tmdbSinopsis(peliculas.at(contador).nombre);
				if(peliculas.at(contador).vista == true) { 
						check = "Sí";
				} else {
						check = "No";
				}
				if(puntuacion <= 2) {
						puntuacionStar = STAR_1;
				}else if(puntuacion <= 4) {
						puntuacionStar = STAR_2;
				}else if(puntuacion <= 6) {
						puntuacionStar = STAR_3;
				}else if(puntuacion <= 8) {
						puntuacionStar = STAR_4;
				}else {
						puntuacionStar = STAR_5;
				}
				Registro << "###" << " " << contador << "\n";
				Registro << "![cover](https://image.tmdb.org/t/p/w600_and_h900_face/" << img_path << ")" << "\n";
				Registro << "Nombre: " << peliculas.at(contador).nombre << "\n";
				Registro << "Genero: " << genero << "\n";
				Registro << "Sinopsis: " << sinopsis << "\n";
				Registro << "Comentario: " << peliculas.at(contador).comentario << "\n";
				Registro << "Puntuacion: " << puntuacionStar << " " << puntuacion << "\n";
				Registro << "Vista: " << check << "\n";
				Registro << "---" << "\n";
				contador++;
				
		}
				Registro.close(); // Cierra el archivo
	}

void verPeliculas() {
		string myText;
		ifstream archivo("peliculas.md"); // Abrimos el archivo
		while(getline (archivo, myText)) {
				cout << myText << "\n";
		}
		archivo.close();
}

int cli() {
		
		bool opcionErronea = true;
		int opcion; 
		while(opcionErronea) {
						cout << "╔═══════════════════════════╗" << "\n";
						cout << "║      Escoja una opción    ║" << "\n";
						cout << "╚═══════════════════════════╝" << "\n";
						cout << "╔═══════════════════════════╗" << "\n";
						cout << "║  1: Registrar película    ║" << "\n";
						cout << "║  2: Ver registro          ║" << "\n";
						cout << "║  3: Modificar película    ║" << "\n";
						cout << "║  4: Exportar lista        ║" << "\n";
						cout << "║  5: Borrar película       ║" << "\n";
						cout << "║  6: Salir                 ║" << "\n";
						cout << "╚═══════════════════════════╝" << "\n" << "\n";
						cin >> opcion;
						if(opcion == 1 || opcion == 2 || opcion == 3 || opcion == 4 || opcion == 5 || opcion == 6) {
								opcionErronea = false;
						} else {
								cout << "Introduce una opción válida" << "\n";
								cin.clear(); // Borramos buffer de entrada
								cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Para ignorar valores no INT 
						}

				}
						return opcion;
		}

bool continuar() {
    string confirmacion;
    
    cout << "¿Deseas continuar?" << "\n";
    cout << "S | N" << "\n";
    cin >> ws; 
    cin >> confirmacion;

    // Retorna true de forma directa si la condición se cumple, o false si no
    return (ranges::contains(AFIRMATIVO, confirmacion));
}

void modificarPelicula() {
		int id;
		string opcion;
		string nuevoNombre;
		string nuevoComentario;
		float nuevaPuntuacion;

		cout << "────────────────────────────────────────────" << "\n";
		cout << "¿Qué película quieres modificar? Introduce ID" << "\n";
		cout << "────────────────────────────────────────────" << "\n";
		cin >> id;
		cout << "¿Qué quieres modificar?" << "\n";
		cout << "Nombre | Comentario | Puntuación | Vista" << "\n";
		cin >> opcion;
		if(opcion == "nombre" || opcion == "Nombre") {
				cout << peliculas.at(id).nombre << "\n";
				cout << "¿Qué nuevo nombre quieres ponerle?" << "\n";
				cin.ignore();
				getline(cin, nuevoNombre);
				peliculas.at(id).nombre = nuevoNombre;
				cout << CAMBIOS << "\n";
				cout << "El nuevo nombre es: " << peliculas.at(id).nombre << "\n";
		}
		if(opcion == "comentario" || opcion == "Comentario") {	
				cout << peliculas.at(id).comentario << "\n";
				cout << "¿Qué nuevo comentario quieres escribir?" << "\n";
				cin.ignore();
				getline(cin, nuevoComentario);
				peliculas.at(id).comentario = nuevoComentario;
				cout << CAMBIOS << "\n";
				cout << "El nuevo comentario es: " << peliculas.at(id).comentario << "\n";
		} 
		if(opcion == "puntuacion" || opcion == "Puntuacion") {	
				cout << peliculas.at(id).puntuacion << "\n";
				cout << "¿Qué nueva puntuacion quieres ponerle?" << "\n";
				cin >> nuevaPuntuacion;
				do {
						cout << "Introduce la puntuación en decimal ( ej: 5.0)" << "\n";
						cin >> nuevaPuntuacion;

						if(nuevaPuntuacion < 0 || nuevaPuntuacion > 10) {
								cout << "Introduce la puntuacion debe estar entre 0 y 10" << "\n";
						}
				} while(nuevaPuntuacion < 0 || nuevaPuntuacion > 10);  
				peliculas.at(id).puntuacion = nuevaPuntuacion;
				cout << CAMBIOS << "\n";
				cout << "La nueva puntuacion es: " << peliculas.at(id).puntuacion << "\n";
		}
		if(opcion == "vista" || opcion == "Vista") {
				cout << peliculas.at(id).vista << "\n";
				cout << "Cambiando estado..." << "\n";
				if(peliculas.at(id).vista == true) {
						peliculas.at(id).vista = false;
				} else {
						peliculas.at(id).vista = true;
				}
				cout << "Cambio realizado, nuevo estado" << peliculas.at(id).vista << "\n";
		}
}

void prePeliculas() {
		int contador = 0;
		string check;
		for(Pelicula pelicula : peliculas) {
		if(pelicula.vista == true) {
				check = "Sí";
		} else {
				check = "No";
		}
				cout << "┌──────────────────────────────────┐" << "\n";
				cout << "  Id: " << contador << "\n";
				cout << "  Nombre: " << pelicula.nombre << "\n";
				cout << "  Comentario: " << pelicula.comentario << "\n";
				cout << "  Puntuacion: " << pelicula.puntuacion << "\n";
				cout << "  Vista: " << check << "\n";
				cout << "└──────────────────────────────────┘" << "\n";
				cout << "\n";
				contador++;
		}
} 

int main() {
		crearDirectorio();
		peliculas = cargarPeliculas();
		bool activo = true;
		while(activo) {
				int opcion = cli();
				switch(opcion) {
						case 1:{ 
												
								string nombre, comentario;
								float puntuacion;
								string check;
								bool vista;

								cout << "Introduce el nombre de la película" << "\n";
								cin.ignore(); // Para limpiar el buffer
								getline(cin, nombre); // Getline sirve para permitir espacios en el titulo
								cout << "¿Has visto la película?" << "\n";
								cout << "Si | No" << "\n";
								// Limpia el buffer de espacios en blanco
								cin >>	ws;
								getline(cin, check);
								if(ranges::contains(AFIRMATIVO, check)) {
										vista = true;
								} else if(check == "no" || check == "No") {
										vista = false;
								}
								if(vista == true) {
										string respC;
										string respP;
										cout << "¿Quieres introducir un comentario?" << "\n";
										cout << "Si | No" << "\n";
										cin >> ws;
										getline(cin, respC);
										if(ranges::contains(AFIRMATIVO, respC)) {
												cout << "Escribe tu comentario: " << "\n";
												getline(cin, comentario);
										} else {
												comentario = " ";
										}
										cout << "¿Quieres introducir una puntuación?" << "\n";
										cout << "Si | No" << "\n";
										cin >> ws;
										getline(cin, respP);
										if(ranges::contains(AFIRMATIVO, respP)) {
												do {
														cout << "Introduce la puntuación en decimal ( ej: 5.0)" << "\n";
														cin >> puntuacion;

														if(puntuacion < 0 || puntuacion > 10) {
																cout << "Introduce la puntuacion debe estar entre 0 y 10" << "\n";
														}
												} while(puntuacion < 0 || puntuacion > 10);
										} else {
												puntuacion = 0;
										}
								} else {
										comentario = " ";
										puntuacion = 0;
								}
								Pelicula pelicula(nombre, comentario, puntuacion, vista);
								// Guardo la película dentro del vector
								peliculas.push_back(pelicula);
								// Guardo la película dentro del json
								guardarPeliculas(peliculas);

								cout << "Película registrada correctamente" << "\n";
								activo = continuar();
						break;
							   }
							   
						case 2:
								prePeliculas();
								activo = continuar();
						break;
						case 3:
								modificarPelicula();
								guardarPeliculas(peliculas);
								activo = continuar();
						break;
						case 4:{
								string res;
								cout << "Exportando peliculas..." << "\n";
								sleep(1);
								cout << "..." << "\n";
								sleep(1);
								exportarPeliculas();
								cout << "Exportado archivo ./peliculas.md" <<
										"\n"; 
								cout << "----------------------------------" <<
										"\n"; 
								cout << "¿Quieres ver el archivo exportado?" <<
										"\n";
								cout << "Si | No" << "\n";
								cin >> ws;
								cin >> res;
								if(ranges::contains(AFIRMATIVO, res)) {
										verPeliculas(); 
								} else {
										activo = continuar();
								}
							   
						break;
							   }
						case 5:{
								borrarPeliculas(peliculas);
								activo = continuar();
							   
						break;
							   }
						case 6:{
								cout << "Bye Bye" << "\n";
								activo = false;
							   }
							   
						default:
						activo = false;
						break;
				
		}
}
		return 0;
}


#include <iostream>
#include <fstream> // para manipular archivos
#include <string>
#include <limits> // Necesario para numeric_limits
#include <vector> // Para incluir vectores
#include <nlohmann/json.hpp> // Para trabajar con json (persistencia de datos)
#include <unistd.h>
#include <filesystem>
#include <cpr/cpr.h>

using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace std;
using namespace fs;

const string CAMBIOS = "Los cambios se han producido correctamente";

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

const string APITOKEN = obtenerApiToken();

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
				string genero;
				float puntuacion;
				// Constructor
				Pelicula(string n, string g, float p) {
						nombre = n;
						genero = g;
						puntuacion = p;
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
        Pelicula p(item["nombre"], item["genero"], item["puntuacion"]);
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
            {"genero", peliculas[i].genero},
            {"puntuacion", peliculas[i].puntuacion}
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
				Registro << "![cover](assets/" << contador << ".webp)" << "\n";
				Registro << "Nombre: " << peliculas.at(contador).nombre << "\n";
				Registro << "Genero: " << peliculas.at(contador).genero << "\n";
				Registro << "Puntuacion: " << puntuacionStar << " " << puntuacion << "\n";
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
						cout << "╚═══════════════════════════╝" << "\n" << "\n";
						cin >> opcion;
						if(opcion == 1 || opcion == 2 || opcion == 3 || opcion == 4 || opcion == 5) {
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
    
    cin.ignore(); // Limpia cualquier 'Enter' residual de operaciones previas
    cin >> confirmacion;

    // Retorna true de forma directa si la condición se cumple, o false si no
    return (confirmacion == "S" || confirmacion == "s");
}

void modificarPelicula() {
		int id;
		string opcion;
		string nuevoNombre;
		string nuevoGenero;
		float nuevaPuntuacion;

		cout << "────────────────────────────────────────────" << "\n";
		cout << "¿Qué película quieres modificar? Introduce ID" << "\n";
		cout << "────────────────────────────────────────────" << "\n";
		cin >> id;
		cout << "¿Qué quieres modificar?" << "\n";
		cout << "Nombre | Genero | Puntuación" << "\n";
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
		if(opcion == "genero" || opcion == "Genero") {	
				cout << peliculas.at(id).genero << "\n";
				cout << "¿Qué nuevo genero quieres ponerle?" << "\n";
				cin.ignore();
				getline(cin, nuevoGenero);
				peliculas.at(id).genero = nuevoGenero;
				cout << CAMBIOS << "\n";
				cout << "El nuevo genero es: " << peliculas.at(id).genero << "\n";
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
}

void prePeliculas() {
		int contador = 0;
		for(Pelicula pelicula : peliculas) {

				cout << "┌──────────────────────────────────┐" << "\n";
				cout << "  Id: " << contador << "\n";
				cout << "  Nombre: " << pelicula.nombre << "\n";
				cout << "  Genero: " << pelicula.genero << "\n";
				cout << "  Puntuacion: " << pelicula.puntuacion << "\n";
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
												
								string nombre, genero;
								float puntuacion;
									
								cout << "Introduce el nombre de la película" << "\n";
								cin.ignore(); // Para limpiar el buffer
								getline(cin, nombre); // Getline sirve para permitir espacios en el titulo
								cout << "Introduce el genero" << "\n";
								getline(cin, genero);
								
								do {
										cout << "Introduce la puntuación en decimal ( ej: 5.0)" << "\n";
										cin >> puntuacion;

										if(puntuacion < 0 || puntuacion > 10) {
												cout << "Introduce la puntuacion debe estar entre 0 y 10" << "\n";
										}
								} while(puntuacion < 0 || puntuacion > 10);  
				
								Pelicula pelicula(nombre, genero, puntuacion);
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
								cout << "Exportalndo peliculas..." << "\n";
								sleep(1);
								cout << "...." << "\n";
								exportarPeliculas();
								cout << "Exportado archivo ./peliculas.md" <<
										"\n"; 
								cout << "----------------------------------" <<
										"\n"; 
								cout << "¿Quieres ver el archivo exportado?" <<
										"\n";
								cout << "Si | No" << "\n";
								cin >> res;
								if(res == "Si" || res == "si" ||
												res == "s") {
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
							   
						default:
						cout << "Bye Bye" << "\n";
						activo = false;
						break;
				
		}
}
		return 0;
}


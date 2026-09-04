# 1. Versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.15)

# 2. Nombre del proyecto
project(Filmster VERSION 1.0 LANGUAGES CXX)

# 3. Requerido por <filesystem> (C++20/C++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Incluir FetchContent para descargar dependencias
include(FetchContent)

# --- Configuración previa de wxWidgets ---
set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)        # Librería estática
set(wxBUILD_SAMPLES OFF CACHE BOOL "" FORCE)       # No compilar ejemplos
set(wxBUILD_DEMOS OFF CACHE BOOL "" FORCE)         # No compilar demos
set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)         # No compilar pruebas unitarias

# Forzar a wxWidgets a usar C++20 internamente (evita el error con C++23)
set(wxBUILD_CXX_STANDARD "20" CACHE STRING "" FORCE)

# --- Descargar CPR ---
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.10.x
)

# --- Descargar nlohmann/json ---
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

# --- Descargar wxWidgets ---
FetchContent_Declare(
    wxwidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG        v3.2.6
    GIT_SHALLOW    TRUE                            # Descarga rápida (solo último commit)
)

# Hacer disponibles todas las dependencias
FetchContent_MakeAvailable(cpr json wxwidgets)

# 5. Declarar el ejecutable (nombre del binario: peliculas.app, archivo fuente: test.cpp)
add_executable(peliculas.app test.cpp)

# 6. Enlazar las librerías necesarias al ejecutable
target_link_libraries(peliculas.app
    PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
        wx::core                                   # Componentes GUI básicos (botones, ventanas)
        wx::base                                   # Funciones base de wxWidgets
)

# 7. Compatibilidad con compiladores antiguos para <filesystem>
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(peliculas.app PRIVATE stdc++fs)
endif()

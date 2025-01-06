#include <iostream>
#include <cmath> // Para usar round
#include <string> // Para usar string
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include <iomanip>
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <fstream>

using namespace std;

// Proyecto de Algoritmos y Estructuras de Datos, Universidad Católica Andrés Bello, Hecho por Germán Moreno ci:31.449083, Jose Barazarte ci 30.464.544 y leonar Quiñónez ci:30.591.879

// Declarando los TDA

struct Zombie {
    string nombre;
    int vida;
    double resistencia;
    int velocidad;
    string inteligencia;
    int dano; // Daño del zombie
};

struct Objeto {
    string nombre;
    int dano;
};
struct ObjetosExistentes {
    Objeto objeto; // Suponiendo que 'Objeto' ya ha sido definido
    ObjetosExistentes* prox; // Puntero al siguiente objeto
};

struct Consumible {
    string nombre;
    int vida;
    int resistencia;
    int velocidad;
    int suerte;
};

struct ArmaCuerpoACuerpo {
    string nombre;
    int dano;
    bool aturdimiento;
    int distancia;
    int ruido;
    string crafteo[3];
};

struct ArmaArrojadiza {
    string nombre;
    int dano;
    bool aturdimiento;
    int distancia;
    int ruido;
    int usos;
    string crafteo[3];
};

struct ArmaADistancia {
    string nombre;
    int dano;
    bool aturdimiento;
    int distancia;
    int cadenciadetiro;
    int municion[2]; // [0] capacidad, [1] actual
    int ruido;
    string crafteo[3];
};

struct Mochila {
    ArmaADistancia arma_a_distancia;
    ArmaArrojadiza arma_arrojadiza;
    ArmaCuerpoACuerpo arma_cuerpo_a_cuerpo;
    Objeto objeto;
    Consumible consumible;

    void mostrarContenido() {
        cout << "Contenido de la mochila:" << endl;
        cout << "Arma a Distancia: " << arma_a_distancia.nombre << endl;
        cout << "Arma Arrojadiza: " << arma_arrojadiza.nombre << endl;
        cout << "Arma Cuerpo a Cuerpo: " << arma_cuerpo_a_cuerpo.nombre << endl;
        cout << "Objeto: " << objeto.nombre << endl;
        cout << "Consumible: " << consumible.nombre << endl;
    }
};
struct EquipoZombies {
    Zombie zombie;
    Mochila mochiladelosmalos;
    EquipoZombies* prox;
    EquipoZombies* anterior;
};
struct Soldado {
    string nombre;
    int vida;
    double resistencia;
    int velocidad;
    int suerte;
    int nivel;
    Mochila mochila;
};
struct EquipoSoldados {
    Soldado soldado;
    EquipoSoldados* prox; // Puntero al siguiente soldado
    EquipoSoldados* anterior;
};
struct EquiposSoldadosExistentes {
    string nombre;
    EquipoSoldados equipo;
    EquiposSoldadosExistentes* prox;
    EquiposSoldadosExistentes* anterior;
};
struct EquipoZombiesExistentes {
    string nombre;
    EquipoZombies equipo;
    EquipoZombiesExistentes* prox;
    EquipoZombiesExistentes* anterior;
};
struct ArmasPerdidas{
    string nombre;
    string tipo;
    ArmasPerdidas* prox;
};
struct SoldadosCaidos{  
    string nombre;
    SoldadosCaidos* prox;
};
struct ZombiesCaidos{
    string nombre;
    ZombiesCaidos* prox;
};

// Estructura para representar un nodo en el grafo
// Estructura para representar un nodo en el grafo
class Nodo {
public:
    string nombreEstacion;
    int cantidadZombies;
    bool tieneCura; // Nuevo atributo para indicar si el nodo tiene la cura
    vector<Nodo*> adyacentes; // Conexiones a otras estaciones
    EquiposSoldadosExistentes* equipoSoldados; // Equipo de soldados en el nodo
    EquipoZombiesExistentes* equipoZombies; // Equipo de zombies en el nodo

    Nodo(string nombre) : nombreEstacion(nombre), cantidadZombies(0), tieneCura(false), equipoSoldados(nullptr), equipoZombies(nullptr) {}
};

// Grafo que representa el mapa
class Grafo {
public:
    unordered_map<string, Nodo*> nodos;
    unordered_map<Nodo*, vector<Nodo*>> adyacencias; // Definición del mapa adyacencias

    void agregarArista(int Nodo1, int Nodo2) {
        string nodo1Str = to_string(Nodo1);
        string nodo2Str = to_string(Nodo2);
        if (nodos.find(nodo1Str) != nodos.end() && nodos.find(nodo2Str) != nodos.end()) {
            adyacencias[nodos[nodo1Str]].push_back(nodos[nodo2Str]);
        }
    }

    void leergrafoArchivo(string Accesorio_zmb, string Mapa_zmb, string Soldado_zmb, string Zombie_zmb) {
        ifstream archivo;
        // abrir cada archivo y leer las aristas
        for (const string& nombreArchivo : {Accesorio_zmb, Mapa_zmb, Soldado_zmb, Zombie_zmb}) {
            archivo.open(nombreArchivo);
            if (!archivo.is_open()) {
                cout << "error al abrir el archivo " << nombreArchivo << endl;
                return;
            }
            int Nodo1, Nodo2;
            while (archivo >> Nodo1 >> Nodo2) {
                agregarArista(Nodo1, Nodo2);
                // si el grafo no es dirigido, agregar también la inversa
                agregarArista(Nodo2, Nodo1);
            }
            archivo.close();
        }
    }

    void agregarNodo(string nombre) {
        nodos[nombre] = new Nodo(nombre);
    }

    void conectarNodos(string nombre1, string nombre2) {
        if (nodos.find(nombre1) != nodos.end() && nodos.find(nombre2) != nodos.end()) {
            nodos[nombre1]->adyacentes.push_back(nodos[nombre2]);
            nodos[nombre2]->adyacentes.push_back(nodos[nombre1]); // Conexión bidireccional
        }
    }

    void agregarZombie(string nombreEstacion, int cantidad) {
        if (nodos.find(nombreEstacion) != nodos.end()) {
            nodos[nombreEstacion]->cantidadZombies += cantidad;
        } else {
            cout << "Estación no encontrada." << endl;
        }
    }

    void asignarCura(string nombre) {
        if (nodos.find(nombre) != nodos.end()) {
            nodos[nombre]->tieneCura = true;
        } else {
            cout << "Nodo no encontrado: " << nombre << endl;
        }
    }

    void moverEquipo(EquiposSoldadosExistentes* equipo, const string& nombreNodo) {
        // Lógica para mover el equipo de soldados al nodo especificado
        Nodo* nodo = buscarNodo(nombreNodo);
        if (nodo != nullptr) {
            nodo->equipoSoldados = equipo;
            cout << "Equipo " << equipo->nombre << " movido al nodo " << nombreNodo << endl;
        } else {
            cout << "Nodo no encontrado." << endl;
        }
    }

    EquipoZombiesExistentes* obtenerEquipoZombies(const string& nombreNodo) {
        // Lógica para obtener el equipo de zombies en el nodo especificado
        Nodo* nodo = buscarNodo(nombreNodo);
        if (nodo != nullptr) {
            return nodo->equipoZombies;
        } else {
            cout << "Nodo no encontrado." << endl;
            return nullptr;
        }
    }

    Nodo* buscarNodo(const string& nombreNodo) {
        // Lógica para buscar y devolver el nodo con el nombre especificado
        if (nodos.find(nombreNodo) != nodos.end()) {
            return nodos[nombreNodo];
        }
        return nullptr;
    }

    ~Grafo() {
        for (auto& par : nodos) {
            delete par.second; // Liberar memoria de los nodos
        }
    }
};


void MostrarMensajeInicio() {
    // Cambiar el color a rojo
    cout << "\033[31m"; // Código ANSI para rojo
    cout << "\n\n\n\n\n\n"; 
    // Mensaje centrado
    string titulo = "        ZOMBIE WAR        ";
    string instruccion = "     Presiona Enter para comenzar...     ";

    // Calcular el espacio en blanco necesario para centrar
    int espacioTitulo = (80 - titulo.length()) / 2; 
    int espacioInstruccion = (80 - instruccion.length()) / 2;

    cout << setw(espacioTitulo) << "" << titulo << endl;
    cout << setw(espacioInstruccion) << "" << instruccion << endl;

    cout << "\033[0m"; // Restablecer el color

    // Esperar a que el usuario presione Enter
    cin.ignore(); 
    cin.get(); 
}

void BorrarPantalla() {
    // Comando para Windows
    #ifdef _WIN32
        system("cls");
    // Comando para Linux y macOS
    #else
        system("clear");
    #endif
}

// Funciones de Validación

bool ValidarSoldadoAlEntrar(Soldado soldado) {
    if (soldado.nombre.empty()) {
        cout << "ERROR001: El nombre del soldado no puede estar vacío." << endl;
        return false;
    }
    if (soldado.vida <= 0) {
        cout << "ERROR002: La vida del soldado debe ser superior a 0." << endl;
        return false;
    }
    if (soldado.nivel <= 0 || soldado.nivel > 10) {
        cout << "ERROR003: El nivel del soldado debe estar entre 1 y 10." << endl;
        return false;
    }
    if (soldado.resistencia < 0 || soldado.resistencia > 1) {
        cout << "ERROR004: La resistencia del soldado debe estar entre 0 y 1." << endl;
        return false;
    }
    if (soldado.suerte < 0 || soldado.suerte > 100) {
        cout << "ERROR005: La suerte del soldado debe estar entre 0 y 100." << endl;
        return false;
    }
    if (soldado.velocidad <= 0) {
        cout << "ERROR006: La velocidad del soldado debe ser mayor a 0." << endl;
        return false;
    }
    return true;
}

bool ValidarZombieAlAgregar(Zombie zombie) {
    if (zombie.nombre.empty()) {
        cout << "ERROR007: El nombre del zombie no puede estar vacío." << endl;
        return false;
    }
    if (zombie.vida <= 0) {
        cout << "ERROR008: La vida del zombie debe ser superior a 0." << endl;
        return false;
    }
    if (zombie.resistencia < 0 || zombie.resistencia > 1) {
        cout << "ERROR009: La resistencia del zombie debe estar entre 0 y 1." << endl;
        return false;
    }
    if (zombie.velocidad <= 0) {
        cout << "ERROR010: La velocidad del zombie debe ser mayor a 0." << endl;
        return false;
    }
    return true;
}

bool ValidarObjetoAlEntrar(Objeto objeto) {
    if (objeto.nombre.empty()) {
        cout << "ERROR012: El nombre del objeto no puede estar vacío." << endl;
        return false;
    }
    if (objeto.dano < 0) {
        cout << "ERROR013: El daño del objeto debe ser mayor o igual a 0." << endl;
        return false;
    }
    return true;
}

bool ValidarArmasADistanciaAlEntrar(ArmaADistancia arma) {
    if (arma.nombre.empty()) {
        cout << "ERROR014: El nombre del arma no puede estar vacío." << endl;
        return false;
    }
    if (arma.dano < 0) {
        cout << "ERROR015: El daño del arma debe ser mayor o igual a 0." << endl;
        return false;
    }
    if (arma.distancia <= 0) {
        cout << "ERROR016: La distancia del arma debe ser mayor a 0." << endl;
        return false;
    }
    if (arma.municion[0] <= 0) {
        cout << "ERROR017: La munición máxima del arma debe ser mayor a 0." << endl;
        return false;
    }
    if (arma.municion[1] < 0 || arma.municion[1] > arma.municion[0]) {
        cout << "ERROR018: La munición actual del arma debe estar entre 0 y su capacidad máxima." << endl;
        return false;
    }
    if (arma.ruido < 0 || arma.ruido > 10) {
        cout << "ERROR019: El ruido del arma debe estar entre 0 y 10." << endl;
        return false;
    }
    if (arma.cadenciadetiro <= 0) {
        cout << "ERROR2-000: La cadencia de tiro debe ser igual o mayor a 1." << endl;
    }
    return true;
}

bool ValidarArmasArrojadizasAlEntrar(ArmaArrojadiza arma) {
    if (arma.nombre.empty()) {
        cout << "ERROR020: El nombre del arma arrojadiza no puede estar vacío." << endl;
        return false;
    }
    if (arma.dano < 0) {
        cout << "ERROR021: El daño del arma arrojadiza debe ser mayor o igual a 0." << endl;
        return false;
    }
    if (arma.distancia <= 0) {
        cout << "ERROR022: La distancia del arma arrojadiza debe ser mayor a 0." << endl;
        return false;
    }
    if (arma.ruido < 0 || arma.ruido > 10) {
        cout << "ERROR023: El ruido del arma arrojadiza debe estar entre 0 y 10." << endl;
        return false;
    }
    if (arma.usos <= 0){
        cout << "ERROR2-001: La cantidad de usos del arma debe ser mayor o igual a 1." << endl;
    }
    return true;
}

bool ValidarArmasCuerpoACuerpoAlEntrar(ArmaCuerpoACuerpo arma) {
    if (arma.nombre.empty()) {
        cout << "ERROR024: El nombre del arma cuerpo a cuerpo no puede estar vacío." << endl;
        return false;
    }
    if (arma.dano < 0) {
        cout << "ERROR025: El daño del arma cuerpo a cuerpo debe ser mayor o igual a 0." << endl;
        return false;
    }
    if (arma.distancia <= 0) {
        cout << "ERROR026: La distancia del arma cuerpo a cuerpo debe ser mayor a 0." << endl;
        return false;
    }
    if (arma.ruido < 0 || arma.ruido > 10) {
        cout << "ERROR027: El ruido del arma cuerpo a cuerpo debe estar entre 0 y 10." << endl;
        return false;
    }
    return true;
}

bool ValidarEquipoSoldadosAlEntrar(EquipoSoldados* equipo){
    EquipoSoldados* actual = equipo;
    while (actual != NULL){
        if (!ValidarSoldadoAlEntrar(actual->soldado)){
            cout << "ERROR2-003: El soldado " << actual->soldado.nombre << " no es válido." << endl;
            return false;
        }
        actual = actual->prox;
    }
    return true;
}

bool ValidarEquipoZOmbieAlEntrar(EquipoZombies* equipo){
    EquipoZombies* actual = equipo;
    while (actual != NULL){
        if (!ValidarZombieAlAgregar(actual->zombie)){
            cout << "ERROR011: El zombie " << actual->zombie.nombre << " no es válido." << endl;
            return false;
        }
        actual = actual->prox;
        return true;
    }
}

bool ValidarEquipoZombiesExistentesAlEntrar(EquipoZombiesExistentes* equipo){
    EquipoZombiesExistentes* actual = equipo;
    while (actual != NULL){
        if (!ValidarZombieAlAgregar(actual->equipo.zombie)){
            cout << "ERROR011: El zombie " << actual->equipo.zombie.nombre << " no es válido." << endl;
            return false;
        }
        actual = actual->prox;
        return true;
    }
}

bool ValidarEquipoSoldadosExistentesAlEntrar(EquiposSoldadosExistentes* equipo){
    EquiposSoldadosExistentes* actual = equipo;
    while (actual != NULL){
        if (!ValidarSoldadoAlEntrar(actual->equipo.soldado)){
            cout << "ERROR011: El soldado " << actual->equipo.soldado.nombre << " no es válido." << endl;
            return false;
        }
        actual = actual->prox;
        return true;
    }
}

//Funcion de Creacion de Equipos y Multiequipos

EquipoSoldados CrearEquipoSoldados(Soldado* soldado){
    if(soldado == NULL){
        cout << "ERROR011: El soldado no es válido." << endl;
        return EquipoSoldados();
    }
    else{
        if(ValidarSoldadoAlEntrar(*soldado)){
            EquipoSoldados equipo;
            equipo.soldado = *soldado;
            cout << "El soldado " << soldado->nombre << " ha sido añadido al equipo." << endl;
            return equipo;
        }
        else{
            cout << "ERROR011: El soldado no es válido." << endl;
            return EquipoSoldados();
        }
    }
}

EquipoZombies CrearEquipoZombies(Zombie* zombie){
    if(zombie == NULL){
        cout << "ERROR011: El zombie no es válido." << endl;
        return EquipoZombies();
    }
    else{
        if(ValidarZombieAlAgregar(*zombie)){
            EquipoZombies equipo;
            equipo.zombie = *zombie;
            cout << "El zombie " << zombie->nombre << " ha sido añadido al equipo." << endl;
            return equipo;
        }
        else{
            cout << "ERROR011: El zombie no es válido." << endl;
            return EquipoZombies();
        }
    }
}

EquiposSoldadosExistentes CrearEquipoSoldadosExistentes(EquipoSoldados* equipo){
    string nombre;
    if(equipo == NULL){
        cout << "ERROR011: El equipo no es válido." << endl;
        return EquiposSoldadosExistentes();
    }
    else{
        if(ValidarEquipoSoldadosAlEntrar(equipo)){
            EquiposSoldadosExistentes equipoexistente;
            equipoexistente.equipo = *equipo;
            cout << "Como quiere que se llame el equipo?" << endl;
            cin >> nombre;
            equipoexistente.nombre = nombre;
            cout << "El equipo " << equipoexistente.nombre << " ha sido añadido al equipo." << endl;
            return equipoexistente;
        }
        else{
            cout << "ERROR011: El equipo no es válido." << endl;
            return EquiposSoldadosExistentes();
        }
    }
}

EquipoZombiesExistentes CrearEquipoZombiesExistentes(EquipoZombies* equipo){
    string nombre;
    if(equipo == NULL){
        cout << "ERROR011: El equipo no es válido." << endl;
        return EquipoZombiesExistentes();
    }
    else{
        if(ValidarEquipoZOmbieAlEntrar(equipo)){
            EquipoZombiesExistentes equipoexistente;
            equipoexistente.equipo = *equipo;
            cout << "Como quiere que se llame el equipo?" << endl;
            cin >> nombre;
            equipoexistente.nombre = nombre;
            cout << "El equipo " << equipoexistente.nombre << " ha sido añadido al equipo." << endl;
            return equipoexistente;
        }
        else{
            cout << "ERROR011: El equipo no es válido." << endl;
            return EquipoZombiesExistentes();
        }
    }
}

// Funciones de Búsqueda

void BuscarEnLaMochilaDelSoldado(Soldado soldado, int seleccion) {
    int respuesta;
    string nombredelarma;
    string nombredelobjetooconsumible;
    if (seleccion == 3) {
        cout << "Que tipo de arma buscas? \n\nNOTA: Selecciona (1) si es a Distancia, Selecciona (2) si es arrojadiza, Selecciona (3) si es cuerpo a cuerpo." << endl;
        cin >> respuesta;
        if (respuesta == 1) {
            cout << "Como se llama el arma que buscas?" << endl;
            cin >> nombredelarma;
            if (soldado.mochila.arma_a_distancia.nombre == nombredelarma) {
                cout << "\nEl arma " << nombredelarma << " si la tiene el soldado " << soldado.nombre << endl;
            } else {
                cout << "\nEl arma " << nombredelarma << " no la tiene el soldado " << soldado.nombre << endl;
            }
        }
        if (respuesta == 2) {
            cout << "Como se llama el arma que buscas?" << endl;
            cin >> nombredelarma;
            if (soldado.mochila.arma_arrojadiza.nombre == nombredelarma) {
                cout << "\nEl arma " << nombredelarma << " si la tiene el soldado " << soldado.nombre << endl;
            } else {
                cout << "\nEl arma " << nombredelarma << " no la tiene el soldado " << soldado.nombre << endl;
            }
        }
        if (respuesta == 3) {
            cout << "Como se llama el arma que buscas?" << endl;
            cin >> nombredelarma;
            if (soldado.mochila.arma_cuerpo_a_cuerpo.nombre == nombredelarma) {
                cout << "\nEl arma " << nombredelarma << " si la tiene el soldado " << soldado.nombre << endl;
            } else {
                cout << "\nEl arma " << nombredelarma << " no la tiene el soldado " << soldado.nombre << endl;
            }
        }
        if (respuesta != 1 && respuesta != 2 && respuesta != 3) {
            cout << "ERROR017: Ha introducido mal el numero, la busqueda se detendra." << endl;
        }
    }
    if (seleccion == 2) {
        cout << "Como se llama el objeto que estas buscando?" << endl;
        cin >> nombredelarma;
        if (soldado.mochila.objeto.nombre == nombredelobjetooconsumible) {
            cout << "\nEl objeto " << nombredelobjetooconsumible << " si la tiene el soldado " << soldado.nombre << endl;
        } else {
            cout << "\nEl objeto " << nombredelobjetooconsumible << " no la tiene el soldado " << soldado.nombre << endl;
        }
    }
    if (seleccion == 1) {
        cout << "Como se llama el consumible que estas buscando?" << endl;
        cin >> nombredelarma;
        if (soldado.mochila.consumible.nombre == nombredelobjetooconsumible) {
            cout << "\nEl consumible " << nombredelobjetooconsumible << " si la tiene el soldado " << soldado.nombre << endl;
        } else {
            cout << "\nEl consumible " << nombredelobjetooconsumible << " no la tiene el soldado " << soldado.nombre << endl;
        }
    }
}

// Función para buscar al soldado para la mochila
bool BuscarAlSoldadoParaLaMochila(EquipoSoldados* team, string nombre) {
    EquipoSoldados* actual = team; // Inicializa el puntero al equipo de soldados
    while (actual != nullptr) { // Recorre la lista de soldados
        if (actual->soldado.nombre == nombre) { // Verifica si el nombre coincide
            cout << "El soldado " << nombre << " ha sido encontrado." << endl;
            // Mostrar el contenido de la mochila del soldado
            actual->soldado.mochila.mostrarContenido();
            return true; // Retorna true si se encuentra el soldado
        }
        actual = actual->prox; // Avanza al siguiente soldado
    }
    cout << "El soldado " << nombre << " no se ha encontrado en el equipo." << endl; // Mensaje si no se encuentra
    return false; // Retorna false si no se encuentra el soldado
}
// Funciones para Busquedas Varias:

bool BuscarEnZombiesPorNombre(EquipoZombies* enemigos, string nombre) {
    EquipoZombies* actual = enemigos;
    while (actual != nullptr) {
        if (actual->zombie.nombre == nombre) {
            cout << "El zombie que busca sí está dentro del equipo enemigo" << endl;
            return true;
        }
        actual = actual->prox;
    }
    cout << "El zombie no se ha encontrado" << endl;
    return false;
}

bool BuscarEnItemsExistentesPorNombre(ObjetosExistentes* objetoabuscar, string nombre) {
    ObjetosExistentes* actual = objetoabuscar;
    while (actual != nullptr) {
        if (actual->objeto.nombre == nombre) {
            cout << "El objeto que busca sí existe" << endl;
            return true;
        }
        actual = actual->prox;
    }
    cout << "El objeto no se ha encontrado" << endl;
    return false;
}


// Función para buscar en soldados por nombre
bool BuscarEnSoldadosPorNombre(EquipoSoldados* team, string nombre) {
    EquipoSoldados* actual = team; // Inicializa el puntero al equipo de soldados
    while (actual != nullptr) { // Recorre la lista de soldados
        if (actual->soldado.nombre == nombre) { // Verifica si el nombre coincide
            cout << "El soldado " << nombre << " sí está en tu equipo." << endl;
            return true; // Retorna true si se encuentra el soldado
        }
        actual = actual->prox; // Avanza al siguiente soldado
    }
    cout << "El soldado " << nombre << " no se ha encontrado." << endl; // Mensaje si no se encuentra
    return false; // Retorna false si no se encuentra el soldado
}

void AnadirAlEquipoSoldadoPorDelante(EquipoSoldados*& team) {
    Soldado soldado;
    cout << "Nombre del soldado: ";
    cin >> soldado.nombre;
    cout << "Vida (1-100): ";
    cin >> soldado.vida;
    cout << "Resistencia (0-1): ";
    cin >> soldado.resistencia;
    cout << "Velocidad (0-100): ";
    cin >> soldado.velocidad;
    cout << "Suerte(0-100)(Suerte al saquear estableciminetos): ";
    cin >> soldado.suerte;
    cout << "Nivel (1-10): ";
    cin >> soldado.nivel;

    if (soldado.vida > 0 && soldado.nivel > 0 && soldado.nivel <= 10) {
        EquipoSoldados* nuevoSoldado = new EquipoSoldados;
        nuevoSoldado->soldado = soldado;
        nuevoSoldado->prox = team;
        team = nuevoSoldado;
        cout << "Soldado " << soldado.nombre << " agregado." << endl;
    } else {
        cout << "Datos del soldado no válidos." << endl;
    }
}

void AgregarZombie(EquipoZombies*& equipo) {
    Zombie nuevoZombie;
    cout << "Nombre del zombie: ";
    cin >> nuevoZombie.nombre;
    cout << "Vida: ";
    cin >> nuevoZombie.vida;
    cout << "Resistencia: ";
    cin >> nuevoZombie.resistencia;
    cout << "Velocidad: ";
    cin >> nuevoZombie.velocidad;
    cout << "Daño: ";
    cin >> nuevoZombie.dano; // Se agrega el daño del zombie

    // No se solicita la inteligencia, se deja como vacío
    nuevoZombie.inteligencia = "";

    if (ValidarZombieAlAgregar(nuevoZombie)) {
        EquipoZombies* nuevoNodo = new EquipoZombies;
        nuevoNodo->zombie = nuevoZombie;
        nuevoNodo->prox = equipo;
        nuevoNodo->anterior = nullptr; // O puedes manejarlo si es necesario
        if (equipo != nullptr) {
            equipo->anterior = nuevoNodo; // Conectar el nodo anterior si existe
        }
        equipo = nuevoNodo;
        cout << "Zombie " << nuevoZombie.nombre << " agregado." << endl;
    }
}

void CrearUnArmaAunSoldado(EquipoSoldados*& team) {
    int aturdimiento;
    int seleccion;
    if (team == NULL) {
        cout << "ERROR018: No puedes anadir armas si no tienes equipo." << endl;
    } else {
        cout << "Que tipo de arma quieres crear?\n\nNOTA: Seleccione (1) si es a distancia, (2) si es cuerpo a cuerpo, (3) si es arrojadiza." << endl;
        cin >> seleccion;
    }

    if (seleccion == 1 || seleccion == 2 || seleccion == 3) {
        if (seleccion == 1) {
            cout << "Cual es el nombre del arma a distancia?" << endl;
            ArmaADistancia CreandoArma;
            cin >> CreandoArma.nombre;
            cout << "Cual es el dano de este arma a distancia? \n\nNOTA: Debe ser mayor a 0." << endl;
            cin >> CreandoArma.dano;
            cout << "Esta arma puede aturdir? \n\nNOTA: Responda (1) si puede, responda (2) si no puede." << endl;
            cin >> aturdimiento;
            CreandoArma.aturdimiento = (aturdimiento == 1);
            cout << "Cual es la capacidad de municion de esta arma? \n\nNOTA: La capacidad debe ser superior a 0." << endl;
            cin >> CreandoArma.municion[0];
            cout << "Cuantas balas tendra ahora el arma?\n\nNOTA: La municion actual debe ser mayor a 0 y menor a la capacidad del arma." << endl;
            cin >> CreandoArma.municion[1];
            cout << "A que distancia podra atacar esta arma? \n\nNOTA: La distancia debe ser mayor a 0." << endl;
            cin >> CreandoArma.distancia;
            cout << "Que tanto ruido tendra el arma? \n\nNOTA: El ruido debe ser mayor a 0 y menor a 10." << endl;
            cin >> CreandoArma.ruido;
            cout << "Cual sera su cadencia de tiro? \n\nNOTA: La cadencia de tiro debe ser mayor o igual a 1." << endl;
            cin >> CreandoArma.cadenciadetiro;

            if (ValidarArmasADistanciaAlEntrar(CreandoArma)) {
                cout << "Como se llama el soldado que tendra el arma?" << endl;
                string nombredelsoldado;
                cin >> nombredelsoldado;
                EquipoSoldados* actual = team;
                while (actual != NULL) {
                    if (actual->soldado.nombre == nombredelsoldado) {
                        actual->soldado.mochila.arma_a_distancia = CreandoArma;
                        cout << "El arma " << CreandoArma.nombre << " fue anadida correctamente al soldado " << actual->soldado.nombre << endl;
                        break;
                    }
                    actual = actual->prox;
                }
            }
        }
        if (seleccion == 2){
            cout << "Cual es el nombre del arma cuerpo a cuerpo?" << endl;
            ArmaCuerpoACuerpo CreandoArma;
            cin >> CreandoArma.nombre;
            cout << "Cual es el dano de este arma cuerpo a cuerpo? \n\nNOTA: Debe ser mayor a 0." << endl;
            cin >> CreandoArma.dano;
            cout << "Esta arma puede aturdir? \n\nNOTA: Responda (1) si puede, responda (2) si no puede." << endl;
            cin >> aturdimiento;
            if(aturdimiento == 1 || aturdimiento == 2){
                if(aturdimiento == 1){
                    CreandoArma.aturdimiento = true;
                }
                else{
                    CreandoArma.aturdimiento = false;
                }
            }
            else{
                cout << "ERROR2-002: Ha introducido mal el valor de aturdimiento. Se pondra que no podra aturdir." << endl;
                CreandoArma.aturdimiento = false;
            }
            cout << "A que distancia podra atacar esta arma? \n\nNOTA: La distancia debe ser mayor a 0." << endl;
            cin >> CreandoArma.distancia;
            cout << "Que tanto ruido tendra el arma? \n\nNOTA: El ruido debe ser mayor a 0 y menor a 10." << endl;
            cin >> CreandoArma.ruido;
            if (ValidarArmasCuerpoACuerpoAlEntrar(CreandoArma)) {
                cout << "Como se llama el soldado que tendra el arma?" << endl;
                string nombredelsoldado;
                cin >> nombredelsoldado;
                EquipoSoldados* actual = team;
                while (actual != NULL) {
                    if (actual->soldado.nombre == nombredelsoldado) {
                        actual->soldado.mochila.arma_cuerpo_a_cuerpo = CreandoArma;
                        cout << "El arma " << CreandoArma.nombre << " fue anadida correctamente al soldado " << actual->soldado.nombre << endl;
                        break;
                    }
                    actual = actual->prox;
                }
            }
        }
        if (seleccion == 3){
            cout << "Cual es el nombre del arma arrojadiza?" << endl;
            ArmaArrojadiza CreandoArma;
            cin >> CreandoArma.nombre;
            cout << "Cual es el dano de este arma arrojadiza? \n\nNOTA: Debe ser mayor a 0." << endl;
            cin >> CreandoArma.dano;
            cout << "Esta arma puede aturdir? \n\nNOTA: Responda (1) si puede, responda (2) si no puede." << endl;
            cin >> aturdimiento;
            if(aturdimiento == 1 || aturdimiento == 2){
                if(aturdimiento == 1){
                    CreandoArma.aturdimiento = true;
                }
                else{
                    CreandoArma.aturdimiento = false;
                }
            }
            else{
                cout << "ERROR2-003: Ha introducido mal el valor de aturdimiento. Se pondra que no podra aturdir." << endl;
                CreandoArma.aturdimiento = false;
            }
            cout << "A que distancia podra atacar esta arma? \n\nNOTA: La distancia debe ser mayor a 0." << endl;
            cin >> CreandoArma.distancia;
            cout << "Que tanto ruido tendra el arma? \n\nNOTA: El ruido debe ser mayor a 0 y menor a 10." << endl;
            cin >> CreandoArma.ruido;
            cout << "Cuantos usos tendra el arma? \n\nNOTA: Los usos deben ser mayor a 0." << endl;
            cin >> CreandoArma.usos;
            if (ValidarArmasArrojadizasAlEntrar(CreandoArma)) {
                cout << "Como se llama el soldado que tendra el arma?" << endl;
                string nombredelsoldado;
                cin >> nombredelsoldado;
                EquipoSoldados* actual = team;
                while (actual != NULL) {
                    if (actual->soldado.nombre == nombredelsoldado) {
                        actual->soldado.mochila.arma_arrojadiza = CreandoArma;
                        cout << "El arma " << CreandoArma.nombre << " fue anadida correctamente al soldado " << actual->soldado.nombre << endl;
                        break;
                    }
                    actual = actual->prox;
                }
            }
        }
        
    } else {
        cout << "ERROR022: Ha introducido mal el valor a evaluar." << endl;
    }
}

// Funciones para manejar el mapa
void VerEstaciones(Grafo& mapa) {
    cout << "\n--- Lista de Estaciones ---\n";
    for (const auto& par : mapa.nodos) {
        cout << "Estación: " << par.first << ", Zombies: " << par.second->cantidadZombies << endl;
    }
}

void ModificarMapa(Grafo& mapa) {
    string nombreEstacion;
    cout << "Ingrese el nombre de la estación a modificar: ";
    cin >> nombreEstacion;

    if (mapa.nodos.find(nombreEstacion) != mapa.nodos.end()) {
        cout << "Modificar cantidad de zombies (actual: " << mapa.nodos[nombreEstacion]->cantidadZombies << "): ";
        int nuevaCantidad;
        cin >> nuevaCantidad;
        mapa.nodos[nombreEstacion]->cantidadZombies = nuevaCantidad;
        cout << "Estación modificada correctamente." << endl;
    } else {
        cout << "Estación no encontrada." << endl;
    }
}

void EliminarMapa(Grafo& mapa) {
    string nombreEstacion;
    cout << "Ingrese el nombre de la estación a eliminar: ";
    cin >> nombreEstacion;

    if (mapa.nodos.find(nombreEstacion) != mapa.nodos.end()) {
        delete mapa.nodos[nombreEstacion]; // Liberar memoria
        mapa.nodos.erase(nombreEstacion); // Eliminar del mapa
        cout << "Estación eliminada correctamente." << endl;
    } else {
        cout << "Estación no encontrada." << endl;
    }
}

void CrearObjeto(ObjetosExistentes*& listaObjetos) {
    Objeto nuevoObjeto;
    cout << "Ingrese el nombre del objeto: ";
    cin >> nuevoObjeto.nombre;
    cout << "Ingrese el daño del objeto: ";
    cin >> nuevoObjeto.dano;

    // Crear un nuevo nodo para el objeto
    ObjetosExistentes* nuevoNodo = new ObjetosExistentes;
    nuevoNodo->objeto = nuevoObjeto;
    nuevoNodo->prox = listaObjetos; // Insertar al principio de la lista
    listaObjetos = nuevoNodo;

    cout << "El objeto " << nuevoObjeto.nombre << " ha sido creado correctamente." << endl;
}

void CrearConsumible(EquipoSoldados*& equipo) {
    Consumible nuevoConsumible;
    cout << "Ingrese el nombre del consumible: ";
    cin >> nuevoConsumible.nombre;
    cout << "Ingrese la vida que otorga: ";
    cin >> nuevoConsumible.vida;
    cout << "Ingrese la resistencia que otorga: ";
    cin >> nuevoConsumible.resistencia;
    cout << "Ingrese la velocidad que otorga: ";
    cin >> nuevoConsumible.velocidad;
    cout << "Ingrese la suerte que otorga: ";
    cin >> nuevoConsumible.suerte;

    // Agregar el consumible a la mochila de un soldado
    string nombreSoldado;
    cout << "Ingrese el nombre del soldado que recibirá el consumible: ";
    cin >> nombreSoldado;

    EquipoSoldados* actual = equipo;
    while (actual != nullptr) {
        if (actual->soldado.nombre == nombreSoldado) {
            actual->soldado.mochila.consumible = nuevoConsumible;
            cout << "El consumible " << nuevoConsumible.nombre << " ha sido agregado al soldado " << nombreSoldado << "." << endl;
            return;
        }
        actual = actual->prox;
    }
    cout << "Soldado no encontrado." << endl;
}
void ModificarSoldado(EquipoSoldados*& equipo) {
    if (equipo == nullptr) {
        cout << "No hay soldados en el equipo." << endl;
        return;
    }

    // Mostrar la lista de soldados
    EquipoSoldados* actual = equipo;
    int index = 1; // Contador para los números
    cout << "Seleccione el soldado a modificar:" << endl;

    while (actual != nullptr) {
        cout << index << ". " << actual->soldado.nombre << endl;
        actual = actual->prox;
        index++;
    }

    int seleccion;
    cout << "Ingrese el número del soldado a modificar: ";
    cin >> seleccion;

    // Validar la selección
    actual = equipo; // Reiniciar el puntero para volver al inicio
    index = 1; // Reiniciar el contador

    while (actual != nullptr) {
        if (index == seleccion) {
            // Se encontró el soldado seleccionado
            cout << "Modificar vida (actual: " << actual->soldado.vida << "): ";
            int nuevaVida;
            cin >> nuevaVida;
            if (nuevaVida > 0) {
                actual->soldado.vida = nuevaVida;
            } else {
                cout << "La vida debe ser mayor a 0." << endl;
            }

            cout << "Modificar resistencia (actual: " << actual->soldado.resistencia << "): ";
            double nuevaResistencia;
            cin >> nuevaResistencia;
            if (nuevaResistencia >= 0 && nuevaResistencia <= 1) {
                actual->soldado.resistencia = nuevaResistencia;
            } else {
                cout << "La resistencia debe estar entre 0 y 1." << endl;
            }

            cout << "Modificar velocidad (actual: " << actual->soldado.velocidad << "): ";
            int nuevaVelocidad;
            cin >> nuevaVelocidad;
            if (nuevaVelocidad > 0) {
                actual->soldado.velocidad = nuevaVelocidad;
            } else {
                cout << "La velocidad debe ser mayor a 0." << endl;
            }

            cout << "Modificar suerte (actual: " << actual->soldado.suerte << "): ";
            int nuevaSuerte;
            cin >> nuevaSuerte;
            if (nuevaSuerte >= 0 && nuevaSuerte <= 100) {
                actual->soldado.suerte = nuevaSuerte;
            } else {
                cout << "La suerte debe estar entre 0 y 100." << endl;
            }

            cout << "Modificar nivel (actual: " << actual->soldado.nivel << "): ";
            int nuevoNivel;
            cin >> nuevoNivel;
            if (nuevoNivel > 0 && nuevoNivel <= 10) {
                actual->soldado.nivel = nuevoNivel;
            } else {
                cout << "El nivel debe estar entre 1 y 10." << endl;
            }

            cout << "Soldado modificado correctamente." << endl;
            return; // Salir después de modificar
        }
        actual = actual->prox; // Avanzar al siguiente soldado
        index++;
    }

    cout << "Selección no válida." << endl; // Mensaje si la selección es inválida
}
void ModificarArma(EquipoSoldados*& equipo) {
    string nombreSoldado;
    cout << "Ingrese el nombre del soldado al que se le modificará el arma: ";
    cin >> nombreSoldado;

    EquipoSoldados* actual = equipo;
    while (actual != nullptr) {
        if (actual->soldado.nombre == nombreSoldado) {
            cout << "¿Qué tipo de arma desea modificar? (1: a distancia, 2: arrojadiza, 3: cuerpo a cuerpo): ";
            int tipo;
            cin >> tipo;

            if (tipo == 1) {
                cout << "Modificar nombre del arma a distancia (actual: " << actual->soldado.mochila.arma_a_distancia.nombre << "): ";
                cin >> actual->soldado.mochila.arma_a_distancia.nombre;
                // Modificar otros atributos de la arma...
            } else if (tipo == 2) {
                cout << "Modificar nombre del arma arrojadiza (actual: " << actual->soldado.mochila.arma_arrojadiza.nombre << "): ";
                cin >> actual->soldado.mochila.arma_arrojadiza.nombre;
                // Modificar otros atributos de la arma...
            } else if (tipo == 3) {
                cout << "Modificar nombre del arma cuerpo a cuerpo (actual: " << actual->soldado.mochila.arma_cuerpo_a_cuerpo.nombre << "): ";
                cin >> actual->soldado.mochila.arma_cuerpo_a_cuerpo.nombre;
                // Modificar otros atributos de la arma...
            } else {
                cout << "Tipo de arma no válido." << endl;
            }
            return;
        }
        actual = actual->prox;
    }
    cout << "Soldado no encontrado." << endl;
}
void ModificarObjeto(ObjetosExistentes*& listaObjetos) {
    string nombreObjeto;
    cout << "Ingrese el nombre del objeto a modificar: ";
    cin >> nombreObjeto;

    ObjetosExistentes* actual = listaObjetos;
    while (actual != nullptr) {
        if (actual->objeto.nombre == nombreObjeto) {
            cout << "Modificar daño del objeto (actual: " << actual->objeto.dano << "): ";
            cin >> actual->objeto.dano;
            cout << "Objeto modificado correctamente." << endl;
            return;
        }
        actual = actual->prox;
    }
    cout << "Objeto no encontrado." << endl;
}
void ModificarConsumible(EquipoSoldados*& equipo) {
    string nombreSoldado;
    cout << "Ingrese el nombre del soldado al que se le modificará el consumible: ";
    cin >> nombreSoldado;

    EquipoSoldados* actual = equipo;
    while (actual != nullptr) {
        if (actual->soldado.nombre == nombreSoldado) {
            cout << "Modificar nombre del consumible (actual: " << actual->soldado.mochila.consumible.nombre << "): ";
            cin >> actual->soldado.mochila.consumible.nombre;
            // Modificar otros atributos del consumible...
            return;
        }
        actual = actual->prox;
    }
    cout << "Soldado no encontrado." << endl;
}
void MostrarZombies(EquipoZombies* enemigos) {
    if (enemigos == nullptr) {
        cout << "No hay zombies en el equipo." << endl;
        return;
    }

    cout << "\n--- Lista de Zombies ---\n";
    int index = 1;
    EquipoZombies* actual = enemigos;
    while (actual != nullptr) {
        cout << index << ". " << actual->zombie.nombre << endl;
        actual = actual->prox;
        index++;
    }
}
void ModificarZombie(EquipoZombies*& enemigos) {
    MostrarZombies(enemigos);

    int seleccion;
    cout << "Seleccione el número del zombie a modificar: ";
    cin >> seleccion;

    EquipoZombies* actual = enemigos;
    int index = 1;

    while (actual != nullptr) {
        if (index == seleccion) {
            cout << "Modificar vida (actual: " << actual->zombie.vida << "): ";
            cin >> actual->zombie.vida;

            cout << "Modificar resistencia (actual: " << actual->zombie.resistencia << "): ";
            cin >> actual->zombie.resistencia;

            cout << "Modificar velocidad (actual: " << actual->zombie.velocidad << "): ";
            cin >> actual->zombie.velocidad;

            cout << "Zombie modificado correctamente." << endl;
            return;
        }
        actual = actual->prox;
        index++;
    }
    
    cout << "Selección no válida." << endl;
}

void EliminarZombie(EquipoZombies*& equipo) {
    if (equipo == nullptr) {
        cout << "No hay zombies en el equipo." << endl;
        return;
    }

    string nombreZombie;
    cout << "Ingrese el nombre del zombie a eliminar: ";
    cin >> nombreZombie;

    EquipoZombies* actual = equipo;
    EquipoZombies* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->zombie.nombre == nombreZombie) {
            if (anterior == nullptr) {
                // Eliminar el primer nodo
                equipo = actual->prox; // Actualizar la cabeza
            } else {
                // Eliminar nodo intermedio o final
                anterior->prox = actual->prox;
            }
            delete actual; // Liberar la memoria
            cout << "Zombie " << nombreZombie << " eliminado." << endl;
            return;
        }
        anterior = actual;
        actual = actual->prox;
    }
    cout << "Zombie no encontrado." << endl;
}
void MostrarSoldados(EquipoSoldados* equipo) {
    if (equipo == nullptr) {
        cout << "No hay soldados en el equipo." << endl;
        return;
    }

    EquipoSoldados* actual = equipo;
    cout << "\n--- Lista de Soldados ---\n";
    while (actual != nullptr) {
        cout << "Nombre: " << actual->soldado.nombre << endl;
        cout << "Vida: " << actual->soldado.vida << endl;
        cout << "Resistencia: " << actual->soldado.resistencia << endl;
        cout << "Velocidad: " << actual->soldado.velocidad << endl;
        cout << "Suerte: " << actual->soldado.suerte << endl;
        cout << "Nivel: " << actual->soldado.nivel << endl;
        // Mostrar armas en la mochila
        cout << "Armas en la mochila:\n";
        cout << "  - A Distancia: " << actual->soldado.mochila.arma_a_distancia.nombre << endl;
        cout << "  - Arrojadiza: " << actual->soldado.mochila.arma_arrojadiza.nombre << endl;
        cout << "  - Cuerpo a Cuerpo: " << actual->soldado.mochila.arma_cuerpo_a_cuerpo.nombre << endl;
        cout << "Objeto: " << actual->soldado.mochila.objeto.nombre << endl;
        cout << "Consumible: " << actual->soldado.mochila.consumible.nombre << endl;
        cout << "------------------------\n";
        actual = actual->prox;
    }
}

void MostrarZombie(EquipoZombies* enemigos) {
    if (enemigos == nullptr) {
        cout << "No hay zombies en el equipo." << endl;
        return;
    }

    EquipoZombies* actual = enemigos;
    cout << "\n--- Lista de Zombies ---\n";
    while (actual != nullptr) {
        cout << "Nombre: " << actual->zombie.nombre << endl;
        cout << "Vida: " << actual->zombie.vida << endl;
        cout << "Resistencia: " << actual->zombie.resistencia << endl;
        cout << "Velocidad: " << actual->zombie.velocidad << endl;
        cout << "------------------------\n";
        actual = actual->prox;
    }
}

void MostrarObjetos(ObjetosExistentes* listaObjetos) {
    if (listaObjetos == nullptr) {
        cout << "No hay objetos en la lista." << endl;
        return;
    }

    ObjetosExistentes* actual = listaObjetos;
    cout << "\n--- Lista de Objetos ---\n";
    while (actual != nullptr) {
        cout << "Nombre: " << actual->objeto.nombre << endl;
        cout << "Daño: " << actual->objeto.dano << endl;
        cout << "------------------------\n";
        actual = actual->prox;
    }
}

void MostrarConsumibles(EquipoSoldados* equipo) {
    if (equipo == nullptr) {
        cout << "No hay soldados en el equipo." << endl;
        return;
    }

    EquipoSoldados* actual = equipo;
    cout << "\n--- Lista de Consumibles ---\n";
    while (actual != nullptr) {
        cout << "Nombre del soldado: " << actual->soldado.nombre << endl;
        cout << "Consumible: " << actual->soldado.mochila.consumible.nombre << endl;
        cout << "------------------------\n";
        actual = actual->prox;
    }
}

void MostrarSoldad(EquipoSoldados* equipo) {
    if (equipo == nullptr) {
        cout << "No hay soldados en el equipo." << endl;
        return;
    }

    cout << "\n--- Lista de Soldados ---\n";
    int index = 1;
    EquipoSoldados* actual = equipo;
    while (actual != nullptr) {
        cout << index << ". " << actual->soldado.nombre << endl;
        actual = actual->prox;
        index++;
    }
}

void EliminarSoldado(EquipoSoldados*& equipo) {
    if (equipo == nullptr) {
        cout << "No hay soldados en el equipo." << endl;
        return;
    }

    MostrarSoldad(equipo);
    int seleccion;
    cout << "Seleccione el número del soldado a eliminar: ";
    cin >> seleccion;

    EquipoSoldados* actual = equipo;
    EquipoSoldados* anterior = nullptr;
    int index = 1;

    while (actual != nullptr) {
        if (index == seleccion) {
            if (anterior == nullptr) {
                // Eliminar el primer nodo
                equipo = actual->prox; // Actualizar la cabeza
            } else {
                // Eliminar nodo intermedio o final
                anterior->prox = actual->prox;
            }
            delete actual; // Liberar la memoria
            cout << "Soldado eliminado." << endl;
            return;
        }
        anterior = actual;
        actual = actual->prox;
        index++;
    }
    cout << "Selección no válida." << endl;
}
void MostrarObj(ObjetosExistentes* listaObjetos) {
    if (listaObjetos == nullptr) {
        cout << "No hay objetos en la lista." << endl;
        return;
    }

    cout << "\n--- Lista de Objetos ---\n";
    int index = 1;
    ObjetosExistentes* actual = listaObjetos;
    while (actual != nullptr) {
        cout << index << ". " << actual->objeto.nombre << endl;
        actual = actual->prox;
        index++;
    }
}
void EliminarObjeto(ObjetosExistentes*& listaObjetos) {
    if (listaObjetos == nullptr) {
        cout << "No hay objetos en la lista." << endl;
        return;
    }

    MostrarObj(listaObjetos);
    int seleccion;
    cout << "Seleccione el número del objeto a eliminar: ";
    cin >> seleccion;

    ObjetosExistentes* actual = listaObjetos;
    ObjetosExistentes* anterior = nullptr;
    int index = 1;

    while (actual != nullptr) {
        if (index == seleccion) {
            if (anterior == nullptr) {
                listaObjetos = actual->prox; // Actualizar la cabeza
            } else {
                anterior->prox = actual->prox;
            }
            delete actual;
            cout << "Objeto eliminado." << endl;
            return;
        }
        anterior = actual;
        actual = actual->prox;
        index++;
    }
    cout << "Selección no válida." << endl;
}
void EliminarConsumible(EquipoSoldados*& equipo) {
    if (equipo == nullptr) {
        cout << "No hay soldados en el equipo." << endl;
        return;
    }

    MostrarSoldad(equipo);
    int seleccion;
    cout << "Seleccione el número del soldado del que desea eliminar el consumible: ";
    cin >> seleccion;

    EquipoSoldados* actual = equipo;
    int index = 1;

    while (actual != nullptr) {
        if (index == seleccion) {
            actual->soldado.mochila.consumible = Consumible(); // Resetear el consumible
            cout << "Consumible eliminado del soldado." << endl;
            return;
        }
        actual = actual->prox;
        index++;
    }
    cout << "Selección no válida." << endl;
}
void MostrarArm(Soldado soldado) {
    cout << "\n--- Armas del Soldado " << soldado.nombre << " ---\n";
    cout << "1. Arma a Distancia: " << soldado.mochila.arma_a_distancia.nombre << endl;
    cout << "2. Arma Arrojadiza: " << soldado.mochila.arma_arrojadiza.nombre << endl;
    cout << "3. Arma Cuerpo a Cuerpo: " << soldado.mochila.arma_cuerpo_a_cuerpo.nombre << endl;
}

void EliminarArma(EquipoSoldados*& equipo) {
    if (equipo == nullptr) {
        cout << "No hay soldados en el equipo." << endl;
        return;
    }

    MostrarSoldad(equipo);
    int seleccion;
    cout << "Seleccione el número del soldado del que desea eliminar un arma: ";
    cin >> seleccion;

    EquipoSoldados* actual = equipo;
    int index = 1;

    while (actual != nullptr) {
        if (index == seleccion) {
            MostrarArm(actual->soldado);
            int armaSeleccionada;
            cout << "Seleccione el número del arma a eliminar: ";
            cin >> armaSeleccionada;

            switch (armaSeleccionada) {
                case 1:
                    actual->soldado.mochila.arma_a_distancia = ArmaADistancia(); // Resetear el arma a distancia
                    cout << "Arma a Distancia eliminada." << endl;
                    break;
                case 2:
                    actual->soldado.mochila.arma_arrojadiza = ArmaArrojadiza(); // Resetear el arma arrojadiza
                    cout << "Arma Arrojadiza eliminada." << endl;
                    break;
                case 3:
                    actual->soldado.mochila.arma_cuerpo_a_cuerpo = ArmaCuerpoACuerpo(); // Resetear el arma cuerpo a cuerpo
                    cout << "Arma Cuerpo a Cuerpo eliminada." << endl;
                    break;
                default:
                    cout << "Selección no válida." << endl;
                    break;
            }
            return;
        }
        actual = actual->prox;
        index++;
    }
    cout << "Selección no válida." << endl;
}

// Funciones de Modificación de MultiEquipos

void ModificarEquipoSoldados(EquiposSoldadosExistentes& listaEquipos) {
    // Verificar si la lista de equipos está vacía
    if (listaEquipos.nombre.empty() && listaEquipos.prox == nullptr) {
        cout << "ERROR011: No hay equipos de soldados disponibles." << endl;
        return;
    }

    // Mostrar la lista de equipos
    int index = 1;
    EquiposSoldadosExistentes* actualEquipo = &listaEquipos;
    cout << "\n--- Equipos de Soldados ---\n";
    while (actualEquipo != nullptr) {
        cout << index << ". " << actualEquipo->nombre << endl;
        actualEquipo = actualEquipo->prox; // Avanzar al siguiente equipo
        index++;
    }

    int seleccion;
    cout << "Seleccione el número del equipo a modificar: ";
    cin >> seleccion;

    // Reiniciar el puntero para volver al inicio
    actualEquipo = &listaEquipos;
    index = 1;

    while (actualEquipo != nullptr) {
        if (index == seleccion) {
            // Se encontró el equipo seleccionado
            EquipoSoldados* actualSoldado = &actualEquipo->equipo; // Trabajando con el objeto directamente
            if (actualSoldado == nullptr || actualSoldado->soldado.nombre.empty()) {
                cout << "ERROR011: El equipo seleccionado no tiene soldados." << endl;
                return;
            }

            // Mostrar la lista de soldados en el equipo
            int indexSoldado = 1;
            cout << "\n--- Lista de Soldados en el Equipo: " << actualEquipo->nombre << " ---\n";
            while (actualSoldado != nullptr) {
                cout << indexSoldado << ". " << actualSoldado->soldado.nombre << endl;
                actualSoldado = actualSoldado->prox; // Avanzar al siguiente soldado
                indexSoldado++;
            }

            cout << "Seleccione el número del soldado a modificar: ";
            cin >> seleccion;

            // Reiniciar el puntero para volver al inicio
            actualSoldado = &actualEquipo->equipo;
            indexSoldado = 1;

            while (actualSoldado != nullptr) {
                if (indexSoldado == seleccion) {
                    // Modificar atributos del soldado
                    cout << "Modificar vida (actual: " << actualSoldado->soldado.vida << "): ";
                    int nuevaVida;
                    cin >> nuevaVida;
                    if (nuevaVida > 0) {
                        actualSoldado->soldado.vida = nuevaVida;
                    } else {
                        cout << "La vida debe ser mayor a 0." << endl;
                    }

                    cout << "Modificar resistencia (actual: " << actualSoldado->soldado.resistencia << "): ";
                    double nuevaResistencia;
                    cin >> nuevaResistencia;
                    if (nuevaResistencia >= 0 && nuevaResistencia <= 1) {
                        actualSoldado->soldado.resistencia = nuevaResistencia;
                    } else {
                        cout << "La resistencia debe estar entre 0 y 1." << endl;
                    }

                    cout << "Modificar velocidad (actual: " << actualSoldado->soldado.velocidad << "): ";
                    int nuevaVelocidad;
                    cin >> nuevaVelocidad;
                    if (nuevaVelocidad > 0) {
                        actualSoldado->soldado.velocidad = nuevaVelocidad;
                    } else {
                        cout << "La velocidad debe ser mayor a 0." << endl;
                    }

                    cout << "Modificar suerte (actual: " << actualSoldado->soldado.suerte << "): ";
                    int nuevaSuerte;
                    cin >> nuevaSuerte;
                    if (nuevaSuerte >= 0 && nuevaSuerte <= 100) {
                        actualSoldado->soldado.suerte = nuevaSuerte;
                    } else {
                        cout << "La suerte debe estar entre 0 y 100." << endl;
                    }

                    cout << "Modificar nivel (actual: " << actualSoldado->soldado.nivel << "): ";
                    int nuevoNivel;
                    cin >> nuevoNivel;
                    if (nuevoNivel > 0 && nuevoNivel <= 10) {
                        actualSoldado->soldado.nivel = nuevoNivel;
                    } else {
                        cout << "El nivel debe estar entre 1 y 10." << endl;
                    }

                    cout << "Soldado modificado correctamente." << endl;
                    return; // Salir después de modificar
                }
                actualSoldado = actualSoldado->prox; // Avanzar al siguiente soldado
                indexSoldado++;
            }

            cout << "Selección no válida." << endl; // Mensaje si la selección es inválida
            return;
        }
        actualEquipo = actualEquipo->prox; // Avanzar al siguiente equipo
        index++;
    }

    cout << "Selección no válida." << endl; // Mensaje si la selección es inválida
}

void ModificarEquipoZombies(EquipoZombiesExistentes& listaEquipos) {
    // Verificar si la lista de equipos está vacía
    if (listaEquipos.nombre.empty() && listaEquipos.prox == nullptr) {
        cout << "ERROR011: No hay equipos de zombies disponibles." << endl;
        return;
    }

    // Mostrar la lista de equipos
    int index = 1;
    EquipoZombiesExistentes* actualEquipo = &listaEquipos;
    cout << "\n--- Equipos de Zombies ---\n";
    while (actualEquipo != nullptr) {
        cout << index << ". " << actualEquipo->nombre << endl;
        actualEquipo = actualEquipo->prox; // Avanzar al siguiente equipo
        index++;
    }

    int seleccion;
    cout << "Seleccione el número del equipo a modificar: ";
    cin >> seleccion;

    // Reiniciar el puntero para volver al inicio
    actualEquipo = &listaEquipos;
    index = 1;

    while (actualEquipo != nullptr) {
        if (index == seleccion) {
            // Se encontró el equipo seleccionado
            EquipoZombies* actualZombie = &actualEquipo->equipo; // Trabajando con el objeto directamente
            if (actualZombie == nullptr || actualZombie->zombie.nombre.empty()) {
                cout << "ERROR011: El equipo seleccionado no tiene zombies." << endl;
                return;
            }

            // Mostrar la lista de zombies en el equipo
            int indexZombie = 1;
            cout << "\n--- Lista de Zombies en el Equipo: " << actualEquipo->nombre << " ---\n";
            while (actualZombie != nullptr) {
                cout << indexZombie << ". " << actualZombie->zombie.nombre << endl;
                actualZombie = actualZombie->prox; // Avanzar al siguiente zombie
                indexZombie++;
            }

            cout << "Seleccione el número del zombie a modificar: ";
            cin >> seleccion;

            // Reiniciar el puntero para volver al inicio
            actualZombie = &actualEquipo->equipo;
            indexZombie = 1;

            while (actualZombie != nullptr) {
                if (indexZombie == seleccion) {
                    // Modificar atributos del zombie
                    cout << "Modificar vida (actual: " << actualZombie->zombie.vida << "): ";
                    int nuevaVida;
                    cin >> nuevaVida;
                    if (nuevaVida > 0) {
                        actualZombie->zombie.vida = nuevaVida;
                    } else {
                        cout << "La vida debe ser mayor a 0." << endl;
                    }

                    cout << "Modificar resistencia (actual: " << actualZombie->zombie.resistencia << "): ";
                    double nuevaResistencia;
                    cin >> nuevaResistencia;
                    if (nuevaResistencia >= 0 && nuevaResistencia <= 1) {
                        actualZombie->zombie.resistencia = nuevaResistencia;
                    } else {
                        cout << "La resistencia debe estar entre 0 y 1." << endl;
                    }

                    cout << "Modificar velocidad (actual: " << actualZombie->zombie.velocidad << "): ";
                    int nuevaVelocidad;
                    cin >> nuevaVelocidad;
                    if (nuevaVelocidad > 0) {
                        actualZombie->zombie.velocidad = nuevaVelocidad;
                    } else {
                        cout << "La velocidad debe ser mayor a 0." << endl;
                    }

                    cout << "Modificar inteligencia (actual: " << actualZombie->zombie.inteligencia << "): ";
                    string nuevaInteligencia;
                    cin >> nuevaInteligencia;
                    actualZombie->zombie.inteligencia = nuevaInteligencia;

                    cout << "Modificar daño (actual: " << actualZombie->zombie.dano << "): ";
                    int nuevoDano;
                    cin >> nuevoDano;
                    if (nuevoDano > 0) {
                        actualZombie->zombie.dano = nuevoDano;
                    } else {
                        cout << "El daño debe ser mayor a 0." << endl;
                    }

                    cout << "Zombie modificado correctamente." << endl;
                    return; // Salir después de modificar
                }
                actualZombie = actualZombie->prox; // Avanzar al siguiente zombie
                indexZombie++;
            }

            cout << "Selección no válida." << endl; // Mensaje si la selección es inválida
            return;
        }
        actualEquipo = actualEquipo->prox; // Avanzar al siguiente equipo
        index++;
    }

    cout << "Selección no válida." << endl; // Mensaje si la selección es inválida
}

//mostrar multiequipos 

void ImprimirEquiposExistentes(EquiposSoldadosExistentes* equipo){
    EquiposSoldadosExistentes* recorrer = equipo;
    if(recorrer == nullptr){
        cout << "No hay equipos de soldados existentes." << endl;
        return;
    }
    else{
        while(recorrer!=nullptr){
            cout << "Nombre del equipo: " << recorrer->nombre << endl;
            cout << "|Integrantes del equipo: " << endl;
            EquipoSoldados* recorrer2 = &recorrer->equipo;
            while(recorrer2!=nullptr){
                cout << "|- " << recorrer2->soldado.nombre << endl;
                recorrer2 = recorrer2->prox;
            }
            recorrer = recorrer->prox;
        }
    }
}

void ImprimirEquiposZombiesExistentes(EquipoZombiesExistentes* equipo) {
    EquipoZombiesExistentes* recorrer = equipo;
    if (recorrer == nullptr) {
        cout << "No hay equipos de zombies existentes." << endl;
        return;
    } else {
        while (recorrer != nullptr) {
            cout << "Nombre del equipo: " << recorrer->nombre << endl;
            cout << "|Integrantes del equipo: " << endl;
            EquipoZombies* recorrer2 = &recorrer->equipo;
            while (recorrer2 != nullptr) {
                cout << "|- " << recorrer2->zombie.nombre << endl;
                recorrer2 = recorrer2->prox;
            }
            recorrer = recorrer->prox;
        }
    }
}



//Funciones para saber si los soldados poseen armas

bool TieneArmaADistancia(Soldado& soldado) {
    return !soldado.mochila.arma_a_distancia.nombre.empty();
}

bool TieneArmaArrojadiza(Soldado& soldado) {
    return !soldado.mochila.arma_arrojadiza.nombre.empty();
}

bool TieneArmaCuerpoACuerpo(Soldado& soldado) {
    return !soldado.mochila.arma_cuerpo_a_cuerpo.nombre.empty();
}

//Funciones de mejora y recarga de arma

void MejorarArma(EquipoSoldados*& equipo, int seleccion, string nombre){
    EquipoSoldados* actual = equipo;
    while(actual!=nullptr){
        if(actual->soldado.nombre==nombre){
            if(seleccion==1){
                if(TieneArmaADistancia(actual->soldado)==true){
                    actual->soldado.mochila.arma_a_distancia.dano += 5;
                    actual->soldado.mochila.arma_a_distancia.distancia += 1;
                    actual->soldado.mochila.arma_a_distancia.municion[0] += 5;
                    cout << "Se mejoro el arma " << actual->soldado.mochila.arma_a_distancia.nombre << "." << endl;
                    break;
                }
                else{
                    cout << "El soldado no tiene un arma a distancia." << endl;
                    break;
                }
            }
            if(seleccion==2){
                if(TieneArmaArrojadiza(actual->soldado)==true){
                    actual->soldado.mochila.arma_arrojadiza.dano += 5;
                    actual->soldado.mochila.arma_arrojadiza.distancia += 1;
                    actual->soldado.mochila.arma_arrojadiza.usos += 1;
                    cout << "Se mejoro el arma " << actual->soldado.mochila.arma_arrojadiza.nombre << "." << endl;
                    break;
                }
                else{
                    cout << "El soldado no tiene un arma arrojadiza." << endl;
                    break;
                }
            }
            if(seleccion==3){
                if(TieneArmaCuerpoACuerpo(actual->soldado)==true){
                    actual->soldado.mochila.arma_cuerpo_a_cuerpo.dano += 10;
                    cout << "Se mejoro el arma " << actual->soldado.mochila.arma_cuerpo_a_cuerpo.nombre << "." << endl;
                    break;
                }
                else{
                    cout << "El soldado no tiene un arma cuerpo a cuerpo." << endl;
                    break;
                }
            }
        }
        else{
            actual = actual->prox;
        }
    }
    return;
}

void RecargarArma(EquipoSoldados*& equipo, int recarga, string nombre){
    int RecargaProvisional;
    EquipoSoldados* actual = equipo;
    while(actual!=nullptr){
        if(actual->soldado.nombre == nombre){
            if(TieneArmaADistancia(actual->soldado)==true){
                RecargaProvisional = actual->soldado.mochila.arma_a_distancia.municion[1]+recarga;
                if(RecargaProvisional>actual->soldado.mochila.arma_a_distancia.municion[0]){
                    actual->soldado.mochila.arma_a_distancia.municion[1] = actual->soldado.mochila.arma_a_distancia.municion[0];
                    RecargaProvisional -= actual->soldado.mochila.arma_a_distancia.municion[0];
                    cout << "Se logro recargar el arma, se perdieron " << RecargaProvisional << " balas (por que sobro)." << endl;
                    break;
                }
                else{
                    actual->soldado.mochila.arma_a_distancia.municion[1] = RecargaProvisional;
                    cout << "Se logro recargar el arma." << endl;
                    break;
                }
            }
            else{
                cout << "El soldado no tiene un arma a distancia." << endl;
                break;
            }
        }
        else{
            actual = actual->prox;
        }
    }
    return;
}

//Funciones para el Gameplay del juego

//Funcion para imprimir lo sucedido en batalla a través de la bitácora
void Bitacora(ArmasPerdidas* armasperdidas, SoldadosCaidos* soldadoscaidos, ZombiesCaidos* zombiescaidos, bool robos) {
    cout << "\n--- Bitácora ---\n";
    cout << "Armas perdidas:\n";
    ArmasPerdidas* actualArmas = armasperdidas;
    while (actualArmas != nullptr) {
        cout << "Nombre: " << actualArmas->nombre << " - Tipo: " << actualArmas->tipo << endl;
        actualArmas = actualArmas->prox;
    }
    if(robos){
        cout << "Se robaron arma/s uno/s zombie/s.\n";
    }

    cout << "\nSoldados caídos:\n";
    SoldadosCaidos* actualSoldados = soldadoscaidos;
    while (actualSoldados != nullptr) {
        cout << "Nombre: " << actualSoldados->nombre << endl;
        actualSoldados = actualSoldados->prox;
    }

    cout << "\nZombies caídos:\n";
    ZombiesCaidos* actualZombies = zombiescaidos;
    while (actualZombies != nullptr) {
        cout << "Nombre: " << actualZombies->nombre << " - Cantidad de zombies: " << endl;
        actualZombies = actualZombies->prox;
    }
    return;
}

//Funcion para las batallas entre 2 equipos

// - Función para saber si hay suficientes balas para atacar
bool BalasSuficientes(int balas, int cadencia, int distancia){
    int balasnecesarias;
    balasnecesarias = cadencia*distancia;
    if(balas>=balasnecesarias){
        return true;
    }
    else{
        return false;
    }
}

// - Función para el ataque de los buenos
void AtacandoBuenos(int dano, EquipoZombies*& enemigos, ZombiesCaidos*& Bitacora, int distancia){
    int contador = 0;
    EquipoZombies* recorrermalos = enemigos;
    EquipoZombies* anterior = recorrermalos;
    while(recorrermalos!=nullptr&&contador<distancia){
        recorrermalos->zombie.vida -= dano*recorrermalos->zombie.resistencia;
        contador++;
        if(recorrermalos->zombie.vida<=0){
            if(anterior==recorrermalos){
                enemigos = recorrermalos->prox;
                if(Bitacora==nullptr){
                    Bitacora = new ZombiesCaidos;
                    Bitacora->nombre = recorrermalos->zombie.nombre;
                    Bitacora->prox = nullptr;
                }
                else{
                    ZombiesCaidos* recorrezombies = Bitacora;
                    while(recorrezombies->prox!=nullptr){
                        recorrezombies = recorrezombies->prox;
                    }
                    recorrezombies->prox = new ZombiesCaidos;
                    recorrezombies->prox->nombre = recorrermalos->zombie.nombre;
                    recorrezombies->prox->prox = nullptr;
                }
                delete recorrermalos;
                recorrermalos = enemigos;
            }
            else{
                anterior->prox = recorrermalos->prox;
                if(Bitacora==nullptr){
                    Bitacora = new ZombiesCaidos;
                    Bitacora->nombre = recorrermalos->zombie.nombre;
                    Bitacora->prox = nullptr;
                }
                else{
                    ZombiesCaidos* recorrezombies = Bitacora;
                    while(recorrezombies->prox!=nullptr){
                        recorrezombies = recorrezombies->prox;
                    }
                    recorrezombies->prox = new ZombiesCaidos;
                    recorrezombies->prox->nombre = recorrermalos->zombie.nombre;
                    recorrezombies->prox->prox = nullptr;
                }
                delete recorrermalos;
                recorrermalos = enemigos;
                delete recorrermalos;
                recorrermalos = anterior->prox;
            }
        }
        else{
            anterior = recorrermalos;
            recorrermalos = recorrermalos->prox;
        }
    }
}

// - Función para el primer ataque de los zombies
void DefendiendoAtaquesLocos(int dano, int velocidad, EquipoSoldados*& equipo, SoldadosCaidos*& soldadoscaidos){
    int contador = 0;
    EquipoSoldados* recorrerbuenos = equipo;
    EquipoSoldados* anterior = recorrerbuenos;
    while(recorrerbuenos!=nullptr&&contador<velocidad){
        recorrerbuenos->soldado.vida -= dano*recorrerbuenos->soldado.resistencia;
        if(recorrerbuenos->soldado.vida<=0){
            if(anterior==recorrerbuenos){
                equipo = recorrerbuenos->prox;
                if(soldadoscaidos==nullptr){
                    soldadoscaidos = new SoldadosCaidos;
                    soldadoscaidos->nombre = recorrerbuenos->soldado.nombre;
                    soldadoscaidos->prox = nullptr;
                }
                else{
                    SoldadosCaidos* recorresoldados = soldadoscaidos;
                    while(recorresoldados->prox!=nullptr){
                        recorresoldados = recorresoldados->prox;
                    }
                    recorresoldados->prox = new SoldadosCaidos;
                    recorresoldados->prox->nombre = recorrerbuenos->soldado.nombre;
                    recorresoldados->prox->prox = nullptr;
                }
                delete recorrerbuenos;
                recorrerbuenos = equipo;
            }
            else{
                anterior->prox = recorrerbuenos->prox;
                if(soldadoscaidos==nullptr){
                    soldadoscaidos = new SoldadosCaidos;
                    soldadoscaidos->nombre = recorrerbuenos->soldado.nombre;
                    soldadoscaidos->prox = nullptr;
                }
                else{
                    SoldadosCaidos* recorresoldados = soldadoscaidos;
                    while(recorresoldados->prox!=nullptr){
                        recorresoldados = recorresoldados->prox;
                    }
                    recorresoldados->prox = new SoldadosCaidos;
                    recorresoldados->prox->nombre = recorrerbuenos->soldado.nombre;
                    recorresoldados->prox->prox = nullptr;
                }
                delete recorrerbuenos;
                recorrerbuenos = equipo;
                delete recorrerbuenos;
                recorrerbuenos = anterior->prox;
            }
        }
        else{
            anterior = recorrerbuenos;
            if(recorrerbuenos->prox==nullptr){
                recorrerbuenos = equipo;
                anterior = recorrerbuenos;
                contador++;
            }
            else{
                recorrerbuenos = recorrerbuenos->prox;
            }
        }
    }
}

// - Función para eliminar a los soldados caídos
void SoldadoMuerto(EquipoSoldados*& equipo, string nombre, SoldadosCaidos*& bitacora){
    EquipoSoldados* recorrer = equipo;
    EquipoSoldados* anterior = recorrer;
    while(recorrer!=nullptr){
        if(recorrer->soldado.nombre==nombre){
            break;
        }
        else{
            anterior = recorrer;
            recorrer = recorrer->prox;
        }
    }
    anterior->prox = recorrer->prox;
    if(bitacora==nullptr){
        bitacora = new SoldadosCaidos;
        bitacora->nombre = recorrer->soldado.nombre;
        bitacora->prox = nullptr;
    }
    else{
        SoldadosCaidos* recorresoldados = bitacora;
        while(recorresoldados->prox!=nullptr){
            recorresoldados = recorresoldados->prox;
        }
        recorresoldados->prox = new SoldadosCaidos;
        recorresoldados->prox->nombre = recorrer->soldado.nombre;
        recorresoldados->prox->prox = nullptr;
    }
    delete recorrer;
}

// - Funciones del segundo ataque de zombies
void DefendiendoAtaquesInteligentes(int dano, int velocidad, string inteligencia, EquipoSoldados*& equipo, SoldadosCaidos*& SoldadosCaidos, ArmasPerdidas*& armascaidas){
    EquipoSoldados* recorrerbuenos = equipo;
    int contador = 0;
    int datomayor = 0;
    EquipoSoldados* anterior = recorrerbuenos;
    if(inteligencia=="resistencia"){
        while(recorrerbuenos!=nullptr&&contador<velocidad){
            if(recorrerbuenos->soldado.resistencia<datomayor){
                datomayor = recorrerbuenos->soldado.resistencia;
            }
            else{
                recorrerbuenos = recorrerbuenos->prox;
                contador++;
            }
        }
        recorrerbuenos = equipo;
        while(recorrerbuenos!=nullptr){
            if(recorrerbuenos->soldado.resistencia==datomayor){
                recorrerbuenos->soldado.vida -= dano;
                if(recorrerbuenos->soldado.vida<=0){
                    SoldadoMuerto(equipo, recorrerbuenos->soldado.nombre, SoldadosCaidos);
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    if(inteligencia=="suerte"){
        while(recorrerbuenos!=nullptr&&contador<velocidad){
            if(recorrerbuenos->soldado.suerte>datomayor){
                datomayor = recorrerbuenos->soldado.suerte;
            }
            else{
                recorrerbuenos = recorrerbuenos->prox;
                contador++;
            }
        }
        recorrerbuenos = equipo;
        while(recorrerbuenos!=nullptr){
            if(recorrerbuenos->soldado.suerte==datomayor){
                recorrerbuenos->soldado.vida -= dano;
                if(recorrerbuenos->soldado.vida<=0){
                    SoldadoMuerto(equipo, recorrerbuenos->soldado.nombre, SoldadosCaidos);
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    if(inteligencia=="vida.mas"){
        while(recorrerbuenos!=nullptr&&contador<velocidad){
            if(recorrerbuenos->soldado.vida>datomayor){
                datomayor = recorrerbuenos->soldado.vida;
            }
            else{
                recorrerbuenos = recorrerbuenos->prox;
                contador++;
            }
        }
        recorrerbuenos = equipo;
        while(recorrerbuenos!=nullptr){
            if(recorrerbuenos->soldado.vida==datomayor){
                recorrerbuenos->soldado.vida -= dano;
                if(recorrerbuenos->soldado.vida<=0){
                    SoldadoMuerto(equipo, recorrerbuenos->soldado.nombre, SoldadosCaidos);
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    if(inteligencia=="vida.menos"){
        while(recorrerbuenos!=nullptr&&contador<velocidad){
            if(recorrerbuenos->soldado.vida<datomayor){
                datomayor = recorrerbuenos->soldado.vida;
            }
            else{
                recorrerbuenos = recorrerbuenos->prox;
                contador++;
            }
        }
        recorrerbuenos = equipo;
        while(recorrerbuenos!=nullptr){
            if(recorrerbuenos->soldado.vida==datomayor){
                recorrerbuenos->soldado.vida -= dano;
                if(recorrerbuenos->soldado.vida<=0){
                    SoldadoMuerto(equipo, recorrerbuenos->soldado.nombre, SoldadosCaidos);
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    if(inteligencia=="arma"){
        recorrerbuenos->soldado.mochila.arma_a_distancia = ArmaADistancia();
        recorrerbuenos->soldado.mochila.arma_arrojadiza = ArmaArrojadiza();
        recorrerbuenos->soldado.mochila.arma_cuerpo_a_cuerpo = ArmaCuerpoACuerpo();
        cout << "walkie-talkie: Comandante! Un soldado le han robado todas sus armas." << endl;
    }
}

// - Funcion que inicia el ataque de los zombies
void IniciandoUnaDefensa(EquipoSoldados*& equipo, EquipoZombies*& enemigos, SoldadosCaidos*& soldadoscaidos, ArmasPerdidas*& armasperdidas){
    EquipoSoldados* recorrerbuenos = equipo;
    int contador = 0;
    int oleadas = rand()%5+1;
    EquipoZombies* recorrermalos = enemigos;
    while(recorrermalos!=nullptr&&contador<oleadas){
        DefendiendoAtaquesLocos(recorrermalos->zombie.dano, recorrermalos->zombie.velocidad, equipo, soldadoscaidos);
        if(recorrermalos->prox==nullptr){
            recorrermalos = enemigos;
            contador++;
        }
        else{
            recorrermalos = recorrermalos->prox;
        }
    }
    ArmasPerdidas* recorrer = armasperdidas;
    recorrermalos = enemigos;
    while(recorrermalos!=nullptr){
        DefendiendoAtaquesInteligentes(recorrermalos->zombie.dano, recorrermalos->zombie.velocidad, recorrermalos->zombie.inteligencia, equipo, soldadoscaidos, armasperdidas);
        recorrermalos = recorrermalos->prox;
    }
}

// - Función que inicia el ataque de los soldados
void IniciandoUnAtaque(EquipoSoldados*& equipo, EquipoZombies*& enemigos, int armasposeidas[3], ZombiesCaidos*& zombiescaidos, ArmasPerdidas*& armasperdidas){
    ArmasPerdidas* recorrer = armasperdidas;
    int contador = 0;
    EquipoSoldados* recorrerbuenos = equipo;
    EquipoZombies* recorrermalos = enemigos;
    while(recorrerbuenos!=nullptr&&contador<2){
        if(TieneArmaADistancia(recorrerbuenos->soldado)==true){
            armasposeidas[0] = 1;
        }
        else{
            armasposeidas[0] = 0;
        }
        if(TieneArmaArrojadiza(recorrerbuenos->soldado)==true){
            armasposeidas[1] = 1;
        }
        else{
            armasposeidas[1] = 0;
        }
        if(TieneArmaCuerpoACuerpo(recorrerbuenos->soldado)==true){
            armasposeidas[2] = 1;
        }
        else{
            armasposeidas[2] = 0;
        }
        if(armasposeidas[0]==1){
            if(BalasSuficientes(recorrerbuenos->soldado.mochila.arma_a_distancia.municion[1], recorrerbuenos->soldado.mochila.arma_a_distancia.cadenciadetiro, recorrerbuenos->soldado.mochila.arma_a_distancia.distancia)==true){
                AtacandoBuenos(recorrerbuenos->soldado.mochila.arma_a_distancia.dano, recorrermalos, zombiescaidos, recorrerbuenos->soldado.mochila.arma_a_distancia.distancia);
                recorrerbuenos->soldado.mochila.arma_a_distancia.municion[1] -= recorrerbuenos->soldado.mochila.arma_a_distancia.cadenciadetiro*recorrerbuenos->soldado.mochila.arma_a_distancia.distancia;
            }
            else{
                cout << "walkie-talkie: Comandante! Un soldado no pudo usar un arma por faltarle balas." << endl;
            }
        }
        if(armasposeidas[1]==1){
            if(recorrerbuenos->soldado.mochila.arma_arrojadiza.usos>0){
                AtacandoBuenos(recorrerbuenos->soldado.mochila.arma_arrojadiza.dano, recorrermalos, zombiescaidos, recorrerbuenos->soldado.mochila.arma_arrojadiza.distancia);
                recorrerbuenos->soldado.mochila.arma_arrojadiza.usos--;
            }
            else{
                armasperdidas = new ArmasPerdidas;
                armasperdidas->nombre = recorrerbuenos->soldado.mochila.arma_arrojadiza.nombre;
                armasperdidas->tipo = "Arma Arrojadiza";
                if(recorrer!=nullptr){
                    ArmasPerdidas* recorrer2 = recorrer;
                    while(recorrer2->prox!=nullptr){
                        recorrer2 = recorrer2->prox;
                    }
                    recorrer2->prox = armasperdidas;
                    armasperdidas->prox = nullptr;
                }
                else{
                    armasperdidas->prox = nullptr;
                    recorrer = armasperdidas;
                }
                recorrerbuenos->soldado.mochila.arma_arrojadiza = ArmaArrojadiza();
                cout << "walkie-talkie: Comandante! Un soldado no pudo usar un arma por faltarle usos, se ha perdido!." << endl;
            }
        }
        if(armasposeidas[2]==1){
            AtacandoBuenos(recorrerbuenos->soldado.mochila.arma_cuerpo_a_cuerpo.dano, recorrermalos, zombiescaidos, recorrerbuenos->soldado.mochila.arma_cuerpo_a_cuerpo.distancia);
        }
        if(recorrerbuenos->prox==nullptr){
            recorrerbuenos = equipo;
            contador++;
        }
        else{
            recorrerbuenos = recorrerbuenos->prox;
        }
        
    }
}

// - Función para iniciar y controlar la batalla
void InicioYControlDeLaBatalla(EquipoSoldados*& equipo, EquipoZombies*& enemigos){
    ZombiesCaidos* zombiesasesinados = nullptr;
    SoldadosCaidos* soldadosasesinados = nullptr;
    ArmasPerdidas* armascaidas = nullptr;
    int contador = 0;
    int poseedordearmas[3] = {0,0,0};
    EquipoSoldados* recorrerbuenos = equipo;
    EquipoZombies* recorrermalos = enemigos;
    if(recorrerbuenos == nullptr||recorrermalos == nullptr){
        cout << "No hay soldados o zombies para la batalla." << endl;
    }
    else{
        if(TieneArmaADistancia(recorrerbuenos->soldado)==true||TieneArmaArrojadiza(recorrerbuenos->soldado)==true||TieneArmaCuerpoACuerpo(recorrerbuenos->soldado)==true){
            IniciandoUnAtaque(equipo, enemigos, poseedordearmas, zombiesasesinados, armascaidas);
            IniciandoUnaDefensa(equipo, enemigos, soldadosasesinados, armascaidas);
            if(equipo==nullptr||enemigos==nullptr){
                cout << "La batalla ha terminado." << endl;
                Bitacora(armascaidas, soldadosasesinados, zombiesasesinados, false);
            }
            else{
                InicioYControlDeLaBatalla(equipo, enemigos);
            }
        }
    }
}


//Funcion para subir de nivel a los soldados

void SubirNivel(EquipoSoldados* equipo, string nombre){
    EquipoSoldados* recorrer = equipo;
    if(equipo == nullptr){
        cout << "No hay soldados en el equipo." << endl;
        return;
    }
    else{
        while(recorrer!=nullptr){
            if(recorrer->soldado.nombre == nombre){
                if(recorrer->soldado.nivel == 10){
                    cout << "El soldado ya está en nivel máximo." << endl;
                }
                else{
                    if(recorrer->soldado.nivel==4||recorrer->soldado.nivel==9){
                        recorrer->soldado.vida += 25;
                        recorrer->soldado.suerte += 5;
                        if(recorrer->soldado.resistencia > 0.05){
                            recorrer->soldado.resistencia -= 0.05;
                        }
                        recorrer->soldado.nivel++;
                        recorrer->soldado.velocidad += 5;
                        break;
                    }
                    else{
                        recorrer->soldado.vida += 10;
                        recorrer->soldado.suerte += 2;
                        if(recorrer->soldado.resistencia > 0.02){
                            recorrer->soldado.resistencia -= 0.02;
                        }
                        recorrer->soldado.nivel++;
                        recorrer->soldado.velocidad += 2;
                        break;
                    }
                }
            }
            else{
                recorrer = recorrer->prox;
            }
        }
    }
    return;
}

// Lista de nombres para cada tipo de arma
vector<string> nombresArmaADistancia = {
    "Rifle de Asalto", "Francotirador", "Ballesta", "Pistola", "Escopeta"
};

vector<string> nombresArmaArrojadiza = {
    "Granada", "Bomba de Humo", "Daga", "Javelina", "Bomba de Gas"
};

vector<string> nombresArmaCuerpoACuerpo = {
    "Espada", "Hacha", "Bate de Béisbol", "Maza", "Cuchillo"
};
// Lista de nombres para los zombies
vector<string> nombresZombies = {
    "Zombie Veloz", "Zombie Gigante", "Zombie Táctico", "Zombie Infectado", "Zombie Mutante"
};
// Lista de nombres para los objetos (sin armas)
vector<string> nombresObjetos = {
    "Poción de Curación", "Capa de Invisibilidad", "Antorcha Mágica", "Cristal de Teleportación", "Elixir de Fuerza"
};
// Lista de nombres para los soldados
vector<string> nombresSoldados = {
    "Arturo", "Beatriz", "Carlos", "Diana", "Eduardo", "Fernanda", "Gustavo", "Helena"
};
// Lista de materiales para crafteo
vector<string> materiales = {
    "Madera", "Hierro", "Piedra", "Cuerda", "Tela", "Polvo", "Aceite", "Flechas", "Cristales"
};

// Función para crear crafteos aleatorios
void CrearCrafteoAleatorio(string crafteo[3]) {
    // Selecciona 3 materiales aleatorios
    for (int i = 0; i < 3; ++i) {
        crafteo[i] = materiales[rand() % materiales.size()];
    }
}

// Función para crear armas aleatorias
void CrearArmaAleatoria(EquipoSoldados*& equipo) {
    if (equipo == nullptr) {
        cout << "ERROR: No puedes anadir armas si no tienes equipo." << endl;
        return;
    }

    // Inicializar el generador de números aleatorios
    srand(static_cast<unsigned int>(time(0))); 

    // Crear un arma aleatoria
    int tipoArma = rand() % 3; // 0: a distancia, 1: arrojadiza, 2: cuerpo a cuerpo
    string nombreArma;
    int dano = rand() % 101; // Daño entre 0 y 100
    bool aturdimiento = rand() % 2; // 0 o 1
    int distancia = rand() % 10 + 1; // Distancia entre 1 y 10
    int ruido = rand() % 11; // Ruido entre 0 y 10

    // Seleccionar un nombre aleatorio según el tipo de arma
    if (tipoArma == 0) { // Arma a distancia
        nombreArma = nombresArmaADistancia[rand() % nombresArmaADistancia.size()];
        ArmaADistancia nuevaArma;
        nuevaArma.nombre = nombreArma;
        nuevaArma.dano = dano;
        nuevaArma.aturdimiento = aturdimiento;
        nuevaArma.distancia = distancia;
        nuevaArma.cadenciadetiro = rand() % 5 + 1; // Cadencia entre 1 y 5
        nuevaArma.municion[0] = rand() % 30 + 1; // Capacidad entre 1 y 30
        nuevaArma.municion[1] = rand() % nuevaArma.municion[0]; // Munición actual
        nuevaArma.ruido = ruido;

        // Asignar el arma al primer soldado del equipo
        equipo->soldado.mochila.arma_a_distancia = nuevaArma;
        cout << "Arma a distancia creada: " << nuevaArma.nombre << endl;
    }
    else if (tipoArma == 1) { // Arma arrojadiza
        nombreArma = nombresArmaArrojadiza[rand() % nombresArmaArrojadiza.size()];
        ArmaArrojadiza nuevaArma;
        nuevaArma.nombre = nombreArma;
        nuevaArma.dano = dano;
        nuevaArma.aturdimiento = aturdimiento;
        nuevaArma.distancia = distancia;
        nuevaArma.ruido = ruido;
        nuevaArma.usos = rand() % 10 + 1; // Usos entre 1 y 10

        // Asignar el arma al primer soldado del equipo
        equipo->soldado.mochila.arma_arrojadiza = nuevaArma;
        cout << "Arma arrojadiza creada: " << nuevaArma.nombre << endl;
    }
    else if (tipoArma == 2) { // Arma cuerpo a cuerpo
        nombreArma = nombresArmaCuerpoACuerpo[rand() % nombresArmaCuerpoACuerpo.size()];
        ArmaCuerpoACuerpo nuevaArma;
        nuevaArma.nombre = nombreArma;
        nuevaArma.dano = dano;
        nuevaArma.aturdimiento = aturdimiento;
        nuevaArma.distancia = distancia;
        nuevaArma.ruido = ruido;

        // Asignar el arma al primer soldado del equipo
        equipo->soldado.mochila.arma_cuerpo_a_cuerpo = nuevaArma;
        cout << "Arma cuerpo a cuerpo creada: " << nuevaArma.nombre << endl;
    }
}

// Función para crear zombies aleatorios
void CrearZombieAleatorio(EquipoZombies*& equipo) {
    if (equipo == nullptr) {
        cout << "ERROR: No puedes anadir zombies si no tienes equipo." << endl;
        return;
    }

    // Inicializar el generador de números aleatorios
    srand(static_cast<unsigned int>(time(0))); 

    // Crear un zombie aleatorio
    string nombreZombie = nombresZombies[rand() % nombresZombies.size()];
    int vida = rand() % 101 + 50; // Vida entre 50 y 150
    int resistencia = rand() % 51; // Resistencia entre 0 y 50
    int velocidad = rand() % 21 + 5; // Velocidad entre 5 y 25
    int inteligencia = rand() % 6; // Inteligencia entre 0 y 5
    int dano = rand() % 21 + 5; // Daño entre 5 y 25

    // Crear el nuevo zombie
    Zombie nuevoZombie;
    nuevoZombie.nombre = nombreZombie;
    nuevoZombie.vida = vida;
    nuevoZombie.resistencia = resistencia;
    nuevoZombie.velocidad = velocidad;
    nuevoZombie.inteligencia = inteligencia;
    nuevoZombie.dano = dano;

    // Asignar el zombie al primer equipo de zombies
    equipo->zombie = nuevoZombie; // Asegúrate de que esto se ajuste a tu estructura
    cout << "Zombie creado: " << nuevoZombie.nombre << endl;
}

// Funciones para generar objetos, armas y consumibles aleatorios
Objeto CrearObjetoAleatorio() {
    vector<string> nombresObjetos = {"Antorcha", "Mapa", "Llave", "Cuerda", "Botiquín"};
    Objeto objeto;
    objeto.nombre = nombresObjetos[rand() % nombresObjetos.size()];
    objeto.dano = rand() % 6 + 1; // Daño entre 1 y 6
    return objeto; // Retorna el objeto con su daño
}
// Funciones para generar armas y consumibles aleatorios
Consumible CrearConsumibleAleatorio() {
    vector<string> nombresConsumibles = {"Poción de Curación", "Elixir de Fuerza", "Banda de Velocidad"};
    Consumible consumible;
    consumible.nombre = nombresConsumibles[rand() % nombresConsumibles.size()];
    consumible.vida = rand() % 21 + 10; // Vida entre 10 y 30
    consumible.resistencia = rand() % 11; // Resistencia entre 0 y 10
    consumible.velocidad = rand() % 6; // Velocidad entre 0 y 5
    consumible.suerte = rand() % 101; // Suerte entre 0 y 100
    return consumible;
}

ArmaCuerpoACuerpo CrearArmaCuerpoACuerpoAleatoria() {
    ArmaCuerpoACuerpo arma;
    arma.nombre = "Espada de Hierro";
    arma.dano = rand() % 11 + 5; // Daño entre 5 y 15
    arma.aturdimiento = rand() % 2; // True o False
    arma.distancia = 1; // Cuerpo a cuerpo
    arma.ruido = rand() % 6; // Ruido entre 0 y 5
    arma.crafteo[0] = "Hierro";
    arma.crafteo[1] = "Madera";
    arma.crafteo[2] = "Piedra";
    return arma;
}

ArmaArrojadiza CrearArmaArrojadizaAleatoria() {
    ArmaArrojadiza arma;
    arma.nombre = "Bomba de Humo";
    arma.dano = rand() % 6 + 1; // Daño entre 1 y 6
    arma.aturdimiento = rand() % 2; // True o False
    arma.distancia = rand() % 11 + 5; // Distancia entre 5 y 15
    arma.ruido = rand() % 4; // Ruido entre 0 y 3
    arma.usos = rand() % 5 + 1; // Usos entre 1 y 5
    arma.crafteo[0] = "Polvo";
    arma.crafteo[1] = "Aceite";
    arma.crafteo[2] = "Mecha";
    return arma;
}

ArmaADistancia CrearArmaADistanciaAleatoria() {
    ArmaADistancia arma;
    arma.nombre = "Arco de Cazador";
    arma.dano = rand() % 11 + 5; // Daño entre 5 y 15
    arma.aturdimiento = rand() % 2; // True o False
    arma.distancia = rand() % 21 + 10; // Distancia entre 10 y 30
    arma.cadenciadetiro = rand() % 3 + 1; // Cadencia entre 1 y 3
    arma.municion[0] = rand() % 20 + 5; // Capacidad entre 5 y 25
    arma.municion[1] = arma.municion[0]; // Munición actual igual a la capacidad
    arma.ruido = rand() % 4; // Ruido entre 0 y 3
    arma.crafteo[0] = "Madera";
    arma.crafteo[1] = "Cuerda";
    arma.crafteo[2] = "Flechas";
    return arma;
}

// Función para crear soldados aleatorios
Soldado CrearSoldadoAleatorio() {
    srand(static_cast<unsigned int>(time(0))); 

    // Crear un soldado aleatorio
    string nombreSoldado = nombresSoldados[rand() % nombresSoldados.size()];
    int vida = rand() % 101 + 50; // Vida entre 50 y 150
    double resistencia = (rand() % 100) / 100.0; // Resistencia entre 0.0 y 1.0
    int velocidad = rand() % 21 + 5; // Velocidad entre 5 y 25
    int suerte = rand() % 101; // Suerte entre 0 y 100
    int nivel = rand() % 11 + 1; // Nivel entre 1 y 11

    // Llenar la mochila con objetos y armas aleatorias
    Mochila mochila;
    mochila.arma_a_distancia = CrearArmaADistanciaAleatoria();
    mochila.arma_arrojadiza = CrearArmaArrojadizaAleatoria();
    mochila.arma_cuerpo_a_cuerpo = CrearArmaCuerpoACuerpoAleatoria();
    mochila.objeto = CrearObjetoAleatorio(); // Asegúrate de tener esta función definida
    mochila.consumible = CrearConsumibleAleatorio();

    // Crear el nuevo soldado
    Soldado nuevoSoldado = { nombreSoldado, vida, resistencia, velocidad, suerte, nivel, mochila };

    cout << "Soldado creado: " << nuevoSoldado.nombre 
         << " - Vida: " << nuevoSoldado.vida 
         << ", Resistencia: " << nuevoSoldado.resistencia 
         << ", Velocidad: " << nuevoSoldado.velocidad 
         << ", Suerte: " << nuevoSoldado.suerte 
         << ", Nivel: " << nuevoSoldado.nivel << endl;

    return nuevoSoldado;
}

// Función para generar un mapa aleatorio
Grafo GenerarMapaAleatorio(int numEstaciones, int numConexiones) {
    Grafo mapa;
    vector<string> nombresEstaciones;

    // Crear estaciones
    for (int i = 0; i < numEstaciones; ++i) {
        string nombre = "Estacion_" + to_string(i + 1);
        mapa.agregarNodo(nombre);
        nombresEstaciones.push_back(nombre);
    }

    // Conectar estaciones aleatoriamente
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < numConexiones; ++i) {
        int index1 = rand() % numEstaciones;
        int index2 = rand() % numEstaciones;

        // Asegurarse de que no se conecte una estación consigo misma
        while (index1 == index2) {
            index2 = rand() % numEstaciones;
        }

        mapa.conectarNodos(nombresEstaciones[index1], nombresEstaciones[index2]);
    }

    // Asignar cantidad aleatoria de zombies a cada estación
    for (const string& nombre : nombresEstaciones) {
        int cantidadZombies = rand() % 21; // Cantidad de zombies entre 0 y 20
        mapa.agregarZombie(nombre, cantidadZombies);
    }

    return mapa;
}

// Función para mostrar el mapa
void MostrarMapaAleatorio(const Grafo& mapa) {
    for (const auto& par : mapa.nodos) {
        cout << "Estación: " << par.first 
             << ", Zombies: " << par.second->cantidadZombies 
             << ", Conexiones: ";
        for (const auto& vecino : par.second->adyacentes) {
            cout << vecino->nombreEstacion << " ";
        }
        cout << endl;
    }
}

void ajustarValorPorSuerte(int& valor, int suerte) {
    if (suerte <= 50) {
        valor -= 5;
    } else {
        valor += 5;
    }
}
void saquear(EquipoSoldados* equipo, const string& nombreSoldado) {
    EquipoSoldados* actual = equipo;
    while (actual != nullptr) {
        if (actual->soldado.nombre == nombreSoldado) {
            srand(time(0));
            int tipoObjeto = rand() % 5 + 1; // Número aleatorio entre 1 y 5

            switch (tipoObjeto) {
                case 1: // Arma a Distancia
                    if (actual->soldado.mochila.arma_a_distancia.nombre.empty()) {
                        ArmaADistancia nuevaArma = CrearArmaADistanciaAleatoria();
                        ajustarValorPorSuerte(nuevaArma.dano, actual->soldado.suerte);
                        actual->soldado.mochila.arma_a_distancia = nuevaArma;
                        cout << "Arma a Distancia saqueada y añadida al inventario del soldado." << endl;
                    } else {
                        cout << "El soldado no tiene espacio para otra Arma a Distancia." << endl;
                    }
                    break;
                case 2: // Arma Arrojadiza
                    if (actual->soldado.mochila.arma_arrojadiza.nombre.empty()) {
                        ArmaArrojadiza nuevaArma = CrearArmaArrojadizaAleatoria();
                        ajustarValorPorSuerte(nuevaArma.dano, actual->soldado.suerte);
                        actual->soldado.mochila.arma_arrojadiza = nuevaArma;
                        cout << "Arma Arrojadiza saqueada y añadida al inventario del soldado." << endl;
                    } else {
                        cout << "El soldado no tiene espacio para otra Arma Arrojadiza." << endl;
                    }
                    break;
                case 3: // Arma Cuerpo a Cuerpo
                    if (actual->soldado.mochila.arma_cuerpo_a_cuerpo.nombre.empty()) {
                        ArmaCuerpoACuerpo nuevaArma = CrearArmaCuerpoACuerpoAleatoria();
                        ajustarValorPorSuerte(nuevaArma.dano, actual->soldado.suerte);
                        actual->soldado.mochila.arma_cuerpo_a_cuerpo = nuevaArma;
                        cout << "Arma Cuerpo a Cuerpo saqueada y añadida al inventario del soldado." << endl;
                    } else {
                        cout << "El soldado no tiene espacio para otra Arma Cuerpo a Cuerpo." << endl;
                    }
                    break;
                    case 4: // Objeto
                    if (actual->soldado.mochila.objeto.nombre.empty()) {
                        Objeto nuevoObjeto = CrearObjetoAleatorio();
                        ajustarValorPorSuerte(nuevoObjeto.dano, actual->soldado.suerte);
                        actual->soldado.mochila.objeto = nuevoObjeto;
                        cout << "Objeto saqueado y añadido al inventario del soldado." << endl;
                    } else {
                        cout << "El soldado no tiene espacio para otro Objeto." << endl;
                    }
                    break;
                case 5: // Consumible
                    if (actual->soldado.mochila.consumible.nombre.empty()) {
                        Consumible nuevoConsumible = CrearConsumibleAleatorio();
                        ajustarValorPorSuerte(nuevoConsumible.vida, actual->soldado.suerte);
                        actual->soldado.mochila.consumible = nuevoConsumible;
                        cout << "Consumible saqueado y añadido al inventario del soldado." << endl;
                    } else {
                        cout << "El soldado no tiene espacio para otro Consumible." << endl;
                    }
                    break;
            }
            return;
        }
        actual = actual->prox;
    }
    cout << "Soldado no encontrado." << endl;
}


// Función principal

int main() {
    MostrarMensajeInicio();
    const int num_Nodo = 10; // Se ajusta según sea necesario
    Nodo* grafo[num_Nodo];
    Grafo mapa; // Inicializar el grafo del mapa
    
    mapa.leergrafoArchivo("Accesorio.zmb", "Mapa.zmb", "Soldado.zmb", "Zombie.zmb");
    
    EquiposSoldadosExistentes* listaEquiposSoldados = nullptr; // Inicializar la lista de equipos de soldados
    EquipoZombiesExistentes* listaEquiposZombies = nullptr; // Inicializar la lista de equipos de zombies
    ObjetosExistentes* listaObjetos = nullptr; // Inicializar la lista de objetos

    int opcion;
    do {
        BorrarPantalla();
        cout << "\n--- Menú Principal ---\n";
        cout << "1. Crear\n";
        cout << "2. Modificar\n";
        cout << "3. Eliminar\n";
        cout << "4. Ver Progreso\n";
        cout << "5. Iniciar Gameplay\n";
        cout << "0. Salir\n";
        cout << "Elija una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: { // Crear
                int crearOpcion;
                do {
                    BorrarPantalla();
                    cout << "\n--- Menú Crear ---\n";
                    cout << "1. Crear equipo de soldados\n";
                    cout << "2. Crear equipo de zombies\n";
                    cout << "3. Crear objeto\n";
                    cout << "4. Crear consumible\n";
                    cout << "5. Crear estación\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> crearOpcion;

                    switch (crearOpcion) {
                        case 1: {
                            BorrarPantalla();
                            EquipoSoldados* nuevoEquipo = new EquipoSoldados;
                            AnadirAlEquipoSoldadoPorDelante(nuevoEquipo);
                            EquiposSoldadosExistentes* nuevoEquipoExistente = CrearEquipoSoldadosExistentes(nuevoEquipo);
                            nuevoEquipoExistente->prox = listaEquiposSoldados;
                            listaEquiposSoldados = nuevoEquipoExistente;
                            break;
                        }
                        case 2: {
                            BorrarPantalla();
                            EquipoZombies* nuevoEquipo = new EquipoZombies;
                            AgregarZombie(nuevoEquipo);
                            EquipoZombiesExistentes* nuevoEquipoExistente = CrearEquipoZombiesExistentes(nuevoEquipo);
                            nuevoEquipoExistente->prox = listaEquiposZombies;
                            listaEquiposZombies = nuevoEquipoExistente;
                            break;
                        }
                        case 3:
                            BorrarPantalla();
                            CrearObjeto(listaObjetos);
                            break;
                        case 4:
                            BorrarPantalla();
                            CrearConsumible(listaEquiposSoldados->equipo);
                            break;
                        case 5: {
                            BorrarPantalla();
                            string nombreEstacion;
                            int cantidadZombies;
                            cout << "Ingrese el nombre de la estación: ";
                            cin >> nombreEstacion;
                            cout << "Ingrese la cantidad de zombies en la estación: ";
                            cin >> cantidadZombies;
                            mapa.agregarNodo(nombreEstacion);
                            mapa.agregarZombie(nombreEstacion, cantidadZombies); // Agregar zombies a la estación
                            break;
                        }
                        case 0: break; 
                        default:
                            cout << "Opción no válida." << endl;
                    }
                } while (crearOpcion != 0);
                break;
            }
            case 2: { // Modificar
                int modificarOpcion;
                do {
                    BorrarPantalla();
                    cout << "\n--- Menú Modificar ---\n";
                    cout << "1. Modificar equipo de soldados\n";
                    cout << "2. Modificar equipo de zombies\n";
                    cout << "3. Modificar objeto\n";
                    cout << "4. Modificar consumible\n";
                    cout << "5. Modificar estación\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> modificarOpcion;

                    switch (modificarOpcion) {
                        case 1:
                            BorrarPantalla();
                            ModificarEquipoSoldados(*listaEquiposSoldados);
                            break;
                        case 2: 
                            BorrarPantalla();
                            ModificarEquipoZombies(*listaEquiposZombies);
                            break;
                        case 3:
                            BorrarPantalla();
                            ModificarObjeto(listaObjetos);
                            break;
                        case 4:
                            BorrarPantalla();
                            ModificarConsumible(listaEquiposSoldados->equipo);
                            break;
                        case 5:
                            BorrarPantalla();
                            ModificarMapa(mapa);
                            break;
                        case 0: break; // Regresar
                        default:
                            cout << "Opción no válida." << endl;
                    }
                } while (modificarOpcion != 0);
                break;
            }
            case 3: { // Eliminar
                int eliminarOpcion;
                do {
                    BorrarPantalla();
                    cout << "\n--- Menú Eliminar ---\n";
                    cout << "1. Eliminar equipo de soldados\n";
                    cout << "2. Eliminar equipo de zombies\n"; 
                    cout << "3. Eliminar objeto\n";
                    cout << "4. Eliminar consumible\n";
                    cout << "5. Eliminar estación\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> eliminarOpcion;

                    switch (eliminarOpcion) {
                        case 1:
                            BorrarPantalla();
                            EliminarSoldado(listaEquiposSoldados->equipo);
                            break;
                        case 2: 
                            BorrarPantalla();
                            EliminarZombie(listaEquiposZombies->equipo);
                            break;
                        case 3:
                            BorrarPantalla();
                            EliminarObjeto(listaObjetos);
                            break;
                        case 4:
                            BorrarPantalla();
                            EliminarConsumible(listaEquiposSoldados->equipo);
                            break;
                        case 5:
                            BorrarPantalla();
                            EliminarMapa(mapa);
                            break;
                        case 0: break; // Regresar
                        default:
                            cout << "Opción no válida." << endl;
                    }
                } while (eliminarOpcion != 0);
                break;
            }
            case 4: { // Ver Progreso
                int verOpcion;
                do {
                    BorrarPantalla();
                    cout << "\n--- Menú Ver Progreso ---\n";
                    cout << "1. Ver equipos de soldados\n";
                    cout << "2. Ver equipos de zombies\n";
                    cout << "3. Ver objetos\n";
                    cout << "4. Ver consumibles\n";
                    cout << "5. Ver estaciones\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> verOpcion;

                    switch (verOpcion) {
                        case 1:
                            BorrarPantalla();
                            ImprimirEquiposExistentes(listaEquiposSoldados);
                            break;
                        case 2:
                            BorrarPantalla();
                            ImprimirEquiposZombiesExistentes(listaEquiposZombies);
                            break;
                        case 3:
                            BorrarPantalla();
                            MostrarObjetos(listaObjetos);
                            break;
                        case 4:
                            BorrarPantalla();
                            MostrarConsumibles(listaEquiposSoldados->equipo);
                            break;
                        case 5: 
                            BorrarPantalla();
                            VerEstaciones(mapa);
                            break;
                        case 0: break; // Regresar
                        default:
                            cout << "Opción no válida." << endl;
                    }
                    cout << "Presione Enter para continuar..." << endl;
                    cin.ignore();
                    cin.get();
                } while (verOpcion != 0);
                break;
            }
            case 5: { // Iniciar Gameplay
                BorrarPantalla();
                string equipoNombre, nodoNombre;
                cout << "Ingrese el nombre del equipo que desea mover: ";
                cin >> equipoNombre;
                cout << "Ingrese el nombre del nodo al que desea mover el equipo: ";
                cin >> nodoNombre;

                // Buscar el equipo de soldados por nombre
                EquiposSoldadosExistentes* equipoSeleccionado = nullptr;
                EquiposSoldadosExistentes* temp = listaEquiposSoldados;
                while (temp != nullptr) {
                    if (temp->nombre == equipoNombre) {
                        equipoSeleccionado = temp;
                        break;
                    }
                    temp = temp->prox;
                }

                if (equipoSeleccionado == nullptr) {
                    cout << "Equipo no encontrado." << endl;
                    break;
                }

                // Mover el equipo al nodo
                mapa.moverEquipo(equipoSeleccionado, nodoNombre);

                // Obtener el equipo de zombies en el nodo
                EquipoZombiesExistentes* equipoZombies = mapa.obtenerEquipoZombies(nodoNombre);

                // Iniciar la batalla
                InicioYControlDeLaBatalla(equipoSeleccionado->equipo, equipoZombies->equipo);

                cout << "Presione Enter para continuar..." << endl;
                cin.ignore();
                cin.get();
                break;
            }
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (opcion != 0);

    // Liberar memoria (si es necesario)
    // Aquí puedes implementar la lógica para liberar la memoria utilizada por los equipos y objetos

    return 0;
}


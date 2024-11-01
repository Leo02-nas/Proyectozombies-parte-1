#include <iostream>
#include <cmath> // Para usar round
#include <string> // Para usar string
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include <iomanip>
using namespace std;

// Proyecto de Algoritmos y Estructuras de Datos, Universidad Católica Andrés Bello, Hecho por Germán Moreno ci:31.449083, Jose Barazarte ci 30.464.544 y leonar Quiñónez ci:30.591.879

// Declarando los TDA

struct Zombie {
    string nombre;
    int vida;
    double resistencia;
    int velocidad;
    string inteligencia;
};

struct Objeto {
    string nombre;
    int dano;
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
    string crafteo[3];
};

struct ArmaADistancia {
    string nombre;
    int dano;
    bool aturdimiento;
    int distancia;
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

// Declarando las estructuras dinámicas

struct Mapa {
    string nombreEstacion; // Nombre de la estación
    int cantidadZombies; // Cantidad de zombies en la estación
    Mapa* der;
    Mapa* izq;
};

struct ObjetosExistentes {
    Objeto objeto;
    ObjetosExistentes* prox;
};

struct EquipoSoldados {
    Soldado soldado;
    EquipoSoldados* prox;
    EquipoSoldados* anterior;
};

struct EquipoZombies {
    Zombie zombie;
    Mochila mochiladelosmalos;
    EquipoZombies* prox;
    EquipoZombies* anterior;
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

// Funciones de Búsqueda

void BuscarEnLaMochilaDelSoldado(Soldado soldado, int seleccion){
    int respuesta;
    string nombredelarma;
    string nombredelobjetooconsumible;
    if(seleccion==3){
        cout << "Que tipo de arma buscas? \n\nNOTA: Selecciona (1) si es a Distancia, Selecciona (2) si es arrojadiza, Selecciona (3) si es cuerpo a cuerpo." << endl;
        cin >> respuesta;
        if(respuesta==1){
            cout << "Como se llama el arma que buscas?" << endl;
            cin >> nombredelarma;
            if(soldado.mochila.arma_a_distancia.nombre==nombredelarma){
                cout << "\nEl arma " << nombredelarma << " si la tiene el soldado " << soldado.nombre << endl;
            }
            else{
                cout << "\nEl arma " << nombredelarma << " no la tiene el soldado " << soldado.nombre << endl;
            }
        }
        if(respuesta==2){
            cout << "Como se llama el arma que buscas?" << endl;
            cin >> nombredelarma;
            if(soldado.mochila.arma_arrojadiza.nombre==nombredelarma){
                cout << "\nEl arma " << nombredelarma << " si la tiene el soldado " << soldado.nombre << endl;
            }
            else{
                cout << "\nEl arma " << nombredelarma << " no la tiene el soldado " << soldado.nombre << endl;
            }
        }
        if(respuesta==3){
            cout << "Como se llama el arma que buscas?" << endl;
            cin >> nombredelarma;
            if(soldado.mochila.arma_cuerpo_a_cuerpo.nombre==nombredelarma){
                cout << "\nEl arma " << nombredelarma << " si la tiene el soldado " << soldado.nombre << endl;
            }
            else{
                cout << "\nEl arma " << nombredelarma << " no la tiene el soldado " << soldado.nombre << endl;
            }
        }
        if(respuesta!=1&&respuesta!=2&&respuesta!=3){
            cout << "ERROR017: Ha introducido mal el numero, la busqueda se dentendra." << endl;
        }
    }
    if(seleccion==2){
        cout << "Como se llama el objeto que estas buscando?" << endl;
        cin >> nombredelarma;
            if(soldado.mochila.objeto.nombre==nombredelobjetooconsumible){
                cout << "\nEl objeto " << nombredelobjetooconsumible << " si la tiene el soldado " << soldado.nombre << endl;
            }
            else{
                cout << "\nEl objeto " << nombredelobjetooconsumible << " no la tiene el soldado " << soldado.nombre << endl;
            }
    }
    if(seleccion==1){
        cout << "Como se llama el consumible que estas buscando?" << endl;
        cin >> nombredelarma;
            if(soldado.mochila.consumible.nombre==nombredelobjetooconsumible){
                cout << "\nEl consumible " << nombredelobjetooconsumible << " si la tiene el soldado " << soldado.nombre << endl;
            }
            else{
                cout << "\nEl consumible " << nombredelobjetooconsumible << " no la tiene el soldado " << soldado.nombre << endl;
            }
    }
}
void BuscarAlSoldadoParaLaMochila(EquipoSoldados*& team, string nombre){
    int buscar;
    EquipoSoldados *actual = new EquipoSoldados;
    actual = team;
    if(actual==NULL)
        cout << "ERROR019: No puedes buscar en la mochila si no tienes soldados en el equipo" << endl;
    else{
    while(actual!=NULL){
        if(actual->soldado.nombre==nombre){
            cout << "EL SOLDADO FUE ENCONTRADO! :D\n" << endl;
            cout << "Que es lo que vas a buscar?\n\nNOTA: Recuerda colocar alguno de los siguientes valores:\n(1) Si es un consumible, (2) Si es un objeto, (3) si es un arma." << endl;
            cin >> buscar;
            if(buscar==3||buscar==2||buscar==1){
                BuscarEnLaMochilaDelSoldado(actual->soldado, buscar);
            }
            else{
                cout << "\n\nERROR017: El numero introducido no es el correcto, la busqueda se detendra." << endl;
            }
            break;
        }
        else{
            actual = actual -> prox;
        }
    }
    }
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

bool BuscarEnSoldadosPorNombre(EquipoSoldados* team, string nombre) {
    EquipoSoldados* actual = team;
    while (actual != nullptr) {
        if (actual->soldado.nombre == nombre) {
            cout << "El soldado sí está en tu equipo" << endl;
            return true;
        }
        actual = actual->prox;
    }
    cout << "El soldado no se ha encontrado" << endl;
    return false;
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
    cout << "Suerte(0-100): ";
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

void CrearUnArmaAunSoldado(EquipoSoldados*& team){
    int aturdimiento;
    int seleccion;
    if(team==NULL){
        cout << "ERROR018: No puedes anadir armas si no tienes equipo" << endl;
    }
    else{
        cout << "Que tipo de arma quieres crear?\n\nNOTA: Seleccione (1) si es a distancia, (2) si es cuerpo a cuerpo, (3) si es arrojadiza" << endl;
        cin >> seleccion;
    }
    if(seleccion==1||seleccion==2||seleccion==3){
        if(seleccion==1){
            cout << "Cual es el nombre del arma a distancia?" << endl;
            ArmaADistancia CreandoArma;
            cin >> CreandoArma.nombre;
            cout << "Cual es el dano de este arma a distancia? \n\nNOTA: Debe ser mayor a 0." << endl;
            cin >> CreandoArma.dano;
            cout << "Esta arma puede aturdir? \n\nNOTA: Responda (1) si puede, responda (2) si no puede. Aturdir, es eliminar un turno al equipo enemigo cuando esten peleando" << endl;
            cin >> aturdimiento;
            if(aturdimiento==1||aturdimiento==2){
                if(aturdimiento==1){
                    CreandoArma.aturdimiento = true;
                }
                else{
                    CreandoArma.aturdimiento = false;
                }
            }
            else{
                cout << "ERROR021: Usted ha introducido mal el valor, el arma no poseera aturdimiento" << endl;
                CreandoArma.aturdimiento = false;
            }
            cout << "Cual es la capacidad de municion de esta arma? \n\nNOTA:La capacidad debe ser superior a 0." << endl;
            cin >> CreandoArma.municion[0];
            cout << "Cuantas balas tendra ahora el arma?\n\nNOTA: La municion actual debe ser mayor a 0 y menor a la capacida del arma" << endl;
            cin >> CreandoArma.municion[1];
            cout << "A que distancia podra atacar esta arma? \n\nNOTA: La distancia debe ser mayor a 0." << endl;
            cin >> CreandoArma.distancia;
            cout << "Que tanto ruido tendra el arma? \n\nNOTA: El ruido debe ser mayor a 0 y menor a 10." << endl;
            cin >> CreandoArma.ruido;
            if(ValidarArmasADistanciaAlEntrar(CreandoArma)==true){
                cout << "Como se llama el soldado que tendra el arma?" << endl;
                string nombredelsoldado;
                cin >> nombredelsoldado;
                EquipoSoldados *actual = new EquipoSoldados;
                actual = team;
                while(actual!=NULL){
                    if(actual->soldado.nombre==nombredelsoldado){
                        actual->soldado.mochila.arma_a_distancia = CreandoArma;
                        cout << "El arma " << CreandoArma.nombre << " fue anadida correctamente al soldado " << actual->soldado.nombre << endl;
                        break;
                    }
                    else{
                        actual = actual -> prox;
                    }
                }
            }
        }
        if(seleccion==2){
            cout << "Cual es el nombre del arma cuerpo a cuerpo?" << endl;
            ArmaCuerpoACuerpo CreandoArma;
            cin >> CreandoArma.nombre;
            cout << "Cual es el dano de este arma cuerpo a cuerpo? \n\nNOTA: Debe ser mayor a 0." << endl;
            cin >> CreandoArma.dano;
            cout << "Esta arma puede aturdir? \n\nNOTA: Responda (1) si puede, responda (2) si no puede. Aturdir, es eliminar un turno al equipo enemigo cuando esten peleando" << endl;
            cin >> aturdimiento;
            if(aturdimiento==1||aturdimiento==2){
                if(aturdimiento==1){
                    CreandoArma.aturdimiento = true;
                }
                else{
                    CreandoArma.aturdimiento = false;
                }
            }
            else{
                cout << "ERROR021: Usted ha introducido mal el valor, el arma no poseera aturdimiento" << endl;
                CreandoArma.aturdimiento = false;
            }
            cout << "A que distancia podra atacar esta arma? \n\nNOTA: La distancia debe ser mayor a 0." << endl;
            cin >> CreandoArma.distancia;
            cout << "Que tanto ruido tendra el arma? \n\nNOTA: El ruido debe ser mayor a 0 y menor a 10." << endl;
            cin >> CreandoArma.ruido;
            if(ValidarArmasCuerpoACuerpoAlEntrar(CreandoArma)==true){
                cout << "Como se llama el soldado que tendra el arma?" << endl;
                string nombredelsoldado;
                cin >> nombredelsoldado;
                EquipoSoldados *actual = new EquipoSoldados;
                actual = team;
                while(actual!=NULL){
                    if(actual->soldado.nombre==nombredelsoldado){
                        actual->soldado.mochila.arma_cuerpo_a_cuerpo = CreandoArma;
                        cout << "El arma " << CreandoArma.nombre << " fue anadida correctamente al soldado " << actual->soldado.nombre << endl;
                        break;
                    }
                    else{
                        actual = actual -> prox;
                    }
                }
            }
        }
        if(seleccion==3){
            cout << "Cual es el nombre del arma arrojadiza?" << endl;
            ArmaArrojadiza CreandoArma;
            cin >> CreandoArma.nombre;
            cout << "Cual es el dano de este arma arrojadiza? \n\nNOTA: Debe ser mayor a 0." << endl;
            cin >> CreandoArma.dano;
            cout << "Esta arma puede aturdir? \n\nNOTA: Responda (1) si puede, responda (2) si no puede. Aturdir, es eliminar un turno al equipo enemigo cuando esten peleando" << endl;
            cin >> aturdimiento;
            if(aturdimiento==1||aturdimiento==2){
                if(aturdimiento==1){
                    CreandoArma.aturdimiento = true;
                }
                else{
                    CreandoArma.aturdimiento = false;
                }
            }
            else{
                cout << "ERROR021: Usted ha introducido mal el valor, el arma no poseera aturdimiento" << endl;
                CreandoArma.aturdimiento = false;
            }
            cout << "A que distancia podra atacar esta arma? \n\nNOTA: La distancia debe ser mayor a 0." << endl;
            cin >> CreandoArma.distancia;
            cout << "Que tanto ruido tendra el arma? \n\nNOTA: El ruido debe ser mayor a 0 y menor a 10." << endl;
            cin >> CreandoArma.ruido;
            if(ValidarArmasArrojadizasAlEntrar(CreandoArma)==true){
                cout << "Como se llama el soldado que tendra el arma?" << endl;
                string nombredelsoldado;
                cin >> nombredelsoldado;
                EquipoSoldados *actual = new EquipoSoldados;
                actual = team;
                while(actual!=NULL){
                    if(actual->soldado.nombre==nombredelsoldado){
                        actual->soldado.mochila.arma_arrojadiza = CreandoArma;
                        cout << "El arma " << CreandoArma.nombre << " fue anadida correctamente al soldado " << actual->soldado.nombre << endl;
                        break;
                    }
                    else{
                        actual = actual -> prox;
                    }
                }
            }
        }
    }
    else{
        cout << "ERROR022: Ha introducido mal el valor a evaluar." << endl;
    }
}
// Función para crear el nuevo nodo
Mapa* crearMapa(string nombre, int cantidad) {
    Mapa* nuevo_mapa = new Mapa();
    nuevo_mapa->nombreEstacion = nombre;
    nuevo_mapa->cantidadZombies = cantidad;
    nuevo_mapa->der = nullptr;
    nuevo_mapa->izq = nullptr;

    return nuevo_mapa;
}

// Función para agregar un nodo
void insertarMapa(Mapa*& arbol, string nombre, int cantidad) {
    if (arbol == nullptr) { // Si el árbol está vacío
        arbol = crearMapa(nombre, cantidad);
    } else { // Si el árbol tiene un nodo o más
        if (nombre < arbol->nombreEstacion) {
            insertarMapa(arbol->izq, nombre, cantidad); // Insertar a la izquierda
        } else {
            insertarMapa(arbol->der, nombre, cantidad); // Insertar a la derecha
        }
    }
}

// Función para buscar una estación por nombre
bool buscarEstacion(Mapa* arbol, string nombre) {
    if (arbol == nullptr) {
        return false; // No se encontró
    }
    if (arbol->nombreEstacion == nombre) {
        return true; // Se encontró
    }
    return buscarEstacion(arbol->izq, nombre) || buscarEstacion(arbol->der, nombre);
}


// Función para liberar la memoria del árbol
void liberarMapa(Mapa* arbol) {
    if (arbol != nullptr) {
        liberarMapa(arbol->izq);
        liberarMapa(arbol->der);
        delete arbol;
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

// Funciones de modificación
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

void mostrarMapa(Mapa* arbol) {
    if (arbol != nullptr) {
        mostrarMapa(arbol->izq);
        cout << "Estación: " << arbol->nombreEstacion << ", Zombies: " << arbol->cantidadZombies << endl;
        mostrarMapa(arbol->der);
    }
}

// Función para modificar una estación
void ModificarMapa(Mapa*& mapa) {
    if (mapa == nullptr) {
        cout << "No hay estaciones en el mapa." << endl;
        return;
    }

    // Mostrar la lista de estaciones
    cout << "\n--- Lista de Estaciones ---\n";
    mostrarMapa(mapa);

    string nombreEstacion;
    cout << "Ingrese el nombre de la estación a modificar: ";
    cin >> nombreEstacion;

    // Buscar la estación
    Mapa* actual = mapa;
    while (actual != nullptr) {
        if (actual->nombreEstacion == nombreEstacion) {
            cout << "Modificar cantidad de zombies (actual: " << actual->cantidadZombies << "): ";
            cin >> actual->cantidadZombies;
            cout << "Estación modificada correctamente." << endl;
            return;
        }
        if (nombreEstacion < actual->nombreEstacion) {
            actual = actual->izq;
        } else {
            actual = actual->der;
        }
    }

    cout << "Estación no encontrada." << endl;
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

// Mostrar la lista de zombies disponibles
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

// Modificar un zombie existente
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

// Función para mostrar información de los soldados
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

// Función para mostrar información de los zombies
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

// Función para mostrar información de los objetos
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

// Función para mostrar información de consumibles
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

// Funciones de eliminación
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

Mapa* eliminarEstacion(Mapa* arbol, string nombre) {
    if (arbol == nullptr) {
        return nullptr; // Árbol vacío
    }
    if (nombre < arbol->nombreEstacion) {
        arbol->izq = eliminarEstacion(arbol->izq, nombre); // Buscar en la izquierda
    } else if (nombre > arbol->nombreEstacion) {
        arbol->der = eliminarEstacion(arbol->der, nombre); // Buscar en la derecha
    } else {
        // Nodo encontrado
        if (arbol->izq == nullptr) {
            Mapa* temp = arbol->der;
            delete arbol;
            return temp; // Devolver el hijo derecho
        } else if (arbol->der == nullptr) {
            Mapa* temp = arbol->izq;
            delete arbol;
            return temp; // Devolver el hijo izquierdo
        }
        // Nodo con dos hijos: obtener el más pequeño de la derecha
        Mapa* temp = arbol->der;
        while (temp && temp->izq != nullptr) {
            temp = temp->izq;
        }
        arbol->nombreEstacion = temp->nombreEstacion; // Copiar el nombre
        arbol->cantidadZombies = temp->cantidadZombies; // Copiar la cantidad de zombies
        arbol->der = eliminarEstacion(arbol->der, temp->nombreEstacion); // Eliminar el nodo duplicado
    }
    return arbol;
}

void EliminarMapa(Mapa*& mapa) {
    if (mapa == nullptr) {
        cout << "No hay estaciones en el mapa." << endl;
        return;
    }

    // Mostrar la lista de estaciones
    cout << "\n--- Lista de Estaciones ---\n";
    mostrarMapa(mapa);

    string nombreEstacion;
    cout << "Ingrese el nombre de la estación a eliminar: ";
    cin >> nombreEstacion;

    // Eliminar la estación
    mapa = eliminarEstacion(mapa, nombreEstacion);
    cout << "Estación eliminada correctamente." << endl;
}

void VerEstaciones(Mapa* mapa) {
    if (mapa == nullptr) {
        cout << "No hay estaciones en el mapa." << endl;
        return;
    }
    cout << "\n--- Lista de Estaciones ---\n";
    mostrarMapa(mapa);
}



// Función principal
int main() {
    MostrarMensajeInicio();
    Mapa* mapa = nullptr; // Inicializar el árbol de mapas
    EquipoSoldados* equipo = nullptr; // Inicializar el equipo de soldados
    EquipoZombies* enemigos = nullptr; // Inicializar el equipo de zombies
    ObjetosExistentes* listaObjetos = nullptr; // Inicializar la lista de objetos

    
    int opcion;
    do {
        BorrarPantalla();
        cout << "\n--- Menú Principal ---\n";
        cout << "1. Crear\n";
        cout << "2. Modificar\n";
        cout << "3. Eliminar\n";
        cout << "4. Ver Progreso\n";
        cout << "0. Salir\n";
        cout << "Elija una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: { // Crear
                int crearOpcion;
                do {
                    BorrarPantalla();
                    cout << "\n--- Menú Crear ---\n";
                    cout << "1. Agregar soldado\n";
                    cout << "2. Agregar zombie\n"; // Nueva opción para agregar zombies
                    cout << "3. Crear objeto\n";
                    cout << "4. Crear consumible\n";
                    cout << "5. Agregar arma a soldado\n";
                    cout << "6. Agregar estación\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> crearOpcion;

                    switch (crearOpcion) {
                        case 1: {
                            BorrarPantalla();
                            AnadirAlEquipoSoldadoPorDelante(equipo);
                            break;
                        }
                        case 2: { 
                            BorrarPantalla();
                            AgregarZombie(enemigos);
                            break;
                        }
                        case 3: {
                            BorrarPantalla();
                            CrearObjeto(listaObjetos);
                            break;
                        }
                        case 4: {
                            BorrarPantalla();
                            CrearConsumible(equipo);
                            break;
                        }
                        case 5: {
                            BorrarPantalla();
                            CrearUnArmaAunSoldado(equipo);
                            break;
                        }
                        case 6: {
                            BorrarPantalla();
                            string nombreEstacion;
                            int cantidadZombies;
                            cout << "Ingrese el nombre de la estación: ";
                            cin >> nombreEstacion;
                            cout << "Ingrese la cantidad de zombies en la estación: ";
                            cin >> cantidadZombies;
                            insertarMapa(mapa, nombreEstacion, cantidadZombies);
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
                    cout << "1. Modificar soldado\n";
                    cout << "2. Modificar zombie\n"; // Nueva opción para modificar zombies
                    cout << "3. Modificar arma\n";
                    cout << "4. Modificar mapa\n";
                    cout << "5. Modificar objeto\n";
                    cout << "6. Modificar consumible\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> modificarOpcion;

                    switch (modificarOpcion) {
                        case 1: {
                            BorrarPantalla();
                            ModificarSoldado(equipo);
                            break;
                        }
                        case 2: { 
                            BorrarPantalla();
                            ModificarZombie(enemigos);
                            break;
                        }
                        case 3: {
                            BorrarPantalla();
                            ModificarArma(equipo);
                            break;
                        }
                        case 4: {
                            BorrarPantalla();
                            ModificarMapa(mapa);
                            break;
                        }
                        case 5: {
                            BorrarPantalla();
                            ModificarObjeto(listaObjetos);
                            break;
                        }
                        case 6: {
                            BorrarPantalla();
                            ModificarConsumible(equipo);
                            break;
                        }
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
                    cout << "1. Eliminar soldado\n";
                    cout << "2. Eliminar zombie\n"; 
                    cout << "3. Eliminar arma\n";
                    cout << "4. Eliminar mapa\n";
                    cout << "5. Eliminar objeto\n";
                    cout << "6. Eliminar consumible\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> eliminarOpcion;

                    switch (eliminarOpcion) {
                        case 1: {
                            BorrarPantalla();
                            EliminarSoldado(equipo);
                            break;
                        }
                        case 2: { 
                            BorrarPantalla();
                            EliminarZombie(enemigos);
                            break;
                        }
                        case 3: {
                            BorrarPantalla();
                            EliminarArma(equipo);
                            break;
                        }
                        case 4: {
                            BorrarPantalla();
                             EliminarMapa(mapa);
                            break;
                        }
                        case 5: {
                            BorrarPantalla();
                            EliminarObjeto(listaObjetos);
                            break;
                        }
                        case 6: {
                            BorrarPantalla();
                            EliminarConsumible(equipo);
                            break;
                        }
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
                    cout << "1. Ver soldados\n";
                    cout << "2. Ver zombies\n";
                    cout << "3. Ver objetos\n";
                    cout << "4. Ver consumibles\n";
                     cout << "5. Ver estaciones\n";
                    cout << "0. Regresar\n";
                    cout << "Elija una opción: ";
                    cin >> verOpcion;

                    switch (verOpcion) {
                        case 1:
                            BorrarPantalla();
                            MostrarSoldados(equipo);
                            break;
                        case 2:
                            BorrarPantalla();
                            MostrarZombie(enemigos);
                            break;
                        case 3:
                            BorrarPantalla();
                            MostrarObjetos(listaObjetos);
                            break;
                        case 4:
                            BorrarPantalla();
                            MostrarConsumibles(equipo);
                            break;
                        case 5: { 
                            BorrarPantalla();
                            VerEstaciones(mapa);
                            break;
                        }
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
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (opcion != 0);

    liberarMapa(mapa);

    return 0;
}
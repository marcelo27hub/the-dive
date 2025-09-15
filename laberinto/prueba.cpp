#include <iostream> // entrada y salida por consola (cin, cout)
#include <vector>   // vector dinámico (matrices 2D con vector<vector<...>>)
#include <queue>    // cola FIFO, usada para BFS
#include <cstdlib>  // funciones generales C: system(), atoi(), etc.
#include <ctime>    // time(0) para seed de rand/srand
#include <algorithm>// utilidades: random_shuffle, reverse, etc.
#include <thread>   // this_thread::sleep_for para pausas/animación
#include <chrono>   // chrono::milliseconds para medir duraciones
using namespace std; // evita escribir std:: en cada uso de la STL/iostream

// Define la estructura que representa cada celda del laberinto
struct Celda {
    bool visitada = false;       // si la DFS ya pasó por esta celda
    bool paredes[4] = {true,true,true,true}; // paredes: ARRIBA(0), DERECHA(1), ABAJO(2), IZQUIERDA(3)
    bool enCamino = false;       // true mientras la DFS está "en" esta celda (animación)
    bool caminoCorto = false;    // true si la BFS marcó esta celda como parte del camino resultante
};

// vectores para movernos por las 4 direcciones (índices coinciden con paredes)
int dx[4] = {0, 1, 0, -1};     // cambio en columna según dirección: arriba(0)->0, derecha->1, abajo->0, izq->-1
int dy[4] = {-1, 0, 1, 0};     // cambio en fila según dirección: arriba->-1, derecha->0, abajo->1, izq->0

// tamaño del laberinto (globales)
int filas = 0, columnas = 0;

// matriz dinámica que contiene las celdas
vector<vector<Celda>> laberinto;

// Comprueba que (columna,fila) esté dentro de los límites del laberinto
bool esValido(int columna, int fila){
    return columna >= 0 && columna < columnas && fila >= 0 && fila < filas;
}

// Elimina la pared entre dos celdas adyacentes: modifica ambas celdas (por referencia)
void removerPared(Celda &a, Celda &b, int direccion){
    a.paredes[direccion] = false;                  // quita la pared en la celda 'a' en la dirección dada
    b.paredes[(direccion + 2) % 4] = false;        // quita la pared opuesta en la celda 'b' (direccion opuesta = +2 mod 4)
}

// Imprime el laberinto en la consola usando ASCII
void imprimirLaberinto(){
    system("CLS"); // limpia la consola (funciona en Windows; en Linux usar "clear")
    for(int fila=0; fila<filas; fila++){
        // Dibujar la línea superior de cada celda (esquinas + paredes horizontales)
        for(int columna=0; columna<columnas; columna++)
            cout << "+" << (laberinto[fila][columna].paredes[0] ? "---" : "   ");
        cout << "+\n"; // cierre de la fila superior

        // Dibujar el contenido de la fila: paredes izquierdas y el interior de cada celda
        for(int columna=0; columna<columnas; columna++){
            cout << (laberinto[fila][columna].paredes[3] ? "|" : " "); // pared izquierda o espacio

            // Prioridad para mostrar: E (entrada), S (salida), caminoCorto (O), enCamino (X), o vacío
            if(fila==0 && columna==0) cout << " E "; // entrada en (0,0)
            else if(fila==filas-1 && columna==columnas-1) cout << " S "; // salida en (filas-1,columnas-1)
            else if(laberinto[fila][columna].caminoCorto) cout << " O "; // parte del camino resuelto por BFS
            else if(laberinto[fila][columna].enCamino) cout << " X ";    // celda actualmente en la DFS (animación)
            else cout << "   "; // celda vacía/pasillo
        }
        cout << "|\n"; // cierre derecho de la fila (siempre se imprime | aquí, aunque la última celda sea espacio)
    }
    // Dibujar la última línea inferior que cierra el laberinto
    for(int columna=0; columna<columnas; columna++) cout << "+---";
    cout << "+\n";
}

// Genera el laberinto usando DFS recursivo (backtracking)
void generarlaberintoDFS(int columna, int fila){
    laberinto[fila][columna].visitada = true;   // marca la celda como visitada
    laberinto[fila][columna].enCamino = true;   // marca que la DFS está aquí (para animación)
    imprimirLaberinto();                        // pinta el estado actual
    this_thread::sleep_for(chrono::milliseconds(100)); // pausa para que se vea la animación

    vector<int> direcciones = {0,1,2,3}; // conjunto de direcciones: ARRIBA(0), DERECHA(1), ABAJO(2), IZQUIERDA(3)
    random_shuffle(direcciones.begin(), direcciones.end()); // baraja el orden de exploración (aleatoriedad)

    for(int direccion : direcciones){
        int nx = columna + dx[direccion]; // columna vecina
        int ny = fila + dy[direccion];    // fila vecina
        // Si la celda vecina está dentro del laberinto y no fue visitada, vamos hacia ella
        if(esValido(nx, ny) && !laberinto[ny][nx].visitada){
            // quitar la pared entre la celda actual y la vecina
            removerPared(laberinto[fila][columna], laberinto[ny][nx], direccion);
            // llamar recursivamente desde la celda vecina
            generarlaberintoDFS(nx, ny);
        }
    }

    // al terminar con todos los vecinos, la celda deja de ser parte del "camino activo"
    laberinto[fila][columna].enCamino = false;
    imprimirLaberinto(); // pintar estado final de esta celda
    this_thread::sleep_for(chrono::milliseconds(50)); // pausa corta al retroceder (backtracking)
}

// Reconstruir y animar el camino más corto usando BFS (desde E hacia S)
void CaminoMasCorto(){
    queue<pair<int,int>> cola; // cola para BFS, almacena pares (columna, fila)
    // matriz 'padre' para reconstruir el camino: padre[fila][columna] = coordenada anterior (col, fila)
    vector<vector<pair<int,int>>> padre(filas, vector<pair<int,int>>(columnas, {-1,-1}));
    cola.push({0,0}); // empezamos desde la entrada (0,0)
    vector<vector<bool>> visitado(filas, vector<bool>(columnas,false)); // matriz de visitado
    visitado[0][0] = true; // marcamos la entrada como visitada

    // Bucle principal de BFS
    while(!cola.empty()){
        auto [columna, fila] = cola.front(); cola.pop(); // obtenemos y sacamos el frente de la cola

        // si llegamos a la salida, podemos terminar el BFS (primer encuentro garantiza camino mínimo)
        if(columna == columnas-1 && fila == filas-1) break;

        // revisar las 4 direcciones desde la celda actual
        for(int direccion=0; direccion<4; direccion++){
            int nuevaColumna = columna + dx[direccion];
            int nuevaFila = fila + dy[direccion];

            // avanzar solo si: dentro de límites, no visitado y no hay pared en esa dirección
            if(esValido(nuevaColumna, nuevaFila) &&
               !visitado[nuevaFila][nuevaColumna] &&
               !laberinto[fila][columna].paredes[direccion]){

                visitado[nuevaFila][nuevaColumna] = true;               // marcamos la celda vecina como visitada
                padre[nuevaFila][nuevaColumna] = {columna, fila};       // guardamos la procedencia (padre)
                cola.push({nuevaColumna, nuevaFila});                   // agregamos la vecina a la cola
            }
        }
    }

    // Reconstruir el camino desde S hacia E usando la matriz padre
    vector<pair<int,int>> camino;
    int columna = columnas - 1, fila = filas - 1; // empezamos desde la salida S
    while(!(columna == 0 && fila == 0)){
        camino.push_back({columna, fila});          // añadimos la celda actual al vector camino
        auto anterior = padre[fila][columna];       // buscamos su padre (de dónde vino)
        columna = anterior.first;                   // actualizamos columna a la del padre
        fila = anterior.second;                     // actualizamos fila a la del padre
    }
    camino.push_back({0,0}); // agregamos la entrada E al final (ya llegamos a ella)

    // invertimos porque reconstruimos S->E, pero queremos animar E->S
    reverse(camino.begin(), camino.end());

    // marcar cada celda del camino como caminoCorto y animar la resolución
    for(auto [c,f] : camino){
        laberinto[f][c].caminoCorto = true;      // marcar como parte del camino solucionado
        imprimirLaberinto();                     // mostrar laberinto con la nueva marca
        this_thread::sleep_for(chrono::milliseconds(50)); // pausa para animación
    }
}

int main(){
    srand(time(0)); // inicializa la semilla del generador aleatorio con el tiempo actual
    char opcion;

    do{
        cout << "\nOpciones:\n";
        cout << "1. Generar laberinto\n";
        cout << "2. Resolver laberinto (camino más corto)\n";
        cout << "3. Salir\n";
        cout << "Opcion: ";
        cin >> opcion; // lee la opción del usuario

        if(opcion=='1'){
            cout << "\nIngrese filas: "; cin >> filas;            // solicita número de filas
            cout << "Ingrese columnas: "; cin >> columnas;        // solicita número de columnas
            // inicializa la matriz laberinto con celdas por defecto
            laberinto = vector<vector<Celda>>(filas, vector<Celda>(columnas));

            cout << "\nGenerando laberinto de " << filas << " x " << columnas << "...\n";
            generarlaberintoDFS(0,0); // arranca la generación desde la esquina superior izquierda
            cout << "¡Laberinto generado!\n";
            imprimirLaberinto();      // muestra el laberinto final
        }
        else if(opcion=='2'){
            if(filas==0 || columnas==0){
                cout << "Primero genera un laberinto.\n"; // protección: no hay laberinto creado aún
                continue;
            }
            cout << "Mostrando camino más corto...\n";
            CaminoMasCorto(); // resuelve y anima el camino
            cout << "¡Laberinto resuelto!\n";
            imprimirLaberinto(); // muestra el laberinto con el camino final marcado
        }
    } while(opcion != '3'); // repetir hasta que el usuario elija salir

    cout << "Saliendo...\n";
    return 0;
}

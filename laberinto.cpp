#include <iostream> //entrada y salida
#include <vector>   // arrays dinámicos (matrices)
#include <queue>    // estructura de datos (cola) para BFS
#include <cstdlib>  // ejecutar comandos de consola
#include <ctime>    // manejar fecha y hora
#include <algorithm>// manipular datos
#include <thread>   // crear y controlar hilos
#include <chrono>   // medir tiempo y pausas

using namespace std; // para no escribir  cada vez

struct Celda {
    bool visitada = false;       // si el DFS pasó por aquí
    bool paredes[4] = {true,true,true,true}; // ARRIBA, DERECHA, ABAJO, IZQUIERDA
    bool enCamino = false;       // muestra la ruta mientras se genera el laberinto
    bool caminoCorto = false;    // muestra el camino más corto
};

// para moverse
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

// tamaño del laberinto
int filas = 0, columnas = 0;

// lista de listas (matriz)
vector<vector<Celda>> laberinto;

// verificar si un movimiento está dentro del laberinto
bool esValido(int columna, int fila){
    return columna >= 0 && columna < columnas && fila >= 0 && fila < filas; 
}

// elimina la pared entre dos celdas adyacentes
void removerPared(Celda &a, Celda &b, int direccion){
    a.paredes[direccion] = false;
    b.paredes[(direccion + 2) % 4] = false;
}

// imprimir laberinto
void imprimirLaberinto(){
    system("CLS"); // limpiar la consola
    for(int fila=0; fila<filas; fila++){ 
        for(int columna=0; columna<columnas; columna++)
            cout << "+" << (laberinto[fila][columna].paredes[0] ? "---" : "   ");
        cout << "+\n";
        for(int columna=0; columna<columnas; columna++){
            cout << (laberinto[fila][columna].paredes[3] ? "|" : " ");
            if(fila==0 && columna==0) cout << " E "; // entrada
            else if(fila==filas-1 && columna==columnas-1) cout << " S "; // salida
            else if(laberinto[fila][columna].caminoCorto) cout << " O "; //dibuja el camino corto
            else if(laberinto[fila][columna].enCamino) cout << " X "; // hace el laberinto con x el back tracking
            else cout << "   ";
        }
        cout << "|\n";
    }
    for(int columna=0; columna<columnas; columna++) cout << "+---";
    cout << "+\n";
}

// generar laberinto recursivamente con DFS
void generarlaberintoDFS(int columna, int fila){
    laberinto[fila][columna].visitada = true;
    laberinto[fila][columna].enCamino = true;
    imprimirLaberinto();
    this_thread::sleep_for(chrono::milliseconds(100));

    vector<int> direcciones = {0,1,2,3}; // ARRIBA, DERECHA, ABAJO, IZQUIERDA
    random_shuffle(direcciones.begin(), direcciones.end());

    for(int direccion : direcciones){
        int nx = columna + dx[direccion];
        int ny = fila + dy[direccion];
        if(esValido(nx, ny) && !laberinto[ny][nx].visitada){
            removerPared(laberinto[fila][columna], laberinto[ny][nx], direccion);
            generarlaberintoDFS(nx, ny);
        }
    }

    laberinto[fila][columna].enCamino = false;
    imprimirLaberinto();
    this_thread::sleep_for(chrono::milliseconds(50));
}

// reconstruir camino más corto desde E a S con BFS (animación desde E)
void CaminoMasCorto(){
    queue<pair<int,int>> cola;
    vector<vector<pair<int,int>>> padre(filas, vector<pair<int,int>>(columnas, {-1,-1}));
    cola.push({0,0}); // empezamos desde E
    vector<vector<bool>> visitado(filas, vector<bool>(columnas,false));
    visitado[0][0] = true;

    while(!cola.empty()){
        auto [columna, fila] = cola.front(); cola.pop();

        // si llegamos a la salida, rompemos el bucle
        if(columna == columnas-1 && fila == filas-1) break;

        // revisar las 4 direcciones
        for(int direccion=0; direccion<4; direccion++){
            int nuevaColumna = columna + dx[direccion];
            int nuevaFila = fila + dy[direccion];

            if(esValido(nuevaColumna, nuevaFila) &&
               !visitado[nuevaFila][nuevaColumna] &&
               !laberinto[fila][columna].paredes[direccion]){

                visitado[nuevaFila][nuevaColumna] = true;
                padre[nuevaFila][nuevaColumna] = {columna, fila};
                cola.push({nuevaColumna, nuevaFila});
            }
        }
    }

    // reconstruir camino completo desde S hacia E
    vector<pair<int,int>> camino;
    int columna = columnas - 1, fila = filas - 1; // empezamos desde S
    while(!(columna == 0 && fila == 0)){
        camino.push_back({columna, fila});
        auto anterior = padre[fila][columna];
        columna = anterior.first;
        fila = anterior.second;
    }
    camino.push_back({0,0}); // agregar E

    // invertir para que la animación empiece desde E hacia S
    reverse(camino.begin(), camino.end());

    // marcar camino y animar
    for(auto [c,f] : camino){
        laberinto[f][c].caminoCorto = true;
        imprimirLaberinto();
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main(){
    srand(time(0));
    char opcion;

    do{
        cout << "\nOpciones:\n";
        cout << "1. Generar laberinto\n";
        cout << "2. Resolver laberinto (camino más corto)\n";
        cout << "3. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        if(opcion=='1'){
            cout << "\nIngrese filas: "; cin >> filas;
            cout << "Ingrese columnas: "; cin >> columnas;
            laberinto = vector<vector<Celda>>(filas, vector<Celda>(columnas));

            cout << "\nGenerando laberinto de " << filas << " x " << columnas << "...\n";
            generarlaberintoDFS(0,0);
            cout << "¡Laberinto generado!\n";
            imprimirLaberinto();
        }
        else if(opcion=='2'){
            if(filas==0 || columnas==0){
                cout << "Primero genera un laberinto.\n";
                continue;
            }
            cout << "Mostrando camino más corto...\n";
            CaminoMasCorto();
            cout << "¡Laberinto resuelto!\n";
            imprimirLaberinto();
        }
    } while(opcion != '3');

    cout << "Saliendo...\n";
    return 0;
}

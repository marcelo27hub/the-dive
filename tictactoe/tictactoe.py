import random

class Tictactoe:
    def __init__(self):
        # Inicializa el tablero vacío con 9 casillas
        self.tablero = [" " for _ in range(9)]
        # Símbolos de los jugadores
        self.humano_jugador = "O"
        self.ia_jugador = "X"
        
    def imprimir_tablero(self):
        """Imprime el estado actual del tablero"""
        for i in range(0, 9, 3):
            # Imprime cada fila
            print(f"{self.tablero[i]} | {self.tablero[i+1]} | {self.tablero[i+2]}")
            if i < 6:
                # Línea divisoria entre filas
                print("---------")
    
    def movimientos_disponibles(self):
        """Devuelve lista de posiciones vacías donde se puede jugar"""
        return [i for i, casilla in enumerate(self.tablero) if casilla == " "]    
    
    def hacer_jugada(self, posicion, jugador):
        """Coloca el símbolo del jugador en la posición indicada si está vacía"""
        if self.tablero[posicion] == " ":
            self.tablero[posicion] = jugador
            return True
        return False
    
    def tablero_lleno(self):
        """Verifica si el tablero ya no tiene casillas vacías"""
        return " " not in self.tablero
    
    def verificar_ganador(self):
        """Revisa si hay un ganador y devuelve su símbolo, o None si no hay"""
        # Revisar filas
        for i in range(0, 9, 3):
            if self.tablero[i] == self.tablero[i+1] == self.tablero[i+2] != " ":
                return self.tablero[i]
        # Revisar columnas
        for i in range(3):
            if self.tablero[i] == self.tablero[i+3] == self.tablero[i+6] != " ":
                return self.tablero[i]
        # Revisar diagonales
        if self.tablero[0] == self.tablero[4] == self.tablero[8] != " ":
            return self.tablero[0]
        if self.tablero[2] == self.tablero[4] == self.tablero[6] != " ":
            return self.tablero[2]
        # Si no hay ganador
        return None
    
    def juego_terminado(self):
        """Devuelve True si hay ganador o el tablero está lleno (empate)"""
        return self.verificar_ganador() is not None or self.tablero_lleno()
    
    def minimax(self, profundidad, es_maximizador):
        """Implementa el algoritmo Minimax para calcular la mejor puntuación"""
        ganador = self.verificar_ganador()
        # Casos base: ganador o empate
        if ganador == self.ia_jugador:
            return 1  # IA gana
        elif ganador == self.humano_jugador:
            return -1  # Humano gana
        elif self.tablero_lleno():
            return 0  # Empate
        
        if es_maximizador:
            # Turno de la IA: buscamos maximizar el puntaje
            mejor_puntaje = float("-inf")
            for movimiento in self.movimientos_disponibles():
                self.tablero[movimiento] = self.ia_jugador  # Simular jugada de IA
                puntaje = self.minimax(profundidad + 1, False)  # Llamada recursiva
                self.tablero[movimiento] = " "  # Deshacer jugada
                mejor_puntaje = max(puntaje, mejor_puntaje)  # Elegir el puntaje máximo
            return mejor_puntaje  
        else:
            # Turno del humano: buscamos minimizar el puntaje de la IA
            mejor_puntaje = float("inf")
            for jugada in self.movimientos_disponibles():
                self.tablero[jugada] = self.humano_jugador  # Simular jugada del humano
                puntaje = self.minimax(profundidad + 1, True)  # Llamada recursiva
                self.tablero[jugada] = " "  # Deshacer jugada
                mejor_puntaje = min(puntaje, mejor_puntaje)  # Elegir el puntaje mínimo
            return mejor_puntaje
    
    def obtener_mejor_jugada(self):
        """Encuentra la mejor jugada posible para la IA usando Minimax"""
        mejor_puntaje = float("-inf")
        mejor_jugada = None
        for jugada in self.movimientos_disponibles():
            self.tablero[jugada] = self.ia_jugador  # Simular jugada de IA
            puntaje = self.minimax(0, False)  # Evaluar con Minimax
            self.tablero[jugada] = " "  # Deshacer jugada
            if puntaje > mejor_puntaje:
                mejor_puntaje = puntaje  # Guardar mejor puntaje
                mejor_jugada = jugada  # Guardar mejor jugada
        return mejor_jugada
    
    def jugar(self):
        """Bucle principal del juego"""
        # Mensajes de bienvenida y explicación
        print("¡Bienvenido al Ta Te Ti!")
        print("Tú eres 'O' y la IA es 'X'")
        print("Ingresa posiciones (0-8) como se muestra abajo:")
        print("0 | 1 | 2")
        print("---------")
        print("3 | 4 | 5")
        print("---------")
        print("6 | 7 | 8")
        print("\n")    
        
        # Decidir aleatoriamente quién empieza
        turno_ia = random.choice([True, False])

        # Bucle del juego hasta que termine
        while not self.juego_terminado():
            self.imprimir_tablero()

            if turno_ia:
                # Turno de la IA
                print("\nTurno de la IA...")
                movimiento = self.obtener_mejor_jugada()
                self.hacer_jugada(movimiento, self.ia_jugador)
            else:
                # Turno del jugador humano
                while True:
                    try:
                        movimiento = int(input("\nTu turno (0-8): "))
                        if 0 <= movimiento <= 8 and self.hacer_jugada(movimiento, self.humano_jugador):
                            break
                        else:
                            print("¡Movimiento inválido! Intenta de nuevo.")
                    except ValueError:
                        print("¡Por favor ingresa un número entre 0 y 8!")

            # Cambiar turno
            turno_ia = not turno_ia
            
        # Juego terminado: mostrar tablero final
        self.imprimir_tablero()
        ganador = self.verificar_ganador()
        if ganador == self.ia_jugador:
            print("\n¡La IA gana!")
        elif ganador == self.humano_jugador:
            print("\n¡Felicidades! ¡Ganaste!")
        else:
            print("\n¡Es un empate!")

# Ejecutar el juego
if __name__ == "__main__":
    juego = Tictactoe()
    juego.jugar()
        
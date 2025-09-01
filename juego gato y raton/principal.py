#importamos crear  e imprimir tableto
#importamos mover raton minimax de minimax
from tableroGyR import crear_tablero, mostrar_tablero
from minimaxGyR import mover_raton_minimax

# Configuración inicial
#pedidmos al usuario la cantidad de filas y columnas
filas = int(input("Filas del tablero: ")) 
columnas = int(input("Columnas del tablero: ")) 
#creamos el tablero
tablero = crear_tablero(filas, columnas)

# posiciones iniciales
pos_gato = (0, 0)
pos_raton = (filas - 1, columnas - 1)

if pos_gato == pos_raton: #si la psosicion del gato es igual a la posicion del raton lo atrapa inmediatamente
    tablero[pos_gato[0]][pos_gato[1]] = " G "  # el gato ganó
else:
    #colcamos G y R en el tablero
    tablero[pos_gato[0]][pos_gato[1]] = " G "
    tablero[pos_raton[0]][pos_raton[1]] = " R "
    #llamamos a nuestra funcion tablero 
mostrar_tablero(tablero)
#le ponemos la cantidad de turnos
turnos_max = 15
#de acuerdo a los movimientos suven los turnos para el gato 
turno = 1
#mientras turno sea menor o igual a turno maximo y posicion del gato sea diferente de posicion raton el ciclo continua
while turno <= turnos_max and pos_gato != pos_raton:
    
    print(f"\nTurno {turno}")
    # turno del gato(usuario)
    mov = input("Movimiento Gato (W/A/S/D): ").upper() 
    f, c = pos_gato
    #movimientos posibles del gato 
    if mov == "W" and f > 0: f -= 1 #arriba
    elif mov == "S" and f < filas - 1: f += 1 #abajo
    elif mov == "A" and c > 0: c -= 1 #izquierda
    elif mov == "D" and c < columnas - 1: c += 1 #derecha
    #aca metemos en una tupla los movimientos del gato 
    pos_gato = (f, c)

    # turno del raton con minimax
    if pos_gato != pos_raton:
        #IA del ratón: calcula y devuelve su nueva posición con minimax
        pos_raton = mover_raton_minimax(tablero, pos_raton, pos_gato, filas, columnas)#ejecuta el minimax raton huyendo del gato 

    # actualizar tablero
    tablero = crear_tablero(filas, columnas)
    tablero[pos_gato[0]][pos_gato[1]] = " G "
    tablero[pos_raton[0]][pos_raton[1]] = " R "
    mostrar_tablero(tablero)
    #pasamos al siguiente turno
    turno += 1

# Resultado
if pos_gato == pos_raton:
    print("\n¡El gato atrapó al ratón!")
else:
    print("\n¡El ratón sobrevivió!")
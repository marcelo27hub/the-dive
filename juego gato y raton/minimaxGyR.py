from movimientosGyR import movimientos_posibles

def evaluar_estado(pos_gato, pos_raton):
    if pos_gato == pos_raton:
        return 1000  # el gato atrapó al ratón
    distancia = abs(pos_gato[0] - pos_raton[0]) + abs(pos_gato[1] - pos_raton[1])
    return -distancia  # mientras más lejos el ratón, mejor

def minimax(pos_gato, pos_raton, profundidad, turno_gato, filas, columnas, tablero):
    if profundidad == 0 or pos_gato == pos_raton:
        return evaluar_estado(pos_gato, pos_raton), pos_raton
    
    if turno_gato:  # turno del gato (maximiza)
        mejor_valor = float("-inf")
        mejor_mov = pos_gato
        for mov in movimientos_posibles(pos_gato, filas, columnas, tablero):
            valor, _ = minimax(mov, pos_raton, profundidad - 1, False, filas, columnas, tablero)
            if valor > mejor_valor:
                mejor_valor, mejor_mov = valor, mov
        return mejor_valor, mejor_mov
    else:  # turno del ratón (minimiza)
        mejor_valor = float("inf")
        mejor_mov = pos_raton
        for mov in movimientos_posibles(pos_raton, filas, columnas, tablero):
            valor, _ = minimax(pos_gato, mov, profundidad - 1, True, filas, columnas, tablero)
            if valor < mejor_valor:
                mejor_valor, mejor_mov = valor, mov
        return mejor_valor, mejor_mov

def mover_raton_minimax(tablero, pos_raton, pos_gato, filas, columnas, profundidad=4):
    _, mejor_mov = minimax(pos_gato, pos_raton, profundidad, False, filas, columnas, tablero)
    tablero[pos_raton[0]][pos_raton[1]] = " . "
    if mejor_mov != pos_gato:
        tablero[mejor_mov[0]][mejor_mov[1]] = " R "
    return mejor_mov

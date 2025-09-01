# movimientosGyR.py

def movimientos_posibles(pos, filas, columnas, tablero):

    #creamos una lista para agregsar los movimientos que hace el usuario con el gato
    movimientos = []
    #calculamos los movimeientos posibles desde una posicion del tablero
    direcciones = [(-1, 0), (1, 0), (0, -1), (0, 1)]  # arriba, abajo, izquierda, derecha
    #crearos un for con direc en direciones 
    for d in direcciones:
        #creamos nuestra nueva posicion  que seria un ejemplo
        #posicion (2,2) en un tablero (5,5) 
        # si d = (0,1) derecha
        nueva_pos = (pos[0] + d[0], pos[1] + d[1])  
        #(2 - 0, 2 + 1) = (2, 3) ahi lo que estamos haciendo es ir hacia la derecha

        # Verificamos que esté dentro de los límites
        if 0 <= nueva_pos[0] < filas and 0 <= nueva_pos[1] < columnas:
            # esa posicion (2, 3) esta in los caracteres ., R, G se movera
            if tablero[nueva_pos[0]][nueva_pos[1]] in [" . ", " R ", " G "]:
                # y si lo esta  lo agreagamos en la lista de movimientos
                movimientos.append(nueva_pos)
    #retornamos la lista de movimientos
    return movimientos
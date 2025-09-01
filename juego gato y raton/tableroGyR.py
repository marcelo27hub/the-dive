#definimos nuestra funcion para crear nuestro tablero
def crear_tablero(filas, columnas):
    #creo mi tablero con columnas y filas
    
    #le digo que me ponga . en mi lista tablero en el rango de columnas 
    # y esa misma lista repetir cantidad de filas en otra lista
    tablero = [[" . " for c in range(columnas)] for f in range(filas)]
    #retornamos el tablero 
    return tablero 
#defino mi funcion mostrar tablero 
def mostrar_tablero(tablero):
    #para fila en mi funcion tablero con argumentos y parametros ya puestos
    for fila in tablero:
    #para fila en mi tablero quiero que me separe cada elemento y me lo imprima como un string con el .join
        print(" ".join(fila))
       
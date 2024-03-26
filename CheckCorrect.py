import csv
import numpy as np

# Función para cargar una matriz desde un archivo CSV
def cargar_matriz(archivo):
    with open(archivo, newline='') as f:
        reader = csv.reader(f)
        matriz = []
        for fila in reader:
            fila_numerica = []
            for elemento in fila:
                elemento_limpiado = elemento.strip().upper()
                if elemento_limpiado == 'INF' or elemento_limpiado == '-1':  # Tratar '-1' como infinito
                    fila_numerica.append(float('inf'))
                else:
                    fila_numerica.append(float(elemento))
            matriz.append(fila_numerica)
        return np.array(matriz)

# Cargar las matrices
matriz1 = cargar_matriz('./Output/ResultParalell_distancias.csv')
matriz2 = cargar_matriz('./Output/ResultSecuential_distancias.csv')
matriz3 = cargar_matriz('./Examples/1000x1000.csv')

# Verificar que las matrices tengan el mismo tamaño
if matriz1.shape != matriz2.shape or matriz1.shape != matriz3.shape or matriz2.shape != matriz3.shape:
    print("Las matrices deben tener el mismo tamaño.")
    print(f"Sequential shape: {matriz1.shape}")
    print(f"Parallel shape: {matriz2.shape}")
    print(f"Inpt shape: {matriz3.shape}")
else:
    # Comparar las matrices elemento por elemento
    resultado = np.array_equal(matriz1, matriz2)  # Corregido para comparar igualdad de matrices
    print(f"Sequential and parallel equals: {resultado}")

    resultadobool = matriz3 >= matriz1
    resultado = np.all(matriz3 >= matriz1)  # Corregido para verificar si todos los elementos cumplen la condición
    print(f"Valid Result: {resultado}")
    ## print(resultadobool)

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
matriz3 = cargar_matriz('./Examples/512.csv')

# Verificar que las matrices tengan el mismo tamaño
if matriz1.shape != matriz2.shape or matriz1.shape != matriz3.shape or matriz2.shape != matriz3.shape:
    print("Las matrices deben tener el mismo tamaño.")
    print(f"Sequential shape: {matriz1.shape}")
    print(f"Parallel shape: {matriz2.shape}")
    print(f"Input shape: {matriz3.shape}")
else:
    # Comparar las matrices elemento por elemento para igualdad
    resultado = np.array_equal(matriz1, matriz2)
    print(f"Sequential and parallel equals: {resultado}")

    # Identificar elementos en matriz3 que no son mayores o iguales a los correspondientes en matriz1
    elementos_no_validos = np.where(matriz3 < matriz1)
    # elementor_no_validos es una tupla de arrays, cada uno representando las coordenadas de los elementos falsos en sus respectivas dimensiones

    # Verificar si todos los elementos de matriz3 son mayores o iguales a los de matriz1
    resultado = np.all(matriz3 >= matriz1)
    print(f"Valid Result: {resultado}")

    if not resultado:
        # Imprimir las coordenadas de los elementos falsos
        print("Coordenadas de los elementos no válidos (donde matriz3 < matriz1):")
        for coordenada in zip(*elementos_no_validos):
            print(coordenada)

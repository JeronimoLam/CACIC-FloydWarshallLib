import csv
import numpy as np
import sys

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
    

    
# Ensure there's a command-line argument provided
if len(sys.argv) != 2:
    print("Usage: python CheckCorrect.py <filename_without_extension>")
    sys.exit(1)

filename = sys.argv[1]  # Get the filename without extension from the command-line argument

# Cargar las matrices de distancias
matriz1 = cargar_matriz('./Output/ResultParalell_distancias.csv')
matriz2 = cargar_matriz('./Output/ResultSecuential_distancias.csv')
matriz3 = cargar_matriz(f'./Examples/{filename}.csv')

# Cargar las matrices de caminos
matriz_caminos1 = cargar_matriz('./Output/ResultParalell_caminos.csv')
matriz_caminos2 = cargar_matriz('./Output/ResultSecuential_caminos.csv')

print("")

# Comparar las matrices de distancias
resultado_distancias = np.array_equal(matriz1, matriz2)
print(f"DISTANCIAS Match: {resultado_distancias}")

# Comparar las matrices de caminos
resultado_caminos = np.array_equal(matriz_caminos1, matriz_caminos2)
print(f"CAMINOS Match: {resultado_caminos}")

# Continúa con la comparación de las matrices de distancias con la matriz de entrada, como lo estabas haciendo


if matriz1.shape != matriz3.shape: 
    print("Las matrices deben tener el mismo tamaño.")
    sys.exit(1)

# Identificar elementos en matriz3 que no son mayores o iguales a los correspondientes en matriz1
elementos_no_validos = np.where(matriz3 < matriz1)
resultado = np.all(matriz3 >= matriz1)
print(f"Valid Result: {resultado}")

if not resultado:
    # Imprimir las coordenadas de los elementos falsos
    print("Coordenadas de los elementos no válidos (donde matriz3 < matriz1):")
    for coordenada in zip(*elementos_no_validos):
        print(coordenada)


print("")
print(f"Paralell shape: {matriz1.shape}")
print(f"Sequential shape: {matriz2.shape}")
print(f"Input shape: {matriz3.shape}")

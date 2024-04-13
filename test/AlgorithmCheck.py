import sys
import csv
import json
import numpy as np

def read_matrix(file_path):
    if file_path.endswith('.csv'):
        return read_csv(file_path)
    elif file_path.endswith('.json'):
        return read_json(file_path)
    else:
        raise ValueError("Unsupported file format")

def read_csv(file_path):
    with open(file_path, mode='r', newline='') as file:
        reader = csv.reader(file)
        return [list(map(standardize_value, row)) for row in reader]

def read_json(file_path):
    with open(file_path, 'r') as file:
        data = json.load(file)
        return [list(map(standardize_value, row)) for row in data['matrix']]

def standardize_value(value):
    if value in ("-1", "INF", "\"INF\""):
        return float('inf')
    return float(value)

def floyd_warshall(matrix):
    n = len(matrix)
    dist = np.array(matrix)
    for k in range(n):
        for i in range(n):
            for j in range(n):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
    return dist

def matrices_are_equal(mat1, mat2):
    return np.allclose(mat1, mat2, atol=1e-8, equal_nan=True)

def main(file_path1, file_path2):
    try:
        matrix1 = read_matrix(file_path1)
        matrix2 = read_matrix(file_path2)

        result_matrix = floyd_warshall(matrix1)
        if matrices_are_equal(result_matrix, matrix2):
            print("Las matrices son iguales.")
        else:
            print("Las matrices no son iguales.")

    except Exception as e:
        print(f"Error: {str(e)}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <path_to_matrix1> <path_to_matrix2>")
    else:
        main(sys.argv[1], sys.argv[2])

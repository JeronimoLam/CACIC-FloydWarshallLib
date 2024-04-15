import sys
import csv
import json

def read_matrix_csv(file_path):
    """Reads a matrix from a CSV file and standardizes its values."""
    matrix = []
    with open(file_path, mode='r', newline='') as file:
        reader = csv.reader(file)
        for row in reader:
            matrix.append([standardize_value(value) for value in row])
    return matrix

def read_matrix_json(file_path):
    """Reads a matrix from a JSON file under the 'matrix' key and standardizes its values."""
    with open(file_path, 'r') as file:
        data = json.load(file)
        matrix = data['matrix']
        return [[standardize_value(value) for value in row] for row in matrix]

def standardize_value(value):
    """Standardizes the matrix values by converting 'INF', '"INF"', and '-1' to -1 and converting all other values to float."""
    if value in ("-1", "INF", "\"INF\""):
        return -1
    return float(value)

def compare_matrices(matrix1, matrix2):
    """Compares two matrices element by element and reports differences."""
    if len(matrix1) != len(matrix2):
        print("Matrices are of different sizes.")
        return False
    differences = []
    for i in range(len(matrix1)):
        if len(matrix1[i]) != len(matrix2[i]):
            print(f"Row {i} is of different length.")
            return False
        for j in range(len(matrix1[i])):
            if matrix1[i][j] != matrix2[i][j]:
                differences.append((i, j, matrix1[i][j], matrix2[i][j]))
    if differences:
        print("Matrices are not equal. Differences found at:")
        for diff in differences:
            print(f"Row {diff[0]}, Column {diff[1]}: {diff[2]} != {diff[3]}")
        return False
    return True

def main(file_path_1, file_path_2):
    """Main function that reads two files and compares their matrices."""
    # Read the first file
    if file_path_1.endswith('.csv'):
        matrix1 = read_matrix_csv(file_path_1)
    elif file_path_1.endswith('.json'):
        matrix1 = read_matrix_json(file_path_1)
    else:
        print("The format of the first file is not supported.")
        return
    
    # Read the second file
    if file_path_2.endswith('.csv'):
        matrix2 = read_matrix_csv(file_path_2)
    elif file_path_2.endswith('.json'):
        matrix2 = read_matrix_json(file_path_2)
    else:
        print("The format of the second file is not supported.")
        return
    
    # Compare the matrices
    if compare_matrices(matrix1, matrix2):
        print("The matrices are equal.")
    else:
        print("The matrices are not equal.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <full_path_to_file_1> <full_path_to_file_2>")
    else:
        main(sys.argv[1], sys.argv[2])

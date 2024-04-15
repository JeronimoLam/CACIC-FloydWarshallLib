import sys
import csv
import json

def read_csv(file_path):
    """Reads a CSV file and returns its matrix, trimming spaces from values."""
    with open(file_path, mode='r', newline='') as file:
        reader = csv.reader(file)
        return [[standardize_value(value.strip()) for value in row] for row in reader]

def read_json(file_path):
    """Reads a JSON file and returns its matrix along with the type and size, handling spaces in values."""
    with open(file_path, 'r') as file:
        data = json.load(file)
        matrix = [[standardize_value(str(value).strip()) for value in row] for row in data['matrix']]
        matrix_type = data['type']
        size = data['size']
        return matrix, matrix_type, size

def standardize_value(value):
    """Converts value to appropriate numeric type or keeps special markers as strings."""
    if value == "INF" or value == "\"INF\"":
        return "INF"
    try:
        # Convert to integer or float if possible
        return float(value) if '.' in value else int(value)
    except ValueError:
        return value

def check_diagonal_zeros(matrix):
    """Checks if all diagonal elements are zero."""
    return all(matrix[i][i] == 0 for i in range(len(matrix)))

def check_positivity_except(matrix):
    """Checks all values are positive, -1, or 'INF', and prints error positions if other negative values are found."""
    valid = True
    for i, row in enumerate(matrix):
        for j, value in enumerate(row):
            # Check if the value is not INF, not -1, and is a negative number
            if value != "INF" and value != -1 and (isinstance(value, (int, float)) and value < 0):
                print(f"Error: Invalid negative value {value} at position ({i}, {j}).")
                valid = False
    return valid



def check_matrix_square_and_size(matrix, expected_size=None):
    """Checks if the matrix is square and matches the expected size, if provided."""
    n = len(matrix)
    if expected_size is not None and n != expected_size:
        return False
    return all(len(row) == n for row in matrix)

def check_int_type_no_decimals(matrix):
    """Ensures all elements are integers with no decimal parts."""
    return all(all(isinstance(x, int) for x in row) for row in matrix)

def validate_matrix(file_path):
    if file_path.endswith('.csv'):
        matrix = read_csv(file_path)
    elif file_path.endswith('.json'):
        matrix, matrix_type, size = read_json(file_path)
        if not check_matrix_square_and_size(matrix, size):
            print("Error: JSON matrix does not match specified size or is not square.")
            return
        if matrix_type == 'int' and not check_int_type_no_decimals(matrix):
            print("Error: JSON specifies type 'int' but contains decimal numbers.")
            return
    else:
        print("Unsupported file format.")
        return
    
    # Checking diagonal zeros
    if not check_diagonal_zeros(matrix):
        print("Error: Not all diagonal elements are zero.")
    else:
        print("Diagonal zero check passed.")
    
    # Checking positivity except allowed negative values
    if not check_positivity_except(matrix):
        print("Error: Matrix contains negative values not allowed.")
    else:
        print("Positivity check passed.")
    
    # Checking if matrix is square
    if not check_matrix_square_and_size(matrix):
        print("Error: Matrix is not square.")
    else:
        print("Matrix square and size check passed.")

    # Final validation
    if check_diagonal_zeros(matrix) and check_positivity_except(matrix) and check_matrix_square_and_size(matrix):
        print("Matrix validation successful.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <path_to_file>")
    else:
        validate_matrix(sys.argv[1])

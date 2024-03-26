import csv
import sys

def check_csv_matrix_and_max_row_length(csv_file_path, maxValue):
    with open(csv_file_path, 'r', newline='') as csvfile:
        matrix_reader = csv.reader(csvfile, delimiter=',')
        matrix = list(matrix_reader)
        n = len(matrix)
        
        # Find the row with the maximum length
        max_row_length = 0
        max_row_index = 0
        for index, row in enumerate(matrix):
            row_length = sum(len(item) for item in row) + (len(row) - 1)  # Adding commas back
            if row_length > max_row_length:
                max_row_length = row_length
                max_row_index = index
        
        print(f'Maximum row length: {max_row_length}, at row: {max_row_index+1}')
        
        for i in range(n):
            for j in range(len(matrix[i])): # Adjusted to iterate based on current row's length
                value = int(matrix[i][j])
                
                # Check diagonal elements are 0
                if i == j and value != 0:
                    print(f'Error: Diagonal element at row {i+1}, column {j+1} is not 0, it is {value}.')
                    return False
                
                # Check off-diagonal elements are within [-1, maxValue]
                if i != j and not(-1 <= value <= maxValue):
                    print(f'Error: Element at row {i+1}, column {j+1} is not within [-1, {maxValue}], it is {value}.')
                    return False
                    
    return True

# Ensure there's a command-line argument provided
if len(sys.argv) != 2:
    print("Usage: python CheckCorrect.py <filename_without_extension>")
    sys.exit(1)

filename = sys.argv[1]  # Get the filename without extension from the command-line argument
# Example usage
if __name__ == '__main__':
    csv_file_path = f'./Examples/{filename}.csv'
    maxValue = 100 # Example maxValue used during matrix generation
    is_correct = check_csv_matrix_and_max_row_length(csv_file_path, maxValue)
    print(f'Matrix in CSV is correct: {is_correct}')

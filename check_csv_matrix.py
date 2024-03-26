
import csv

def check_csv_matrix_and_first_line_length(csv_file_path, maxValue):
    with open(csv_file_path, 'r', newline='') as csvfile:
        matrix_reader = csv.reader(csvfile, delimiter=',')
        matrix = list(matrix_reader)
        n = len(matrix)
        
        # Count the length of the first line (including digits and commas)
        first_line_length = sum(len(item) for item in matrix[0]) + (n - 1)  # Adding commas back
        print(f'Length of the first line (characters, digits, and commas): {first_line_length}')
        
        for i in range(n):
            for j in range(n):
                value = int(matrix[i][j])
                
                # Check diagonal elements are 0
                if i == j and value != 0:
                    return False
                
                # Check off-diagonal elements are within [-1, maxValue]
                if i != j and not(-1 <= value <= maxValue):
                    return False
                    
    return True

# Example usage
if __name__ == '__main__':
    csv_file_path = 'path_to_your_csv_file.csv'
    maxValue = 10 # Example maxValue used during matrix generation
    is_correct = check_csv_matrix_and_first_line_length(csv_file_path, maxValue)
    print(f'Matrix in CSV is correct: {is_correct}')

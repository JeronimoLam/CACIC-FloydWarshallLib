import ctypes

# Load the shared library
lib_path = os.path.join(os.path.dirname(__file__), 'libFloydWarshall_dynamic.so')
lib = ctypes.CDLL(lib_path)


# Define data types
class FW_Matrix(ctypes.Structure):
    _fields_ = [
        ('dist', ctypes.c_void_p),
        ('path', ctypes.POINTER(ctypes.c_int)),
        ('fileType', ctypes.c_int),  # Assuming FileType is an enum mapped to int
        ('decimal_length', ctypes.c_uint),
        ('datatype', ctypes.c_int),  # Assuming DataType is an enum mapped to int
        ('size', ctypes.c_uint),
        ('norm_size', ctypes.c_uint),
        ('BS', ctypes.c_uint)
    ]

class FW_attr_t(ctypes.Structure):
    _fields_ = [
        ('text_in_output', ctypes.c_uint),
        ('print_distance_matrix', ctypes.c_uint),
        ('no_path', ctypes.c_uint),
        ('thread_num', ctypes.c_uint)
    ]

# Map the create_structure function
lib.create_structure.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(FW_attr_t)]
lib.create_structure.restype = FW_Matrix

# Example usage
if __name__ == "__main__":
    attr = FW_attr_t(1, 1, 0, 4)  # Sample attributes
    matrix = lib.create_structure(1, b"matrix.csv", 2, ctypes.byref(attr))  # Example call

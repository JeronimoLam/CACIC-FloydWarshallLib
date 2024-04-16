import ctypes
from ctypes import Structure, POINTER, c_int, c_uint, c_void_p, c_char_p

# Define the data types as specified in the C headers
class DataType(ctypes.c_int):
    TYPE_INT = 0
    TYPE_FLOAT = 1
    TYPE_DOUBLE = 2
    UNDEFINED = 3

class FileType(ctypes.c_int):
    CSV = 0
    JSON = 1

class FW_Matrix(Structure):
    _fields_ = [("dist", c_void_p),
                ("path", POINTER(c_int)),
                ("fileType", FileType),
                ("decimal_length", c_uint),
                ("datatype", DataType),
                ("size", c_uint),
                ("norm_size", c_uint),
                ("BS", c_uint)]

class FW_attr_t(Structure):
    _fields_ = [("text_in_output", c_uint),
                ("print_distance_matrix", c_uint),
                ("no_path", c_uint),
                ("thread_num", c_uint)]

# Load the shared library
lib = ctypes.CDLL('./FloydWarshall_dynamic.dll')

# Specify argument and return types for functions
lib.create_structure.argtypes = [DataType, c_char_p, c_int, POINTER(FW_attr_t)]
lib.create_structure.restype = FW_Matrix

lib.compute_FW_paralell.argtypes = [FW_Matrix, POINTER(FW_attr_t)]
lib.compute_FW_paralell.restype = None

lib.compute_FW_sequential.argtypes = [FW_Matrix, POINTER(FW_attr_t)]
lib.compute_FW_sequential.restype = None

lib.save_structure.argtypes = [FW_Matrix, c_char_p, c_char_p, FileType, POINTER(FW_attr_t)]
lib.save_structure.restype = None

lib.free_FW_Matrix.argtypes = [POINTER(FW_Matrix)]
lib.free_FW_Matrix.restype = None

lib.FW_details_to_string.argtypes = [POINTER(FW_Matrix), POINTER(FW_attr_t)]
lib.FW_details_to_string.restype = c_char_p

lib.print_FW_matrixes.argtypes = [POINTER(FW_Matrix), c_char_p, c_int]
lib.print_FW_matrixes.restype = None

lib.new_FW_attr.restype = FW_attr_t
lib.init_FW_attr.argtypes = [POINTER(FW_attr_t)]
lib.init_FW_attr.restype = None

lib.get_FW_creation_time.restype = ctypes.c_double
lib.get_FW_processing_time.restype = ctypes.c_double
lib.get_FW_save_time.restype = ctypes.c_double
lib.get_FW_total_time.restype = ctypes.c_double

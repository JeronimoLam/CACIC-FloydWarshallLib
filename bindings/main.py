import ctypes
from bindings.python_binding import lib, DataType, FW_attr_t, FW_Matrix

def main():
    # Create default attributes
    attr = lib.new_FW_attr()
    lib.init_FW_attr(ctypes.byref(attr))

    # Create a structure
    fw_matrix = lib.create_structure(DataType.TYPE_INT, b"path_to_data_file", 64, ctypes.byref(attr))
    
    # Use the structure
    lib.compute_FW_paralell(fw_matrix, ctypes.byref(attr))

    # Convert details to string and print
    details = lib.FW_details_to_string(ctypes.byref(fw_matrix), ctypes.byref(attr))
    print(ctypes.string_at(details))

    # Free the matrix
    lib.free_FW_Matrix(ctypes.byref(fw_matrix))

if __name__ == "__main__":
    main()

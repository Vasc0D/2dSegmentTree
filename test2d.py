from segment_tree import SegmentTree2D
import numpy as np

def test_segment_tree_2d():
    # Crear una matriz de prueba
    test_matrix = [
        [1, 2, 3, 4],
        [5, 6, 7, 8],
        [9, 10, 11, 12],
        [13, 14, 15, 16]
    ]
    
    # Inicializar el SegmentTree2D
    st = SegmentTree2D(test_matrix)
    
    # Prueba 1: Suma de toda la matriz
    total_sum = st.sum(0, 3, 0, 3)
    expected_sum = sum(sum(row) for row in test_matrix)
    print(f"Prueba 1 - Suma total: Obtenido {total_sum}, Esperado {expected_sum}")
    assert total_sum == expected_sum, "La suma total no coincide"
    
    # Prueba 2: Suma de una submatriz
    submatrix_sum = st.sum(1, 2, 1, 2)
    expected_subsum = 6 + 7 + 10 + 11
    print(f"Prueba 2 - Suma de submatriz: Obtenido {submatrix_sum}, Esperado {expected_subsum}")
    assert submatrix_sum == expected_subsum, "La suma de la submatriz no coincide"
    
    # Prueba 3: Actualización de un valor
    st.update(1, 1, 100)
    updated_sum = st.sum(1, 1, 1, 1)
    print(f"Prueba 3 - Valor actualizado: Obtenido {updated_sum}, Esperado 100")
    assert updated_sum == 100, "La actualización no se realizó correctamente"
    
    # Prueba 4: Suma después de la actualización
    new_total_sum = st.sum(0, 3, 0, 3)
    expected_new_sum = expected_sum - 6 + 100
    print(f"Prueba 4 - Nueva suma total: Obtenido {new_total_sum}, Esperado {expected_new_sum}")
    assert new_total_sum == expected_new_sum, "La nueva suma total después de la actualización no coincide"
    
    # Prueba 5: Suma de una fila
    row_sum = st.sum(2, 2, 0, 3)
    expected_row_sum = 9 + 10 + 11 + 12
    print(f"Prueba 5 - Suma de fila: Obtenido {row_sum}, Esperado {expected_row_sum}")
    assert row_sum == expected_row_sum, "La suma de la fila no coincide"
    
    # Prueba 6: Suma de una columna
    col_sum = st.sum(0, 3, 1, 1)
    expected_col_sum = 2 + 100 + 10 + 14
    print(f"Prueba 6 - Suma de columna: Obtenido {col_sum}, Esperado {expected_col_sum}")
    assert col_sum == expected_col_sum, "La suma de la columna no coincide"
    
    print("Todas las pruebas pasaron exitosamente!")

# Ejecutar las pruebas
test_segment_tree_2d()
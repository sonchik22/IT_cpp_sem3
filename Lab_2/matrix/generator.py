import numpy as np

def generate_and_solve(size=5):    #Генерирует случайную матрицу size x size, считает её детерминант.
    matrix_np = np.random.randint(-10, 10, (size, size))
    det = np.linalg.det(matrix_np)
    matrix_list = matrix_np.tolist()
    
    return matrix_list, det  #кортеж: матрица_списком, детерминант

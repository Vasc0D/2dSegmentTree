import matplotlib.pyplot as plt

# Nuevos Datos para KD-Tree, QuadTree, y SegmentTree2D
sizes = [10, 100, 1000, 10000]

# Datos KD-Tree
build_times_kdtree = [0.002348, 0.022055, 0.22119, 1.81346]
build_memory_kdtree = [1001608, 1060044, 1658606, 7585792]
query_times_kdtree = [0.00013, 0.000275, 0.000339, 0.000255]
query_memory_kdtree = [1001608, 1060044, 1658606, 7585792]

# Datos QuadTree
build_times_quadtree = [0.011102, 0.097377, 0.753194, 5.02678]
build_memory_quadtree = [1131042, 2388241, 13242641, 103546333]
query_times_quadtree = [8.8e-05, 8.4e-05, 0.000102, 8e-05]
query_memory_quadtree = [1131042, 2388241, 13242641, 103546333]

# Datos SegmentTree2D
build_times_segmenttree = [0.055218, 0.675459, 28.985, 3060.49]
build_memory_segmenttree = [1081344, 4734976, 92078080, 6338248704]
query_times_segmenttree = [0.000118, 0.000119, 7.7e-05, 0.000155]
query_memory_segmenttree = [1081344, 4734976, 92078080, 6338248704]

# Configurar estilo
plt.style.use('ggplot')

# Gráfico de tiempos de construcción
plt.figure(figsize=(14, 7))
plt.plot(sizes, build_times_kdtree, marker='o', label='KDTree', linestyle='-', color='blue')
plt.plot(sizes, build_times_quadtree, marker='o', label='QuadTree', linestyle='-', color='orange')
plt.plot(sizes, build_times_segmenttree, marker='o', label='SegmentTree2D', linestyle='-', color='green')
plt.xlabel('Number of Points/Nodes')
plt.ylabel('Build Time (ms)')
plt.title('Build Time Comparison')
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.xscale('log')
plt.yscale('log')
plt.savefig('build_time_comparison.png')
plt.show()

# Gráfico de uso de memoria en construcción
plt.figure(figsize=(14, 7))
plt.plot(sizes, build_memory_kdtree, marker='o', label='KDTree', linestyle='-', color='blue')
plt.plot(sizes, build_memory_quadtree, marker='o', label='QuadTree', linestyle='-', color='orange')
plt.plot(sizes, build_memory_segmenttree, marker='o', label='SegmentTree2D', linestyle='-', color='green')
plt.xlabel('Number of Points/Nodes')
plt.ylabel('Build Memory (KB)')
plt.title('Build Memory Usage Comparison')
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.xscale('log')
plt.yscale('log')
plt.savefig('build_memory_comparison.png')
plt.show()

# Gráfico de tiempos de consulta
plt.figure(figsize=(14, 7))
plt.plot(sizes, query_times_kdtree, marker='o', label='KDTree', linestyle='-', color='blue')
plt.plot(sizes, query_times_quadtree, marker='o', label='QuadTree', linestyle='-', color='orange')
plt.plot(sizes, query_times_segmenttree, marker='o', label='SegmentTree2D', linestyle='-', color='green')
plt.xlabel('Number of Points/Nodes')
plt.ylabel('Query Time (ms)')
plt.title('Query Time Comparison')
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.xscale('log')
plt.yscale('log')
plt.savefig('query_time_comparison.png')
plt.show()

# Gráfico de uso de memoria en consulta
plt.figure(figsize=(14, 7))
plt.plot(sizes, query_memory_kdtree, marker='o', label='KDTree', linestyle='-', color='blue')
plt.plot(sizes, query_memory_quadtree, marker='o', label='QuadTree', linestyle='-', color='orange')
plt.plot(sizes, query_memory_segmenttree, marker='o', label='SegmentTree2D', linestyle='-', color='green')
plt.xlabel('Number of Points/Nodes')
plt.ylabel('Query Memory (KB)')
plt.title('Query Memory Usage Comparison')
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.xscale('log')
plt.yscale('log')
plt.savefig('query_memory_comparison.png')
plt.show()

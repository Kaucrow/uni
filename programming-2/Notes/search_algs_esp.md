# Algoritmos de Busqueda

En este documento estan detalladas las caracteristicas de cada uno de los algoritmos de busqueda basicos, junto a una explicacion sencilla del funcionamiento de cada algoritmo. Cada algoritmo tiene un enlace a un articulo donde se encuentra una implementacion en distintos lenguajes incluyendo C++, junto a una explicacion mas detallada del mismo.

Se utilizaran los conceptos basicos de algoritmos, detallados en el archivo de [algoritmos de ordenamiento](./sorting_algs_esp.md).

## [Linear Search](https://www.geeksforgeeks.org/linear-search/)

* **Complejidad de tiempo:**
    * Mejor caso: O(1), si la busqueda esta en el primer indice de la lista.
    * Caso promedio: O(N)
    * Peor caso: O(N), si la busqueda esta en el ultimo indice de la lista.
* **Espacio auxiliar:** O(1)

### Ventajas
* Puede ser utilizado independientemente de si la lista esta ordenada o no.
* No requiere memoria adicional.
* Es util para listas de pocos elementos.

### Desventajas:
* Muy ineficiente para listas de muchos elementos.

### Funcionamiento
El funcionamiento de Linear Search es muy sencillo. Este algoritmo simplemente compara cada elemento de la lista con el argumento de busqueda, y si el elemento es igual a lo que se esta buscando, devuelve su posicion. Si llega al final de la lista sin encontrar un elemento que coincida con la busqueda, devuelve -1.

## [Binary Search](https://www.geeksforgeeks.org/binary-search/)

* **Complejidad de tiempo:**
    * Mejor caso: O(1)
    * Caso promedio: O(log(N))
    * Peor caso: O(log(N))
* **Espacio auxiliar:** O(1)

### Ventajas
* Mas eficiente que Linear Search, en especial para listas grandes.
* Mas eficiente que otros algoritmos de busqueda con un tiempo de complejidad similar.

### Desventajas
* La lista debe estar ordenada para poder usar este algoritmo.

### Funcionamiento
1. Se toma el elemento en la mitad de la lista, y la lista se divide en una porcion a la izquierda del elemento y una porcion a la derecha del elemnto.
2. Se compara la busqueda con el elemento en la mitad de la lista. Si la busqueda es *mayor* al elemento, se repite el mismo proceso desde el paso anterior, pero ahora utilizando la porcion derecha de la lista. Si la busqueda es *menor* al elemento, se repite el mismo proceso desde el paso anterior, utilizando la porcion izquierda de la lista. Si la busqueda no es menor ni mayor al elemento de la mitad, entonces es igual, lo cual quiere decir que se halló el elemento que se buscaba.

## [Jump Search](https://www.geeksforgeeks.org/jump-search/)

* **Complejidad de tiempo:**
    * Mejor caso: O(1)
    * Caso promedio: O(sqrt(N))
    * Peor caso: O(sqrt(N))
* **Espacio auxiliar:** O(1)

### Ventajas
* Mejor que Linear Search para listas en las que los elementos estan uniformemente distribuidos (es decir, no hay una gran diferencia entre el valor de cada elemento).
* Tiene menor complejidad de tiempo comparado a Linear Search para listas grandes.
* Es facil de implementar.
* Binary Search es mejor que Jump Search, pero en situaciones en las que el elemento que se busca es esta cerca del menor o mayor de la lista, es preferible usar Jump Search.

### Desventajas
* Solo funciona con listas ordenadas.

### Funcionamiento
Supongamos que tenemos una lista: (0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610). Hay 16 elementos. Jump Search intentara hallar el valor 55, asumiendo que la cantidad de elementos a saltar es sqrt(16), es decir, 4.

1. Salta del indice 0 al indice 4.
2. Salta del indice 4 al indice 8.
3. Salta del indice 8 al indice 12.
4. Ya que el elemento en el indice 12 (144) es menor que 55, saltaremos un paso atras para volver al indice 8.
5. Se realiza una Linear Search desde el indice 8 para hallar el indice 55.

<br>

<p align="center">
   Gracias por leer :)
</p>

<br>

<p align="center">
   Necesito -->
   <img src="./images/its-dangerous-to-go-alone.jpg" height=250></img>
</p>

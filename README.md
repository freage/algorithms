algorithms
===========
Incomplete collection of algorithms I have implemented.

Number theory
--------------

### General ###
* GCD - greatest common divisor - [utils.py](number-theory/utils.py) 
* EGCD - extended Euclidean algorithm - [utils.py](number-theory/utils.py)
* modular inverse - [utils.py](number-theory/utils.py)
* modular exponentiation (square-and-multiply) - [utils.py](number-theory/utils.py)

### Primes and factorisation
* Miller Rabin primality checking [millerrabin.py](number-theory/millerrabin.py)
* Elliptic curves factorisation (Lenstra's algorithm) 
    * in Python [ecpoints.py](number-theory/ecpoints.py), [lenstra.py](number-theory/lenstra.py)
    * in C++


Graph theory
------

### Minimal spanning tree (MST)
Kruskal's algorithm (union-find with disjoint sets)

* in Python: [kruskal.py](graph-theory/kruskal.py) and used in Kattis-problem [cats.py](graph-theory/cats.py)
* in C++: [mst.hpp](graph-theory/tsp/mst.hpp) and [mst.cpp](graph-theory/tsp/mst.cpp)

Prim's algorithm (with a priority queue)

* in C++: [mst.hpp](graph-theory/tsp/mst.hpp) and [mst.cpp](graph-theory/tsp/mst.cpp)

The MST solver can be used to implement approxmation solvers for the Travelling Salesman Problem (TSP).

* A simple 2-APX solver of TSP can be done based on MST: [mst_heuristic.hpp](graph-theory/tsp/mst_heuristic.hpp) and [mst_heuristic.cpp](graph-theory/tsp/mst_heuristic.cpp). 
* One can also implement Christofides algorithm (here a simplified version with greedy matching) [christofides.hpp](graph-theory/tsp/christofides.hpp) and [christofides.cpp](graph-theory/tsp/christofides.cpp).


Cryptography
-----

### Advanced Encryption Standard (AES)
An implementation of the symmetric-key encryption algorithm AES in C++: [aes.cpp](number-theory/aes/aes.cpp) and in Python: [aes_final.py](number-theory/aes/aes_final.py) as well as the maths that it is founded on, the Rijndael Galois Field [aes_gfscalar.py](number-theory/aes/aes_gfscalar.py). The Python solution was improved with a profiler and then rewritten into C++.


Data structures
--------------
Vector in C++, other things in Python?

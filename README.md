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

### More specific
* Miller Rabin primality checking [millerrabin.py](number-theory/millerrabin.py)
* Elliptic curves factorisation (Lenstra's algorithm) 
    * in Python [ecpoints.py](number-theory/ecpoints.py), [lenstra.py](number-theory/lenstra.py)
    * in C++

Graph theory
------

### Minimal spanning tree (MST)
Kruskal's algorithm (union-find with disjoint sets)

* in Python: [kruskal.py](graph-theory/kruskal.py) and used in Kattis-problem [cats.py](graph-theory/cats.py)
* in C++: [mst.hpp](graph-theory/mst.hpp) and [mst.cpp](graph-theory/mst.cpp)

Prim's algorithm (with a priority queue)

* in C++: [mst.hpp](graph-theory/mst.hpp) and [mst.cpp](graph-theory/mst.cpp)

An 2-APX solver of the Travelling Salesman Problem (TSP) can be done based on MST: [mst_heuristic.hpp](graph-theory/mst_heuristic.hpp) and [mst_heuristic.cpp](graph-theory/mst_heuristic.cpp). (Taken from a course in advanced algorithms where we implemented several TSP approxmation solvers.)


Other
-----

### AES
AES implemented in C++.


Data structures
--------------
Vector in C++, other things in Python?

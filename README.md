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
* in C++

Prim's algorithm (with a priority queue)

* in C++


Other
-----

### AES
AES implemented in C++.


Data structures
--------------
Vector in C++, other things in Python?

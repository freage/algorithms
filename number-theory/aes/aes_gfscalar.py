"""
///////////////////// The Rijndael Galois field //////////////////////////
Calculate the table used in AES.
"""

from sympy import Poly, half_gcdex, symbols

x = symbols('x')

def poly2int(p):
    return vector2int(poly2list(p))

def list2poly(li):
    p = sum(c*x**i for i,c in enumerate(li))
    return Poly(p, x, modulus=2)

def poly2list(p):
    if p == 0:
        return [0]
    return Poly.all_coeffs(p)

def vector2int(byte):
    return int("0b"+"".join(map(str,byte)),2)

def int2vector(byte):
    return [ (byte&(1<<i))>>i for i in range(8) ][::-1]


class gfscalar:
    def __init__(self, c):
        if isinstance(c, int):
            self.c = c
        elif isinstance(c, list):
            self.c = vector2int(c)
        elif isinstance(c, Poly):
            self.c = poly2int(c)
        else:
            raise TypeError()
        
    quotient = Poly(x**8 + x**4 + x**3 + x + 1, modulus=2)

    def __add__(self, other):
        s = self.c^other.c
        return gfscalar(s)

    def __mul__(self, other):
        a = self.poly()
        b = other.poly()
        s = (a * b) % gfscalar.quotient
        return gfscalar(poly2int(s))

    def __int__(self):
        return self.c

    def __hex__(self):
        return hex(self.c)

    def vector(self):
        return int2vector(self.c)
        
    def poly(self):
        return list2poly(self.vector()[::-1])

    def inv(self):
        p = self.poly()
        """solve a*p + k*q = 1  ==>  a is out"""
        """s*f = h (mod g)    <==> s*f + _ * g = 1"""
        if p == 0:
            return 0
        a, one = half_gcdex(p, gfscalar.quotient)
        assert one == 1
        return gfscalar(a)

    def __pow__(self, n):
        if n == 0:
            return gfscalar(1)
        product = self + gfscalar(0)
        for _ in range(1,n):
            product *= self
        return product

    def __str__(self):
        return "gfscalar({})".format(self.c)

    def __repr__(self):
        return str(self)

    def bitparity(self):
        r = 0
        for i in range(8):
            r ^= (self.c&(1<<i))>>i
        return r

    def scalarprod(self, other):
        return gfscalar(self.c & other.c).bitparity()
 
    def reverse(self):
        return gfscalar(self.vector()[::-1])

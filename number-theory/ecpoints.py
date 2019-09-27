"""
Class for elliptic curve points.
"""
from utils import modinv, div

class ECpoint:
    """Class for point on the elliptic curve."""

    @staticmethod
    def set_modulus(p):
        ECpoint.p = p

    @staticmethod
    def set_curve(a,b):
        ECpoint.a = a
        ECpoint.b = b

        
    def __init__(self, x, y):
        """Invariant: 0 <= x, y < p"""
        self.x = x
        self.y = y

    def isInfty(self):
        """Check if point is infinity point."""
        return self.x == None and self.y == None

    def __eq__(self, rhs):
        """Equality, used in assertions."""
        return isinstance(rhs, ECpoint) and self.x == rhs.x and self.y == rhs.y

    def __add__(self, rhs):
        """Add two points.

        Following the description from 
        https://en.wikipedia.org/wiki/Elliptic_curve#The_group_law"""

        p = ECpoint.p
        a = ECpoint.a

        ## Addition of identity
        if rhs.isInfty():
            return ECpoint(self.x, self.y)
        elif self.isInfty():
            return ECpoint(rhs.x, rhs.y)

        ## Following notation from wikipedia
        xp, yp = self.x, self.y
        xq, yq = rhs.x, rhs.y
        if xp != xq:
            s = div((yp - yq),(xp - xq), p)
            xr = s**2 - xp - xq
            yr = s*(xp - xr) - yp 
            return ECpoint(xr % p,yr % p)
        # else if xp == xq, there are two cases
        elif yp == -yq:
            return ECpoint.infty()
        else:
            s = div((3*xp**2 + a),(yp<<1),p)
            xr = s**2 - (xp<<1)
            yr = s*(xp - xr) - yp 
            return ECpoint(xr % p,yr % p)

    def copy(self):
        """Return a copy of self."""
        return ECpoint(self.x, self.y)

    def __mul__(self, e):
        """
        Group exponentiation, as multiplication from the right.

        Fast group exponentiation, where the group operation is +. 
        Same as the `power` function.
        """
        a = self.copy()
        res = ECpoint.infty()
        while e > 1:
            if e % 2 == 1:
                res += a
            a += a
            e >>= 1
        if e == 1:
            return res+a
        else:
            return res

    def __rmul__(self, e):
        """
        Group exponentiation, as multiplication from the left.
        """
        return self.__mul__(e)
        
    def __str__(self):
        """
        String representation:
        Two space-separated hexadecimal numbers.
        """
        return "0x%x 0x%x" % (self.x, self.y)

    def __repr__(self):
        return "ECpoint(%x,%x)" % (self.x, self.y)

    @staticmethod
    def infty():
        return ECpoint(None, None)

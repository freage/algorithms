def power(a,e,n):
    """
    modular exponentiation:
    *************************
    Own version of `pow`: calculate
    a**e % n
    using the square-and-multiply algorithm.
    """
    res = 1
    while e > 1:
        if e % 2 == 1:
            res = (res*a)%n
        a = (a*a)% n
        e >>= 1
    if e == 1:
        return (res*a)%n
    else:
        return res

def gcd(a,b):
    """gcd of a and b"""
    if a == 0:
        return b
    else:
        return gcd(b%a, a)

def gcd_iter(a,b):
    while a > 0:
        a, b = b%a, a
    return b

def extgcd_iter(a,b):
    g = None
    quotients = []
    while a > 0:
        quotients.append(b//a)
        a, b = b%a, a
    g, y, x = b, 0, 1
    # print("length", len(quotients))
    for quotient in quotients[::-1]:
        y, x = x - quotient*y, y
    return g, y, x
        
    
def extgcd(a, b):
    """
    Extended gcd:
    ****************
    find g = gcd(a,b) and
    x, y such that
    ax + by = g
    """
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = extgcd(b % a, a)
        return (g, x - (b // a) * y, y)

class ModularInverseError(Exception):
    
    def __init__(self, a, n, gcd):
        super(ModularInverseError, self).__init__("modular inverse of %d (mod %d) does not exist" % (a,n))
        self.a = a
        self.n = n
        self.gcd = gcd

    def divisor(self):
        return self.gcd
    
def modinv(a, m):
    """
    Modular inverse of a mod m, where a > 0.
    """
    g, x, y = extgcd_iter(a, m)
    if g != 1:
        raise ModularInverseError(a,m,g)
    else:
        return x % m 


def div(x,y,p):
    """
    Divide x with y in Z_p.
    """
    if y < 0:
        return -x*modinv(-y,p)
    else:
        return x*modinv(y,p)

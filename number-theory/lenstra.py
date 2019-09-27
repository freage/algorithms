"""
Lenstra's elliptic curve factorisation.
"""

from random import randint
from ecpoints import ECpoint
from utils import ModularInverseError
from millerrabin import isprime_v2 as mr_isprime
# from gmpy import is_prime as mr_isprime
# from rosetta_mr import is_probable_prime as mr_isprime

def isprime(n):
    return None, mr_isprime(n)

def random_point(n):
    a,x,y = [ randint(0,n-1) for _ in range(3) ]
    b = (y**2 - x**3 - a*x) % n
    if (4*a**3 + 27*b**2)%n == 0:
        return random_point(n) # resample
    else:
        return x,y,a,b

class FailedToFactor(Exception):
    pass

def find_factor(n, attempts=100,B=100):
    ECpoint.set_modulus(n)
    for _ in range(attempts):
        x,y,a,b = random_point(n)
        P = ECpoint(x,y)
        ECpoint.set_curve(a,b)
        try:
            for b in range(1,B+1):
                P *= b
        except ModularInverseError as e:
            return abs(e.divisor())
    raise FailedToFactor("failed to factor %d" % n)
    return False

def factorise(n):
    """
    Factorise n. Note: the factors do not have to be primes.
    """
    factors = []
    # while n % 2 == 0:
    #     factors.append(2)
    #     n >>= 1
    failed = False
    while n > 1 and not failed:
        d = find_factor(n)
        if d == False:
            failed = True
        else:
            n //= d
            factors.append(d)
    if n > 1:
        factors.append(n)
    return factors

def true_factorise(n):
    factors = factorise(n)
    prime_factors = []
    i = 0
    while i < len(factors): # list grows
        factor = factors[i]
        divisor, is_prime = isprime(factor)
        if is_prime:
            prime_factors.append(factor)
            i += 1
        else:
            factors.pop(i)
            if divisor != None:
                factors.extend(factorise(divisor))
                factors.extend(factorise(factor//divisor))
            else:
                factors.extend(factorise(factor))
    return prime_factors


def true_factorise_v2(n):
    # print("trying to factorise %d" % n)
    # remove trivial factors
    prime_factors = []
    while n % 2 == 0:
        prime_factors.append(2)
        n >>= 1

    def inner(n):
        if mr_isprime(n):
            return [n]
        else:
           d = find_factor(n)
           assert(d != False)
           # print("found %d" % d)
           return inner(d) + inner(n//d)
    return prime_factors + inner(n)
            

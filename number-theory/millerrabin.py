from utils import gcd, power
from random import randint


def findtwopow(n):
    """
    decompose n = 2**twopow * r
    """
    r = n
    twopow = 0
    while r % 2 == 0:
        r >>= 1
        twopow += 1
    assert r*(1<<twopow) == n
    return r, twopow

def isprime(n, attempts=10):
    """
    Return (factor, isprime) where
    * isprime is whether n is prime
    * factor is a factor the algorithm might have found (no guarantees)
    """
    if n < 4:
        if n < 2: return None, False
        else: return None, True
    r, twopow = findtwopow(n-1)
    def find_witness():
        """
        Find a potential witness.
        Find a such that a**r % n != 1.
        Also return g = gcd(a,n).
        """
        a = 1
        while a == 1:
            a = randint(2,n-1)
            g = gcd(a,n)
            a = power(a,r,n)
        return a,g

    def itr():
        a,g = find_witness()
        if g != 1:
            return a,g
        prev = None
        #print(twopow)
        #print("chain: ")
        for _ in range(twopow):
            prev = a
            a = (a*a)%n
            #print(prev,a)
            if a == 1:
                return prev, g
        return None, None
            
    found = False
    factor = None
    for _ in range(attempts):    
        #print("start iteration")
        x, g = itr()
        if x == None and g == None:
            continue
        #print("end iteration")
        if g != 1:
            #print("returning g: %d" %g)
            found = True
            factor = g
            break
        if x != n-1:
            found = True
            a, b = x-1, x+1
            c, d = gcd(a,n), gcd(b,n)
            #print(c,d)
            if c != 1 and c != n:
                factor = c
            else:
                factor = d
                break
    return factor, not found


def isprime_v2(n, attempts=5):
    """
    Miller-Rabin primality checking.
    Return 
    * True if n probably is a prime
    * False if n is not a prime
    """
    if n < 4:
        if n < 2: return False
        else: return True
    r, twopow = findtwopow(n-1)

    def find_witness():
        """
        Try to find a witness for compositeness of n.
        (One attempt.)
        Returns True if successful.
        """
        a = randint(2,n-1)
        ar = power(a,r,n)
        if ar == 1 or ar == n-1:
            return False
        apow = ar
        print("chain")
        print("start: a**r = %d" % apow)
        for i in range(1,twopow):
            apow = (apow*apow)%n
            print("start: a**(r*%d) = %d" % (2**i, apow))
            if apow == n-1:
                return False
        return True


    for _ in range(attempts):
        if find_witness():
            return False

    return True


def test():
    primes_less_than_100 = [ n for n in range(100) if isprime(n) ]
    print(primes_less_than_100)

if __name__ == "__main__":
    test()
    

from random import randint

def findtwopow(n):
    """
    decompose n = 2**twopow * r, with r odd
    """
    r = n
    twopow = 0
    while r % 2 == 0:
        r //= 2
        twopow += 1
    assert r*(2**twopow) == n
    return r, twopow


def isprime(n, attempts=5):
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
        ar = pow(a,r,n)
        if ar == 1 or ar == n-1:
            return False
        apow = ar
        # print("chain")
        # print("start: a**r = %d" % apow)
        for i in range(1,twopow):
            apow = (apow*apow)%n
            # print("start: a**(r*%d) = %d" % (2**i, apow))
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
    

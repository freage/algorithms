from aes_gfscalar import x, gfscalar, int2vector, vector2int


#generate inverses
def gen_invs():
    d = {}
    for byte in range(1<<8): 
        gfs = gfscalar(byte)
        gfsinv = gfs.inv()
        d[byte] = int(gfsinv)
    return d


row = [1,0,0,0,1,1,1,1]
matrix = [ row[-i:] + row[:-i] for i in range(8) ]


def wrong_endianess_matrix_mult(matrix, byte):
    """swap endianess and multiply matrix * byte"""
    rev = byte.reverse()
    return gfscalar([ rev.scalarprod(gfscalar(row)) for row in matrix ]).reverse()


def transf(byte):
    b = gfscalar(byte)
    v = gfscalar(99)
    mult = wrong_endianess_matrix_mult(matrix, b)
    res = mult+v
    return int(res)

def at(byte):
    return transf(d[byte])

def gen_sbox():
    s = {i:at(i) for i in range(1<<8)}
    return s

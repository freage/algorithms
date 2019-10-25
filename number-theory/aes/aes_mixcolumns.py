from aes_gfscalar import gfscalar

def gen_mul_tables():
    global mul2, mul3
    mul2 = { i : int(gfscalar(i)*gfscalar(2)) for i in range(1<<8) }
    mul3 = { i : int(gfscalar(i)*gfscalar(3)) for i in range(1<<8) }

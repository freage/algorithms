# from aes_subbytes import sbox
# from aes_mixcolumns import mixcolumns
# from aes_addroundkey import roundkey, keyschedule
import sys

PROFILE = False 
#PROFILE = True 

if PROFILE:
    outputfile = open("foobar", "wb")

############################################## SUBBYTES #######################################################

sbox = [99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118, 202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192, 183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21, 4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117, 9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132, 83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207, 208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168, 81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210, 205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115, 96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219, 224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121, 231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8, 186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138, 112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158, 225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223, 140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22]


################################################ MIXCOLUMNS ##################################################

mul2 = [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 27, 25, 31, 29, 19, 17, 23, 21, 11, 9, 15, 13, 3, 1, 7, 5, 59, 57, 63, 61, 51, 49, 55, 53, 43, 41, 47, 45, 35, 33, 39, 37, 91, 89, 95, 93, 83, 81, 87, 85, 75, 73, 79, 77, 67, 65, 71, 69, 123, 121, 127, 125, 115, 113, 119, 117, 107, 105, 111, 109, 99, 97, 103, 101, 155, 153, 159, 157, 147, 145, 151, 149, 139, 137, 143, 141, 131, 129, 135, 133, 187, 185, 191, 189, 179, 177, 183, 181, 171, 169, 175, 173, 163, 161, 167, 165, 219, 217, 223, 221, 211, 209, 215, 213, 203, 201, 207, 205, 195, 193, 199, 197, 251, 249, 255, 253, 243, 241, 247, 245, 235, 233, 239, 237, 227, 225, 231, 229]


mul3 = [0, 3, 6, 5, 12, 15, 10, 9, 24, 27, 30, 29, 20, 23, 18, 17, 48, 51, 54, 53, 60, 63, 58, 57, 40, 43, 46, 45, 36, 39, 34, 33, 96, 99, 102, 101, 108, 111, 106, 105, 120, 123, 126, 125, 116, 119, 114, 113, 80, 83, 86, 85, 92, 95, 90, 89, 72, 75, 78, 77, 68, 71, 66, 65, 192, 195, 198, 197, 204, 207, 202, 201, 216, 219, 222, 221, 212, 215, 210, 209, 240, 243, 246, 245, 252, 255, 250, 249, 232, 235, 238, 237, 228, 231, 226, 225, 160, 163, 166, 165, 172, 175, 170, 169, 184, 187, 190, 189, 180, 183, 178, 177, 144, 147, 150, 149, 156, 159, 154, 153, 136, 139, 142, 141, 132, 135, 130, 129, 155, 152, 157, 158, 151, 148, 145, 146, 131, 128, 133, 134, 143, 140, 137, 138, 171, 168, 173, 174, 167, 164, 161, 162, 179, 176, 181, 182, 191, 188, 185, 186, 251, 248, 253, 254, 247, 244, 241, 242, 227, 224, 229, 230, 239, 236, 233, 234, 203, 200, 205, 206, 199, 196, 193, 194, 211, 208, 213, 214, 223, 220, 217, 218, 91, 88, 93, 94, 87, 84, 81, 82, 67, 64, 69, 70, 79, 76, 73, 74, 107, 104, 109, 110, 103, 100, 97, 98, 115, 112, 117, 118, 127, 124, 121, 122, 59, 56, 61, 62, 55, 52, 49, 50, 35, 32, 37, 38, 47, 44, 41, 42, 11, 8, 13, 14, 7, 4, 1, 2, 19, 16, 21, 22, 31, 28, 25, 26]



# def mixcolumns(block):
#     for j in range(4):
#         a, b, c, d = block[0+4*j], block[1+4*j], block[2+4*j], block[3+4*j]
#         block[0+4*j] = mul2[a]^mul3[b]^c^d
#         block[1+4*j] = a^mul2[b]^mul3[c]^d
#         block[2+4*j] = a^b^mul2[c]^mul3[d]
#         block[3+4*j] = mul3[a]^b^c^mul2[d]
#     return block

def mixcolumns(block):
    a0, b0, c0, d0 = block[0], block[1], block[2], block[3]
    block[0] = mul2[a0]^mul3[b0]^c0^d0
    block[1] = a0^mul2[b0]^mul3[c0]^d0
    block[2] = a0^b0^mul2[c0]^mul3[d0]
    block[3] = mul3[a0]^b0^c0^mul2[d0]
    
    a1, b1, c1, d1 = block[4], block[5], block[6], block[7]
    block[4] = mul2[a1]^mul3[b1]^c1^d1
    block[5] = a1^mul2[b1]^mul3[c1]^d1
    block[6] = a1^b1^mul2[c1]^mul3[d1]
    block[7] = mul3[a1]^b1^c1^mul2[d1]
    
    a2, b2, c2, d2 = block[8], block[9], block[10], block[11]
    block[8] = mul2[a2]^mul3[b2]^c2^d2
    block[9] = a2^mul2[b2]^mul3[c2]^d2
    block[10] = a2^b2^mul2[c2]^mul3[d2]
    block[11] = mul3[a2]^b2^c2^mul2[d2]
    
    a3, b3, c3, d3 = block[12], block[13], block[14], block[15]
    block[12] = mul2[a3]^mul3[b3]^c3^d3
    block[13] = a3^mul2[b3]^mul3[c3]^d3
    block[14] = a3^b3^mul2[c3]^mul3[d3]
    block[15] = mul3[a3]^b3^c3^mul2[d3]

    return block



############################################################# ADDROUNDKEY ###################################################################

def rotate(word):
    return bytearray(word[(i+1)%4] for i in range(4))

def rcon(i):
    return rcon_d[i]

rcon_d = {1: 1, 2: 2, 3: 4, 4: 8, 5: 16, 6: 32, 7: 64, 8: 128, 9: 27, 10: 54, 11: 108}

def core(word, i):
    word = rotate(word)
    word = bytearray(sbox[by] for by in word)
    word[0] ^= rcon(i)
    return word

def keyschedule(enckey):
    # global roundkeys
    n = 16
    b = 176
    roundkeys = enckey
    i = 1
    while len(roundkeys) < b:
        # 4 more bytes
        t = roundkeys[-4:]
        t = core(t, i)
        i += 1 
        for j in range(4): # xor with the four byte block `n` bytes before
            t[j] ^= roundkeys[-n+j]
        roundkeys += t # concat
        # 12 more bytes
        for _ in range(3):
            t = roundkeys[-4:]
            for j in range(4): # xor with the four byte block `n` bytes before
                t[j] ^= roundkeys[-n+j]
            roundkeys += t # concat        
    return roundkeys

# def roundkey(n):
#     return roundkeys[n*16:(n+1)*16]


####################################################################################################################################

def subbytes(block):
    block[0] = sbox[block[0]]
    block[1] = sbox[block[1]]
    block[2] = sbox[block[2]]
    block[3] = sbox[block[3]]
    block[4] = sbox[block[4]]
    block[5] = sbox[block[5]]
    block[6] = sbox[block[6]]
    block[7] = sbox[block[7]]
    block[8] = sbox[block[8]]
    block[9] = sbox[block[9]]
    block[10] = sbox[block[10]]
    block[11] = sbox[block[11]]
    block[12] = sbox[block[12]]
    block[13] = sbox[block[13]]
    block[14] = sbox[block[14]]
    block[15] = sbox[block[15]]
    
    return block

# def subbytes(block):
#     for i in range(16):
#         block[i] = sbox[block[i]]
#     return block


# def shiftrows(b):
#     for i in range(1,4):
#         b[i+4*0], b[i+4*1], b[i+4*2], b[i+4*3] = b[i+4*((0+i)%4)], b[i+4*((1+i)%4)], b[i+4*((2+i)%4)], b[i+4*((3+i)%4)]
#     return b


def addsubshift(b, key):
    b[0] = sbox[b[0]^key[0]]
    b[4] = sbox[b[4]^key[4]]
    b[8] = sbox[b[8]^key[8]]
    b[12] = sbox[b[12]^key[12]]
    b[1], b[5], b[9], b[13] = sbox[b[5]^key[5]], sbox[b[9]^key[9]], sbox[b[13]^key[13]], sbox[b[1]^key[1]]

    b[2], b[6], b[10], b[14] = sbox[b[10]^key[10]], sbox[b[14]^key[14]], sbox[b[2]^key[2]], sbox[b[6]^key[6]]
    
    b[3], b[7], b[11], b[15] = sbox[b[15]^key[15]], sbox[b[3]^key[3]], sbox[b[7]^key[7]], sbox[b[11]^key[11]]
    return b


def subshift(b):
    b[0] = sbox[b[0]]
    b[4] = sbox[b[4]]
    b[8] = sbox[b[8]]
    b[12] = sbox[b[12]]
    b[1], b[5], b[9], b[13] = sbox[b[5]], sbox[b[9]], sbox[b[13]], sbox[b[1]]

    b[2], b[6], b[10], b[14] = sbox[b[10]], sbox[b[14]], sbox[b[2]], sbox[b[6]]

    b[3], b[7], b[11], b[15] = sbox[b[15]], sbox[b[3]], sbox[b[7]], sbox[b[11]]
    return b


def shiftrows(b):
    b[1], b[5], b[9], b[13] = b[5], b[9], b[13], b[1]

    b[2], b[6], b[10], b[14] = b[10], b[14], b[2], b[6]

    b[3], b[7], b[11], b[15] = b[15], b[3], b[7], b[11]

    return b



def addroundkey(block, n, roundkeys):
    key = roundkeys[n*16:(n+1)*16]
    block[0] ^= key[0]
    block[1] ^= key[1]
    block[2] ^= key[2]
    block[3] ^= key[3]
    block[4] ^= key[4]
    block[5] ^= key[5]
    block[6] ^= key[6]
    block[7] ^= key[7]
    block[8] ^= key[8]
    block[9] ^= key[9]
    block[10] ^= key[10]
    block[11] ^= key[11]
    block[12] ^= key[12]
    block[13] ^= key[13]
    block[14] ^= key[14]
    block[15] ^= key[15]

    return block

# def addroundkey(block, n):
#     key = roundkeys[n*16:(n+1)*16]
#     # key = roundkey(roundno)
#     for i in range(16):
#         block[i] ^= key[i]
#     return block


# def main():
#     mainkey = bytearray(sys.stdin.buffer.read(16))
#     roundkeys = keyschedule(mainkey)
#     allinp = bytearray(sys.stdin.buffer.read())
#     N = len(allinp)
#     alloutp = bytearray(N)
#     for n in range(0,N,16):
#         block = allinp[n:n+16]
#         # block = addroundkey(block, 0, roundkeys)
#         for r in range(1,11):
#             # block = subbytes(block)
#             # block = shiftrows(block)
#             block = addsubshift(block, roundkeys[(r-1)*16:r*16])
#             if r < 10:
#                 block = mixcolumns(block)
#             # block = addroundkey(block, r, roundkeys)
#         block = addroundkey(block, 10, roundkeys)
#         alloutp[n:n+16] = block

#     if PROFILE:
#         outputfile.write(alloutp)
#     else:
#         sys.stdout.buffer.write(alloutp)

def main():
    mainkey = bytearray(sys.stdin.buffer.read(16))
    roundkeys = keyschedule(mainkey)
    allinp = bytearray(sys.stdin.buffer.read())
    N = len(allinp)
    alloutp = bytearray(N)
    for n in range(0,N,16):
        block = allinp[n:n+16]
        # block = addroundkey(block, 0, roundkeys)
        for r in range(1,11):
            # block = subbytes(block)
            # block = shiftrows(block)
            key = roundkeys[(r-1)*16:r*16]
            block[0] = sbox[block[0]^key[0]]
            block[4] = sbox[block[4]^key[4]]
            block[8] = sbox[block[8]^key[8]]
            block[12] = sbox[block[12]^key[12]]
            block[1], block[5], block[9], block[13] = sbox[block[5]^key[5]], sbox[block[9]^key[9]], sbox[block[13]^key[13]], sbox[block[1]^key[1]]

            block[2], block[6], block[10], block[14] = sbox[block[10]^key[10]], sbox[block[14]^key[14]], sbox[block[2]^key[2]], sbox[block[6]^key[6]]
    
            block[3], block[7], block[11], block[15] = sbox[block[15]^key[15]], sbox[block[3]^key[3]], sbox[block[7]^key[7]], sbox[block[11]^key[11]]

            # block = addsubshift(block, roundkeys[(r-1)*16:r*16])
            if r < 10:
                a0, b0, c0, d0 = block[0], block[1], block[2], block[3]
                block[0] = mul2[a0]^mul3[b0]^c0^d0
                block[1] = a0^mul2[b0]^mul3[c0]^d0
                block[2] = a0^b0^mul2[c0]^mul3[d0]
                block[3] = mul3[a0]^b0^c0^mul2[d0]
                
                a1, b1, c1, d1 = block[4], block[5], block[6], block[7]
                block[4] = mul2[a1]^mul3[b1]^c1^d1
                block[5] = a1^mul2[b1]^mul3[c1]^d1
                block[6] = a1^b1^mul2[c1]^mul3[d1]
                block[7] = mul3[a1]^b1^c1^mul2[d1]
                
                a2, b2, c2, d2 = block[8], block[9], block[10], block[11]
                block[8] = mul2[a2]^mul3[b2]^c2^d2
                block[9] = a2^mul2[b2]^mul3[c2]^d2
                block[10] = a2^b2^mul2[c2]^mul3[d2]
                block[11] = mul3[a2]^b2^c2^mul2[d2]
                
                a3, b3, c3, d3 = block[12], block[13], block[14], block[15]
                block[12] = mul2[a3]^mul3[b3]^c3^d3
                block[13] = a3^mul2[b3]^mul3[c3]^d3
                block[14] = a3^b3^mul2[c3]^mul3[d3]
                block[15] = mul3[a3]^b3^c3^mul2[d3]

                # block = mixcolumns(block)
            # block = addroundkey(block, r, roundkeys)
        key = roundkeys[160:]

        block[0] ^= key[0]
        block[1] ^= key[1]
        block[2] ^= key[2]
        block[3] ^= key[3]
        block[4] ^= key[4]
        block[5] ^= key[5]
        block[6] ^= key[6]
        block[7] ^= key[7]
        block[8] ^= key[8]
        block[9] ^= key[9]
        block[10] ^= key[10]
        block[11] ^= key[11]
        block[12] ^= key[12]
        block[13] ^= key[13]
        block[14] ^= key[14]
        block[15] ^= key[15]

        # block = addroundkey(block, 10, roundkeys)
        alloutp[n:n+16] = block

    if PROFILE:
        outputfile.write(alloutp)
    else:
        sys.stdout.buffer.write(alloutp)


main()


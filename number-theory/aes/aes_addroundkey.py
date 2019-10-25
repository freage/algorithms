from aes_gfscalar import gfscalar


# generate rcon lookup table
def gen_rcon():
    def calc_rcon(i):
        return int(gfscalar(2)**(i-1))
    global rcon_d
    rcon_d = { i : calc_rcon(i) for i in range(1,12) }
gen_rcon()

# The file to implement the PerlinNoise functionality

# The permutation table for the unique hash making of each of the integral unit cell co-ords.
# PERM_TABLE has all the numbers 0-255 (0 and 255 inclusive) included in it which will be useful
# later on.
# The permutation table can be hard-coded or can be read from a file or can be created on the fly 
# but it must contain 0-255 inclusive random numbers which are uniformly distributed throughout the 
# table. Rn, will be hard-coding it.
# This permutation table is the one which Ken Perlin used.
PERM_TABLE: list[int] = [151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180]

# double the permutation to avoid overflow
P: list[int] = []
for idx in range(512):
    P.append(PERM_TABLE[idx%255])

class PerlinNoise:
    """
    The class for generation of Perlin Noise
    """
    def __init__(self, inp_x, inp_y=0, inp_z=0, rep_amt=255):
        # by default y and z components will be 0.
        # by default the inp_vals will "wrap-around" if repeat > 0
        self.rep_amt = rep_amt
        self.inp_x, self.inp_y, self.inp_z = (inp_x, inp_y, inp_z)

        if self.rep_amt > 0:
            self.inp_x, self.inp_y, self.inp_z = (inp_x%self.rep_amt, inp_y%self.rep_amt, inp_z%self.rep_amt)

        # the self.*i are the integral co-ords of the unit cell, our input point lies in.
        # the self.*f are the relative co-ords of the input point from the unit cell co-ords
        # Eg. (0.5, 0.5, 0.5) lies in the centre of the unit cube
        self.xi = int(self.inp_x) & 255
        self.yi = int(self.inp_y) & 255
        self.zi = int(self.inp_z) & 255

        # NOTE: we wrap-around the self.*i to avoid overflow later while accessing the P[] list
        # Perlin Noise will repeat every 256 co-ordinates but its okay, as decimal values are
        # allowed

        self.xf = self.inp_x - self.xi
        self.yf = self.inp_y - self.yi
        self.zf = self.inp_z - self.zi

    # private:
    def _lerp(lo: float, hi: float, t: float):
        """
        Linearly interpolate between the lo and hi vals.
        0.0 <= t <= 1.0
        """
        # this guarantees res = hi when t == 1.
        return (1 - t) * lo + t * hi

    def _grad(self, hash: int, x: float, y: float, z: float):
        """
        Performs a dot product between the random gradient vectors and the 
        distance vectors.
        """
        # ? the hash represents the unique hash of the unit cell co-ord

        # For finding the grad dot product:
        # - Get the first 4 bits from the hash value (0xF = 15 = 0b1111) i.e. h
        # - if the most significant bit (MSB) of the h is 0 (compare with 8 "0b1000"), set u = x else u = y
        # - if first and the second significant bits are 0, set v = x 
        # - else if the first and the second significant bits are 1, set v = y
        # - else set v = z
        # - use the last two bits to decide if u and v are positive or negative

        h = hash & 0xF
        u = x if h < 8 else y
        
        if h < 4: # ? 4 = "0b0100"
            v = x
        elif h == 12 or h == 14: # ? 12 = "0b1100" and 14 = "0b1110"
            v = y
        else:
            v = z

        u = u if h&1 == 0 else -u
        v = v if h&2 == 0 else -v

        return (u + v)

    def _inc(self, num: int):
        # make sure the num wrap around, to repeat the noise
        num+=1
        if self.rep_amt > 0:
            num%=self.rep_amt

        return num

    def _hash(self) -> tuple:
        """
        Hashes the unit cell co-ords and return as tuple
        """
        # for a unit cube there are 8 possible hashes
        # returns the tuple of with all 8 hashes
        aaa = P[P[P[self.xi] + self.yi] + self.zi]
        aab = P[P[P[self.xi] + self.yi] + self._inc(self.zi)]
        aba = P[P[P[self.xi] + self._inc(self.yi)] + self.zi]
        abb = P[P[P[self.xi] + self._inc(self.yi)] + self._inc(self.zi)]
        baa = P[P[P[self._inc(self.xi)] + self.yi] + self.zi]
        bab = P[P[P[self._inc(self.xi)] + self.yi] + self._inc(self.zi)]
        bba = P[P[P[self._inc(self.xi)] + self._inc(self.yi)] + self.zi]
        bbb = P[P[P[self._inc(self.xi)] + self._inc(self.yi)] + self._inc(self.zi)]

        return (aaa, aab, aba, abb, baa, bab, bba, bbb)



#     ____           _ _         _   _       _             ____            
#    |  _ \ ___ _ __| (_)_ __   | \ | | ___ (_)___  ___   / ___| ___ _ __  
#    | |_) / _ \ '__| | | '_ \  |  \| |/ _ \| / __|/ _ \ | |  _ / _ \ '_ \ 
#    |  __/  __/ |  | | | | | | | |\  | (_) | \__ \  __/ | |_| |  __/ | | |
#    |_|   \___|_|  |_|_|_| |_| |_| \_|\___/|_|___/\___|  \____|\___|_| |_|


# The permutation table for the unique hash making of each of the integral unit cell co-ords.
# PERM_TABLE has all the numbers 0-255 (0 and 255 inclusive) included in it which will be useful
# later on.
# The permutation table can be hard-coded or can be read from a file or can be created on the fly 
# but it must contain 0-255 inclusive random numbers which are uniformly distributed throughout the 
# table. Rn, will be hard-coding it.
# This permutation table is the one which Ken Perlin used.

PERM_TABLE = [151,160,137,91,90,15,
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
P = []
for idx in range(512):
    P.append(PERM_TABLE[idx%255])

# ========================== BEGIN ========================== #
class PerlinNoise:
    """
    The class for generation of Perlin Noise
    """
    def __init__(self, inp_x, inp_y=0, inp_z=0, rep_amt=-1):
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

        self.xf = self.inp_x - int(self.inp_x)
        self.yf = self.inp_y - int(self.inp_y)
        self.zf = self.inp_z - int(self.inp_z)

        # store the hashes 
        self.hashes = {}
        self.value = None

    # private:
    def _lerp(self, lo: float, hi: float, t: float):
        """
        Linearly interpolate between the lo and hi vals.
        0.0 <= t <= 1.0
        """
        return lo + t * (hi - lo)

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
        u = x if (h<0x8) else y
        
        if h<0x4: # ? 4 = "0b0100"
            v = y
        elif h==0xC or h==0xE:     # ? 12 = "0b1100" and 14 = "0b1110"
            v = x
        else:
            v = z

        u = u if (h&1) == 0 else -u
        v = v if (h&2) == 0 else -v

        return (u + v)

    def _inc(self, num: int) -> int:
        # make sure the num wrap around, to repeat the noise
        num+=1
        if self.rep_amt > 0:
            num%=self.rep_amt

        return num

    def _hash(self) -> None:
        """
        Hashes the unit cell co-ords and return as tuple
        """
        # for a unit cube there are 8 possible hashes
        # returns the tuple of with all 8 hashes

        self.hashes["aaa"] = P[P[P[self.xi] + self.yi] + self.zi]
        self.hashes["aab"] = P[P[P[self.xi] + self.yi] + self._inc(self.zi)]
        self.hashes["aba"] = P[P[P[self.xi] + self._inc(self.yi)] + self.zi]
        self.hashes["abb"] = P[P[P[self.xi] + self._inc(self.yi)] + self._inc(self.zi)]
        self.hashes["baa"] = P[P[P[self._inc(self.xi)] + self.yi] + self.zi]
        self.hashes["bab"] = P[P[P[self._inc(self.xi)] + self.yi] + self._inc(self.zi)]
        self.hashes["bba"] = P[P[P[self._inc(self.xi)] + self._inc(self.yi)] + self.zi]
        self.hashes["bbb"] = P[P[P[self._inc(self.xi)] + self._inc(self.yi)] + self._inc(self.zi)]

    def _fadefunc(self, t):
        """
        Slope is 0 at the extreme points for 0.0 <= t <= 1.0
        """
        return t * t * t * (t * (t * 6 - 15) + 10)

    # All the things required to calculate the noise value has been declared and defined
    # till here. Only thing remaining is to calculate the noise value now
    # u, v and w are the "faded" values of the self.*f for smoothness while 
    # linearly interpolating
    def _calc(self):
        """
        Calculates the noise value at the input point
        """
        u = self._fadefunc(self.xf)
        v = self._fadefunc(self.yf)
        w = self._fadefunc(self.zf)

        # populate the hashes dict
        self._hash()
        
        # once the hash dict is populated, start calculating the dot product between 
        # the gradient vector and the distance vectors, which is done in the _grad method.
        # finally linearly interpolate the values to get the avg value
        # first interpolate in the x-dir, then in y-dir
        x1: float = self._lerp(self._grad(self.hashes["aaa"], self.xf, self.yf, self.zf),
                self._grad(self.hashes["baa"], self.xf - 1, self.yf, self.zf), u)

        x2: float = self._lerp(self._grad(self.hashes["aba"], self.xf, self.yf - 1, self.zf),
                self._grad(self.hashes["bba"], self.xf - 1, self.yf - 1, self.zf), u)

        # the first y-dir lerp
        y1: float = self._lerp(x1, x2, v)

        x1: float = self._lerp(self._grad(self.hashes["aab"], self.xf, self.yf, self.zf - 1),
                self._grad(self.hashes["bab"], self.xf - 1, self.yf, self.zf - 1), u)

        x2: float = self._lerp(self._grad(self.hashes["abb"], self.xf, self.yf - 1, self.zf - 1),
                self._grad(self.hashes["bbb"], self.xf-1, self.yf-1, self.zf-1), u)

        # the second y-dir lerp
        y2: float = self._lerp(x1, x2, v)

        # the final noise value, which will be in the range [0, 1]
        self.value = (self._lerp(y1, y2, w) + 1)/2
        return self.value

    def val(self):
        """
        Return the noise value calculated
        """
        if not self.value:
            self._calc()

        return self.value


# ? From the above noise generator, get the detailed noise by changing the frequency and 
# ? amplitude of the input point's noise value by some amount and adding it to the contribution
class OctavePerlin:
    """
    Calculate the noise value using octaves which implicitly uses PerlinNoise
    """
    def __init__(self, inp_x, inp_y=0, inp_z=0, octaves=8, persist=0.5):
        self.inp_x = inp_x
        self.inp_y = inp_y
        self.inp_z = inp_z

        # persistance represent which amplitudes contribute in the details
        # of the noise value
        self.octaves = octaves
        self.persist = persist

        # final noise value
        self.value = None

    def _calc(self):
        """
        Calculate using the PerlinNoise instance
        """
        tot_sum: float = 0   # ? total sum of the noise values
        max_amp: float = 0   # ? keep the sum in [0,1]
        amp: float = 1.0     # ? amplitude of each noise value
        freq: float = 1.0    # ? frequency for getting the detailed noise

        # for each octave we twice the frequency and multiply the amplitude 
        # by persistance to get the detailed noise value
        # to keep the final sum value in the range [0, 1] we keep track of the 
        # max amplitude (sum of all the amplitudes)
        for octave in range(self.octaves):
            noise_obj = PerlinNoise(self.inp_x*freq, self.inp_y*freq, self.inp_z*freq)
            # ? multiply the noise value by the amplitude
            tot_sum += noise_obj.val() * amp
            max_amp += amp

            amp *= self.persist
            freq *= 2.0     # double the freq each iteration

        # value is in the range [0,1]
        self.value = tot_sum / max_amp

    def val(self):
        """
        Return the final detailed noise value
        """
        if not self.value:
            self._calc()

        return self.value

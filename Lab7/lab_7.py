from codecs import getdecoder
from codecs import getencoder


def strxor(a, b):
    mlen = min(len(a), len(b))
    a, b, xor = bytearray(a), bytearray(b), bytearray(mlen)
    for i in range(mlen):
        xor[i] = a[i] ^ b[i]
    return bytes(xor)


_hexdecoder = getdecoder("hex")
_hexencoder = getencoder("hex")


def hexdec(data):
    return _hexdecoder(data)[0]


def hexenc(data):
    return _hexencoder(data)[0].decode("ascii")


def bytes2long(raw):
    return int(hexenc(raw), 16)


def long2bytes(n, size=32):
    res = hex(int(n))[2:].rstrip("L")
    if len(res) % 2 != 0:
        res = "0" + res
    s = hexdec(res)
    if len(s) != size:
        s = (size - len(s)) * b"\x00" + s
    return s


def modinvert(a, n):
    if a < 0:
        # k^-1 = p - (-k)^-1 mod p
        return n - modinvert(-a, n)
    t, newt = 0, 1
    r, newr = n, a
    while newr != 0:
        quotinent = r // newr
        t, newt = newt, t - quotinent * newt
        r, newr = newr, r - quotinent * newr
    if r > 1:
        return -1
    if t < 0:
        t = t + n
    return t


MODE2SIZE = {
    2001: 32,
    2012: 64,
}

DEFAULT_CURVE = "GostR3410_2001_CryptoPro_A_ParamSet"
# Curve parameters are the following: p, q, a, b, x, y
CURVE_PARAMS = {
    "GostR3410_2001_CryptoPro_A_ParamSet": (
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
        "00000000000000000000000000000000000000000000000000000000000000a6",
    )
}
for c, params in CURVE_PARAMS.items():
    CURVE_PARAMS[c] = [hexdec(param) for param in params]


class Curve(object):
    def __init__(self, p, a, b):
        self.p = bytes2long(p)
        self.a = bytes2long(a)
        self.b = bytes2long(b)

    def _pos(self, v):
        if v < 0:
            return v + self.p
        return v

    def _add(self, p1x, p1y, p2x, p2y):
        if p1x == p2x and p1y == p2y:
            # double
            t = ((3 * p1x * p1x + self.a) * modinvert(2 * p1y, self.p)) % self.p
        else:
            tx = self._pos(p2x - p1x) % self.p
            ty = self._pos(p2y - p1y) % self.p
            t = (ty * modinvert(tx, self.p)) % self.p
        tx = self._pos(t * t - p1x - p2x) % self.p
        ty = self._pos(t * (p1x - tx) - p1y) % self.p
        return tx, ty

    def exp(self, degree, x=None, y=None):
        x = x or self.x
        y = y or self.y
        tx = x
        ty = y
        degree -= 1
        if degree == 0:
            raise ValueError("Bad degree value")
        while degree != 0:
            if degree & 1 == 1:
                tx, ty = self._add(tx, ty, x, y)
            degree = degree >> 1
            x, y = self._add(x, y, x, y)
        return tx, ty


def ellept(d_a, d_b, G):
    curve = Curve(*CURVE_PARAMS["GostR3410_2001_CryptoPro_A_ParamSet"])

    H_a = curve.exp(d_a, *G)
    H_b = curve.exp(d_b, *G)

    secret_a = curve.exp(d_a, *H_b)
    secret_b = curve.exp(d_b, *H_a)

    print('First: \n')
    print('\t public: ', H_a)
    print('\t private: ', secret_a)

    print('First: \n')
    print('\t public: ', H_b)
    print('\t private: ', secret_b)

    assert secret_a == secret_b
    return secret_a


if __name__ == '__main__':
    d_a = 123123
    d_b = 432123
    G = (123123, 12312311)
    key = ellept(d_a, d_b, G)
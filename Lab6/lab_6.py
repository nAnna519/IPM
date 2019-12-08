import random
import sys

from elliptic_curve import EllipticCurvePoint


def initial_data():
    p = 0x8000000000000000000000000000000000000000000000000000000000000431
    a = 0x7
    b = 0x5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E
    m = 0x8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3
    q = 0x8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3
    x_p = 0x2
    y_p = 0x8E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8
    x_q = 0x7F2B49E270DB6D90D8595BEC458B50C58585BA1D4E9B788F6689DBD8E56FD80B
    y_q = 0x26F1B489D6701DD185C8413A977B3CBBAF64D1C593D26627DFFB101A87FF77DA

    if not is_valid_q(q, m):
        raise ValueError('q is not valid')

    return p, a, b, m, q, x_p, y_p, x_q, y_q


def is_valid_q(q, m):
    n = m / q
    if not (m == n * q and n >= 1 and n % 1 == 0):
        return False
    if (2 ** 254 < q < 2 ** 256) or (2 ** 508 < q < 2 ** 512):
        return True
    return False


def is_valid_d(d, q):
    return 0 < d < q


def get_l(q):
    if 2 ** 254 < q < 2 ** 256:
        return 256
    elif 2 ** 508 < q < 2 ** 512:
        return 512
    else:
        return None


def get_gost3410(message, length):
    import stribog  # gost 3411
    return stribog.entry().FromString(message, length)


def inverse(num, module):  # ** -1
    def gcd_extended(a, b):
        if a == 0:
            return b, 0, 1

        divider, x, y = gcd_extended(b % a, a)
        return divider, y - (b // a) * x, x

    gcd, x, _ = gcd_extended(num, module)
    if gcd == 1:
        return x % module

    raise ValueError("invalid arguments")


def generate_gost_digital_signature(d, M):
    p, a, b, m, q, x_p, y_p, x_q, y_q = initial_data()
    if not is_valid_d(d, q):
        raise ValueError('d is not correct')

    # step 1
    l = get_l(q)
    h_ = get_gost3410(M, l)

    # step 2
    alpha = int(h_, 16)
    e = alpha % q
    e = 1 if e == 0 else e

    r = 0
    s = 0
    while True:
        k = random.randint(0, q)

        P = EllipticCurvePoint([x_p, y_p], [a, b, p])
        C = k * P
        r = C.x % q
        if r == 0:
            continue

        s = (r * d + k * e) % q
        if s == 0:
            continue
        else:
            break

    sign = (r, s)
    return sign


def check_gost_digital_signature(sign, M, d):
    p, a, b, m, q, x_p, y_p, x_q, y_q = initial_data()

    r, s = sign
    if not (0 < r < q and 0 < s < q):
        raise ValueError('Sign is not correct!')

    l = get_l(q)
    h_ = get_gost3410(M, l)

    alpha = int(h_, 16)
    e = alpha % q
    e = 1 if e == 0 else e

    v = inverse(e, q)

    z1 = (s * v) % q
    z2 = (-r * v) % q

    P = EllipticCurvePoint([x_p, y_p], [a, b, p])
    Q = EllipticCurvePoint([x_q, y_q], [a, b, p])

    P = z1 * P
    Q = z2 * Q
    C = P + Q
    R = C.x % q

    if r == R:
        return True
    else:
        print('Sign is not correct, R != r')
        return False


def main():
    if len(sys.argv) < 2:
        exit(1)

    file_in = sys.argv[1]
    file_out = sys.argv[2]
    with open(file_in, 'r+') as f:
        M = f.read()

    print('Input text: ', M)

    d = 0x7A929ADE789BB9BE10ED359DD39A72C11B60961F49397EEE1D19CE9891EC3B28

    try:
        sign = generate_gost_digital_signature(d, M)
    except Exception as ex:
        print(ex)
        sys.exit(-1)

    print('The sign is: ', ''.join([str(i) for i in sign]))

    with open(file_out, 'w+') as f:
        f.write(''.join([str(i) for i in sign]))

    try:
        validation = check_gost_digital_signature(sign, M, d)
    except Exception as ex:
        print(ex)
        sys.exit(-1)

    print('The key is: ', validation)


if __name__ == '__main__':
    main()
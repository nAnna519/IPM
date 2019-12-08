# coding=utf-8
import collections


EllipticCurve = collections.namedtuple("EllipticCurve", ["a", "b", "modulo"])
Point = collections.namedtuple("Point", ["x", "y"])


class EllipticCurvePoint(object):

    def __init__(self, point, curve):
        assert EllipticCurvePoint.is_belong_curve(point, curve)

        self.x, self.y = point
        self.curve = curve

    @staticmethod
    def is_belong_curve(point, curve):
        x, y = point
        a, b, modulo = curve

        return y ** 2 % modulo == (x ** 3 + a * x + b) % modulo

    def as_point(self):
        return Point(self.x, self.y)

    def __add__(self, other):
        if self.curve != other.curve:
            raise ValueError("Points should be on the same curve")

        a, b, modulo = self.curve
        x1, y1 = self.as_point()
        x2, y2 = other.as_point()

        if x1 == x2 and y1 == y2:
            l = ((3 * x1 ** 2 + a) * inverse(2 * y1, modulo)) % modulo
        else:
            if x1 == x2:
                raise ValueError("inf point")

            dx = (x2 - x1) % modulo
            dy = (y2 - y1) % modulo
            l = (dy * inverse(dx, modulo)) % modulo

        x = (l ** 2 - x1 - x2) % modulo
        y = (l * (x1 - x) - y1) % modulo

        return EllipticCurvePoint(Point(x, y), self.curve)

    def __rmul__(self, other):
        def bin_mul(point, multiplier):
            if multiplier == 1:
                return point
            elif multiplier % 2 == 0:
                half = bin_mul(point, multiplier // 2)
                return half + half
            else:
                return point + bin_mul(point, multiplier - 1)

        return bin_mul(self, other)


def inverse(num, modulo):
    def gcd_extended(a, b):
        if a == 0:
            return b, 0, 1

        divider, x, y = gcd_extended(b % a, a)
        return divider, y - (b // a) * x, x

    gcd, x, _ = gcd_extended(num, modulo)
    if gcd == 1:
        return x % modulo

    raise ValueError("invalid arguments")
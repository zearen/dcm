sq = lambda x: x*x

class Imaginary():
    def __init__(self, real, imag):
        self.real = float(real)
        self.imag = float(imag)

    def __str__(self):
        return str(self.real) + 'i' + str(self.imag)

    def __add__(self, other):
        return other.__class__(
            self.real + other.real,
            self.imag + other.imag)

    def __sub__(self, other):
        return other.__class__(
            self.real - other.real,
            self.imag - other.imag)

    def __mul__(self, other):
        return other.__class__(
            self.real * other.real - self.imag * other.imag,
            self.real * other.imag + self.imag * other.real)

    def __div__(self, other):
        denom = sq(other.real) + sq(other.imag)
        return other.__class__(
            (other.real * self.real + other.imag * self.imag) / denom,
            (other.real * self.imag - other.imag * self.real) / denom)

class ConjugableImaginary(Imaginary):
    def conjugate(self):
        return self.__class__(self.real, -self.imag)

a = Imaginary(1,2)
b = ConjugableImaginary(3,4)

print 'a = ',
print a

print 'b = ',
print b

print 'a + b = ',
print a + b

print 'a - b = ',
print a - b

print 'a * b = ',
print a * b

print 'a / b = ',
print a / b

print 'b conjugate = ',
print b.conjugate()

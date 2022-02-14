class Complex {
    // ...
    friend Complex operator+(double lhs, const Complex & rhs);
};

Complex operator+(double lhs, const Complex & rhs) {
    return Complex(lhs+rhs.real, rhs.imaginary);
}
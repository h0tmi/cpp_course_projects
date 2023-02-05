#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <map>

class Poly {
public:
    Poly() = default;
    Poly(std::initializer_list<int64_t> coefficients);
    Poly(std::initializer_list<std::pair<size_t, int64_t>> coefficients);

    Poly operator+(const Poly& other);
    Poly operator-(const Poly& other);
    Poly operator-() const;
    Poly operator*(const Poly& other);
    Poly& operator*=(const Poly& other);
    Poly& operator+=(const Poly& other);
    Poly& operator-=(const Poly& other);
    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const;
    int64_t operator()(int64_t x) const;

    friend std::ostream& operator<<(std::ostream& output, const Poly& poly);

    ~Poly() = default;

private:
    std::map<size_t, int64_t> coefficients_;
};

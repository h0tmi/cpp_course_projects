#include "poly.h"
#include <cmath>
#include <string>

Poly::Poly(std::initializer_list<int64_t> coefficients) {
    size_t id = 0;
    for (const auto& value : coefficients) {
        if (value != 0) {
            coefficients_[id] = value;
        }
        ++id;
    }
}
Poly::Poly(std::initializer_list<std::pair<size_t, int64_t>> coefficients) {
    for (const auto& [id, value] : coefficients) {
        if (value != 0) {
            coefficients_[id] = value;
        }
    }
}
Poly Poly::operator+(const Poly& other) {
    Poly current;
    for (const auto& [key, value] : coefficients_) {
        current.coefficients_[key] = value;
    }
    for (const auto& [key, value] : other.coefficients_) {
        current.coefficients_[key] += value;
        if (current.coefficients_[key] == 0) {
            current.coefficients_.erase(key);
        }
    }
    return current;
}
Poly Poly::operator-(const Poly& other) {
    Poly current;
    for (const auto& [key, value] : coefficients_) {
        current.coefficients_[key] = value;
    }
    for (const auto& [key, value] : other.coefficients_) {
        current.coefficients_[key] -= value;
        if (current.coefficients_[key] == 0) {
            current.coefficients_.erase(key);
        }
    }
    return current;
}
Poly Poly::operator-() const {
    Poly current;
    for (const auto& [key, value] : coefficients_) {
        current.coefficients_[key] = -value;
    }
    return current;
}
Poly Poly::operator*(const Poly& other) {
    Poly current;
    for (auto& [key1, value1] : coefficients_) {
        for (const auto& [key2, value2] : other.coefficients_) {
            current.coefficients_[key1 + key2] += value1 * value2;
        }
    }
    return current;
}
Poly& Poly::operator+=(const Poly& other) {
    for (const auto& [key, value] : other.coefficients_) {
        coefficients_[key] += value;
        if (coefficients_[key] == 0) {
            coefficients_.erase(key);
        }
    }
    return *this;
}
Poly& Poly::operator-=(const Poly& other) {
    for (const auto& [key, value] : other.coefficients_) {
        coefficients_[key] -= value;
        if (coefficients_[key] == 0) {
            coefficients_.erase(key);
        }
    }
    return *this;
}
bool Poly::operator==(const Poly& other) const {
    if (coefficients_.size() != other.coefficients_.size()) {
        return false;
    }
    for (const auto& [key, value] : coefficients_) {
        auto current = other.coefficients_.find(key);
        if (current == other.coefficients_.end()) {
            return false;
        }
        if (value != current->second) {
            return false;
        }
    }
    return true;
}
bool Poly::operator!=(const Poly& other) const {
    return !(*this == other);
}
int64_t Poly::operator()(int64_t x) const {
    int64_t result = 0;
    for (const auto& [key, value] : coefficients_) {
        result += value * static_cast<int64_t>(std::pow(x, key));
    }
    return result;
}
std::ostream& operator<<(std::ostream& output, const Poly& poly) {
    std::vector<std::pair<size_t, int64_t>> result;
    output << "y = ";
    for (const auto& [key, value] : poly.coefficients_) {
        result.push_back({key, value});
    }
    if (result.empty()) {
        output << "0";
        return output;
    }
    if (result.size() == 1) {
        if (result.back().first == 0) {
            output << (result.back().second >= 0 ? "" : "-") << std::abs(result.back().second);
        } else {
            output << (std::abs(result.back().second) >= 0 ? "" : "-") << std::abs(result.back().second) << "x^"
                   << result.back().first;
        }
        return output;
    }
    for (size_t i = result.size() - 1; i != std::numeric_limits<size_t>::max(); --i) {
        if (i == result.size() - 1) {
            output << (result[i].second >= 0 ? "" : "-") << std::abs(result[i].second) << "x^" << result[i].first;
        } else if (result[i].first == 0) {
            output << (result[i].second >= 0 ? " + " : " - ") << std::abs(result[i].second);
        } else {
            output << (result[i].second >= 0 ? " + " : " - ") << std::abs(result[i].second) << "x^" << result[i].first;
        }
    }
    return output;
}
Poly& Poly::operator*=(const Poly& other) {
    Poly current;
    for (auto& [key1, value1] : coefficients_) {
        for (const auto& [key2, value2] : other.coefficients_) {
            current.coefficients_[key1 + key2] += value1 * value2;
        }
    }
    coefficients_ = current.coefficients_;
    return *this;
}

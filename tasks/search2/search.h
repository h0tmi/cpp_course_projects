#pragma once

#include <map>
#include <string_view>
#include <vector>

class SearchEngine {
private:
    static const size_t MODULE = 1000000007;
    static const size_t PRIME = 57;
    static constexpr long double EPS = 1e-10;

    struct TfIdf {
        long double value;
        size_t pos;
        bool operator()(TfIdf a, TfIdf b) {
            if (std::abs(a.value - b.value) < EPS) {
                return a.pos < b.pos;
            } else {
                if (a.value - b.value < EPS) {
                    return false;
                } else {
                    return true;
                }
            }
        }
    };

    struct StringOptions {
        size_t str_begin = 0;
        size_t str_size = 0;
        size_t words = 0;
    };

    mutable std::string_view text_;
    mutable std::vector<std::map<size_t, size_t>> words_cnt_;
    mutable std::vector<StringOptions> string_options_;
    mutable std::vector<TfIdf> tf_idf_;
    mutable std::vector<std::map<size_t, long double>> tf_;
    mutable std::map<size_t, long double> idf_;

    size_t GetHash(std::string_view& text, size_t& i) const;

public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;
};

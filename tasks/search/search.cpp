#include "search.h"
#include <map>
#include <cctype>
#include <math.h>

const size_t MODULE = 1000000007;
const size_t PRIME = 57;
const long double EPS = 1e-10;

struct TfIdf {
    long double value;
    size_t pos;
    bool operator()(TfIdf a, TfIdf b) {
        if (abs(a.value - b.value) < EPS) {
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

size_t GetHash(std::string_view& text, size_t& i) {
    size_t word_hash = 0;
    while (i < text.size() && std::isalpha(text[i])) {
        word_hash = (word_hash * PRIME + tolower(text[i])) % MODULE;
        ++i;
    }

    while (i + 1 < text.size() && !std::isalpha(text[i + 1])) {
        ++i;
    }
    return word_hash;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::vector<std::string_view> result;
    size_t text_strings_number = std::count(text.begin(), text.end(), '\n') + 1;
    std::vector<std::map<size_t, size_t>> words_cnt(text_strings_number);
    std::vector<StringOptions> string_options(text_strings_number);
    std::vector<TfIdf> tf_idf;
    std::vector<std::map<size_t, long double>> tf(text_strings_number);
    std::map<size_t, long double> idf;

    size_t current_str = 0;
    bool end_of_str = true;
    for (size_t i = 0; i < text.size(); ++i) {
        if (end_of_str) {
            string_options[current_str] = {i, 0, 0};
            end_of_str = false;
        }
        size_t it_begin = i;
        size_t word_hash = GetHash(text, i);
        string_options[current_str].str_size += i - it_begin;
        ++string_options[current_str].words;
        ++words_cnt[current_str][word_hash];
        if (text[i] == '\n') {
            end_of_str = true;
            ++current_str;
        } else {
            ++string_options[current_str].str_size;
        }
    }

    for (size_t i = 0; i < query.size(); ++i) {
        size_t word_cnt = 0;
        size_t word_hash = GetHash(query, i);
        for (size_t j = 0; j < words_cnt.size(); ++j) {
            if (words_cnt[j][word_hash] > 0 && string_options[j].words > 0) {
                tf[j][word_hash] = static_cast<long double>(words_cnt[j][word_hash]) /
                                   static_cast<long double>(string_options[j].words);
            }
            if (words_cnt[j][word_hash] > 0) {
                ++word_cnt;
            }
        }
        if (word_cnt > 0) {
            idf[word_hash] = static_cast<long double>(words_cnt.size()) / static_cast<long double>(word_cnt);
        }
    }
    for (size_t i = 0; i < words_cnt.size(); ++i) {
        long double current_tf_idf = 0;
        for (const auto& current_tf : tf[i]) {
            if (idf[current_tf.first] > EPS) {
                current_tf_idf += current_tf.second * log(idf[current_tf.first]);
            }
        }
        tf_idf.push_back({current_tf_idf, i});
    }
    sort(tf_idf.begin(), tf_idf.end(), TfIdf());
    size_t number_of_results = std::min(tf_idf.size(), results_count);
    for (size_t i = 0; i < number_of_results && tf_idf[i].value > EPS; ++i) {
        result.push_back(text.substr(string_options[tf_idf[i].pos].str_begin, string_options[tf_idf[i].pos].str_size));
    }
    return result;
}

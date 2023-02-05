#include "search.h"
#include <cctype>
#include <math.h>

void SearchEngine::BuildIndex(std::string_view text) {
    text_ = text;
    size_t text_strings_number = std::count(text.begin(), text.end(), '\n') + 1;
    words_cnt_.clear();
    string_options_.clear();
    tf_.clear();
    words_cnt_.resize(text_strings_number);
    string_options_.resize(text_strings_number);
    tf_.resize(text_strings_number);
    size_t current_str = 0;
    bool end_of_str = true;
    for (size_t i = 0; i < text.size(); ++i) {
        if (end_of_str) {
            string_options_[current_str] = {i, 0, 0};
            end_of_str = false;
        }
        size_t it_begin = i;
        size_t word_hash = GetHash(text, i);
        string_options_[current_str].str_size += i - it_begin;
        ++string_options_[current_str].words;
        ++words_cnt_[current_str][word_hash];
        if (text[i] == '\n') {
            end_of_str = true;
            ++current_str;
        } else {
            ++string_options_[current_str].str_size;
        }
    }
}
std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    std::vector<std::string_view> result;
    for (size_t i = 0; i < query.size(); ++i) {
        size_t word_cnt = 0;
        size_t word_hash = SearchEngine::GetHash(query, i);
        for (size_t j = 0; j < words_cnt_.size(); ++j) {
            if (words_cnt_[j][word_hash] > 0 && string_options_[j].words > 0) {
                tf_[j][word_hash] = static_cast<long double>(words_cnt_[j][word_hash]) /
                                    static_cast<long double>(string_options_[j].words);
            }
            if (words_cnt_[j][word_hash] > 0) {
                ++word_cnt;
            }
        }
        if (word_cnt > 0) {
            idf_[word_hash] = static_cast<long double>(words_cnt_.size()) / static_cast<long double>(word_cnt);
        }
    }
    for (size_t i = 0; i < words_cnt_.size(); ++i) {
        long double current_tf_idf = 0;
        for (const auto& current_tf : tf_[i]) {
            if (idf_[current_tf.first] > EPS) {
                current_tf_idf += current_tf.second * log(idf_[current_tf.first]);
            }
        }
        tf_idf_.push_back({current_tf_idf, i});
    }
    std::sort(tf_idf_.begin(), tf_idf_.end(), TfIdf());
    size_t number_of_results = std::min(tf_idf_.size(), results_count);
    for (size_t i = 0; i < number_of_results && tf_idf_[i].value > EPS; ++i) {
        result.push_back(
            text_.substr(string_options_[tf_idf_[i].pos].str_begin, string_options_[tf_idf_[i].pos].str_size));
    }

    return result;
}
size_t SearchEngine::GetHash(std::string_view& text, size_t& i) const {
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

#include "word2vec.h"

#include <vector>
#include <utility>

int64_t DotProduct(const std::vector<int>& a, const std::vector<int>& b) {
    int64_t result = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }
    return result;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::pair<int64_t, std::vector<std::string>> best_words =
        std::make_pair(std::numeric_limits<int64_t>::min(), std::vector<std::string>());
    for (size_t i = 1; i < vectors.size(); ++i) {
        int64_t current_sum = DotProduct(vectors[0], vectors[i]);
        if (current_sum == best_words.first) {
            best_words.second.push_back(words[i]);
        } else if (current_sum > best_words.first) {
            best_words = std::make_pair(current_sum, std::vector<std::string>{words[i]});
        }
    }
    return best_words.second;
}

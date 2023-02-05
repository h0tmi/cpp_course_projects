#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    if (str.empty()) {
        return true;
    }
    //  "i__________"
    size_t it_end = str.size() - 1;
    size_t it_begin = 0;
    while (it_begin < it_end) {
        while (it_begin < str.size() - 1 && str[it_begin] == ' ') {
            ++it_begin;
        }
        while (it_end > 0 && str[it_end] == ' ') {
            --it_end;
        }
        if (str[it_begin] != str[it_end]) {
            return false;
        }
        ++it_begin;
        if (it_end > 0) {
            --it_end;
        }
    }
    return true;
}

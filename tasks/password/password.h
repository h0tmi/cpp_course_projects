#pragma once

#include <cctype>
#include <string>

struct PasswordRequirementsCounter {
    bool big_letters = false;
    bool digits = false;
    bool other = false;
    bool small_letters = false;
};

bool ValidatePassword(const std::string& password);

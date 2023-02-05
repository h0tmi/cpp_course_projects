#include "password.h"

constexpr size_t MinPasswordLength = 8;
constexpr size_t MaxPasswordLength = 14;
constexpr uint16_t MinRequiredConditionsCount = 3;
constexpr uint16_t MinRequiredCharacterId = 33;
constexpr uint16_t MaxRequiredCharacterId = 126;

bool ValidatePassword(const std::string& password) {
    if (password.size() < MinPasswordLength || password.size() > MaxPasswordLength) {
        return false;
    }
    PasswordRequirementsCounter cnt;
    for (auto current_letter : password) {
        if (current_letter < MinRequiredCharacterId || current_letter > MaxRequiredCharacterId) {
            return false;
        }
        if (isdigit(current_letter)) {
            cnt.digits = true;
        } else if (islower(current_letter)) {
            cnt.small_letters = true;
        } else if (isupper(current_letter)) {
            cnt.big_letters = true;
        } else {
            cnt.other = true;
        }
    }
    return cnt.digits + cnt.big_letters + cnt.small_letters + cnt.other >= MinRequiredConditionsCount;
}

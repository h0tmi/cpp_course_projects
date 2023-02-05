#include "tests_checking.h"
#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::vector<std::string> result;
    std::deque<std::string> works;
    for (const auto& current_action : student_actions) {
        if (current_action.side == Side::Bottom) {
            works.push_back(current_action.name);
        } else {
            works.push_front(current_action.name);
        }
    }
    for (auto current_position : queries) {
        result.push_back(works[current_position - 1]);
    }
    return result;
}

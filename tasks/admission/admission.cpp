#include "admission.h"

#include <algorithm>
#include <tuple>

bool Comp(const Applicant* a, const Applicant* b) {
    if ((*a).points == (*b).points) {
        if (std::tie((*a).student.birth_date.year, (*a).student.birth_date.month, (*a).student.birth_date.day) ==
            std::tie((*b).student.birth_date.year, (*b).student.birth_date.month, (*b).student.birth_date.day)) {
            return (*a).student.name < (*b).student.name;
        } else {
            return std::tie((*a).student.birth_date.year, (*a).student.birth_date.month, (*a).student.birth_date.day) <
                   std::tie((*b).student.birth_date.year, (*b).student.birth_date.month, (*b).student.birth_date.day);
        }
    }
    return (*a).points > (*b).points;
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    AdmissionTable list_of_entered_students;
    std::unordered_map<std::string, size_t> universities_remaining_positions;
    for (const auto& university : universities) {
        universities_remaining_positions[university.name] = university.max_students;
        list_of_entered_students[university.name] = std::vector<const Student*>();
    }
    std::vector<const Applicant*> sorted_applicants_pointers;
    for (size_t i = 0; i < applicants.size(); ++i) {
        sorted_applicants_pointers.push_back(&applicants[i]);
    }
    std::sort(sorted_applicants_pointers.begin(), sorted_applicants_pointers.end(), Comp);
    for (const auto student : sorted_applicants_pointers) {
        for (const auto& university : student->wish_list) {
            if (universities_remaining_positions.find(university) != universities_remaining_positions.end() &&
                universities_remaining_positions[university] - 1 != std::numeric_limits<size_t>::max()) {
                universities_remaining_positions[university] = universities_remaining_positions[university] - 1;
                list_of_entered_students[university].push_back(&student->student);
                break;
            }
        }
    }
    return list_of_entered_students;
}

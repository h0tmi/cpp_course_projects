#include "scorer.h"
void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    student_task_list_[Scorer::StudentTask{student_name, task_name}][EventType::CheckSuccess] = false;
}
void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    student_task_list_[Scorer::StudentTask{student_name, task_name}][EventType::CheckSuccess] = true;
}
void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    student_task_list_[Scorer::StudentTask{student_name, task_name}][EventType::MergeRequestOpen] = true;
}
void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    student_task_list_[Scorer::StudentTask{student_name, task_name}][EventType::MergeRequestOpen] = false;
}
void Scorer::Reset() {
    student_task_list_.clear();
}
ScoreTable Scorer::GetScoreTable() const {
    ScoreTable result;
    for (auto current_task : student_task_list_) {
        if (current_task.second[EventType::MergeRequestOpen]) {
            continue;
        }
        if (current_task.second[EventType::CheckSuccess]) {
            result[current_task.first.student_name].insert(current_task.first.task_name);
        }
    }
    return result;
}

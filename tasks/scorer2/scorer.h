#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
public:
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();

    ScoreTable GetScoreTable() const;

private:
    enum class EventType {
        CheckFailed,
        CheckSuccess,
        MergeRequestOpen,
        MergeRequestClosed,
    };
    struct Event {
        StudentName student_name;
        TaskName task_name;
        time_t time;
        EventType event_type;
    };
    struct StudentTask {
        StudentName student_name;
        TaskName task_name;
        bool operator==(const StudentTask& other) const {
            return student_name == other.student_name && task_name == other.task_name;
        }
    };
    struct StudentTaskComparator {
        size_t operator()(const StudentTask& other) const {
            size_t result_hash = 17;
            result_hash = result_hash * 31 + std::hash<std::string>()(other.student_name);
            result_hash = result_hash * 31 + std::hash<std::string>()(other.task_name);
            return result_hash;
        }
    };
    std::unordered_map<StudentTask, std::map<EventType, bool>, StudentTaskComparator> student_task_list_;
};

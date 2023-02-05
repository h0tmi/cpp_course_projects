#include "scorer.h"
#include <utility>

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::map<std::pair<StudentName, TaskName>, std::map<EventType, std::pair<bool, time_t>>> current_processing_tasks;
    ScoreTable result;
    for (const auto& current_event : events) {
        if (current_event.time > score_time) {
            continue;
        }
        switch (current_event.event_type) {
            case EventType::CheckSuccess:
                if (current_event.time >
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [current_event.event_type]
                                                .second) {
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [current_event.event_type]
                                                .second = current_event.time;
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [current_event.event_type]
                                                .first = true;
                }
                break;
            case EventType::CheckFailed:
                if (current_event.time >
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [EventType::CheckSuccess]
                                                .second) {
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [EventType::CheckSuccess]
                                                .second = current_event.time;
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [EventType::CheckSuccess]
                                                .first = false;
                }
                break;
            case EventType::MergeRequestClosed:
                if (current_event.time >
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [EventType::MergeRequestOpen]
                                                .second) {
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [EventType::MergeRequestOpen]
                                                .second = current_event.time;
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [EventType::MergeRequestOpen]
                                                .first = false;
                }
                break;
            case EventType::MergeRequestOpen:
                if (current_event.time >
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [current_event.event_type]
                                                .second) {
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [current_event.event_type]
                                                .second = current_event.time;
                    current_processing_tasks[std::make_pair(current_event.student_name, current_event.task_name)]
                                            [current_event.event_type]
                                                .first = true;
                }
                break;
        }
    }
    for (auto current_task : current_processing_tasks) {
        if (current_task.second[EventType::MergeRequestOpen].first) {
            continue;
        }
        if (current_task.second[EventType::CheckSuccess].first) {
            result[current_task.first.first].insert(current_task.first.second);
        }
    }
    return result;
}

#include <vector>
#include "unixpath.h"

const std::string PREVIOUS = "..";
const std::string CURRENT = ".";
const std::string SEPARATOR = "/";
const char SEPARATOR_C = '/';

UnixPath::UnixPath(std::string_view initial_dir) {
    initial_directories_ = NormalizePath(static_cast<std::string>(initial_dir), CURRENT);
    absolute_directories_ = initial_directories_;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    absolute_directories_ = NormalizePath(absolute_directories_, static_cast<std::string>(path));
    relative_directories_ = RelativePath(initial_directories_, absolute_directories_);
}
std::string UnixPath::GetAbsolutePath() const {
    return absolute_directories_;
}
std::string UnixPath::GetRelativePath() const {
    return relative_directories_;
}

std::string UnixPath::NormalizePath(const std::string &current_working_directory, const std::string &path) {
    std::string result_path;
    std::string normalized_path;
    size_t last_separation = 0;
    size_t current_separation = 0;
    if (path.front() == SEPARATOR_C) {
        result_path = std::string(path) + SEPARATOR;
    } else {
        result_path = std::string(current_working_directory) + SEPARATOR + std::string(path) + SEPARATOR;
    }
    while ((current_separation = result_path.find(SEPARATOR_C, last_separation + 1)) != std::string::npos) {
        std::string directory = result_path.substr(last_separation + 1, current_separation - last_separation - 1);
        last_separation = current_separation;
        if (directory == PREVIOUS) {
            while (!normalized_path.empty() && normalized_path.back() != SEPARATOR_C) {
                normalized_path.pop_back();
            }
            if (!normalized_path.empty()) {
                normalized_path.pop_back();
            }
            continue;
        }
        if (directory.empty() || directory == CURRENT) {
            continue;
        }
        normalized_path += SEPARATOR + directory;
    }
    if (normalized_path.empty()) {
        normalized_path = SEPARATOR;
    }
    return normalized_path;
}

std::string UnixPath::RelativePath(const std::string &current_working_directory, const std::string &path) {
    std::vector<size_t> current_working_directory_split;
    std::vector<size_t> path_split;
    std::string relative_path = CURRENT;
    size_t common = 0;
    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == SEPARATOR_C) {
            path_split.push_back(i);
        }
    }
    path_split.push_back(path.size());
    for (size_t i = 0; i < current_working_directory.size(); ++i) {
        if (current_working_directory[i] == SEPARATOR_C) {
            current_working_directory_split.push_back(i);
        }
    }
    current_working_directory_split.push_back(current_working_directory.size());
    for (size_t i = 0; i < std::min(path_split.size(), current_working_directory_split.size()); ++i) {
        if (path.substr(0, path_split[i]) == current_working_directory.substr(0, current_working_directory_split[i])) {
            common = i;
        } else {
            break;
        }
    }
    if (common != current_working_directory_split.size() - 1) {
        relative_path = PREVIOUS;
        for (size_t i = 0; i < current_working_directory_split.size() - common - 2; ++i) {
            relative_path.append("/..");
        }
    }
    if (common != path_split.size() - 1) {
        relative_path.append(path.substr(path_split[common], path.size() - path_split[common]));
    }
    return relative_path;
}

#include "unixpath.h"
#include <vector>

void PathStringToPathVector(std::vector<std::string_view>& directories, std::string_view str) {
    for (size_t i = 0; i < str.size(); ++i) {
        const auto it_end = str.find_first_of("/", i);
        if (it_end == std::string::npos) {
            std::string_view current_directory = str.substr(i, str.size() - i);
            directories.push_back(current_directory);
            break;
        }
        if (i != it_end) {
            std::string_view current_directory = str.substr(i, it_end - i);
            directories.push_back(current_directory);
        }
        i = it_end;
    }
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::string normalized_path = "/";
    std::vector<std::string_view> directories;
    if (path.front() == '/') {
        PathStringToPathVector(directories, path);
    } else {
        PathStringToPathVector(directories, current_working_dir);
        for (size_t i = 0; i < path.size(); ++i) {
            const auto it_end = path.find_first_of("/", i);
            if (it_end == std::string::npos) {
                std::string_view current_directory = path.substr(i, path.size() - i);
                if (current_directory == "..") {
                    if (!directories.empty()) {
                        directories.pop_back();
                    }
                } else if (current_directory != ".") {
                    directories.push_back(current_directory);
                }
                break;
            }
            if (i != it_end) {
                std::string_view current_directory = path.substr(i, it_end - i);
                if (current_directory == "..") {
                    if (!directories.empty()) {
                        directories.pop_back();
                    }
                } else if (current_directory != ".") {
                    directories.push_back(current_directory);
                }
            }
            i = it_end;
        }
    }
    for (size_t i = 0; i < directories.size(); ++i) {
        normalized_path += static_cast<std::string>(directories[i]);
        if (i != directories.size() - 1) {
            normalized_path.push_back('/');
        }
    }
    return normalized_path;
}

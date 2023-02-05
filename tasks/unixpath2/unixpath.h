#pragma once

#include <string>
#include <string_view>

class UnixPath {
public:
    explicit UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;

private:
    static std::string NormalizePath(const std::string& current_working_directory, const std::string& path);
    static std::string RelativePath(const std::string& current_working_directory, const std::string& path);
    std::string initial_directories_;
    std::string relative_directories_;
    std::string absolute_directories_;
};

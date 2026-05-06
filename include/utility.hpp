#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "editors.hpp"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace proj {

std::optional<std::filesystem::path> find_project(const std::vector<std::filesystem::path> &scan_directories,
                                                  const std::string &project_name);

std::vector<std::string> list_projects(const std::vector<std::filesystem::path> &scan_directories);

int open_in_editor(Editors editor, const std::filesystem::path &repo_path);

} // namespace proj

#endif // UTILITY_HPP
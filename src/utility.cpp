#include "utility.hpp"
#include "editors.hpp"

#include <cstdlib>
#include <filesystem>
#include <format>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace proj {

std::optional<std::filesystem::path> find_project(const std::vector<std::filesystem::path> &scan_directories,
                                                  const std::string &project_name) {

  for (const auto &root : scan_directories) {
    if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root)) {
      continue;
    }

    for (const auto &entry : std::filesystem::directory_iterator(root)) {
      if (!entry.is_directory()) {
        continue;
      }
      const auto &candidate{entry.path()};
      if (candidate.filename() != project_name) {
        continue;
      }
      const auto git_dir{candidate / ".git"};
      if (std::filesystem::exists(git_dir) && std::filesystem::is_directory(git_dir)) {
        return candidate;
      }
    }
  }

  return std::nullopt;
}

std::vector<std::string> list_projects(const std::vector<std::filesystem::path> &scan_directories) {
  std::set<std::string> project_names;

  for (const auto &root : scan_directories) {
    if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root)) {
      continue;
    }

    for (const auto &entry : std::filesystem::directory_iterator(root)) {
      if (!entry.is_directory()) {
        continue;
      }

      const auto &candidate{entry.path()};
      const auto git_dir{candidate / ".git"};
      if (std::filesystem::exists(git_dir) && std::filesystem::is_directory(git_dir)) {
        project_names.insert(candidate.filename().string());
      }
    }
  }

  return {project_names.begin(), project_names.end()};
}

bool is_editor_available(Editors editor) {
  switch (editor) {
  case Editors::VSCODE:
    return std::system("command -v code >/dev/null 2>&1") == 0 ||
           std::system("open -Ra \"Visual Studio Code\" >/dev/null 2>&1") == 0;
  case Editors::NONE:
  default:
    return false;
  }
}

int open_in_editor(Editors editor, const std::filesystem::path &repo_path) {

  switch (editor) {
  case Editors::VSCODE:
    return std::system(std::format("open -a \"Visual Studio Code\" \"{}\"", repo_path.string()).c_str());
  case Editors::NONE:
  default:
    throw std::runtime_error("No supported editor configured.");
  }

  return 0;
}

} // namespace proj
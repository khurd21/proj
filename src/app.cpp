#include "app.hpp"
#include "cli.hpp"
#include "config_settings.hpp"
#include "utility.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace proj {

namespace {

constexpr auto config_path{"config/proj.yaml"};

std::filesystem::path expand_home_path(const std::filesystem::path &path) {
  const auto raw{path.string()};
  if (raw.empty() || raw[0] != '~') {
    return path;
  }

  const auto *home{std::getenv("HOME")};
  if (home == nullptr || *home == '\0') {
    return path;
  }

  if (raw == "~") {
    return std::filesystem::path{home};
  }

  if (raw.rfind("~/", 0) == 0) {
    return std::filesystem::path{home} / raw.substr(2);
  }

  return path;
}

std::filesystem::path normalize_path(const std::filesystem::path &path) {
  const auto expanded{expand_home_path(path)};
  if (expanded.is_relative()) {
    std::error_code ec;
    const auto absolute{std::filesystem::absolute(expanded, ec)};
    if (!ec) {
      return absolute.lexically_normal();
    }
  }
  return expanded.lexically_normal();
}

bool paths_equal(const std::filesystem::path &lhs, const std::filesystem::path &rhs) {
  return normalize_path(lhs) == normalize_path(rhs);
}

} // namespace

App::App(int argc, char **argv) : m_argc{argc}, m_argv{argv} {}

int App::run() {
  const CLI cli{m_argc, m_argv};
  const auto &cli_args{cli.get_cli_args()};

  const bool has_config_action{cli_args.add_path.has_value() || cli_args.remove_path.has_value() ||
                               cli_args.remove_path_interactive || cli_args.clear_paths ||
                               cli_args.set_editor.has_value()};

  ConfigSettings settings;
  settings.load_from_settings(config_path, !has_config_action);

  if (cli_args.add_path.has_value()) {
    const auto resolved_path{normalize_path(cli_args.add_path.value())};
    if (!std::filesystem::exists(resolved_path) || !std::filesystem::is_directory(resolved_path)) {
      throw std::runtime_error("path does not exist or is not a directory: " + resolved_path.string());
    }

    const auto existing{std::find_if(settings.scan_directories.begin(), settings.scan_directories.end(),
                                     [&](const auto &scan_path) { return paths_equal(scan_path, resolved_path); })};
    if (existing != settings.scan_directories.end()) {
      throw std::runtime_error("path already exists in config: " + resolved_path.string());
    }

    settings.scan_directories.push_back(resolved_path);
    settings.save_to_settings(config_path);
    std::cout << "Added path: " << resolved_path.string() << '\n';
    return 0;
  }

  if (cli_args.remove_path.has_value() || cli_args.remove_path_interactive) {
    if (settings.scan_directories.empty()) {
      throw std::runtime_error("no configured paths to remove");
    }

    std::filesystem::path to_remove;
    if (cli_args.remove_path.has_value()) {
      to_remove = normalize_path(cli_args.remove_path.value());
    } else {
      std::cout << "Configured paths:\n";
      for (std::size_t i = 0; i < settings.scan_directories.size(); ++i) {
        std::cout << (i + 1) << ") " << settings.scan_directories[i].string() << '\n';
      }

      std::cout << "Select path number or path to remove: ";

      std::string selection;
      std::getline(std::cin >> std::ws, selection);
      if (selection.empty()) {
        throw std::runtime_error("invalid selection for path removal");
      }

      const bool is_number{
          std::all_of(selection.begin(), selection.end(), [](const unsigned char c) { return std::isdigit(c) != 0; })};

      if (is_number) {
        std::size_t index{};
        try {
          index = static_cast<std::size_t>(std::stoull(selection));
        } catch (...) {
          throw std::runtime_error("invalid selection for path removal");
        }

        if (index == 0 || index > settings.scan_directories.size()) {
          throw std::runtime_error("invalid selection for path removal");
        }

        to_remove = settings.scan_directories[index - 1];
      } else {
        to_remove = normalize_path(std::filesystem::path{selection});
      }
    }

    const auto before{settings.scan_directories.size()};
    settings.scan_directories.erase(
        std::remove_if(settings.scan_directories.begin(), settings.scan_directories.end(),
                       [&](const auto &scan_path) { return paths_equal(scan_path, to_remove); }),
        settings.scan_directories.end());

    if (settings.scan_directories.size() == before) {
      throw std::runtime_error("path not found in config: " + to_remove.string());
    }

    settings.save_to_settings(config_path);
    std::cout << "Removed path: " << to_remove.string() << '\n';
    return 0;
  }

  if (cli_args.clear_paths) {
    settings.scan_directories.clear();
    settings.save_to_settings(config_path);
    std::cout << "Cleared all scan directories\n";
    return 0;
  }

  if (cli_args.set_editor.has_value()) {
    const auto editor{editors_from_string(cli_args.set_editor.value())};
    if (!is_editor_available(editor)) {
      throw std::runtime_error("requested editor is not available on this system: " + cli_args.set_editor.value());
    }

    settings.editor = editor;
    settings.save_to_settings(config_path);
    std::cout << "Updated editor: " << settings.editor << '\n';
    return 0;
  }

  if (cli_args.view_settings) {
    std::cout << settings << '\n';
    return 0;
  }

  if (cli_args.list_projects) {
    const auto projects{list_projects(settings.scan_directories)};
    for (const auto &project : projects) {
      std::cout << project << '\n';
    }
    return 0;
  }

  const auto project{find_project(settings.scan_directories, cli_args.project_name)};
  if (project.has_value()) {
    return open_in_editor(settings.editor, project.value());
  }
  throw std::runtime_error("could not find project name `" + cli_args.project_name + "` in scan directories.");
}

} // namespace proj

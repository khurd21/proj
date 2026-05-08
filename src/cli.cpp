#include "cli.hpp"

#include <CLI/CLI.hpp>

#include <filesystem>
#include <stdexcept>

namespace {

using App = CLI::App;

}

namespace proj {

CLI::CLI(int argc, char **argv) {
  App app{"Open a configured git project in your editor."};

  std::filesystem::path add_path;
  std::string remove_path_value;
  std::string set_editor;

  app.add_option("project-name", m_args.project_name, "Name of the project to open.");
  app.add_flag("--list-projects", m_args.list_projects,
               "List configured project names (for shell completion integration).");
  app.add_flag("--view-settings", m_args.view_settings, "Print the configured scan directories and editor.");
  app.add_option("--add-path", add_path, "Add a scan directory path to config/proj.yaml.");
  auto *remove_path_option =
      app.add_option("--remove-path", remove_path_value,
                     "Remove a scan directory path from config/proj.yaml. If omitted, prompts interactively.")
          ->expected(0, 1);
  app.add_flag("--clear-paths", m_args.clear_paths, "Clear all scan directory paths from config/proj.yaml.");
  app.add_option("--set-editor", set_editor, "Set editor in config/proj.yaml (currently: vscode).");

  app.parse(argc, argv);

  if (add_path != std::filesystem::path{}) {
    m_args.add_path = add_path;
  }

  if (remove_path_option->count() > 0 && remove_path_value.empty()) {
    m_args.remove_path_interactive = true;
  }
  if (!remove_path_value.empty()) {
    m_args.remove_path = std::filesystem::path{remove_path_value};
  }

  if (!set_editor.empty()) {
    m_args.set_editor = set_editor;
  }

  const bool has_config_action = m_args.add_path.has_value() || m_args.remove_path.has_value() ||
                                 m_args.remove_path_interactive || m_args.clear_paths || m_args.set_editor.has_value();

  const int action_count = (m_args.add_path.has_value() ? 1 : 0) + (m_args.remove_path.has_value() ? 1 : 0) +
                           (m_args.remove_path_interactive ? 1 : 0) + (m_args.clear_paths ? 1 : 0) +
                           (m_args.set_editor.has_value() ? 1 : 0);

  if (action_count > 1) {
    throw std::runtime_error("use only one config edit action at a time");
  }

  if (has_config_action && (!m_args.project_name.empty() || m_args.list_projects || m_args.view_settings)) {
    throw std::runtime_error(
        "config edit actions cannot be combined with project-name, --list-projects, or --view-settings");
  }

  if (m_args.project_name.empty() && !m_args.list_projects && !m_args.view_settings && !has_config_action) {
    throw std::runtime_error("project-name is required unless --list-projects or --view-settings is provided");
  }
}

const CLIArgs &CLI::get_cli_args() const { return m_args; }

} // namespace proj
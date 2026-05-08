#include "app.hpp"
#include "cli.hpp"
#include "config_settings.hpp"
#include "utility.hpp"

#include <iostream>
#include <stdexcept>

namespace proj {

App::App(int argc, char **argv) : m_argc{argc}, m_argv{argv} {}

int App::run() {
  const CLI cli{m_argc, m_argv};
  const auto &cli_args{cli.get_cli_args()};

  if (cli_args.completion_shell == "zsh") {
    std::cout << R"(#compdef proj

_proj() {
  local context state line
  typeset -A opt_args
  local -a projects
  projects=("${(@f)$(proj --list-projects 2>/dev/null)}")

  _arguments -C \
    '--list-projects[List configured git repositories]' \
    '--completion[Print completion script]:shell:(zsh)' \
    '1:project name:->project'

  case $state in
    project)
      compadd -a projects
      ;;
  esac
}

compdef _proj proj
)";
    return 0;
  }

  const ConfigSettings settings{"config/proj.yaml"};

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

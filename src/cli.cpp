#include "cli.hpp"

#include <CLI/CLI.hpp>

#include <stdexcept>

namespace {

using App = CLI::App;

}

namespace proj {

CLI::CLI(int argc, char **argv) {
  App app{"Open a configured git project in your editor."};

  app.add_option("project-name", m_args.project_name, "Name of the project to open.");
  app.add_flag("--list-projects", m_args.list_projects,
               "List configured project names (for shell completion integration).");
  app.add_option("--completion", m_args.completion_shell, "Print shell completion script. Supported values: zsh.");

  app.parse(argc, argv);

  if (!m_args.completion_shell.empty() && m_args.completion_shell != "zsh") {
    throw std::runtime_error("unsupported completion shell: " + m_args.completion_shell);
  }

  if (m_args.project_name.empty() && !m_args.list_projects && m_args.completion_shell.empty()) {
    throw std::runtime_error("project-name is required unless --list-projects or --completion is provided");
  }
}

const CLIArgs &CLI::get_cli_args() const { return m_args; }

} // namespace proj
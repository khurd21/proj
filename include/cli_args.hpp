#ifndef CLI_ARGS_HPP
#define CLI_ARGS_HPP

#include <filesystem>
#include <optional>
#include <ostream>
#include <string>

namespace proj {

struct CLIArgs {
  std::string project_name;
  bool list_projects{false};
  bool view_settings{false};
  std::optional<std::filesystem::path> add_path;
  std::optional<std::filesystem::path> remove_path;
  bool remove_path_interactive{false};
  bool clear_paths{false};
  std::optional<std::string> set_editor;
};

} // namespace proj

inline std::ostream &operator<<(std::ostream &os, const proj::CLIArgs &args) {
  os << "CLIArgs{ .project_name=" << args.project_name << ", .list_projects=" << std::boolalpha << args.list_projects
     << ", .view_settings=" << args.view_settings
     << ", .add_path=" << (args.add_path.has_value() ? args.add_path->string() : "<none>")
     << ", .remove_path=" << (args.remove_path.has_value() ? args.remove_path->string() : "<none>")
     << ", .remove_path_interactive=" << args.remove_path_interactive << ", .clear_paths=" << args.clear_paths
     << ", .set_editor=" << (args.set_editor.has_value() ? args.set_editor.value() : "<none>") << " }";
  return os;
}

#endif // CLI_ARGS_HPP
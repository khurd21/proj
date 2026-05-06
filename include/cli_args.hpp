#ifndef CLI_ARGS_HPP
#define CLI_ARGS_HPP

#include <ostream>
#include <string>

namespace proj {

struct CLIArgs {
  std::string project_name;
  bool list_projects{false};
  std::string completion_shell;
};

} // namespace proj

inline std::ostream &operator<<(std::ostream &os, const proj::CLIArgs &args) {
  os << "CLIArgs{ .project_name=" << args.project_name << ", .list_projects=" << std::boolalpha << args.list_projects
     << ", .completion_shell=" << args.completion_shell << " }";
  return os;
}

#endif // CLI_ARGS_HPP
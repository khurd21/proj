#ifndef CLI_HPP
#define CLI_HPP

#include "cli_args.hpp"

#include <stdexcept>
#include <string>

namespace proj {

class CLIExit : public std::runtime_error {
public:
  CLIExit(int exit_code, const std::string &message);

  int exit_code() const noexcept;

private:
  int m_exit_code{};
};

class CLI {
public:
  CLI(int argc, char **argv);

  const CLIArgs &get_cli_args() const;

private:
  CLIArgs m_args;
};

} // namespace proj

#endif // CLI_HPP
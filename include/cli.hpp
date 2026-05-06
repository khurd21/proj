#ifndef CLI_HPP
#define CLI_HPP

#include "cli_args.hpp"

#include <string>

namespace proj {

class CLI {
public:
  CLI(int argc, char **argv);

  const CLIArgs &get_cli_args() const;

private:
  CLIArgs m_args;
};

} // namespace proj

#endif // CLI_HPP
#ifndef EDITORS_HPP
#define EDITORS_HPP

#include <algorithm>
#include <cctype>
#include <ostream>
#include <stdexcept>
#include <string>

namespace proj {

enum class Editors {
  VSCODE,
  NONE,
};

inline Editors editors_from_string(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(), [](auto c) { return static_cast<char>(std::tolower(c)); });
  if (value == "vscode") {
    return Editors::VSCODE;
  }
  throw std::runtime_error("Unsupported editor in config: " + value);
}

} // namespace proj

inline std::ostream &operator<<(std::ostream &os, const proj::Editors &editor) {
  switch (editor) {
  case proj::Editors::VSCODE:
    os << "vscode";
    break;
  case proj::Editors::NONE:
    os << "none";
    break;
  default:
    os << "unknown";
    break;
  }
  return os;
}

#endif // EDITORS_HPP
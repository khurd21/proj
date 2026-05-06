#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include "editors.hpp"

#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

namespace proj {

struct ConfigSettings {
  ConfigSettings() = default;
  ConfigSettings(const std::filesystem::path &config_file);

  std::vector<std::filesystem::path> scan_directories;
  Editors editor{Editors::NONE};

  void load_from_settings(const std::filesystem::path &config_file);
};

} // namespace proj

inline std::ostream &operator<<(std::ostream &os, const proj::ConfigSettings &settings) {
  os << "Settings{ .scan_directories=[";
  auto first{true};
  for (const auto &directory : settings.scan_directories) {
    if (!first) {
      os << ", ";
    }
    os << directory.string();
    first = false;
  }
  os << "]";
  os << ", .editor=" << settings.editor << " }";
  return os;
}

#endif // ARGUMENTS_HPP
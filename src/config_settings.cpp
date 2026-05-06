#include "config_settings.hpp"
#include "editors.hpp"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>

namespace proj {

ConfigSettings::ConfigSettings(const std::filesystem::path &config_file) { load_from_settings(config_file); }

void ConfigSettings::load_from_settings(const std::filesystem::path &config_file) {
  const auto root{YAML::LoadFile(config_file.string())};
  const auto scan_dirs{root["scan_directories"]};
  if (!scan_dirs || !scan_dirs.IsSequence()) {
    throw std::runtime_error("config error: scan_directories must be a sequence.");
  }

  this->scan_directories.clear();
  for (const auto &entry : scan_dirs) {
    if (!entry.IsScalar()) {
      throw std::runtime_error("config error: each scan_directories entry must be a string.");
    }
    this->scan_directories.emplace_back(entry.as<std::string>());
  }

  if (this->scan_directories.empty()) {
    throw std::runtime_error("config error: scan_directories cannot be empty.");
  }

  const auto editor_node{root["editor"]};
  if (!editor_node || !editor_node.IsScalar()) {
    throw std::runtime_error("config error: editor must be a string.");
  }
  this->editor = editors_from_string(editor_node.as<std::string>());
}

} // namespace proj
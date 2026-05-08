#include "config_settings.hpp"
#include "editors.hpp"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace proj {

ConfigSettings::ConfigSettings(const std::filesystem::path &config_file) { load_from_settings(config_file); }

void ConfigSettings::load_from_settings(const std::filesystem::path &config_file, const bool strict) {
  const auto root{YAML::LoadFile(config_file.string())};
  const auto scan_dirs{root["scan_directories"]};
  if (strict && (!scan_dirs || !scan_dirs.IsSequence())) {
    throw std::runtime_error("config error: scan_directories must be a sequence.");
  }

  this->scan_directories.clear();
  if (scan_dirs && scan_dirs.IsSequence()) {
    for (const auto &entry : scan_dirs) {
      if (!entry.IsScalar()) {
        if (strict) {
          throw std::runtime_error("config error: each scan_directories entry must be a string.");
        }
        continue;
      }

      const auto value{entry.as<std::string>()};
      if (value.empty()) {
        if (strict) {
          throw std::runtime_error("config error: scan_directories entries cannot be empty.");
        }
        continue;
      }

      this->scan_directories.emplace_back(value);
    }
  }

  if (strict && this->scan_directories.empty()) {
    throw std::runtime_error("config error: scan_directories cannot be empty.");
  }

  const auto editor_node{root["editor"]};
  if (!editor_node || !editor_node.IsScalar()) {
    throw std::runtime_error("config error: editor must be a string.");
  }
  this->editor = editors_from_string(editor_node.as<std::string>());
}

void ConfigSettings::save_to_settings(const std::filesystem::path &config_file) const {
  YAML::Node root;
  root["scan_directories"] = YAML::Node(YAML::NodeType::Sequence);
  for (const auto &path : scan_directories) {
    root["scan_directories"].push_back(path.string());
  }
  root["editor"] = to_string(editor);

  std::ofstream output{config_file};
  if (!output) {
    throw std::runtime_error("failed to write config file: " + config_file.string());
  }

  output << root;
}

} // namespace proj
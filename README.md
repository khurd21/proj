# proj

A macOS CLI tool to quickly open configured local git repositories in your editor of choice.

## Installation

```bash
brew tap khurd21/proj
brew install khurd21/proj/proj
```

## Setup

After installing, you need at least one scan directory configured. You can either edit the config file directly or use the CLI.

**Option 1 — CLI:**
```bash
proj --add-path ~/Code
```

**Option 2 — Edit config directly:**

The config file lives at `$(brew --prefix)/var/proj/config/proj.yaml`:
```yaml
scan_directories:
  - ~/Code
  - ~/Projects

editor: vscode
```

## CLI Reference

### Show help

```bash
proj --help
```

Prints command usage, options, and examples.

### Open a project

```bash
proj <project-name>
```

Scans configured directories for a git repo matching `<project-name>` and opens it in your configured editor.

### List discovered projects

```bash
proj --list-projects
```

Prints all git repository names found in your scan directories.

### View current settings

```bash
proj --view-settings
```

Prints the configured scan directories and editor.

### Manage scan directories

```bash
proj --add-path <path>        # Add a scan directory
proj --remove-path <path>     # Remove a specific path
proj --remove-path            # Interactively select a path to remove
proj --clear-paths            # Remove all configured scan directories
```

### Change editor

```bash
proj --set-editor vscode
```

Validates that the editor is available on your system before saving.

### Supported editors

- `vscode`

## Troubleshooting zsh completion

If completion does not load in a shell session, initialize zsh completion and retry:

```bash
autoload -Uz compinit
compinit -i
```

## License

MIT

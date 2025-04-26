# note-db

A C++ high-performance local knowledge base for managing Markdown notes with full-text search, tagging, snapshots, backups, and more — all from your terminal.

---

## 🧰 Features

- `init <directory>`: Initialize a notes directory with .templates and .snapshots, and setup config/commands.json.
- `new <filename>`: Create a new note with optional flags (`--template`, `--editor`, `--overwrite`, `--directory`).
- `edit <filename>`: Open a note in your editor and update its metadata (`--editor`, `--directory`).
- `delete <filename>`: Delete a note (`--directory`).
- `list`: Show all notes.
- `search <query>`: Full-text search with filters (`--tag`, `--limit`, `--sort-by`).
- `tag <filename> <tag>`: Add a tag to a note (`--directory`).
- `import <filepath>`: Import a Markdown file from elsewhere (`--overwrite`, `--directory`).
- `snapshot`: Save a snapshot of current note states.
- `diff <snapshot>`: Compare current notes to a previous snapshot.
- `backup`: Save a full backup (content + metadata) of your notes.
- `restore <timestamp>`: Restore your notes from a previous backup.
- `template <filename>`: Create a new template Markdown file.
- `run <command>`: Run a custom automation defined in `~/.note-db/commands.json`, with automatic rollback on failure.
- `help` / `--help`: Display usage guide.
- `--version`: Display the current version.

---

## 🛠 Installation

### Build from source

```bash
git clone https://github.com/matthewwangg/note-db.git
cd note-db
./build.sh
```

This script will:
- Create the build directory
- Run CMake with testing disabled
- Build the project using all available cores
- Optionally install the `note-db` binary to `/usr/local/bin` (if `make` succeeds)

> 🔧 Requires: CMake 3.27+, a C++20-compatible compiler (e.g., GCC 11+ or Clang 13+)

---

## 🧪 Usage

Initialize your notes vault:

```bash
note-db init ~/vault
```

Create a new note:

```bash
note-db new my-note.md --template default --editor vim
```

> 💡 You can also set the environment variable `NOTEDB_EDITOR` to change the default editor (defaults to `nano`):

```bash
export NOTEDB_EDITOR=vim
```

Edit a note:

```bash
note-db edit my-note.md --editor nano
```

Delete a note:

```bash
note-db delete my-note.md
```

Tag a note:

```bash
note-db tag my-note.md c++
```

Import an existing file:

```bash
note-db import ~/Downloads/lecture.md --overwrite
```

Search notes:

```bash
note-db search algorithms --tag graph --limit 10 --sort-by updated
```

Snapshot notes:

```bash
note-db snapshot
```

Diff with a previous snapshot:

```bash
note-db diff 1713418010.json
```

Backup all notes:

```bash
note-db backup
```

Restore notes from a backup:

```bash
note-db restore 1713418010
```

Create a new template:

```bash
note-db template cs-patterns.md
```

Run a custom workflow:

```bash
note-db run daily
```

> 💡 `run` automatically creates a backup before executing commands. If any step fails, note-db will roll back to the previous backup.

> 💡 You can define custom commands in `.note-db/commands.json` and run them with `note-db run <command>`

```json
{
  "daily": {
    "description": "Create a new note for today using the daily template",
    "steps": [
      "new {{date}}.md --template daily --directory daily --editor none",
      "tag {{date}}.md daily --directory daily",
      "edit {{date}}.md daily --directory daily"
    ]
  }
}
```


Display help:

```bash
note-db help
note-db --help
```

Check version:

```bash
note-db --version
```

---

## ⚙️ Configuration

note-db stores your config at:

```plaintext
~/.note-db/config.json
```

Example contents:

```json
{
  "root": "/absolute/path/to/your/notes"
}
```

---

## 📁 Notes Structure

```
vault/
├── my-note.md
├── templates/
│   └── default.md
├── snapshots/
│   └── 1713418010.json
└── .backups/
    └── 1713418010.json
```

Each note is a Markdown file with frontmatter:

```markdown
---
title: My Note
created: 2025-04-17
updated: 2025-04-17
tags: [c++, cli]
---
This is the content of the note.
```

---

## 📦 Roadmap

- [ ] Semantic search (e.g., embedding-based)
- [ ] Archive/restore notes
- [ ] Encrypted notes (e.g., GPG integration)
- [ ] Git-based syncing or backup support

---

## 🧠 Philosophy

note-db is designed to be keyboard-first and scriptable — for developers who live in the terminal.

---

## 🪪 License
MIT © 2025 Matthew Wang


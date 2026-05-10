# Terminal Text Editor (Myst Editor)

A terminal based text editor built with C++ and ncurses.
The editor focuses on fast keyboard driven editing, customizable themes, and lightweight performance in terminal environments.

## Overview

Myst is designed for efficient text editing directly in the terminal.
It handles user input, rendering, file operations, and a built-in command runner without relying on external editors, with a focus on responsiveness and control.

It also includes a terminal-style output panel for running system commands inside the editor.

## Core Features

- Keyboard driven editing workflow
- Syntax highlighting for Python and C/C++
- Theme system with configurable color schemes
- Multi file navigation and switching
- Save and Save As functionality
- Configurable user preferences
- Lightweight and fast execution
- Built-in command runner with output panel
- Terminal-style command history stack
- Scrollable command output rendering

## Screenshots

![Logo](resources/screenshots/Logo.png)
![Code](resources/screenshots/Code.png)
![Settings](resources/screenshots/Settings.png)

## Technical Details

### Input System

- Uses ncurses for raw keyboard and mouse input handling
- Processes key events for navigation, editing commands, and UI panels
- Supports special key handling for arrow keys, escape, and control shortcuts
- Mouse input support used for cursor interaction and UI selection

### Text Buffer

- Manages text using a structured in memory buffer
- Handles insertion, deletion, and navigation efficiently
- Maintains cursor position and viewport state

### Rendering

- Uses ncurses for terminal rendering
- Redraws only necessary regions for performance
- Applies syntax highlighting during render pass
- Supports multiple windows, pads, and panels for UI separation

### Panels and UI System

- Uses ncurses panels for layered UI elements
- Editor, file menu, status bar, and command output are separated into independent views
- Command output is rendered in a dedicated bottom panel

### Built-in Terminal Command System

- Runs system commands using popen
- Captures stdout and stderr output
- Command results are displayed in a scrollable terminal panel
- Supports wrapping long output lines based on screen width

### Syntax Highlighting

- Token based parsing for Python and C/C++
- Identifies keywords, strings, and comments
- Applies color mapping based on theme configuration

### Configuration

- Reads user preferences from configuration files
- Supports theme switching and editor behavior customization
- Applies settings at startup

## Requirements

- C++17 compatible compiler
- ncurses
- make
- xclip

## Build

Clone the repository:

git clone [https://github.com/Mysty-exe/Myst.git](https://github.com/Mysty-exe/Myst.git)
cd Myst

Build and run:

make

## Run

./build/myst

Open a file:

./build/myst filename.txt

## Controls

General

- Ctrl Q: quit
- Ctrl S: save
- Ctrl E: open menu
- Ctrl T: see files in directory
- Ctrl P: preferences
- Ctrl R: command line

Navigation

- Ctrl B: previous file
- Ctrl N: next file

Editing

- Ctrl F: find and replace
- Ctrl /: comment out lines

Command System

- Enter command in status bar input
- Command output appears in bottom panel
- Scroll through output history stack

## Project Structure

src/ core editor logic
include/ headers
config/ user settings
resources/ themes and assets
build/ generated files

## Future Work

- Support for more programming languages
- Improved syntax parsing system
- Plugin system for extensions
- Persistent integrated terminal
- Improved mouse driven UI interactions

## Notes

- Designed for Linux terminal environments
- Requires proper ncurses support

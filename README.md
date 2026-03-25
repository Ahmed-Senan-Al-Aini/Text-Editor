# Simple C++ Text Editor

![Project Cover](assets/cover.png)

## Overview

This project is a simple console-based text editor developed using the C++ programming language. It provides fundamental text editing functionalities such as insertion, deletion, undo, and redo, along with the ability to save and load files.

## Features

*   **Text Insertion**: Insert text at the current cursor position.
*   **New Line Insertion**: Add a new line anywhere within the text.
*   **Character and Line Deletion**: Delete a single character or an entire line.
*   **Cursor Navigation**: Move the cursor easily within the text.
*   **Undo/Redo**: Full support for undoing and redoing changes.
*   **File Save/Load**: Save the current editor content to a text file and load content from an existing file.
*   **Search and Replace**: Search for specific text and replace it with new text (with an option to replace all occurrences).
*   **Statistics**: Display text statistics, such as word and character counts.

## Data Structure

The editor relies on a **Doubly Linked List** to represent text lines and utilizes two **Stacks** to implement the undo and redo functionalities. The following diagram illustrates the general data structure used in the editor:

![Data Structure Diagram](assets/data_structure.png)

## How to Run

To run this project, you will need a C++ compiler (such as g++) and a command-line environment or an Integrated Development Environment (IDE).

### Prerequisites

*   A C++ compiler (e.g., `g++`).
*   An operating system that supports a command-line interface (Linux, macOS, Windows).

### 1. Compiling and Running (Development Environment)

#### Using g++ (Linux/macOS/Windows - MinGW/Cygwin)

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/Ahmed-Senan-Al-Aini/Text-Editor.git
    cd Text-Editor
    ```

2.  **Compile**:
    Use the following command to compile the source files:
    ```bash
    g++ main.cpp editor.cpp -o text_editor
    ```
    This command will create an executable file named `text_editor`.

3.  **Run**:
    After successful compilation, you can run the editor using:
    ```bash
    ./text_editor
    ```

#### Using Visual Studio (Windows)

1.  **Create a New Project**: Open Visual Studio and create a new **Console App** project (C++).
    ![Visual Studio - Create New Project](assets/vs_setup_1.png)
2.  **Add Source Files**: Add `main.cpp`, `editor.cpp`, and `editor.h` to your project.
3.  **Compile and Run**: Build the project (Ctrl+Shift+B) and then run it (F5 or Ctrl+F5).
    ![Visual Studio - Build and Run](assets/vs_setup_2.png)

#### Using Dev-C++ (Windows)

1.  **Create a New Project**: Open Dev-C++ and create a new **Console Application** project.
    ![Dev-C++ - Create New Project](assets/dev_cpp_setup.jpg)
2.  **Add Source Files**: Add `main.cpp`, `editor.cpp`, and `editor.h` to your project.
3.  **Compile and Run**: Compile the project (F9) and then run it (F10).

### 2. Running as a Standalone Program (Production)

Once the project is compiled, the executable file (`text_editor` on Linux/macOS/MinGW, or `text_editor.exe` on Windows) becomes a standalone program. You can distribute and run it directly on any compatible system.

1.  **Transfer the Executable**: Move the `text_editor` (or `text_editor.exe`) file to the desired location.
2.  **Run**: Execute the file directly:
    ```bash
    /path/to/your/text_editor
    ```

## Usage

When the editor runs, a menu of available options will be displayed. You can enter the corresponding numbers for commands to execute them. For example:

*   `1`: Insert text.
*   `2`: Delete character.
*   `16`: Save file.
*   `18`: Exit editor.

## Contributing

Contributions to this project are welcome. If you have any suggestions or improvements, feel free to open an issue or submit a pull request.
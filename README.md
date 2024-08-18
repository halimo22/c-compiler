# Syntax and Semantic Analyzer

## Overview

This project is a compiler toolchain consisting of a Lexical Analyzer, Syntax Analyzer, and Semantic Analyzer for a simple programming language. The analyzers parse and validate the source code, generating parse trees and checking for semantic correctness. The tools are implemented in C and are designed to provide detailed analysis reports.

## Features

- **Lexical Analysis**: Tokenizes the input source code, identifying keywords, identifiers, numbers, strings, and special symbols.
- **Syntax Analysis**: Parses tokens into a hierarchical parse tree based on predefined grammar rules.
- **Semantic Analysis**: Validates the semantic correctness of the code, including type checking and identifier resolution.
- **Error Reporting**: Captures and reports syntax and semantic errors with detailed messages.

## Prerequisites

- C Compiler (e.g., GCC)
- Text Editor or IDE (e.g., VSCode, Sublime Text)
- Basic knowledge of C programming and compiler theory

## Installation

1. **Clone the Repository**

    ```bash
    git clone https://github.com/yourusername/syntax-semantic-analyzer.git
    cd syntax-semantic-analyzer
    ```

2. **Build the Project**

    Compile the source files using a C compiler. For example, using GCC:

    ```bash
    gcc -o analyzer main.c
    ```

3. **Prepare Test Files**

    Place your source code files (e.g., `test.c`) in the project directory.

## Usage

1. **Run the Analyzer**

    Execute the compiled program and provide the input source file:

    ```bash
    ./analyzer
    ```

    The program will read from `test.c` and generate output files.

2. **Check Output**

    - **Lexical Output**: Check `Lexical Output.txt` for tokenized output.
    - **Syntax Output**: Check `Syntax Output.txt` for syntax analysis results and parse tree.
    - **Semantic Output**: Check `Semantic Output.txt` for semantic analysis results and parse tree.

## Files

- **`main.c`**: The main file containing the entry point and logic for lexical, syntax, and semantic analysis.
- **`lexical.c`**: Contains functions for lexical analysis.
- **`syntax.c`**: Contains functions for syntax analysis.
- **`semantic.c`**: Contains functions for semantic analysis.
- **`README.md`**: This documentation file.

## Contributing

1. **Fork the Repository**

    Click the "Fork" button on the top-right corner of the repository page.

2. **Create a New Branch**

    ```bash
    git checkout -b feature/your-feature
    ```

3. **Make Your Changes**

    Edit or add files as necessary.

4. **Commit Your Changes**

    ```bash
    git add .
    git commit -m "Add your message here"
    ```

5. **Push to the Branch**

    ```bash
    git push origin feature/your-feature
    ```

6. **Create a Pull Request**

    Go to the repository on GitHub and open a pull request from your branch.
---

For any questions or issues, please open an issue or contact me directly.



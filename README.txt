Hello, I am young programmer from kazakhstan.
I am trying to learn system programming so I am making my interpreter on C.

Project overview
This project is a custom, lightweight interpreter that parses and evaluates mathematical expressions. Instead of using high-level libraries, I am implementing everything from the ground up to master memory management, pointers, and language design.

Current features
* **Tokenization (Lexer): ** Splits input string into structured tokens with validation.
* **Recursive Descent Parser: ** Handles operator precedence  ($*$ and $/$ before $+$ and $-$) beautifully.
* **Interactive REPL: ** A live terminal interface with Windows UTF-8 support for smooth, real time testing.
* **Robust Syntax Validation: ** Catches edge cases and formatting errors.

Tech Stack & Constraints
* ** Language: ** Pure C (gcc)
* ** Architecture: ** Modular design
* ** Syntax rule: ** Explicit spacing is required between tokens by design (e.g., '7 + 8')

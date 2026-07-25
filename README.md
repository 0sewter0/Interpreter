# Custom Lightweight C interpreter

A zero-dependency, modular programming language interpreter built completely from scratch in C. Designed to demonstrate systems programming concepts: custom lexing, recursive descent parsing, dynamic heap allocation, call stack-based scoping, and I/O integration.

---

## Key Features
* **Custom Tokenizer (lexer):** Converts source code into structured tokens with built-in validation and detailed syntax error reporting.
* **Recursive Descent Parser:** Supports full operator precedence, complex expressions, dynamic arrays, and nested statement evaluation.
* **Call Stack & Scoping:** Implements scoped variables and multi-frame call stacks (`call_stack`) for user-defined functions and dynamic variable resolution.
* **Heap-based Dynamic Arrays:** Features dynamic collection handling (`malloc` / `realloc`) with strict bounds checking and memory safety guarantees.
* **Control Flow & Functions:** Supports conditional branching, subroutines, user-defined functions with parameter passing, and recursive calls.
* **Built-in Standard I/O:** Built-in primitives for console input (`read()`), output(`write()`), and pseudo-random generation(`random()`).
* **Interactive REPL & CLI:** Built-in read-eval-print loop with multi-platform UTF-8 terminal support for real-time code evaluation.

---

## Code example
Here is a snippet showing syntax capabilities, including functions, dynamic arrays, and control flow:

```c
//example program in custom language syntax:
num = [1, 2, 3, 4, 5]
let i = 0
let count = 5
let sum = 0

while(i < count) {
    sum = sum + num[i]
    i = i + 1
}

## How to run:

**gcc main.c lexer.c parser.c -o name.exe**

Windows:
./name.exe

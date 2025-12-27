# C-Tail: Tail Implementation

![Build Status](https://github.com/MarekHusenica/tail_c/actions/workflows/tests.yml/badge.svg)
![Language](https://img.shields.io/badge/language-C11-blue.svg)
![Testing](https://img.shields.io/badge/tests-PyTest-green.svg)

A Unix-style `tail` utility written in **C11**, designed with memory efficiency and modern DevOps practices in mind. This project implements the core logic of reading the final $N$ lines of a file using a custom circular buffer.



## Key Features
* **Memory Efficient:** Utilizes a custom circular buffer to ensure $O(1)$ space complexity relative to the number of lines being tracked.
* **Hybrid Testing Suite:** Uses **Python (PyTest)** to perform black-box integration testing, comparing binary output against the standard system `tail`.
* **Automated CI/CD:** Integrated with **GitHub Actions** to trigger automated builds and test execution on every push.
* **Robust Build System:** Managed via a `Makefile` for streamlined compilation and environment setup.

## Tech Stack
* **Core:** C11, GCC
* **Testing:** Python 3, PyTest, Subprocess API
* **Automation:** GNU Make, GitHub Actions
* **Environment:** Linux / WSL

## Project Structure
```text
├── src/                 # C source files (tail.c, circ_buffer.c)
├── tests/               # Python integration tests
├── Makefile             # Build and test automation
└── .github/workflows/   # CI/CD pipeline configuration
```
### TODO: Add valgrind memory leak testing
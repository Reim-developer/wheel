# Project Structure & Technical Notes

## High-level modules

- `wheel_utils`: Header-only utility layer for shared helpers (currently logging/config macros).
- `wheel_lexer`: Header-only lexer subsystem (`Lexer`, token model, cursor, handlers, source map).
- `wheel_parser`: Static library that depends on lexer and utils; currently minimal parser skeleton.
- `analyzer`: Static library for analysis logic (currently tab-related checks).
- `tests`: CMake test harness + unit tests for lexer/analyzer/parser.
- `scripts`: Python build/test scripts.

## Build graph

Root CMake config adds subdirectories in this order:

1. `wheel_utils`
2. `analyzer`
3. `wheel_lexer`
4. `wheel_parser`
5. executable `wheel`
6. `tests`

Main app links: `wheel_lexer`, `wheel_utils`, `analyzer`, `wheel_parser`.

## Current technical characteristics

- The lexer is the most feature-complete part, with token handlers and source mapping.
- The parser currently only exposes constructor-level scaffolding (`WheelParser`).
- Test targets are marked `EXCLUDE_FROM_ALL`, and are usually built by explicit targets/scripts.
- Python scripts are intended to streamline build/testing workflows.

## Risks / improvement opportunities

1. **Python script syntax issues**: build helper scripts currently contain f-string formatting bugs that break execution.
2. **Header/library declaration mismatch**: `wheel_utils/CMakeLists.txt` references `include/logging.hxx`, while source tree uses `include/wheel_utils/logging.hxx`.
3. **Namespace macro inconsistency risk**: lexer header closes namespace with `END_NAMESPACE`, while opening macro is `WHEEL_LEXER_NAMESPACE`.
4. **Compiler selection timing**: root CMake sets `CMAKE_CXX_COMPILER` after `project()`, which is too late for normal CMake toolchain selection.
5. **Warning pressure**: lexer handlers generate repeated `always_inline` warnings with GCC.

## Suggested near-term roadmap

- Fix Python scripts first (`scripts/build.py`, `scripts/run_test.py`) to make daily workflows reliable.
- Normalize CMake include file lists and compiler/toolchain configuration.
- Expand parser implementation + parser tests to reduce architecture imbalance.
- Add CI gates for script linting + warnings policy.

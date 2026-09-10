# To do

- [X] Read file by characters
- [X] Create a map counter for each characters
- [X] Create a Huffman table from the map counter
- [X] Encode the file content to text data using the table
- [X] Read the encode text to reconstruct content from a table
- [X] Create a compressed text which include the table and encoded content
- [X] Read the compressed text to extract the table, and the content
- [X] Output the content from the compressed text
- [X] Link the functions cleanly to the CLI loop
- [X] Refactor from using text content to a binary file

# Code Review Improvements

## Bugs
- [X] Fix delimiter mismatch: encoder writes `;;` but decoder searches for `;;;` (main.cpp:200 vs 220)
- [X] Add missing `i++` in `-m`/`--mode` CLI parser (main.cpp:125-131)
- [X] Initialize `character` to `'\0'` in `Node(int frequency)` constructor (main.cpp:50)
- [X] Remove shadowed `char c;` in `build_counter_map` (main.cpp:32)
- [X] Fix memory leaks: free Huffman tree nodes after use

## Architecture
- [X] Split `main.cpp` into separate files: `huffman.h/cpp`, `main.cpp`
- [X] Replace raw `Node*` ownership with `std::shared_ptr<Node>` Huffman tree
- [X] Implement real binary packing (bits into bytes) instead of text-based `0`/`1` characters
- [X] Replace `read_file_content` with `std::istreambuf_iterator`

## Style & Robustness
- [X] Use `std::cerr` for error messages
- [X] Pass strings by `const&` in `build_counter_map`
- [X] Use `const auto&` in range-for loops
- [X] Add compiler warnings: `-Wall -Wextra -Wpedantic`

## Testing & Edge Cases
- [X] Add roundtrip test (encode then decode, compare to original)
- [X] Handle edge cases: empty file, single unique character

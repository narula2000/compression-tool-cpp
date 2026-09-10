# Compression Tool Coding Challenges

This repo is created as a part of the [codingchallenges.fyi](https://codingchallenges.fyi/challenges/intro) challenges.
This repo specifically tackle the [challenge](https://codingchallenges.fyi/challenges/challenge-huffman/) of implementing a compression tool via Huffman code with C++.

This repo was a C++ translation of the existing [Python implementation](https://github.com/narula2000/compression-tool), extended with true binary bit-packing so the compressed file is smaller than the input.

## The Challenge

Build your own file compression tool via Huffman code.
This help with understanding of binary tree and heap data structure, it will also help with understanding binary tree traversal.
This challenge will also tackle the understanding of writing data in bytes, handling file header, and reading said data.

```bash
./build/compressor -h
```

## Modes

```bash
-m, --mode {encode,decode}
    Select operation mode.
    encode  Compress an input file into a binary file
    decode  Decompress a binary file back to the original content
```

## Options

```bash
--input FILE
    Input file to compress (required for encode mode)

--bin FILE
    Compressed binary file

--output FILE
    Output decompressed file (required for decode mode)
```

## Examples

Encode a file:

```bash
./build/compressor -m encode --input input.txt --bin compressed.bin
```

Decode a file:

```bash
./build/compressor -m decode --bin compressed.bin --output output.txt
```

## Development

To setup this project you will have to have [cmake](https://cmake.org) installed.

To build the project:
```bash
cmake -S . -B build
cmake --build build
```

To run the commands it is simply running:
```bash
./build/compressor -h

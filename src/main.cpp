#include <iostream>

#include "cli.h"
#include "file_handler.h"
#include "huffman.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cli::printHelp(std::cout);
    return 0;
  }

  cli::Command command;
  const cli::ParseResult result = cli::parseArgs(argc, argv, command);
  if (result == cli::ParseResult::Help) {
    cli::printHelp(std::cout);
    return 0;
  }
  if (result == cli::ParseResult::Error) {
    return 1;
  }

  if (command.mode == cli::Mode::Encode) {
    std::cout << "Input Path: " << command.input.string() << "\n";
    std::cout << "Bin Path: " << command.bin.string() << "\n";

    const auto input = io::readFile(command.input);
    if (!input) {
      std::cerr << "Please provide a valid file input\n";
      return 1;
    }

    const auto compressed = huffman::compress(*input);
    if (!compressed) {
      std::cerr << "Failed to compress the input file\n";
      return 1;
    }

    if (!io::writeFile(command.bin, *compressed)) {
      std::cerr << "Failed to write the compressed file\n";
      return 1;
    }
  } else {
    std::cout << "Bin Path: " << command.bin.string() << "\n";
    std::cout << "Output Path: " << command.output.string() << "\n";

    const auto compressed = io::readFile(command.bin);
    if (!compressed) {
      std::cerr << "Please provide a valid binary file input\n";
      return 1;
    }

    const auto output = huffman::decompress(*compressed);
    if (!output) {
      std::cerr << "Failed to decompress the binary file\n";
      return 1;
    }

    if (!io::writeFile(command.output, *output)) {
      std::cerr << "Failed to write the output file\n";
      return 1;
    }
  }

  return 0;
}
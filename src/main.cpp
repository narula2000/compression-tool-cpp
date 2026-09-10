#include <iostream>
#include <string>

#include "cli.h"
#include "file_handler.h"
#include "huffman.h"

namespace {

std::string bytesToString(const std::vector<uint8_t> &bytes) {
  return std::string(reinterpret_cast<const char *>(bytes.data()), bytes.size());
}

} // namespace

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

    std::string input_content = bytesToString(*input);
    const std::string encoded_content = build_encoded_file(input_content);

    if (!io::writeFile(command.bin, std::span<const uint8_t>(
                                        reinterpret_cast<const uint8_t *>(
                                            encoded_content.data()),
                                        encoded_content.size()))) {
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

    const std::string output_content =
        decode_raw_content(bytesToString(*compressed));

    if (!io::writeFile(command.output, std::span<const uint8_t>(
                                          reinterpret_cast<const uint8_t *>(
                                              output_content.data()),
                                          output_content.size()))) {
      std::cerr << "Failed to write the output file\n";
      return 1;
    }
  }

  return 0;
}
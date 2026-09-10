#include "cli.h"

#include <iostream>
#include <string>

namespace cli {

namespace {

bool parseMode(const std::string &value, Mode &mode) {
  if (value == "encode") {
    mode = Mode::Encode;
    return true;
  }
  if (value == "decode") {
    mode = Mode::Decode;
    return true;
  }
  return false;
}

} // namespace

ParseResult parseArgs(int argc, char *argv[], Command &command) {
  std::string mode_name;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      return ParseResult::Help;
    }

    if (i + 1 >= argc) {
      std::cerr << "Error: " << arg << " requires a value\n";
      return ParseResult::Error;
    }

    if (arg == "-m" || arg == "--mode") {
      mode_name = argv[++i];
    } else if (arg == "-i" || arg == "--input") {
      command.input = argv[++i];
    } else if (arg == "-b" || arg == "--bin") {
      command.bin = argv[++i];
    } else if (arg == "-o" || arg == "--output") {
      command.output = argv[++i];
    } else {
      std::cerr << "Error: unknown argument: " << arg << '\n';
      return ParseResult::Error;
    }
  }

  if (mode_name.empty()) {
    std::cerr << "Error: mode is required\n";
    return ParseResult::Error;
  }

  if (!parseMode(mode_name, command.mode)) {
    std::cerr << "Error: mode must be 'encode' or 'decode'\n";
    return ParseResult::Error;
  }

  if (command.mode == Mode::Encode) {
    if (command.input.empty()) {
      std::cerr << "Error: input path is required for encode mode\n";
      return ParseResult::Error;
    }
    if (command.bin.empty()) {
      std::cerr << "Error: binary path is required for encode mode\n";
      return ParseResult::Error;
    }
  }

  if (command.mode == Mode::Decode) {
    if (command.bin.empty()) {
      std::cerr << "Error: binary path is required for decode mode\n";
      return ParseResult::Error;
    }
    if (command.output.empty()) {
      std::cerr << "Error: output path is required for decode mode\n";
      return ParseResult::Error;
    }
  }

  return ParseResult::Success;
}

void printHelp(std::ostream &stream) {
  stream << "Usage: compressor [-m|-i|-b|-o] FILE\n"
         << "\t-m, --mode {encode,decode}\n\t\tSelect operation mode.\n"
         << "\t\tencode  Compress an input file into a binary file\n"
         << "\t\tdecode  Decompress a binary file back to the original content\n"
         << "\t-i, --input FILE\n\t\tInput file to compress (required for encode "
            "mode)\n"
         << "\t-b, --bin FILE\n\t\tCompressed binary file\n"
         << "\t-o, --output FILE\n\t\tOutput decompressed file (required for "
            "decode mode)\n";
}

} // namespace cli
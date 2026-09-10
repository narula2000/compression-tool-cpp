#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "huffman.h"

void print_help_message() {
  std::cout
      << "Usage: compressor [-m|-i|-b|-o] FILE\n"
      << "\t-m, --mode {encode,decode}\n\t\tSelect operation mode.\n"
      << "\t\tencode  Compress an input file into a binary file\n"
      << "\t\tdecode  Decompress a binary file back to the original content\n"
      << "\t-i, --input FILE\n\t\tInput file to compress (required for encode "
         "mode)\n"
      << "\t-b, --bin FILE\n\t\tCompressed binary file\n"
      << "\t-o, --output FILE\n\t\tOutput decompressed file (required for "
         "decode "
         "mode)\n";
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_help_message();
    return 0;
  }
  int i = 1;

  std::string mode;
  std::string input_path;
  std::string bin_path;
  std::string output_path;

  while (i < argc) {
    const std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      print_help_message();
      return 0;
    }

    if (i + 1 >= argc) {
      std::cerr << "Error: " << arg << " requires a value\n";
      return 1;
    }

    if (arg == "-m" || arg == "--mode") {
      mode = argv[++i];
    } else if (arg == "-i" || arg == "--input") {
      input_path = argv[++i];
    } else if (arg == "-b" || arg == "--bin") {
      bin_path = argv[++i];
    } else if (arg == "-o" || arg == "--output") {
      output_path = argv[++i];
    } else {
      std::cerr << "Error: unknown argument: " << arg << '\n';
      return 1;
    }

    ++i;
  }

  if (mode.empty()) {
    std::cerr << "Error: mode is required\n";
    return 1;
  }

  if (mode != "encode" && mode != "decode") {
    std::cerr << "Error: mode must be 'encode' or 'decode'\n";
    return 1;
  }

  if (mode == "encode") {
    if (input_path.empty()) {
      std::cerr << "Error: input path is required for encode mode\n";
      return 1;
    }

    if (bin_path.empty()) {
      std::cerr << "Error: binary path is required for encode mode\n";
      return 1;
    }
  }

  if (mode == "decode") {
    if (bin_path.empty()) {
      std::cerr << "Error: binary path is required for decode mode\n";
      return 1;
    }

    if (output_path.empty()) {
      std::cerr << "Error: output path is required for decode mode\n";
      return 1;
    }
  }

  if (mode == "encode") {
    std::cout << "Input Path: " << input_path << "\n";
    std::cout << "Bin Path: " << bin_path << "\n";

    std::ifstream input_file(input_path);
    if (!input_file) {
      std::cerr << "Please provide a valid file input\n";
      return 1;
    }

    std::string content((std::istreambuf_iterator<char>(input_file)),
                        (std::istreambuf_iterator<char>()));
    std::string encoded_file_content = build_encoded_file(content);

    std::ofstream bin_file;
    bin_file.open(bin_path);
    bin_file << encoded_file_content;
    bin_file.close();
  } else if (mode == "decode") {
    std::cout << "Bin Path: " << bin_path << "\n";
    std::cout << "Output Path: " << output_path << "\n";

    std::ifstream bin_file(bin_path);
    if (!bin_file) {
      std::cerr << "Please provide a valid binary file input\n";
      return 1;
    }

    std::string content((std::istreambuf_iterator<char>(bin_file)),
                        (std::istreambuf_iterator<char>()));
    std::string output_content = decode_raw_content(content);

    std::ofstream output_file;
    output_file.open(output_path);
    output_file << output_content;
    output_file.close();
  } else {
    std::cerr << "No argument matches the comamands please consult the help "
                 "message\n";
    print_help_message();
    return 1;
  }

  return 0;
}

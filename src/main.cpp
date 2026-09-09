#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
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

  int i = 0;
  std::string mode;
  std::string input_path;
  std::string bin_path;
  std::string output_path;
  while (i < argc) {
    std::string arg = argv[i];

    if (arg == "-h" or arg == "--help") {
      print_help_message();
      return 0;
    } else if (arg == "-m" or arg == "--mode") {
      if (i + 1 >= argc) {
        std::cerr << "Please provide a mode [encode|decode]\n";
        return 1;
      }

      mode = argv[i + 1];
      i++;
    } else if (arg == "-i" or arg == "--input") {
      if (i + 1 >= argc) {
        std::cerr << "Please provide a file to compress\n";
        return 1;
      }

      input_path = argv[i + 1];
      i++;
    } else if (arg == "-b" or arg == "--bin") {
      if (i + 1 >= argc) {
        std::cerr << "Please provide a binary path\n";
        return 1;
      }

      bin_path = argv[i + 1];
      i++;
    } else if (arg == "-o" or arg == "--output") {
      if (i + 1 >= argc) {
        std::cerr << "Please provide a file output path\n";
        return 1;
      }

      output_path = argv[i + 1];
      i++;
    }
    i++;
  }

  std::map<char, int> counter;
  Node *head;
  std::map<char, std::string> huffman_table;
  std::string encoded_table;
  std::string encoded_content;
  std::string output_content;
  if (mode == "encode") {
    if (input_path.empty() or bin_path.empty()) {
      std::cerr << "Please provide a file input and a binary output path\n";
      return 1;
    }

    std::cout << "Input Path: " << input_path << "\n";
    std::cout << "Bin Path: " << bin_path << "\n";

    std::ifstream input_file(input_path);
    if (!input_file) {
      std::cerr << "Please provide a valid file input\n";
      return 1;
    }

    std::string content((std::istreambuf_iterator<char>(input_file)),
                        (std::istreambuf_iterator<char>()));

    counter = build_counter(content);
    head = build_graph(counter);
    huffman_table = build_table(head, "", huffman_table);
    encoded_table = build_encoded_table(huffman_table);
    encoded_content = build_encoded_content(content, huffman_table);
    free(head);

    std::ofstream bin_file;
    bin_file.open(bin_path);
    bin_file << encoded_table << ";;" << encoded_content;
    bin_file.close();
  } else if (mode == "decode") {
    if (output_path.empty() or bin_path.empty()) {
      std::cerr
          << "Please provide a file output_path and a binary input path\n";
      return 1;
    }

    std::cout << "Bin Path: " << bin_path << "\n";
    std::cout << "Output Path: " << output_path << "\n";

    std::ifstream bin_file(bin_path);
    if (!bin_file) {
      std::cerr << "Please provide a valid binary file input\n";
      return 1;
    }

    std::string content((std::istreambuf_iterator<char>(bin_file)),
                        (std::istreambuf_iterator<char>()));
    output_content = decode_raw_content(content);

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

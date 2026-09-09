#include <iostream>
#include <string>

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
        std::cout << "Please provide a mode [encode|decode]\n";
        return 1;
      }

      mode = argv[i + 1];
    } else if (arg == "-i" or arg == "--input") {
      if (i + 1 >= argc) {
        std::cout << "Please provide a file to compress\n";
        return 1;
      }

      input_path = argv[i + 1];
      i++;
    } else if (arg == "-b" or arg == "--bin") {
      if (i + 1 >= argc) {
        std::cout << "Please provide a binary path\n";
        return 1;
      }

      bin_path = argv[i + 1];
      i++;
    } else if (arg == "-o" or arg == "--output") {
      if (i + 1 >= argc) {
        std::cout << "Please provide a file output path\n";
        return 1;
      }

      output_path = argv[i + 1];
      i++;
    }
    i++;
  }

  if (mode == "encode") {
    if (input_path.empty() or bin_path.empty()) {
      std::cout << "Please provide a file input and a binary output path\n";
      return 1;
    }

    std::cout << "Input Path: " << input_path << "\n";
    std::cout << "Bin Path: " << bin_path << "\n";
  } else if (mode == "decode") {
    if (output_path.empty() or bin_path.empty()) {
      std::cout
          << "Please provide a file output_path and a binary input path\n";
      return 1;
    }

    std::cout << "Output Path: " << output_path << "\n";
    std::cout << "Bin Path: " << bin_path << "\n";
  } else {
    std::cout << "No argument matches the comamands please consult the help "
                 "message\n";
    print_help_message();
    return 1;
  }

  return 0;
}

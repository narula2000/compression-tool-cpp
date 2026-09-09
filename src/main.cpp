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
  if (argc < 3) {
    print_help_message();
    return 0;
  }

  std::string command = argv[1];

  if (command == "-h" or command == "--help") {
    print_help_message();
  } else if (command == "-m" or command == "--mode") {
  } else if (command == "-i" or command == "--input") {
  } else if (command == "-b" or command == "--bin") {
  } else if (command == "-o" or command == "--output") {
  } else {
    std::cout << "No argument matches the comamands please consult the help "
                 "message\n";
    print_help_message();
  }

  return 0;
}

#pragma once

#include <filesystem>
#include <ostream>

namespace cli {

enum class Mode { Encode, Decode };
enum class ParseResult { Success, Help, Error };

struct Command {
  Mode mode;
  std::filesystem::path input;
  std::filesystem::path bin;
  std::filesystem::path output;
};

ParseResult parseArgs(int argc, char *argv[], Command &command);

void printHelp(std::ostream &stream);

} // namespace cli
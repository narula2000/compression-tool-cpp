#include "file_handler.h"

#include <fstream>
#include <iterator>

namespace io {

std::optional<std::vector<uint8_t>> readFile(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    return std::nullopt;
  }

  std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
  return data;
}

bool writeFile(const std::filesystem::path &path, std::span<const uint8_t> data) {
  std::ofstream file(path, std::ios::binary | std::ios::trunc);
  if (!file) {
    return false;
  }

  file.write(reinterpret_cast<const char *>(data.data()),
             static_cast<std::streamsize>(data.size()));
  return static_cast<bool>(file);
}

} // namespace io
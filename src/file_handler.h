#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <vector>

namespace io {

std::optional<std::vector<uint8_t>> readFile(const std::filesystem::path &path);

bool writeFile(const std::filesystem::path &path, std::span<const uint8_t> data);

} // namespace io
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace huffman {

std::optional<std::vector<uint8_t>> compress(std::span<const uint8_t> data);

std::optional<std::vector<uint8_t>> decompress(std::span<const uint8_t> data);

} // namespace huffman
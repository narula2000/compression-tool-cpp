#include "huffman.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "huffman_tree.h"

namespace huffman {

namespace {

constexpr std::size_t kTableCountSize = sizeof(uint64_t);
constexpr std::size_t kMinimumPayloadSize = kTableCountSize + 1;

void appendLittleEndian(std::vector<uint8_t> &output, uint64_t value) {
  for (std::size_t i = 0; i < sizeof(uint64_t); ++i) {
    output.push_back(static_cast<uint8_t>((value >> (8 * i)) & 0xFF));
  }
}

uint64_t readLittleEndian(std::span<const uint8_t> bytes) {
  uint64_t value = 0;
  for (std::size_t i = 0; i < bytes.size(); ++i) {
    value |= static_cast<uint64_t>(bytes[i]) << (8 * i);
  }
  return value;
}

std::vector<uint8_t> serializeCode(const Code &code) {
  std::vector<uint8_t> bytes;
  const uint8_t byte_count = static_cast<uint8_t>((code.length + 7) / 8);
  const uint64_t shifted = code.bits << (8 * byte_count - code.length);
  for (uint8_t i = byte_count; i > 0; --i) {
    bytes.push_back(static_cast<uint8_t>((shifted >> (8 * (i - 1))) & 0xFF));
  }
  return bytes;
}

class BitWriter {
public:
  void writeBit(bool bit) {
    if (bit_count_ == 0) {
      buffer_.push_back(0);
    }
    if (bit) {
      buffer_.back() |= static_cast<uint8_t>(1u << (7u - bit_count_));
    }
    if (++bit_count_ == 8) {
      bit_count_ = 0;
    }
  }

  void writeBits(uint64_t value, uint8_t bit_count) {
    for (uint8_t i = bit_count; i > 0; --i) {
      writeBit((value >> (i - 1)) & 1u);
    }
  }

  uint8_t bitsInLastByte() const {
    if (buffer_.empty()) {
      return 0;
    }
    return bit_count_ == 0 ? 8 : bit_count_;
  }

  const std::vector<uint8_t> &data() const { return buffer_; }

private:
  std::vector<uint8_t> buffer_;
  uint8_t bit_count_ = 0;
};

class BitReader {
public:
  BitReader(std::span<const uint8_t> data, uint64_t bit_count)
      : data_(data), remaining_(bit_count) {}

  bool readBit() {
    --remaining_;
    const bool bit = (data_[byte_index_] >> (7 - bit_index_)) & 1u;
    if (++bit_index_ == 8) {
      bit_index_ = 0;
      ++byte_index_;
    }
    return bit;
  }

  bool hasBits() const { return remaining_ > 0; }

private:
  std::span<const uint8_t> data_;
  uint64_t remaining_;
  std::size_t byte_index_ = 0;
  uint8_t bit_index_ = 0;
};

} // namespace

std::optional<std::vector<uint8_t>> compress(std::span<const uint8_t> data) {
  FrequencyTable frequencies;
  for (const uint8_t byte : data) {
    ++frequencies[byte];
  }

  const std::shared_ptr<Node> root = buildTree(frequencies);
  const CodeTable codes = root != nullptr ? buildCodeTable(root) : CodeTable{};

  std::vector<uint8_t> output;
  appendLittleEndian(output, frequencies.size());

  for (const auto &[character, code] : codes) {
    output.push_back(character);
    output.push_back(code.length);
    const std::vector<uint8_t> packed = serializeCode(code);
    output.insert(output.end(), packed.begin(), packed.end());
  }

  BitWriter writer;
  for (const uint8_t byte : data) {
    const Code &code = codes.at(byte);
    writer.writeBits(code.bits, code.length);
  }

  output.push_back(writer.bitsInLastByte());
  const std::vector<uint8_t> &packed_data = writer.data();
  output.insert(output.end(), packed_data.begin(), packed_data.end());

  return output;
}

std::optional<std::vector<uint8_t>> decompress(std::span<const uint8_t> data) {
  if (data.size() < kMinimumPayloadSize) {
    return std::nullopt;
  }

  std::size_t offset = 0;
  const uint64_t table_count =
      readLittleEndian(data.subspan(offset, kTableCountSize));
  offset += kTableCountSize;

  std::map<std::string, uint8_t> decode_table;
  for (uint64_t i = 0; i < table_count; ++i) {
    if (offset + 2 > data.size()) {
      return std::nullopt;
    }
    const uint8_t character = data[offset++];
    const uint8_t length = data[offset++];
    if (length == 0) {
      return std::nullopt;
    }

    const uint8_t byte_count = static_cast<uint8_t>((length + 7) / 8);
    if (offset + byte_count > data.size()) {
      return std::nullopt;
    }

    std::string code;
    code.reserve(length);
    for (uint8_t i = 0; i < byte_count; ++i) {
      const uint8_t byte = data[offset++];
      const uint8_t bits_here =
          std::min<uint8_t>(8, length - static_cast<uint8_t>(code.size()));
      for (uint8_t j = 0; j < bits_here; ++j) {
        code += ((byte >> (7 - j)) & 1u) ? '1' : '0';
      }
    }
    decode_table[code] = character;
  }

  if (offset >= data.size()) {
    return std::nullopt;
  }
  const uint8_t valid_bits_last_byte = data[offset++];

  const std::span<const uint8_t> payload = data.subspan(offset);
  const std::size_t payload_bytes = payload.size();
  if (payload_bytes > 0 && valid_bits_last_byte == 0) {
    return std::nullopt;
  }

  const uint64_t total_bits = payload_bytes == 0
                                  ? 0
                                  : (payload_bytes - 1) * 8 +
                                        valid_bits_last_byte;

  BitReader reader(payload, total_bits);
  std::vector<uint8_t> output;
  std::string buffer;
  while (reader.hasBits()) {
    buffer += reader.readBit() ? '1' : '0';
    const auto match = decode_table.find(buffer);
    if (match != decode_table.end()) {
      output.push_back(match->second);
      buffer.clear();
    }
  }
  if (!buffer.empty()) {
    return std::nullopt;
  }

  return output;
}

} // namespace huffman
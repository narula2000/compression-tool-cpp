#pragma once

#include <cstdint>
#include <map>
#include <memory>

namespace huffman {

struct Node {
  uint8_t character = 0;
  uint64_t frequency = 0;
  std::shared_ptr<Node> left;
  std::shared_ptr<Node> right;

  Node(uint8_t character, uint64_t frequency);
  explicit Node(uint64_t frequency);
};

struct Code {
  uint64_t bits = 0;
  uint8_t length = 0;
};

using FrequencyTable = std::map<uint8_t, uint64_t>;
using CodeTable = std::map<uint8_t, Code>;

std::shared_ptr<Node> buildTree(const FrequencyTable &frequencies);

CodeTable buildCodeTable(const std::shared_ptr<Node> &root);

} // namespace huffman
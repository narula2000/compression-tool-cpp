#include "huffman_tree.h"

#include <queue>
#include <utility>

namespace huffman {

Node::Node(uint8_t character, uint64_t frequency)
    : character(character), frequency(frequency) {}

Node::Node(uint64_t frequency) : frequency(frequency) {}

namespace {

struct NodeGreater {
  bool operator()(const std::shared_ptr<Node> &a,
                  const std::shared_ptr<Node> &b) const {
    return a->frequency > b->frequency;
  }
};

} // namespace

std::shared_ptr<Node> buildTree(const FrequencyTable &frequencies) {
  if (frequencies.empty()) {
    return nullptr;
  }

  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>,
                      NodeGreater>
      queue;
  for (const auto &[character, frequency] : frequencies) {
    queue.push(std::make_shared<Node>(character, frequency));
  }

  if (queue.size() == 1) {
    auto leaf = queue.top();
    queue.pop();
    auto root = std::make_shared<Node>(leaf->frequency);
    root->left = std::move(leaf);
    return root;
  }

  while (queue.size() > 1) {
    auto left = queue.top();
    queue.pop();
    auto right = queue.top();
    queue.pop();
    auto parent = std::make_shared<Node>(left->frequency + right->frequency);
    parent->left = std::move(left);
    parent->right = std::move(right);
    queue.push(parent);
  }

  auto root = queue.top();
  queue.pop();
  return root;
}

namespace {

void buildCodes(const std::shared_ptr<Node> &node, Code code,
                CodeTable &table) {
  if (node == nullptr) {
    return;
  }
  if (node->left == nullptr && node->right == nullptr) {
    table[node->character] = code;
    return;
  }
  if (node->left != nullptr) {
    Code left_code = code;
    left_code.bits <<= 1;
    ++left_code.length;
    buildCodes(node->left, left_code, table);
  }
  if (node->right != nullptr) {
    Code right_code = code;
    right_code.bits = (right_code.bits << 1) | 1;
    ++right_code.length;
    buildCodes(node->right, right_code, table);
  }
}

} // namespace

CodeTable buildCodeTable(const std::shared_ptr<Node> &root) {
  CodeTable table;
  buildCodes(root, Code{}, table);
  return table;
}

} // namespace huffman
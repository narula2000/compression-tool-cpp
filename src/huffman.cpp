#include <cstddef>
#include <cstdlib>
#include <map>
#include <queue>
#include <string>
#include <tuple>

#include "huffman.h"

std::map<char, int> build_counter(const std::string &content) {
  std::map<char, int> counter;

  for (const char c : content) {
    counter[c]++;
  }

  return counter;
}

std::priority_queue<Node *, std::vector<Node *>, MinHeapCompare>
build_queue(const std::map<char, int> &counter) {
  std::priority_queue<Node *, std::vector<Node *>, MinHeapCompare> queue;

  for (const auto &item : counter) {
    queue.push(new Node(item.first, item.second));
  }
  return queue;
}

Node *build_graph(const std::map<char, int> &counter) {
  std::priority_queue<Node *, std::vector<Node *>, MinHeapCompare> queue =
      build_queue(counter);

  Node *left;
  Node *right;
  int frequency;
  Node *head;
  while (queue.size() > 1) {
    left = queue.top();
    queue.pop();
    right = queue.top();
    queue.pop();
    frequency = left->frequency + right->frequency;

    Node *dummy = new Node(frequency);
    dummy->left = left;
    dummy->right = right;
    queue.push(dummy);
  }

  head = queue.top();
  queue.pop();

  return head;
}

std::map<char, std::string> build_table(Node *node, std::string code,
                                        std::map<char, std::string> &table) {
  if (node != nullptr) {
    if (node->character) {
      table[node->character] = code;
    }
    build_table(node->left, code + "0", table);
    build_table(node->right, code + "1", table);
  }

  return table;
}

std::string build_encoded_table(std::map<char, std::string> &table) {
  std::string encoded_table;

  for (const auto &item : table) {
    encoded_table = encoded_table + item.first + ":" + item.second + ";";
  }
  return encoded_table;
}

std::string build_encoded_content(std::string &content,
                                  std::map<char, std::string> table) {
  std::string encoded_content;
  for (const char c : content) {
    encoded_content += table[c];
  }
  return encoded_content;
}

std::tuple<std::string, std::string> split_raw_content(std::string &content) {
  int delimter_location = content.find(";;;");
  return {content.substr(0, delimter_location + 1),
          content.substr(delimter_location + 3)};
}

std::map<std::string, std::string> build_decode_table(std::string &table) {
  std::map<std::string, std::string> decode_table;

  std::string buffer;
  int splitter;
  std::string key;
  std::string value;
  for (size_t i = 0; i < table.size(); i++) {
    if (table[i] == ';') {
      splitter = buffer.find(":");

      key = buffer.substr(0, splitter);
      value = buffer.substr(splitter + 1);

      decode_table[value] = key;

      buffer = "";
    } else {
      buffer += table[i];
    }
  }

  return decode_table;
}

std ::string decode_raw_content(std::string &raw_content) {

  std::string encoded_table;
  std::string content;
  std::tie(encoded_table, content) = split_raw_content(raw_content);
  std::map<std::string, std::string> table = build_decode_table(encoded_table);
  std::string buffer;
  std::string output;
  for (size_t i = 0; i < content.size(); i++) {
    buffer += content[i];

    if (table.count(buffer)) {
      output += table[buffer];
      buffer = "";
    }
  }

  return output;
}

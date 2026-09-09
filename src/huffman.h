#pragma once

#include <cstddef>
#include <cstdlib>
#include <map>
#include <queue>
#include <string>

struct Node {
  char character = '\0';
  int frequency = 0;

  Node *left = nullptr;
  Node *right = nullptr;

  Node(char character, int frequency)
      : character(character), frequency(frequency) {}
  Node(int frequency) : frequency(frequency) {}
};

struct MinHeapCompare {
  bool operator()(const Node *a, const Node *b) const {
    return a->frequency > b->frequency;
  }
};

std::map<char, int> build_counter(const std::string &content);

Node *build_graph(const std::map<char, int> &counter);
std::map<char, std::string> build_table(Node *node, std::string code,
                                        std::map<char, std::string> &table);

std::string build_encoded_table(std::map<char, std::string> &table);

std::string build_encoded_content(std::string &content,
                                  std::map<char, std::string> table);

std ::string decode_raw_content(std::string &raw_content);

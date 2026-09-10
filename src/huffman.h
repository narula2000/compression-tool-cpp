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

std::string build_encoded_file(std::string &content);

std ::string decode_raw_content(std::string &raw_content);

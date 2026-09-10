#include <cstdint>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cli.h"
#include "file_handler.h"
#include "huffman.h"

namespace {

int failures = 0;

#define CHECK(condition)                                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      std::cerr << "FAILED: " #condition " (line " << __LINE__ << ")\n";       \
      ++failures;                                                              \
    }                                                                          \
  } while (false)

bool roundTrips(const std::vector<uint8_t> &bytes) {
  const auto compressed = huffman::compress(bytes);
  if (!compressed) {
    return false;
  }
  const auto decompressed = huffman::decompress(*compressed);
  return decompressed.has_value() && *decompressed == bytes;
}

void testRoundTripEmpty() {
  CHECK(roundTrips({}));
}

void testRoundTripSingleCharacter() {
  CHECK(roundTrips(std::vector<uint8_t>(5, 'a')));
}

void testRoundTripText() {
  const std::string text =
      "The quick brown fox jumps over the lazy dog. "
      "Pack my box with five dozen liquor jugs.\n"
      "How vexingly quick daft zebras jump!\n";
  const std::vector<uint8_t> data(text.begin(), text.end());
  CHECK(roundTrips(data));
}

void testRoundTripAllByteValues() {
  std::vector<uint8_t> data;
  data.reserve(512);
  for (std::size_t i = 0; i < 512; ++i) {
    data.push_back(static_cast<uint8_t>(i % 256));
  }
  CHECK(roundTrips(data));
}

void testCompressionShrinks() {
  const std::string text =
      "the quick brown fox jumps over the lazy dog "
      "the quick brown fox jumps over the lazy dog "
      "the quick brown fox jumps over the lazy dog";
  std::vector<uint8_t> data;
  for (int repeat = 0; repeat < 20; ++repeat) {
    data.insert(data.end(), text.begin(), text.end());
  }

  const auto compressed = huffman::compress(data);
  CHECK(compressed.has_value());
  CHECK(compressed->size() < data.size());
}

void testMalformedPayloads() {
  CHECK(!huffman::decompress(std::vector<uint8_t>{}).has_value());
  CHECK(!huffman::decompress(std::vector<uint8_t>{1, 2, 3, 4, 5, 6, 7, 8})
             .has_value());
}

void testCliParsing() {
  cli::Command command;

  const char *encode_args[] = {"compressor", "-m", "encode",
                               "-i",         "in.txt", "-b",
                               "out.bin"};
  CHECK(cli::parseArgs(7, const_cast<char **>(encode_args), command) ==
        cli::ParseResult::Success);
  CHECK(command.mode == cli::Mode::Encode);
  CHECK(command.input == "in.txt");
  CHECK(command.bin == "out.bin");

  const char *decode_args[] = {"compressor", "--mode", "decode",
                               "--bin",      "in.bin", "--output",
                               "out.txt"};
  CHECK(cli::parseArgs(7, const_cast<char **>(decode_args), command) ==
        cli::ParseResult::Success);
  CHECK(command.mode == cli::Mode::Decode);
  CHECK(command.bin == "in.bin");
  CHECK(command.output == "out.txt");

  const char *help_args[] = {"compressor", "-h"};
  CHECK(cli::parseArgs(2, const_cast<char **>(help_args), command) ==
        cli::ParseResult::Help);

  std::ostringstream error_buffer;
  std::streambuf *original_cerr = std::cerr.rdbuf();
  std::cerr.rdbuf(error_buffer.rdbuf());

  const char *no_mode_args[] = {"compressor", "-i", "in.txt", "-b", "out.bin"};
  CHECK(cli::parseArgs(5, const_cast<char **>(no_mode_args), command) ==
        cli::ParseResult::Error);

  const char *bad_mode_args[] = {"compressor", "-m", "compress", "-i", "in.txt"};
  CHECK(cli::parseArgs(5, const_cast<char **>(bad_mode_args), command) ==
        cli::ParseResult::Error);

  const char *missing_value_args[] = {"compressor", "-m"};
  CHECK(cli::parseArgs(2, const_cast<char **>(missing_value_args), command) ==
        cli::ParseResult::Error);

  std::cerr.rdbuf(original_cerr);
}

void testFileIO() {
  const std::filesystem::path path =
      std::filesystem::temp_directory_path() / "compression_tool_io_test.bin";
  const std::vector<uint8_t> payload = {0, 1, 2, 3, 0x0D, 0x0A, 0xFF, 0x80, 0};

  CHECK(io::writeFile(path, payload));
  const auto read = io::readFile(path);
  CHECK(read.has_value());
  CHECK(read.has_value() && *read == payload);
  std::filesystem::remove(path);
}

} // namespace

int main() {
  testRoundTripEmpty();
  testRoundTripSingleCharacter();
  testRoundTripText();
  testRoundTripAllByteValues();
  testCompressionShrinks();
  testMalformedPayloads();
  testCliParsing();
  testFileIO();

  if (failures == 0) {
    std::cout << "All tests passed\n";
    return 0;
  }
  std::cerr << failures << " test(s) failed\n";
  return 1;
}
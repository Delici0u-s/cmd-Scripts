// Mini binary viewer

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip> // For setw and setfill

int printusage();
template <size_t Max>
std::array<std::string, Max> generateBitsetTable();

int vId(auto A) { return static_cast<int>(A); }
enum class InputArgv { File = 1, mode, chunks, bytes };
enum class InputVec { mode = 0, chunks, bytes };

int main(int argc, char *argv[]) {
  if (argc < 2) return printusage();
  bool trigger{true};
  std::vector<int> inputs{0, 1, 4};
  std::filesystem::path FilePath{argv[vId(InputArgv::File)]};
  switch (argc) {
  case 5:
    try {
      inputs[vId(InputVec::bytes)] = atoi(argv[vId(InputArgv::bytes)]);
      if (inputs[vId(InputVec::bytes)] < 1) throw 0;
    } catch (...) {
      std::cout << "Make sure your byte selection is >= 1\n";
      trigger = false;
    };
    [[fallthrough]];
  case 4:
    try {
      inputs[vId(InputVec::chunks)] = atoi(argv[vId(InputArgv::chunks)]);
      if (inputs[vId(InputVec::chunks)] < 1) throw 0;
    } catch (...) {
      std::cout << "Make sure your chuck selection is >= 1\n";
      trigger = false;
    };
    [[fallthrough]];
  case 3:
    try {
      inputs[vId(InputVec::mode)] = atoi(argv[vId(InputArgv::mode)]);
      if (inputs[vId(InputVec::mode)] < 0 || inputs[vId(InputVec::mode)] > 3) throw 0;
    } catch (...) {
      std::cout << "Make sure you have a valid mode selected\n";
      trigger = false;
    };
    [[fallthrough]];
  case 2:
    if (!std::filesystem::exists(FilePath) || std::filesystem::is_directory(FilePath)) {
      std::cout << "Make sure your file exists and is not a directory\n";
      trigger = false;
    }
    break;
  }
  if (trigger) {
    std::ifstream File{FilePath, std::ios::binary};
    if (!File.is_open()) {
      std::cout << "File couldn't open";
      return 2;
    }

    std::ostringstream outbuffer{};

    std::vector<uint8_t> chunk(inputs[vId(InputVec::bytes)]); // Resize instead of reserve
    int counter{0}, Linecount{0};

    static const auto bitsetTable = generateBitsetTable<256>(); // Generate once
    int mode = inputs[vId(InputVec::mode)];
    int maxChunks = inputs[vId(InputVec::chunks)];

    while (File.read(reinterpret_cast<char *>(chunk.data()), chunk.size())) {
      auto *ptr = chunk.data();
      auto *end = ptr + File.gcount();

      // Apply formatting based on mode
      switch (mode) {
      case 0:
        // Binary mode
        while (ptr < end)
          outbuffer << bitsetTable[*ptr++];
        break;
      case 3:
        // Char mode (print printable chars, non-printable as '.')
        while (ptr < end) {
          if (std::isprint(*ptr)) {
            outbuffer << static_cast<char>(*ptr++);
          } else {
            outbuffer << '.';
            ++ptr;
          }
        }
        break;
      default:
        // Hexadecimal or Octal mode
        while (ptr < end) {
          if (mode == 2) {
            // Octal
            outbuffer << std::setw(3) << std::setfill('0') << std::oct << static_cast<int>(*ptr++) << ' ';
          } else {
            // Hexadecimal
            outbuffer << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(*ptr++) << ' ';
          }
        }
      }
      outbuffer << ' ';

      // Line break after max chunks
      if (++counter >= maxChunks) {
        outbuffer << ++Linecount << '\n';
        counter = 0;
      }
    }

    std::cout << outbuffer.str();

    return 0;
  }
  return 1;
}

int printusage() {
  std::ostringstream buffer{};
  buffer << "Prints files in binary or such. " << '\n';
  buffer << "Usage:\n\t.exe <File> <mode> <chunks per line> <how many bytes per chunk>\n";
  buffer << "\tEverything after File is optional\n";
  buffer << "Modes:\n\t[0] dual\n\t[1] hex\n\t[2] oct\n\t[3] char\n";
  std::cout << buffer.str();
  return 0;
}

template <size_t Max>
std::array<std::string, Max> generateBitsetTable() {
  std::array<std::string, Max> table;
  for (size_t i = 0; i < Max; ++i)
    table[i] = std::bitset<8>(i).to_string() + ' ';
  return table;
}


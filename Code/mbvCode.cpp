// mini binary printer
// Warning, code is not optimized, might lead to pain

#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

int printUsage();
template <size_t Max>
std::array<std::string, Max> generateBitsetTable();

void printNumber(const std::vector<uint8_t> &data, bool isSigned, std::ostringstream &out);

constexpr int vId(auto A)
{
  return static_cast<int>(A);
}

enum class InputArgv
{
  File = 1,
  Mode,
  Chunks,
  Bytes,
  IntDisplay
};
enum class InputVec
{
  Mode = 0,
  Chunks,
  Bytes,
  IntDisplay
};

int main(int argc, char *argv[])
{
  if (argc < 2) return printUsage();

  std::vector<int> inputs{0, 1, 4, 0}; // Default values
  std::filesystem::path filePath{argv[vId(InputArgv::File)]};
  bool validInput = true;

  for (int i = 2; i < argc; ++i)
  {
    try
    {
      inputs[i - 2] = std::stoi(argv[i]);
      if ((i == vId(InputArgv::Bytes) && inputs[i - 2] < 1) || (i == vId(InputArgv::Chunks) && inputs[i - 2] < 1) ||
          (i == vId(InputArgv::Mode) && (inputs[i - 2] < 0 || inputs[i - 2] > 3)) ||
          (i == vId(InputArgv::IntDisplay) && (inputs[i - 2] < 0 || inputs[i - 2] > 2)))
      {
        throw std::invalid_argument("Invalid input");
      }
    }
    catch (...)
    {
      std::cout << "Invalid argument for parameter " << i << "\n";
      validInput = false;
    }
  }

  if (!std::filesystem::exists(filePath) || std::filesystem::is_directory(filePath))
  {
    std::cout << "Make sure your file exists and is not a directory\n";
    return 1;
  }
  if (!validInput) return 1;

  std::ifstream file{filePath, std::ios::binary};
  if (!file.is_open())
  {
    std::cout << "File couldn't open\n";
    return 2;
  }

  std::ostringstream outBuffer;
  std::vector<uint8_t> chunk(inputs[vId(InputVec::Bytes)]);
  static const auto bitsetTable = generateBitsetTable<256>();
  int counter = inputs[vId(InputVec::Chunks)], lineCount = 0;

  while (file.read(reinterpret_cast<char *>(chunk.data()), chunk.size()) || file.gcount() > 0)
  {
    auto *ptr = chunk.data();
    auto *end = ptr + file.gcount();

    if (++counter >= inputs[vId(InputVec::Chunks)])
    {
      outBuffer << '\n' << ++lineCount;
      counter = 0;
    }
    outBuffer << "  ";

    switch (inputs[vId(InputVec::Mode)])
    {
    case 0:
      while (ptr < end) outBuffer << bitsetTable[*ptr++];
      break;
    case 3:
      while (ptr < end) outBuffer << (std::isprint(*ptr) ? static_cast<char>(*ptr++) : '.'), ++ptr;
      break;
    case 2:
      while (ptr < end) { outBuffer << std::setw(3) << std::setfill('0') << std::oct << static_cast<int>(*ptr++); }
      break;
    default:
      while (ptr < end) { outBuffer << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(*ptr++); }
      break;
    }

    if (inputs[vId(InputVec::IntDisplay)] > 0)
    {
      static const int inpBitsP{inputs[vId(InputVec::Bytes)] * 8};
      static const int width{static_cast<int>(std::floor(inpBitsP * std::log10(2))) +
                             2}; // +1 for sign original +1 is cuz formula
      static const bool asSigned{inputs[vId(InputVec::IntDisplay)] == 1};

      outBuffer << " : " << std::setw(width) << std::setfill(' ');
      printNumber(chunk, asSigned, outBuffer);

    }
  }

  std::cout << outBuffer.str();
  return 0;
}

int printUsage()
{
  std::cout << "Prints files in binary or such. \n";
  std::cout << "Usage:\n\t.exe <File> <mode> <chunks per line> <bytes per chunk> <int display>\n";
  std::cout << "\tEverything after File is optional\n";
  std::cout << "Modes:\n\t[0] binary\n\t[1] hex\n\t[2] oct\n\t[3] char\n";
  std::cout << "Int Display:\n\t[0] None\n\t[1] Signed Integer\n\t[2] Unsigned Integer\n";
  return 0;
}

template <size_t Max>
std::array<std::string, Max> generateBitsetTable()
{
  std::array<std::string, Max> table;
  for (size_t i = 0; i < Max; ++i) table[i] = std::bitset<8>(i).to_string() + ' ';
  return table;
}

void printNumber(const std::vector<uint8_t> &data, bool isSigned, std::ostringstream &outBuffer)
{
  static boost::multiprecision::cpp_int result{};
  result = 0;
  for (uint8_t i : data) result = (result << 8) | i;

  if (data[0] & 1 << 7 && isSigned)
  {
    result -= boost::multiprecision::cpp_int(1) << (data.size() * 8);
  }
  outBuffer << result;
}

#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>

int main(int argc, char *argv[])
{
  if (argc == 1 || (std::string(argv[1]) ==  "--help") )
  {
    std::cout << "Usage: " << argv[0] << " And then all items to be removed.\n:";
    std::cout << "Example: \n";
    std::cout << argv[0] << " file1 Folder1 D:\\example/heart.txt \n";
    std::cout << "For filepath's: / or \\ is of irrellevance\n";
    return 0;
  }

  unsigned int TotRemItems{0};
  for (int i{1}; i < argc; ++i)
  {
    // Check if the file/directory exists, then remove accordingly with user feedback if file doesnt exist
    if (std::filesystem::exists(argv[i]))
    {
      int success{system(std::format("rm -f -r -- {}", argv[i]).c_str())};
      if (success)
      {
        std::cout << argv[i] << " failed execute command on this item\n";
      }
      else if (std::filesystem::exists(argv[i]))
      {
        std::cout << "failed to remove: " << argv[i] << '\n';
      }
      else
      {
        ++TotRemItems;
      }
    }
    else
    {
      std::cout << argv[i] << " does not exist\n";
    }
  }

  std::cout << TotRemItems << " out of " << argc - 1 << " items successfully removed\n";

  return 0;
}

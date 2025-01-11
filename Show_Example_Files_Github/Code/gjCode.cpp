#include <filesystem>
#include <iostream>

int main(int argc, char *argv[])
{
  if (argc == 1 || (std::string(argv[1]) == "--help"))
  {
    std::cout << "Usage: " << argv[0] << " And then all items to be removed.\n:";
    std::cout << "Example: \n";
    std::cout << argv[0] << " file1 Folder1 D:\\example/heart.txt \n";
    std::cout << "For filepath's: / or \\ is of irrellevance\n";
    return 0;
  }

  unsigned int count{0};
  for (int i{1}; i < argc; ++i)
  {
    if (std::filesystem::remove_all(argv[i]) == 0)
    {
      std::cout << '"' << argv[i] << '"' << " could not be removed\n";
    }
    else
    {
      ++count;
    }
  }
  std::cout << count << " out of " << argc - 1 << " items successfully removed\n";
}

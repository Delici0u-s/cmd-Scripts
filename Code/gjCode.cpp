#include <cstring>
#include <filesystem>
#include <iostream>
#include <unordered_set>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << fs::path(argv[0]).filename() << "<optional: -d(no output)> <path1> <path2> ...\n";

    return 1;
  }

  bool Ouput{!!strcmp(argv[1], "-d")};

  // Using unordered_set to avoid duplicate processing
  std::unordered_set<fs::path> paths;
  for (int i{Ouput ? 1 : 2}; i < argc; ++i)
  {
    try
    {
      // Construct a filesystem path; using fs::path ensures proper type safety
      fs::path p(argv[i]);
      paths.emplace(std::move(p));
    }
    catch (const fs::filesystem_error &e)
    {
      if (Ouput) std::cerr << "Error processing argument '" << argv[i] << "': " << e.what() << '\n';
      return 1;
    }
  }

  // Process each unique path
  for (const auto &path : paths)
  {
    try
    {
      if (fs::exists(path)) { fs::remove_all(path); }
      else if (Ouput) { std::cout << "Path does not exist: " << path << '\n'; }
      // Double-check that the path is deleted
      if (fs::exists(path) && Ouput) { std::cerr << "Couldn't delete: " << path << '\n'; }
    }
    catch (const fs::filesystem_error &e)
    {
      if (Ouput) std::cerr << "Error removing '" << path << "': " << e.what() << '\n';
    }
  }

  return 0;
}

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

fs::path Cr(fs::path a)
{
  if (a.has_parent_path()) { fs::create_directories(a.parent_path()); }

  if (a.has_filename())
  {
    FILE *tmp = fopen(a.string().c_str(), "a");
    if (tmp) fclose(tmp);
  }

  return a.parent_path();
}

int main(int argc, char *argv[])
{
  std::vector<std::string_view> Args;
  for (int i = 1; i < argc; ++i) Args.emplace_back(argv[i]);

  if (argc < 2 || Args[0] == "--help")
  {
    std::cerr << R"(cr.exe - Create multiple files and folders at once  

Usage:  
    cr [folders/files using the syntax below]  

Syntax:  
    - Space and "/" act as separators  
    - Folders:  
        Folder/     -> Create & move into it  
        Folder1/Folder2/ -> Nested folders  
    - Files:  
        No "/" in or after the filename 
)";
    return 1;
  }

  fs::path CurDir = fs::current_path();
  for (const auto i : Args) { CurDir = Cr(CurDir / i); }

  return 0;
}

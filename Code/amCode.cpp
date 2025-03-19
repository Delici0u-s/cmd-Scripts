#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string_view>
#include <unordered_set>

int HelpMessage()
{
  std::cout << "\nAutoMakeFileGenerator (am) V1.1\n"
            << "by Delici0us_ or Delici0u-s\n"
            << "\n"
            << "Generate makefile based on input (similar to just using gcc/g++)\n"
            << "Currently only .c and .cpp supported (as Mainfile)\n\n"
            << "Usage: am.exe Mainfile OtherArgs (only required input is Mainfile)\n"
            << "OtherArgs can be replaced by anything accepted by g++/gcc (in arbitrary order)\n"
            << "SpecialArgument: '-o' makes the following argument the output\n\n"
            << "Example: am.exe main.cpp --std=c++17 utils.cpp -o compiled/ClickMe moreUtils.c\n\n";
  return 0;
}

template<typename a>
std::ostream& operator<<(std::ostream& os, std::unordered_set<a> &T)
{
  for (const a& i : T) os << ' ' << i;
  return os;
}

int main (int argc, char *argv[]) {
  if (argc < 2 || (std::string_view(argv[1]) == "--help")) return HelpMessage();
  const std::string_view MainFile{argv[1]};
  std::string_view outfile{"a"};
  bool type{MainFile.ends_with(".cpp") ? true : false }; // True means c++
  std::string LanguageStandartc{"23"}; 
  std::string LanguageStandartcpp{"23"}; 
  std::unordered_set<std::string_view> args{};

  for (int i{2}; i < argc; ++i ){
    std::string_view tmp{argv[i]};
    if (tmp == "-o" && i != (argc -1)) {
      outfile = argv[++i];
    }
    // i know -std= works too but even the help message suggests to use --. 
    else if(tmp.starts_with("--std=")){
      if (tmp.starts_with("--std=c++")) LanguageStandartcpp = tmp.begin() + 9;
      else LanguageStandartc = tmp.begin() + 7;
    }
    else args.emplace(argv[i]);
  }

  std::ofstream file{"makefile"};
  // Writing the file:
  file << "# Compiler to use (g++ or gcc):\n";
  file << "CC = " << (type ? "g++" : "gcc") << "\n\n";

  file << "# Main source file:\n";
  file << "MAINFILE = " << MainFile << "\n\n";

  file << "# output executable:\n";
  file << "OUTPUT = " << outfile << "\n\n";

  file << "# Language Standart:\n";
  if (type)
    file << "LanguageSTD = --std=c++" << LanguageStandartcpp << "\n\n"; 
  else 
    file << "LanguageSTD = --std=c" << LanguageStandartc << "\n\n"; 

  file << "# Additional args: \nAdditionals = " << args << "\n\n";
  file << "Err = -O2 -Wall -Wextra -Wpedantic\n";
  file << "Debug = -Og -g -ggdb -fno-omit-frame-pointer\n";
  file << "Prod = -Ofast -Oz -s -Os -DNDEBUG\n\n";

  file << "all: $(OUTPUT)\n\n";

  file << "$(OUTPUT) : $(MAINFILE)\n";
  file << "\t$(CC) $(MAINFILE) $(LanguageSTD) $(Additionals) -o $(OUTPUT) $(Err)\n\t$(OUTPUT)\n\n";

  file << "d : $(MAINFILE)\n";
  file << "\t$(CC) $(MAINFILE) $(LanguageSTD) $(Additionals) -o $(OUTPUT) $(Err) $(Debug)\n\n";

  file << "debug : $(MAINFILE)\n";
  file << "\t$(CC) $(MAINFILE) $(LanguageSTD) $(Additionals) -o $(OUTPUT) $(Err) $(Debug)\n\n";

  file << "p : $(MAINFILE)\n";
  file << "\t$(CC) $(MAINFILE) $(LanguageSTD) $(Additionals) -o $(OUTPUT) $(Err) $(Prod)\n\n";

  file << "prod : $(MAINFILE)\n";
  file << "\t$(CC) $(MAINFILE) $(LanguageSTD) $(Additionals) -o $(OUTPUT) $(Err) $(Prod)\n\n";

  file << "clean: \n\trm $(OUTPUT)\nclear: \n\trm $(OUTPUT)";
  file.close();


  return 0;
}

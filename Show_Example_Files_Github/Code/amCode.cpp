#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

// had to add this cuz of linux backslash acceptance ;-;
#include <algorithm>
#include <string>

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
            << "Example: am.exe main.cpp --std=c++11 utils.cpp -o compiled/ClickMe moreUtils.c\n\n";
  return 0;
}

int FileOpenErr()
{
  std::cout << "makefile could not be opened";
  return 1;
}

std::string replaceSlashes(const auto &Inp)
{
  std::string result(Inp);
  std::replace(result.begin(), result.end(), '\\', '/');
  return result;
}

int main(int argc, char *argv[])
{
  if (argc < 2 || (std::string_view(argv[1]) == "--help")) return HelpMessage();

  // Create two vectors for user flags and additional files to compile too
  // Both hold the index of argv to the corresponse
  std::vector<int> cFlags{};
  std::vector<int> cFiles{};

  // The mainfile to compile
  const std::string_view MainFile{argv[1]};

  // If its a .c file use gcc, else use g++
  const bool isCfile{MainFile.ends_with('c')};
  const auto &Compiler{isCfile ? "gcc" : "g++"};

  // Set outfiles Default to the name of MainFile without the extension
  auto Outfile{MainFile.substr(0, MainFile.find_last_of('.'))};

  // Seperate Files and Flags, Loop through rest of files for that
  // This is done so manually editing the makefile is easier
  for (int i{2}; i < argc; ++i)
  {
    // if it starts with '-' I'd assume its a flag not a file, the compiler dont cares anyways
    if (argv[i][0] == '-')
      // check if the -o parameter is present
      if ((argv[i][1] == 'o') && (argv[i][2] == '\0'))
        // if it is the next argument is the dedicated OutputFile
        Outfile = argv[++i];
      else
        cFlags.emplace_back(i);
    else
      cFiles.emplace_back(i);
  }

  std::ofstream MakeFile{"makefile"};
  if (!MakeFile) return FileOpenErr();

  // Writing the file:
  MakeFile << "# Compiler to use (g++ or gcc):\n";
  MakeFile << "CC = " << Compiler << "\n\n";

  MakeFile << "# Main source file:\n";
  MakeFile << "MAINFILE = " << replaceSlashes(MainFile) << "\n\n";

  MakeFile << "# output executable:\n";
  MakeFile << "OUTPUT = " << replaceSlashes(Outfile) << "\n\n";

  MakeFile << "# Other Files to compile Too:\n";
  MakeFile << "OTHERFILES =";
  for (auto &i : cFiles) MakeFile << ' ' << replaceSlashes(argv[i]);

  MakeFile << "\n\n# Other Flags to add:\n";
  MakeFile << "OTHERFLAGS =";
  for (auto &i : cFlags) MakeFile << ' ' << argv[i];

  // add std=c++VERSION if its not a c file
  if (!isCfile)
  {
    MakeFile << "\n\n# Language standart the compiler should use:\n";
    MakeFile << "# (automatically the version amCode.cpp was compiled in)\n";
    MakeFile << "# Can be replaced by an std=c++VERSION in OTHERFLAGS as Last takes priority \n";
    // gets the current cplusplus compiler(when compiling) version,
    // for example 202302 and extracts the last two year digits, in this case 23.
    // Should work for every version (and future) except for 98, which has date 199711 (results in 97 not 98)
    MakeFile << "LANGUAGESTD = --std=c++" << (__cplusplus % 10000) / 100;
  }

  // I know i could have made it a function or sum, but this makes it easier to see the structure of the future makefile, thus making changes easier. Also no cpu branching ^_^
  MakeFile << "\n\n# Default target (for development/debugging)\n";
  MakeFile << "all: $(OUTPUT)\n\n";
  MakeFile << "$(OUTPUT): $(MAINFILE) $(OTHERFILES)\n";
  // --std=c++VERSION before the OTHERFLAGS as the later flag (if provides as input arg) overwrites the provious
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Og -Wall -Wextra -pedantic -O2\n\n";

  MakeFile << "# Production target (optimized and stripped binary)\n";
  MakeFile << "prod: $(MAINFILE) $(OTHERFILES)\n";
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Os -s -DNDEBUG -Wall -Wextra -pedantic\n\n";

  MakeFile << "# optimized for speed\n";
  MakeFile << "fast: $(MAINFILE) $(OTHERFILES)\n";
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Ofast -Og -Wall -Wextra -pedantic\n\n";

  MakeFile << "# optimized for speed, but without debug\n";
  MakeFile << "pfast: $(MAINFILE) $(OTHERFILES)\n";
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Ofast -s -DNDEBUG -Wall -Wextra -pedantic\n\n";

  MakeFile << "# optimized for space/memory efficiency\n";
  MakeFile << "space: $(MAINFILE) $(OTHERFILES)\n";
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Oz -Og -Wall -Wextra -pedantic -O2\n\n";

  MakeFile << "# optimized for space/memory efficiency, but without debug\n";
  MakeFile << "pspace: $(MAINFILE) $(OTHERFILES)\n";
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Oz -s -DNDEBUG -Wall -Wextra -pedantic\n\n";

  MakeFile << "# Just everything thrown together\n";
  MakeFile << "max: $(MAINFILE) $(OTHERFILES)\n";
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Os -Ofast -Oz -Og -Wall -Wextra -pedantic\n\n";

  MakeFile << "# Just everything thrown together, but without debug\n";
  MakeFile << "pmax: $(MAINFILE) $(OTHERFILES)\n";
  MakeFile << "\t$(CC) $(MAINFILE) $(OTHERFILES) -o $(OUTPUT) " << (!isCfile ? "$(LANGUAGESTD) " : "")
           << "$(OTHERFLAGS) -Os -Ofast -Oz -s -DNDEBUG -Wall -Wextra -pedantic\n\n";

  MakeFile << "# Clean target (works for Linux and Windows)\n";
  MakeFile << "clean:\n";
  MakeFile << "\trm $(OUTPUT)\n";
  MakeFile << "clear:\n";
  MakeFile << "\trm $(OUTPUT)";

  MakeFile.close();

  return 0;
}

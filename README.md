# CMD-Scripts
As im driving more to working with the cmd im developing these commands.  
This is to make my, *and if you want your,* life easier.  
> All commands were writen on and for Windows; idk if it works in other operating systems.

### How to use the scripts
> Download them via git clone, as a zip, or by any method you prefer.  
> Put these files in a directory that is in your PATH environment, or add that folder to your PATH.  
> Done!

## Updated README
This README is now updated with all available commands and their usage.

---
## Commands:

1. ### lsd
    > * **FILE:** lsd.exe  
    > * ***Description***  
    >   * Shows you all files in the current directory, colored and compact  
    >   * Example:  
    >       ![ls-command-example](https://raw.githubusercontent.com/Delici0u-s/cmd-Scripts/refs/heads/master/Show_Example_Files_Github/OtherFiles/ls_Example.png)  
    > * ***Usage***  
    >   * ```lsd```  
    >   * ```lsd <characters till newline>```  
    > * ***ColorCoding***  
    >     ![Color Coding Example](https://raw.githubusercontent.com/Delici0u-s/cmd-Scripts/refs/heads/master/Show_Example_Files_Github/OtherFiles/ColorCoding.png)

2. ### cr
    > * **FILE:** cr.exe  
    > * ***Description***  
    >   * Create bunch of Files and Folders at once (short for create)  
    >   * Example:  
    >       ```D:\example>cr samples/totalamount.txt code.c headers.h .tmp/otherfiles/helper.py```  
    >       * Creates the specified structure  
    > * ***Usage:***  
    >   * ```cr [combination of folders and files according to following syntax]```  
    > * **Syntax:**  
    >   * Space and "/" act as separators  
    >       * **Folders:**  
    >           * ```Folder/``` creates a folder and moves into it  
    >           * ```Folder/../``` creates a folder and moves back
    >           * ```Folder/Folder2/Folder3/``` creates nested folders  
    >       * **Files:**  
    >           * Ensure no "/" is in or directly after the filename and extension
    >   * Starts out in your current cwd


3. ### am
    > * **FILE:** am.exe  
    > * ***Description***  
    >   * Creates a makefile based on input arguments, reducing writing effort  
    >   * Short for AutoMakeFileGenerator  
    > * ***Usage***  
    >   * ```am mainFile <otherfiles> <args>```  
    >   * **Examples:**  
    >       * ```am main.c```  
    >           * Creates a makefile that compiles main.c (resulting in main.exe on Windows, main on Linux)  
    >       * ```am main.c utils.c -g```  
    >           * Uses main.c as the Mainfile with additional source files and debugging flags  
    >       * ```am main.cpp -o D:\example\output.exe```  
    >           * Sets the output executable to D:\example\output.exe  
    >       * ```am Walrus.cpp helper.c -O3 -o ./Releases/Final/App```  
    >           * Custom makefile generation with specific flags  
    > * ***Arguments:***  
    >   * ```-o compilationName``` defines the output name (can be a path)

4. ### gj
    > * **FILE:** gj.exe  
    > * ***Description***  
    >   * Removes all specified Files and Folders at once  
    >   * “gj” stands for GoodJob, as a final goodbye to the files  
    > * ***Usage***  
    >   * ```gj``` or ```gj --help``` displays help information  
    >   * ```gj <files>``` removes the specified files  
    >   * **Examples:**  
    >       * ```gj main.c``` removes main.c from the current directory  
    >       * ```gj main.cpp Samples D:\example\output.exe``` removes the listed files (handles absolute paths as well)

5. ### mbv
    > * **FILE:** mbv.exe  
    > * ***Description***  
    >   * A mini viewer printer for dumping file contents in various formats  
    >   * Modes include binary, hexadecimal, octal, and character display  
    > * ***Usage***  
    >   * ```mbv <File> <mode> <chunks per line> <bytes per chunk> <int display>```  
    > * **Modes:**  
    >   0  Binary  
    >   1  Hexadecimal  
    >   2  Octal  
    >   3  Character  
    > * **Integer Display:**  
    >   1  None  
    >   2  Signed Integer  
    >   2  Unsigned Integer

---
## Thanks for Reading!
> I hope these commands prove useful, at least they are for me.  
> If you have any Questions, Suggestions, or Feedback, feel free to contact me.

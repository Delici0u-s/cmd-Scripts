# CMD-Scripts
As im driving more to working with the cmd im developing these commands.  
This is to make my, *and if you want your,*  life easier.
> All commands were writen on and for windows, idk if it works in other operating systems
>
> **!NOTE!** I dont know why but my antivirus sometimes flagges the compiled version of cr. Please look at the code and compile it yourself too, as far as i know it shouldn't be malicious

### How to use the scripts
>Download them via got clone or downloading via git as zip, or any other way you want.  
>Put these files in a directory that is in your PATH environment, or add that folder to your PATH environment.  
>Done!
  
---
## Commands:
1. ### lsd
    >* **FILE:** lsd.exe
    >* ***Description***  
    >   * Shows you all files in current directory, colored and compact
    >   * Example:  
    >       ![ls-command-example](https://raw.githubusercontent.com/Delici0u-s/cmd-Scripts/refs/heads/master/Show_Example_Files_Github/OtherFiles/ls_Example.png)
    >* ***Usage***
    >   * ``` lsd```
    >   * ``` lsd <characters till newline>```
    >* ***ColorCoding***  
    >     ![ls-command-example](https://raw.githubusercontent.com/Delici0u-s/cmd-Scripts/refs/heads/master/Show_Example_Files_Github/OtherFiles/ColorCoding.png)
2. ### cr
    > * **FILE:** cr.exe
    > * ***Description***  
    >   * Create bunch of Files and Folders at once
    >   * short for create
    > * Example:  
    >    * ``` D:\example>cr samples/totalamount.txt code.c headers.h .tmp/otherfiles/..useless/helper.py ```  
    >    * Creates this **structure**:  
    >       ![cr-command-example](https://github.com/Delici0u-s/cmd-Scripts/blob/master/Show_Example_Files_Github/OtherFiles/crExample.png?raw=true)
    > * ***Usage:***
    >   * ``` cr [combination of folders and files according to following syntax] ```
    >   * **Syntax:**
    >       * " "*(space)* and "/" count as seperators
    >           * place File or Folder right before/after Seperator
    >       * Folders:
    >           * ```Folder/``` Creates a folder and moves to it
    >           * ```.Folder/``` Creates a folder and leave it empty, don't move to it
    >           * ```Folder/Folder2/Folder3/``` Creates a the three folders in each other
    >       * Files:
    >           * just make sure no "/" is in or directly after the file name and extension
3. ### qc
    >* **FILE:** qc.bat
    >* ***Description***  
    >   * compiles cpp and c files
    >   * reducing writing effort
    >   * short for QuickCompile
    >* ***Usage***  
    >   * ``` qc mainFile.ext <otherfiles> <args>```
    >       * args and otherfiles dont care about positioning. Mix them up as you like, just the mainfile's position is important.
    >   * example:
    >       * ``` qc main.c```
    >           * tries to compile main.exe in the current dir
    >       * ``` qc main.c utils.c -g```
    >           * tries to compile main.exe in the current dir. Also puts utils.c into the compilation command. With the -g flag
    >       * ``` qc main.cpp -o D:\example\output.exe```
    >           * tries to compile main.cpp to D:\example\output.exe
    >       * ``` qc Walrus.cpp helper.c -O3 -o ./Releases/Final/App.exe```
    >           * tries to compile Walrus.cpp with helper.c with O3 optimization in the relative location /Releases/Final/App.exe
    >* ***args***  
    >   * ``` -g ```
    >       * Adds the -g flag to the compilation
    >       * Usually -s is used, but -g 'replaces' the -s
    >   * ``` -o compilationName ```
    >       * defines the output name.
    >       * can be Path too
    >   * ``` -w ```
    >       * removes the -Wall -Wextra flags to the compilation





## Thanks for Reading!
> I hope these commands can be useful, for me they at least are.  
> If you have any Questions/Suggestions/Feedback and or anything else feel free to contact me

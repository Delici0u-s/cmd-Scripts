# CMD-Scripts
As im driving more to working with the cmd im developing these commands.  
This is to make my, *and if you want your,*  life easier.
> All commands were writen on and for windows, idk if it works in other operating systems

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
    >   * ```lsd```
    >   * ```lsd <characters till newline>```
    >* ***ColorCoding***  
    >     ![ls-command-example](https://raw.githubusercontent.com/Delici0u-s/cmd-Scripts/refs/heads/master/Show_Example_Files_Github/OtherFiles/ColorCoding.png)
2. ### cr
    > * **FILE:** cr.exe
    > * ***Description***  
    >   * Create bunch of Files and Folders at once
    >   * short for create
    > * Example:  
    >    * ```D:\example>cr samples/totalamount.txt code.c headers.h .tmp/otherfiles/..useless/helper.py```  
    >    * Creates this **structure**:  
    >       ![cr-command-example](https://github.com/Delici0u-s/cmd-Scripts/blob/master/Show_Example_Files_Github/OtherFiles/crExample.png?raw=true)
    > * ***Usage:***
    >   * ```cr [combination of folders and files according to following syntax]```
    >   * **Syntax:**
    >       * " "*(space)* and "/" count as seperators
    >           * place File or Folder right before/after Seperator
    >       * Folders:
    >           * ```Folder/``` Creates a folder and moves to it
    >           * ```.Folder/``` Creates a folder and leave it empty, don't move to it
    >           * ```Folder/Folder2/Folder3/``` Creates a the three folders in each other
    >       * Files:
    >           * just make sure no "/" is in or directly after the file name and extension
3. ### am
    >* **FILE:** am.exe
    >* ***Description***  
    >   * Creates a makefile based in input arguments
    >   * reducing writing effort
    >   * short for AutoMakeFileGenerator
    >* ***Usage***  
    >   * ```am mainFile <otherfiles> <args>```
    >       * args and otherfiles dont care about positioning. Mix them up as you like, just the mainfile's position is important.
    >   * examples:
    >       * ```am main.c```
    >           * creates a makefile that on run results in main.exe on windows and main on linux
    >       * ```am main.c utils.c -g```
    >           * creates a makefile with main.c as the Mainfile, utils.c as a otherfile and -g as an arg (useless because of -Og being default added)
    >       * ```am main.cpp -o D:\example\output.exe```
    >           * creates a makefile, that on run creates output.exe in the directory: "D:\\example\\"
    >       * ```am Walrus.cpp helper.c -O3 -o ./Releases/Final/App```
    >           * creates a maikefile with Walrus.cpp as Mainfile, helper.c as otherfile,  
    >             -O3 as argument which will be used in ```make``` but not in ```make prod```  
    >             (as custom args are disabled (have to be manually added))  
    >             and on running make creates App (App.exe if on linux) in the relative ./Releases/Final/ directory
    >* ***args***  
    >   * ```-o compilationName```
    >       * defines the output name.
    >       * can be Path too
    >* ***DefaultModes***
    >   * just ```make```
    >       * compiles with -Og (for debugging) by Default, and your other args and files
    >       * Also compiles with enablin warning flags arguments
    >   * ```make prod```
    >       * compiles for production
    >       * enables your given files, flags and the additional flags: 
    >           * -Os (better than -O2, more stable-ish than -O3) 
    >           * -s (removes unnecessary stuff) 
    >           * -DNDEBUG (no asserts etc.)
    >       * Also compiles with enablin warning flags arguments
    >   * ```make fast```
    >       * compiles optimized for speed
    >       * enables your given files, flags and the additional flags: 
    >           * -Ofast (as far as i know optimal for fast)
    >           * -Og (for debugging)
    >       * Also compiles with enablin warning flags arguments
    >   * ```make pfast```
    >       * like fast, but without debug stuff and such
    >       * adds flags -s and -DNDEBUG instead of -Og
    >       * Also compiles with enablin warning flags arguments
    >   * ```make space```
    >       * compiles optimized for space efficiency (i think memory and storage, idk exactly though)
    >       * enables your given files, flags and the additional flags: 
    >           * -Oz (optimized for space efficiency)
    >           * -Og (for debugging)
    >       * Also compiles with enablin warning flags arguments
    >   * ```make pspace```
    >       * like space, but without debug stuff and such
    >       * adds flags -s and -DNDEBUG instead of -Og
    >       * Also compiles with enablin warning flags arguments
    >   * ```make max```
    >       * brutally shoves all the previous modes together
    >       * might make it unstable
    >       * enables your given files, flags and the additional flags: 
    >           * -Os (better than -O2, more stable-ish than -O3) 
    >           * -Ofast (as far as i know optimal for fast)
    >           * -Oz (optimized for space efficiency)
    >           * -Og (for debugging)
    >       * Also compiles with enablin warning flags arguments
    >   * ```make pmax```
    >       * like max, but without debug stuff and such
    >       * adds max -s and -DNDEBUG instead of -Og
    >       * Also compiles with enablin warning flags arguments
    >   * ```make clean```
    >       * removes compiled executable
    >   * ```make clear```
    >       * removes compiled executable
    >       * Added it because i sometimes misstype or missthink
4. ### gj
    >* **FILE:** gj.exe
    >* ***Description***  
    >   * Removes all specified Files and Folders
    >   * Many file removals at the same time
    >   * By the way, gj stands for GoodJob. As per GoodJob Files as a final goodbye to the files
    >* ***Usage***  
    >   * ```gj``` / ```gj --help```
    >       * Displays help Information
    >   * ```gj <files>```
    >       * args and otherfiles dont care about positioning. Mix them up as you like, just the mainfile's position is important.
    >   * example:
    >       * ```gj main.c```
    >           * removes main.c in the current directory
    >       * ```gj main.cpp Sampler.exe D:\example\output.exe```
    >           * removes main.cpp Sampler.exe in the current directory
    >           * removes ```D:\example\output.exe```



## Thanks for Reading!
> I hope these commands can be useful, for me they at least are.  
> If you have any Questions/Suggestions/Feedback and or anything else feel free to contact me

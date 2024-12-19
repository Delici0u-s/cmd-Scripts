#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>

#define MAX_PATH_LENGTH 260
#define MAX_INCLUDES 100
#define MAX_LINE_LENGTH 1024

typedef struct {
    char* files[MAX_INCLUDES];
    int count;
} FileList;

// Function declarations
void initializeFileList(FileList* list);
void addFile(FileList* list, const char* filename);
int fileExists(const char* filename);
void parseIncludes(const char* filename, FileList* includes, const char* baseDir);
void generateMakefile(const char* mainFile, const char* outputName, FileList* includes);
char* getFileExtension(const char* filename);
char* getBaseDir(const char* filepath);
void cleanup(FileList* includes);
void printUsage(const char* programName);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    char* mainFile = argv[1];
    char* outputName = argv[2];

    // Validate input file extension
    char* ext = getFileExtension(mainFile);
    if (!ext || (strcmp(ext, ".c") != 0 && strcmp(ext, ".cpp") != 0)) {
        printf("Error: Main file must have .c or .cpp extension\n");
        return 1;
    }

    // Check if main file exists
    if (!fileExists(mainFile)) {
        printf("Error: Main file '%s' not found\n", mainFile);
        return 1;
    }

    FileList includes;
    initializeFileList(&includes);

    // Get base directory of main file
    char* baseDir = getBaseDir(mainFile);
    
    // Parse includes recursively
    parseIncludes(mainFile, &includes, baseDir);
    
    // Generate Makefile
    generateMakefile(mainFile, outputName, &includes);

    // Cleanup
    cleanup(&includes);
    free(baseDir);

    printf("Makefile generated successfully.\n");
    return 0;
}

void printUsage(const char* programName) {
    printf("Usage: %s <source_file> <output_exe>\n\n", programName);
    printf("Arguments:\n");
    printf("  source_file    Path to the main source file (.c or .cpp)\n");
    printf("  output_exe     Name or path of the output executable\n\n");
    printf("Examples:\n");
    printf("  %s main.c app.exe\n", programName);
    printf("  %s ./src/main.cpp ../bin/app.exe\n", programName);
}

void initializeFileList(FileList* list) {
    list->count = 0;
    for (int i = 0; i < MAX_INCLUDES; i++) {
        list->files[i] = NULL;
    }
}

void addFile(FileList* list, const char* filename) {
    // Check if file already exists in list
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->files[i], filename) == 0) {
            return;
        }
    }

    if (list->count < MAX_INCLUDES) {
        list->files[list->count] = _strdup(filename);
        list->count++;
    }
}

int fileExists(const char* filename) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(filename, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }
    FindClose(hFind);
    return 1;
}

void parseIncludes(const char* filename, FileList* includes, const char* baseDir) {
    FILE* file = fopen(filename, "r");
    if (!file) return;

    char line[MAX_LINE_LENGTH];
    char includePath[MAX_PATH_LENGTH];
    
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "#include \"", 9) == 0) {
            // Extract filename from #include "filename"
            char* start = line + 9;
            char* end = strchr(start, '"');
            if (end) {
                int length = end - start;
                strncpy(includePath, start, length);
                includePath[length] = '\0';

                // Convert header to source file if exists
                char sourcePath[MAX_PATH_LENGTH];
                snprintf(sourcePath, sizeof(sourcePath), "%s\\%.*s", 
                    baseDir, (int)(strlen(includePath) - 2), includePath);

                char* sourceExt = getFileExtension(filename);
                strcat(sourcePath, sourceExt);

                if (fileExists(sourcePath)) {
                    addFile(includes, sourcePath);
                    parseIncludes(sourcePath, includes, baseDir);
                }
            }
        }
    }

    fclose(file);
}

void generateMakefile(const char* mainFile, const char* outputName, FileList* includes) {
    FILE* makefile = fopen("Makefile", "w");
    if (!makefile) {
        printf("Error: Unable to create Makefile\n");
        return;
    }

    char* compiler = strstr(mainFile, ".cpp") ? "g++" : "gcc";
    
    // Write compiler variables
    fprintf(makefile, "CC=%s\n", compiler);
    fprintf(makefile, "CFLAGS=-Wall -O2\n");
    fprintf(makefile, "TARGET=%s\n", outputName);
    fprintf(makefile, "RM=rm -f\n\n");  // Add rm command variable

    // Create output directory if it contains a path
    fprintf(makefile, "# Create output directory if needed\n");
    fprintf(makefile, "OUTDIR=$(dir $(TARGET))\n");
    fprintf(makefile, "$(shell if not exist $(OUTDIR) mkdir $(OUTDIR))\n\n");

    // Write object files
    fprintf(makefile, "OBJECTS=");
    for (int i = 0; i < includes->count; i++) {
        char* filename = includes->files[i];
        char* basename = strrchr(filename, '\\');
        basename = basename ? basename + 1 : filename;
        fprintf(makefile, "%.*s.o ", (int)(strrchr(basename, '.') - basename), basename);
    }
    char* mainBasename = strrchr(mainFile, '\\');
    mainBasename = mainBasename ? mainBasename + 1 : mainFile;
    fprintf(makefile, "%.*s.o\n\n", (int)(strrchr(mainBasename, '.') - mainBasename), mainBasename);

    // Write default target
    fprintf(makefile, ".PHONY: all clean\n\n");  // Add .PHONY declaration
    fprintf(makefile, "all: $(TARGET)\n\n");
    fprintf(makefile, "$(TARGET): $(OBJECTS)\n");
    fprintf(makefile, "\t$(CC) $(OBJECTS) -o $(TARGET)\n\n");

    // Write object file rules
    for (int i = 0; i < includes->count; i++) {
        char* filename = includes->files[i];
        char* basename = strrchr(filename, '\\');
        basename = basename ? basename + 1 : filename;
        fprintf(makefile, "%.*s.o: %s\n", 
            (int)(strrchr(basename, '.') - basename), 
            basename, 
            filename);
        fprintf(makefile, "\t$(CC) $(CFLAGS) -c %s\n\n", filename);
    }

    // Write main file rule
    fprintf(makefile, "%.*s.o: %s\n",
        (int)(strrchr(mainBasename, '.') - mainBasename),
        mainBasename,
        mainFile);
    fprintf(makefile, "\t$(CC) $(CFLAGS) -c %s\n\n", mainFile);

    // Write clean target with cross-platform commands
    fprintf(makefile, "clean:\n");
    fprintf(makefile, "\t$(RM) $(OBJECTS)\n");
    fprintf(makefile, "\t$(RM) $(TARGET)\n");

    fclose(makefile);
}

char* getFileExtension(const char* filename) {
    char* ext = strrchr(filename, '.');
    return ext;
}

char* getBaseDir(const char* filepath) {
    char* lastSlash = strrchr(filepath, '\\');
    if (!lastSlash) {
        char* currentDir = (char*)malloc(2);
        strcpy(currentDir, ".");
        return currentDir;
    }

    int length = lastSlash - filepath;
    char* baseDir = (char*)malloc(length + 1);
    strncpy(baseDir, filepath, length);
    baseDir[length] = '\0';
    return baseDir;
}

void cleanup(FileList* includes) {
    for (int i = 0; i < includes->count; i++) {
        free(includes->files[i]);
    }
}

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fileapi.h>

#define MAXLEN       40

int isnumber(char* input) {
    int length,i;

    length = strlen (input);
    for (i=0;i<length; i++)
        if (!isdigit(input[i]))
        {
            return 1;
        }
    return 0;
}
// Function to convert an integer to a string
char *IntToChar(int num) {
    // Dynamically determine the required buffer size
    int length = snprintf(NULL, 0, "%d", num);
    char *out = malloc(length + 1);
    if (!out) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    sprintf(out, "%d", num);
    return out;
}

// Function to colorize a string using ANSI escape codes
char *StringColored(const char *StringToColor, int AnsiColorCode) {
    const char *prefix = "\033[0;";
    const char *suffix = "m";
    const char *reset = "\033[0m";

    // Dynamically determine buffer size
    size_t length = strlen(prefix) + snprintf(NULL, 0, "%d", AnsiColorCode) +
                    strlen(suffix) + strlen(StringToColor) + strlen(reset) + 1;

    char *output = malloc(length);
    if (!output) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Construct the colored string
    sprintf(output, "%s%d%s%s%s", prefix, AnsiColorCode, suffix, StringToColor, reset);
    return output;
}

char *GetBeforeLastDot(const char *stringFileName) {
    const char *lastDot = strrchr(stringFileName, '.');
    if (lastDot == NULL) {
        return strdup(stringFileName);
    }
    size_t length = lastDot - stringFileName;
    char *result = malloc(length + 1);  // Allocate space for the substring
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strncpy(result, stringFileName, length);
    result[length] = '\0';  // Null-terminate the string
    return result;
}

char *ColorFileSeperatetly(const char *stringFileName, int color) {
    const char *fileEnd = strrchr(stringFileName, '.');
    if (fileEnd == NULL) {
        fileEnd = "";
        // return strdup(".ERR");
    }
    char *out = malloc(strlen(stringFileName)+1);
    char *coloredPre = StringColored(GetBeforeLastDot(stringFileName), color);

    sprintf(out, "%s%s", coloredPre, fileEnd);
    // printf("End: %s, Str: %s, Total: %s\n", fileEnd, coloredPre, out);
    return out;
}

char *getFullPath(const char *dirPath, const char *fileName) {
    size_t dirPathLen = strlen(dirPath);
    size_t fileNameLen = strlen(fileName);

    // Allocate enough memory for the full path + '/' + null terminator
    char *fullPath = malloc(dirPathLen + fileNameLen + 2);
    if (!fullPath) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    strcpy(fullPath, dirPath);

    // Ensure the directory path ends with a '/'
    if (dirPath[dirPathLen - 1] != '/') {
        strcat(fullPath, "/");
    }

    strcat(fullPath, fileName);
    return fullPath;
}

int main(int argc, char **argv) {
    // get len argument
    int NewLen;
    if (argc > 1 && isnumber(argv[0])) {
        NewLen = atoi(argv[1]);
    }
    else {
        NewLen = MAXLEN;
    }
    struct dirent *de;

    DIR *dr = opendir(".");
    if (dr == NULL) {
        perror("Could not open current directory");
        return EXIT_FAILURE;
    }

    // Skip . and ..
    readdir(dr);
    readdir(dr);
    int currentLen = 0;

    while ((de = readdir(dr)) != NULL) {
        char *fullPath = getFullPath(".", de->d_name);
        if (!fullPath) {
            fprintf(stderr, "Failed to construct full path for: %s\n", de->d_name);
            continue;
        }

        DWORD attributes = GetFileAttributes(fullPath);
        free(fullPath); // Free memory allocated by getFullPath

        if (attributes == INVALID_FILE_ATTRIBUTES) {
            perror("Error retrieving file attributes");
            continue; // Skip inaccessible files
        }

        char *coloredName = NULL;

        if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Handle directories
            coloredName = StringColored(de->d_name, 33 + 60*(!(attributes & FILE_ATTRIBUTE_HIDDEN)) ); // Yellow color for directories
        } else if (attributes & FILE_ATTRIBUTE_HIDDEN) {
            // Handle hidden files
            coloredName = ColorFileSeperatetly(de->d_name, 35); // Purple for hidden files
        } else {
            // Handle regular files
            coloredName = ColorFileSeperatetly(de->d_name, 32); // Green for regular files
        }

        int wordlen = strlen(de->d_name)+2; // +1 for space
        if (currentLen > NewLen) {
            printf("\n");
            currentLen = wordlen;
        }
        else{
            currentLen += wordlen;
        }
        if (coloredName) {
            printf("%s  ", coloredName);
            free(coloredName); // Free memory after use
        }
    }

    closedir(dr);
    return 0;
}

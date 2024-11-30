#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fileapi.h>
#include <windows.h>

#define MAXLEN 40

int isnumber(char *input) {
    int length, i;
    length = strlen(input);
    for (i = 0; i < length; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

// Function to convert an integer to a string
char *IntToChar(int num) {
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

    size_t length = strlen(prefix) + snprintf(NULL, 0, "%d", AnsiColorCode) +
                    strlen(suffix) + strlen(StringToColor) + strlen(reset) + 1;

    char *output = malloc(length);
    if (!output) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    sprintf(output, "%s%d%s%s%s", prefix, AnsiColorCode, suffix, StringToColor, reset);
    return output;
}

// Function to get the part of the filename before the last dot
char *GetBeforeLastDot(const char *stringFileName) {
    const char *lastDot = strrchr(stringFileName, '.');
    if (lastDot == NULL) {
        return strdup(stringFileName);
    }
    size_t length = lastDot - stringFileName;
    char *result = malloc(length + 1);
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strncpy(result, stringFileName, length);
    result[length] = '\0';
    return result;
}

// Function to colorize filename with extension handling
char *ColorFileSeperatetly(const char *stringFileName, int color) {
    const char *fileEnd = strrchr(stringFileName, '.');
    if (fileEnd == NULL) {
        fileEnd = "";
    }

    char *beforeLastDot = GetBeforeLastDot(stringFileName);
    char *coloredPre = StringColored(beforeLastDot, color);

    size_t length = strlen(coloredPre) + strlen(fileEnd) + 1;
    char *out = malloc(length);
    if (!out) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    sprintf(out, "%s%s", coloredPre, fileEnd);

    free(beforeLastDot); // Free intermediate memory
    free(coloredPre);    // Free intermediate memory
    return out;
}

int main(int argc, char **argv) {
    int NewLen;
    if (argc > 1 && isnumber(argv[1])) {
        NewLen = atoi(argv[1]);
    } else {
        NewLen = MAXLEN;
    }

    int currentLen = 0;

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile("*", &FindFileData);
    FindNextFile(hFind, &FindFileData); // Skip `.` and `..`

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("File search failed. Error: %lu\n", GetLastError());
        return EXIT_FAILURE;
    }

    while (FindNextFile(hFind, &FindFileData) != 0) {
        char *coloredName = NULL;

        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
                coloredName = StringColored(FindFileData.cFileName, 33); // Yellow
            } else {
                coloredName = StringColored(FindFileData.cFileName, 93); // Bright yellow
            }
        } else {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
                coloredName = ColorFileSeperatetly(FindFileData.cFileName, 35); // Magenta
            } else {
                coloredName = ColorFileSeperatetly(FindFileData.cFileName, 32); // Green
            }
        }

        printf("%s  ", coloredName);

        int wordlen = strlen(FindFileData.cFileName) + 2; // +2 for space
        if (currentLen + wordlen > NewLen) {
            printf("\n");
            currentLen = wordlen;
        } else {
            currentLen += wordlen;
        }

        free(coloredName); // Free dynamically allocated memory
    }

    FindClose(hFind);
    return EXIT_SUCCESS;
}


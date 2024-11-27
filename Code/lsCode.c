#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fileapi.h>
#include <windows.h>

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
int main(int argc, char **argv) {
    // get len argument
    int NewLen;
    if (argc > 1 && isnumber(argv[0])) {
        NewLen = atoi(argv[1]);
    }
    else {
        NewLen = MAXLEN;
    }
    int currentLen = 0;

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile("*", &FindFileData);
    FindNextFile(hFind, &FindFileData); // skip the ..

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FileSearchFailed... Error: %lu\n", GetLastError());
    } else {
        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            char *coloredName = NULL;
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
                {
                    coloredName = StringColored(FindFileData.cFileName, 33);
                }
                else
                {
                    coloredName = StringColored(FindFileData.cFileName, 93);
                }
            }
            else
            {
                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
                {
                    coloredName = ColorFileSeperatetly(FindFileData.cFileName, 35);
                }
                else
                {
                    coloredName = ColorFileSeperatetly(FindFileData.cFileName, 32);
                }
            }

            printf("%s  ", coloredName);

            int wordlen = strlen(FindFileData.cFileName)+2; // +1 for space
            if (currentLen + wordlen > NewLen) {
                printf("\n");
                currentLen = wordlen;
            }
            else{
                currentLen += wordlen;
            }
        }
        FindClose(hFind);
    }

}

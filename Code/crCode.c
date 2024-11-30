#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <fileapi.h>
#include <sys/stat.h>

void main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Usage:\n");
        printf("  %s <path/to/folder> <file1> <folder2/folder3> <file2> ...\n\n", argv[0]);
        printf("Description:\n");
        printf("  - Create directories and files based on the specified arguments.\n");
        printf("  - Use '/' to define nested directories (e.g., folder1/folder2).\n");
        printf("  - Use spaces to separate files and directories.\n");
        printf("  - Files and directories can be mixed in any order.\n\n");
        printf("Examples:\n");
        printf("  %s folder1/folder2 test.txt folder3/walls.txt sand/sandman.txt\n", argv[0]);
        return;
    }

    // Calculate the total length of the combined arguments string
    int total_length = 0;
    for (int i = 1; i < argc; i++) {
        total_length += strlen(argv[i]) + 1;
    }

    // Create a buffer to hold the combined string
    char combined[total_length];
    combined[0] = '\0';

    // Concatenate all arguments into the combined string
    for (int i = 1; i < argc; i++) {
        strcat(combined, argv[i]);
        if (i < argc - 1) {
            strcat(combined, " ");  // Add space between arguments
        }
    }

    char *input = combined;
    char *start = input;  // track the start of each token
    char *current = input;  // Pointer for iteration

    while (*current != '\0') {
        // Skip leading delimiters
        while (*current == ' ' || *current == '/') {
            current++;
        }

        start = current;

        // Find end of the token
        while (*current != '\0' && *current != ' ' && *current != '/') {
            current++;
        }

        if (start != current || (*current == '\0' && start != input)) {
            int token_length = current - start;
            char* token = (char*)malloc(token_length + 1);
            if (token != NULL) {
                strncpy(token, start, token_length);
                token[token_length] = '\0';

                // Check which delimiter follows
                if (*current == ' ' || *current == '\0') {
                    // Create file
                    HANDLE hFile = CreateFileA(
                        token,
                        GENERIC_WRITE,
                        0,
                        NULL,
                        CREATE_NEW,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL
                    );

                    if (hFile == INVALID_HANDLE_VALUE) {
                        printf("Creating File failed (%s): (%d)\n", token, GetLastError());
                    } else {
                        CloseHandle(hFile);
                    }

                } else if (*current == '/') {
                    struct stat path_stat;
                    if (stat(token, &path_stat) == 0 && !S_ISDIR(path_stat.st_mode)) {
                        printf("Cannot create directory '%s': A file with the same name exists.\n", token);
                    } else {
                        if (CreateDirectory(token, NULL)) {
                            if (SetCurrentDirectoryA(token)) {
                            } else {
                                printf("Failed to move to %s: (%d)\n", token, GetLastError());
                            }
                        } else {
                            printf("Creating Directory failed (%s): (%d)\n", token, GetLastError());
                        }
                    }
                }
                free(token);
            }
        }
    }
}


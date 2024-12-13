#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILES 100
#define MAX_LINE 1024
#define MAX_DEPENDENCIES 1000

// Function to extract dependencies from a C file
void extract_dependencies(const char *filename, char dependencies[MAX_DEPENDENCIES][MAX_LINE], int *dep_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        // Check for #include statements
        if (strncmp(line, "#include", 8) == 0) {
            char *start = strchr(line, '"');
            if (start) {
                char *end = strchr(start + 1, '"');
                if (end) {
                    *end = '\0';
                    strcpy(dependencies[*dep_count], start + 1);
                    (*dep_count)++;
                }
            }
        }
    }

    fclose(file);
}

// Function to write the Makefile
void generate_makefile(const char *output, char files[MAX_FILES][MAX_LINE], int file_count) {
    FILE *makefile = fopen("Makefile", "w");
    if (!makefile) {
        fprintf(stderr, "Error: Cannot create Makefile\n");
        return;
    }

    fprintf(makefile, "CC = gcc\n");
    fprintf(makefile, "CFLAGS = -Wall -Wextra\n");
    fprintf(makefile, "OUTPUT = %s\n", output);
    fprintf(makefile, "\n");

    fprintf(makefile, "OBJS = ");
    for (int i = 0; i < file_count; i++) {
        fprintf(makefile, "%s.o ", files[i]);
    }
    fprintf(makefile, "\n\n");

    fprintf(makefile, "$(OUTPUT): $(OBJS)\n");
    fprintf(makefile, "\t$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJS)\n\n");

    for (int i = 0; i < file_count; i++) {
        fprintf(makefile, "%s.o: %s.c ", files[i], files[i]);

        char dependencies[MAX_DEPENDENCIES][MAX_LINE];
        int dep_count = 0;
        char source_file[MAX_LINE];
        snprintf(source_file, sizeof(source_file), "%s.c", files[i]);
        extract_dependencies(source_file, dependencies, &dep_count);

        for (int j = 0; j < dep_count; j++) {
            fprintf(makefile, "%s ", dependencies[j]);
        }
        fprintf(makefile, "\n");
        fprintf(makefile, "\t$(CC) $(CFLAGS) -c %s.c\n\n", files[i]);
    }

    fprintf(makefile, "clean:\n");
    fprintf(makefile, "\tdel -f $(OBJS) $(OUTPUT)\n");

    fclose(makefile);
    printf("Makefile generated successfully.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s Main.c [CompiledLocationAndName] [Extra.c ...]\n", argv[0]);
        return 1;
    }

    char output[MAX_LINE] = "out.exe";
    if (argc > 2 && strchr(argv[2], '.')) {
        strncpy(output, argv[2], sizeof(output));
    }

    char files[MAX_FILES][MAX_LINE];
    int file_count = 0;

    for (int i = 1; i < argc; i++) {
        char *dot = strrchr(argv[i], '.');
        if (dot && strcmp(dot, ".c") == 0) {
            strncpy(files[file_count], argv[i], dot - argv[i]);
            files[file_count][dot - argv[i]] = '\0';
            file_count++;
        }
    }

    if (file_count == 0) {
        fprintf(stderr, "Error: No .c files provided\n");
        return 1;
    }

    generate_makefile(output, files, file_count);
    return 0;
}


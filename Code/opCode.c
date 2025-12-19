#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Step 1: Calculate total length of input args
  size_t total = 0;
  for (int i = 1; i < argc; i++) {
    total += 1 + strlen(argv[i]);
  }

  char *str = malloc(total + 1);
  if (str == NULL) {
    perror("cannot allocate string");
    return 1;
  }

  size_t pos = 0;
  *str = '\0';
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], ";") && *argv[i]) {
      if (pos > 0)
        str[pos++] = ' ';
      pos += strlen(strcpy(str + pos, argv[i]));
    }
  }

  // Step 2: Define imports
  const char *imported_libraries = "import math, sys, builtins, itertools, "
                                   "functools, operator, re, random, os; ";

  // Step 3: Allocate space for full command string
  const char *prefix = "py -c \"exec(\\\"";
  const char *middle = "s:str=\\\\\\\"";
  const char *suffix = "\\\\\\\"\\ntry:(r:=eval(s))and print(r)\\nexcept "
                       "SyntaxError:exec(s)\\\")\"";

  size_t result_len = strlen(prefix) + strlen(imported_libraries) +
                      strlen(middle) + strlen(str) + strlen(suffix) + 1;

  char *result = malloc(result_len);
  if (!result)
    return system("echo \"String allocation failed\"");

  // Step 4: Build the final command string
  strcpy(result, prefix);
  strcat(result, imported_libraries);
  strcat(result, middle);
  strcat(result, str);
  strcat(result, suffix);

  system(result);

  free(str);
  free(result);

  return 0;
}

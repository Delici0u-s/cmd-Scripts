#include <stdlib.h>
#include <string.h>

// execute all argc's combined as python command, as it'd be in shell python
int main(int argc, char *argv[]) {
  // read user input
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

  // convert to command and execute

  char *result = malloc(total + 1 + 25 + 66);
  if (!result)
    return system("echo \"String allocation failed\"");

  // py -c "exec(\"s:str=\\\"2 + 3 * 8\\\"\ntry:(r:=eval(s))and
  // print(r)\nexcept SyntaxError:exec(s)\")"

  strcpy(result, "py -c \"exec(\\\"s:str=\\\\\\\"");
  strcat(result, str);
  strcat(result, "\\\\\\\"\\ntry:(r:=eval(s))and print(r)\\nexcept "
                 "SyntaxError:exec(s)\\\")");
  system(result);

  return 0;
}
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LINES 3

int main(int argc, char *argv[]) {
  size_t lines_to_rem =
             ((argc < 2) ? DEFAULT_LINES : (size_t)atoi(argv[1])) + 2,
         i;
  if (lines_to_rem <= 2) {
    puts("Please enter a valid amount of lines");
    return 1;
  }

  for (i = 0; i < lines_to_rem; ++i)
    printf("\033[2K\033[F");
}

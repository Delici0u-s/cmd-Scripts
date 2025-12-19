#include <errno.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* Build the command string */
  size_t total = 0;
  for (int i = 1; i < argc; i++) {
    total += 1 + strlen(argv[i]);
  }
  char *cmd = malloc(total + 1);
  if (!cmd) {
    perror("malloc");
    return EXIT_FAILURE;
  }
  size_t pos = 0;
  cmd[0] = '\0';
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], ";") != 0 && argv[i][0] != '\0') {
      if (pos > 0) {
        cmd[pos++] = ' ';
      }
      pos += strlen(strcpy(cmd + pos, argv[i]));
    }
  }

  /* Time measurement */
  struct timespec t_start, t_end;
  if (clock_gettime(CLOCK_MONOTONIC, &t_start) != 0) {
    perror("clock_gettime");
    free(cmd);
    return EXIT_FAILURE;
  }

  int ret = system(cmd);

  if (clock_gettime(CLOCK_MONOTONIC, &t_end) != 0) {
    perror("clock_gettime");
    free(cmd);
    return EXIT_FAILURE;
  }

  free(cmd);

  /* Compute elapsed nanoseconds in a 128-bit integer */
  unsigned __int128 ns_total =
      (unsigned __int128)(t_end.tv_sec - t_start.tv_sec) * 1000000000ULL +
      (t_end.tv_nsec - t_start.tv_nsec);

  /* Break down into units */
  uint64_t hours = (uint64_t)(ns_total / (1000000000ULL * 3600));
  uint64_t rem_ns1 = (uint64_t)(ns_total % (1000000000ULL * 3600));
  uint64_t minutes = rem_ns1 / (1000000000ULL * 60);
  uint64_t rem_ns2 = rem_ns1 % (1000000000ULL * 60);
  uint64_t seconds = rem_ns2 / 1000000000ULL;
  uint64_t rem_ns3 = rem_ns2 % 1000000000ULL;
  uint64_t milliseconds = rem_ns3 / 1000000ULL;
  uint64_t rem_ns4 = rem_ns3 % 1000000ULL;
  uint64_t microseconds = rem_ns4 / 1000ULL;
  uint64_t nanoseconds = rem_ns4 % 1000ULL;

  setlocale(LC_CTYPE, "");
  const char *us_label =
      strstr(setlocale(LC_CTYPE, NULL), "UTF-8") ? "µs" : "micS";

  puts("\n\n\t--- execution time ---");
  /* Always print all units */
  printf("nanoseconds : %llu ns\n", (unsigned long long)ns_total);
  printf("microseconds: %llu %s\n", (unsigned long long)(ns_total / 1000ULL),
         us_label);
  printf("milliseconds: %llu ms\n",
         (unsigned long long)(ns_total / 1000000ULL));
  printf("seconds     : %llu s\n",
         (unsigned long long)(ns_total / 1000000000ULL));
  printf("minutes     : %llu min\n",
         (unsigned long long)((ns_total / 1000000000ULL) / 60ULL));
  printf("hours       : %llu h\n",
         (unsigned long long)((ns_total / 1000000000ULL) / 3600ULL));

  /* Condensed summary: hours, whole microseconds, remaining nanoseconds */
  /* First, detect UTF-8 for the µ */

  struct {
    uint64_t val;
    const char *suffix;
  } scales[] = {
      {hours, "h"},         {minutes, "min"},         {seconds, "s"},
      {milliseconds, "ms"}, {microseconds, us_label}, {nanoseconds, "ns"},
  };

  printf("\nSummary:");
  int start = 0;
  /* 1) find and print the first non-zero unit */
  for (int i = 0; i < (int)(sizeof(scales) / sizeof(*scales)); i++) {
    if (scales[i].val > 0) {
      printf(" %llu%s", (unsigned long long)scales[i].val, scales[i].suffix);
      start = i + 1;
      break;
    }
  }
  /* 2) print all remaining non-zero units */
  for (int i = start; i < (int)(sizeof(scales) / sizeof(*scales)); i++) {
    if (scales[i].val > 0) {
      printf(" %llu%s", (unsigned long long)scales[i].val, scales[i].suffix);
    }
  }
  printf("\n");

  return ret;
}

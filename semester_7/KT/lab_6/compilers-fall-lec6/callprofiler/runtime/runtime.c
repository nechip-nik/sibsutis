#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long Time;

static FILE *FileFD = NULL;

static void cleanup() {
  if (FileFD != NULL) {
    fclose(FileFD);
    FileFD = NULL;
  }
}

static void init() {
  if (FileFD == NULL) {
    FileFD = fopen("ppprofile_print.csv", "w");
    if (FileFD == NULL) {
      perror("Failed to open ppprofile.csv");
      exit(EXIT_FAILURE);
    }
    atexit(cleanup);
  }
}

static Time get_time() {
  struct timespec ts;
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) == -1) {
    perror("Failed to get time");
    exit(EXIT_FAILURE);
  }
  return 1000000000L * ts.tv_sec + ts.tv_nsec;
}

void __call_enter(const char *FnName) {
  init();
  Time T = get_time();
  void *Frame = __builtin_frame_address(1);
  if (fprintf(FileFD, "enter|%s|%llu|%p\n", FnName, T, Frame) < 0) {
    perror("Failed to write to ppprofile.csv");
    exit(EXIT_FAILURE);
  }
}

void __call_exit(const char *FnName) {
  init();
  Time T = get_time();
  void *Frame = __builtin_frame_address(1);
  if (fprintf(FileFD, "exit|%s|%llu|%p\n", FnName, T, Frame) < 0) {
    perror("Failed to write to ppprofile.csv");
    exit(EXIT_FAILURE);
  }
}
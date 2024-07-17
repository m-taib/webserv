#include <stdio.h>
#include <sys/stat.h>

int main() {
  struct stat statbuf;
  int ret = stat("includes", &statbuf);

  if (ret == 0) {
    if (S_ISREG(statbuf.st_mode)) {
      printf("Path points to a regular file\n");
    } else if (S_ISDIR(statbuf.st_mode)) {
      printf("Path points to a directory\n");
    } else {
      // Check for other file types using additional macros if needed
      printf("Path points to something else (not a regular file or directory)\n");
    }
  } else {
    perror("stat");
  }

  return 0;
}

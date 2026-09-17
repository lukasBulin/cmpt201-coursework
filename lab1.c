#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *buff = NULL;
  char *saveptr = NULL;
  size_t len = 0;
  ssize_t n = 0;

  while (1) {

    printf("Please enter some text: ");
    n = getline(&buff, &len, stdin);
    if (n == -1) {
      perror("getline has failed");
      free(buff);
      exit(EXIT_FAILURE);
    }
    buff[strcspn(buff, "\n")] = '\0';
    printf("Tokens: \n");
    char *ret = strtok_r(buff, " ", &saveptr);
    if (ret != NULL) {
      printf("  %s\n", ret);
    }

    while (ret != NULL) {
      ret = strtok_r(NULL, " ", &saveptr);
      if (ret != NULL) {
        printf("  %s\n", ret);
      }
    }
  }
  return 0;
}

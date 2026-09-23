#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *saveptr = NULL;
size_t len = 0;
ssize_t n = 0;

int main() {
  while (1) {

    printf("Enter programs to run.\n");
    printf("> ");
    n = getline(&saveptr, &len, stdin);
    if (n == -1) {
      perror("getline has failed");
      free(saveptr);
      exit(EXIT_FAILURE);
    }

    saveptr[strcspn(saveptr, "\n")] = '\0';

    pid_t pid = fork();

    if (pid == -1) {
      printf("Fork failed!\n");
    } else if (pid == 0) {
      execlp(saveptr, saveptr, NULL);
      // If exec reaches here, we know exec failed
      printf("Exec failure\n");
    } else {
      waitpid(pid, NULL, 0);
    }
  }
}

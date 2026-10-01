#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *history[5];
int historyIndex = 0;

void add_to_history(char *ptr, size_t size);
void print_history();
void initializeMemory();
void cleanMemory();

int main(void) {
  char *pSave = NULL;
  size_t size = 0;
  ssize_t n = 0;

  initializeMemory();

  while (1) {
    printf("Enter input: ");
    n = getline(&pSave, &size, stdin);
    if (n == -1) {
      perror("getline has failed");
      free(pSave);
      cleanMemory();
      exit(EXIT_FAILURE);
    }

    add_to_history(pSave, size);

    if (strcmp(pSave, "print\n") == 0) {
      print_history();
    }
  }

  cleanMemory();

  return 0;
}

void print_history() {
  for (int i = 0; i < 5; i++) {
    int realIndex = historyIndex + i;
    realIndex = realIndex % 5;
    if (history[realIndex] != NULL) {
      printf("%s", history[realIndex]);
    }
  }
}

void add_to_history(char *ptr, size_t size) {

  char *pString = malloc(size + 1);

  if (pString == NULL) {
    return;
  }

  strcpy(pString, ptr);

  if (history[historyIndex] != NULL) {
    free(history[historyIndex]);
  }

  history[historyIndex] = pString;

  if (historyIndex == 4) {
    historyIndex = 0;
  } else {
    historyIndex++;
  }
}

void initializeMemory() {
  for (int i = 0; i < 5; i++) {
    history[i] = NULL;
  }
}

void cleanMemory() {
  for (int i = 0; i < 5; i++) {
    if (history[i] != NULL) {
      free(history[i]);
      history[i] = NULL;
    }
  }
}

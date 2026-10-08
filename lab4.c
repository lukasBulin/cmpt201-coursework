#define _DEFAULT_SOURCE
#define BUF_SIZE 1024
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

void *increase_heap_size(int EXTRA_SIZE) {
  void *pNewBlock = sbrk(EXTRA_SIZE);
  if (pNewBlock == (void *)-1) {
    perror("sbrk failed");
    exit(EXIT_FAILURE);
  }
  return pNewBlock;
}

void initialize_block(struct header *block, uint64_t block_size, struct header *next_block) {
  block->size = block_size;
  block->next = next_block;
}

int main() {
  int EXTRA_SIZE = 256;
  void *heap_start = increase_heap_size(EXTRA_SIZE);

  struct header *pfirst_block = (struct header *)heap_start;
  struct header *psecond_block = (struct header *)((uint8_t *)heap_start + 128);

  print_out("first block:      %p\n", &pfirst_block, sizeof(void *));
  print_out("second block:     %p\n", &psecond_block, sizeof(void *));

  initialize_block(pfirst_block, 128, NULL);
  initialize_block(psecond_block, 128, pfirst_block);

  print_out("first block size: %lu\n", &pfirst_block->size, sizeof(uint64_t));
  print_out("first block next: %p\n", &pfirst_block->next, sizeof(void *));

  print_out("second block si*e: %lu\n", &psecond_block->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &psecond_block->next, sizeof(void *));

  int data_size = 128 - sizeof(struct header);
  uint8_t *pfirstbyte = (uint8_t *)pfirst_block + sizeof(struct header);
  uint8_t *psecondbyte = (uint8_t *)psecond_block + sizeof(struct header);
  memset(pfirstbyte, 0, data_size);
  memset(psecondbyte, 1, data_size);

  // for first block
  for (int i = 0; i < data_size; i++) {
    printf("%d\n", pfirstbyte[i]);
  }

  // for second block
  for (int i = 0; i < data_size; i++) {
    printf("%d\n", psecondbyte[i]);
  }
}

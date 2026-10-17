#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement first fit
  if (free_list_ptr == NULL) {
    return -1;
  }
  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size) {
      return free_list_ptr->id;
    } else {
      free_list_ptr = free_list_ptr->next;
    }
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement best fit
  if (free_list_ptr == NULL) {
    return -1;
  }

  int best_fit_id = -1;
  int dif = 0;
  int smallest_dif = -1;

  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size) {
      dif = free_list_ptr->size - size;
      if (smallest_dif == -1) {
        smallest_dif = dif;
        best_fit_id = free_list_ptr->id;
      }
      if (dif < smallest_dif) {
        smallest_dif = dif;
        best_fit_id = free_list_ptr->id;
      }
      if (dif == 0) {
        best_fit_id = free_list_ptr->id;
        return best_fit_id;
      }
    }
    free_list_ptr = free_list_ptr->next;
  }

  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement worst fit
  if (free_list_ptr == NULL) {
    return -1;
  }
  int worst_fit_id = -1;
  int dif = 0;
  int largest_dif = -1;

  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size) {
      dif = free_list_ptr->size - size;
      if (largest_dif == -1) {
        largest_dif = dif;
        worst_fit_id = free_list_ptr->id;
      }
      if (dif > largest_dif) {
        largest_dif = dif;
        worst_fit_id = free_list_ptr->id;
      }
    }
    free_list_ptr = free_list_ptr->next;
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // TODO: Print out the IDs
  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

// Part 2
// Coalescing contiguous free blocks:
//
// 1. Assume the existing free list is already coalesced.
//
// 2. When a new block Z is freed, check whether the block immediately
//    before Z in memory (M) is free and contiguous with Z.
//
// 3. Check whether the block immediately after Z in memory (N) is free
//    and contiguous with Z.
//
// 4. If both M and N are free and contiguous with Z:
//     Add the sizes of M, Z, and N.
//     Keep the header at the start of M.
//     Update the next pointer to skip the nodes for Z and N.
//
// 5. If only M is free and contiguous with Z:
//     Add the size of Z to M.
//     Keep the header at the start of M.
//     Update M's next pointer to skip Z.
//
// 6. If only N is free and contiguous with Z:
//     Add the size of N to Z.
//     Keep the header at the start of Z.
//     Update Z's next pointer to skip N.
//
// 7. If neither M nor N is free and contiguous with Z:
//     Add Z to the free list as a separate free block.
//
// 8. Ensure the free list links are updated correctly and that each
//     free block is represented by only one header node.

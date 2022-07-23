#ifndef NX_SUDOKU_H
#define NX_SUDOKU_H

#include <stdint.h>
#include <stdio.h>

typedef struct sudoku_t {
    uint16_t cells[9][9];
} sudoku_t;

void sudoku_init_from_file(sudoku_t* self, FILE* fp);
void sudoku_resolve(sudoku_t* self);
void sudoku_print(sudoku_t* self);

#endif

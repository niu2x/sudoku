#include "sudoku.h"
#include <string.h>

void sudoku_init_from_file(sudoku_t* self, FILE* fp)
{
    for (int i = 0; i < 9; i++) {
        int n;
        for (int j = 0; j < 9; j++) {
            fscanf(fp, "%1d", &n);
            if (n == 0) {
                self->cells[i][j] = 0x01FF;
            } else {
                self->cells[i][j] = 1 << (n - 1);
            }
        }
        fscanf(fp, "\n");
    }
}

int cell_get(uint16_t u)
{
    int n = 1;
    while (u != 1) {
        if (u & 1)
            return 0;
        u >>= 1;
        n++;
    }
    return n;
}

void sudoku_print(sudoku_t* self)
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int n = cell_get(self->cells[i][j]);
            if (n == 0)
                printf(" ");
            else
                printf("%d", n);

            if ((j + 1) % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if ((i + 1) % 3 == 0)
            printf("\n");
    }
}

static int sudoku_test(sudoku_t* self, int y, int x, int kk)
{
    for (int i = y / 3 * 3; i < y / 3 * 3 + 3; i++)
        for (int j = x / 3 * 3; j < x / 3 * 3 + 3; j++) {
            if (!(i == y && j == x)) {
                int t;
                if ((t = cell_get(self->cells[i][j])) == kk) {
                    printf("%d %d %d\n", y, x, kk);
                    return 1;
                }
                printf("t %d %d %d %d %d %d %d\n", y, x, kk, t,
                    self->cells[i][j], i, j);
            }
        }

    for (int j = 0; j < 9; j++) {
        int i = y;
        if (!(i == y && j == x)) {
            int t;
            if ((t = cell_get(self->cells[i][j])) == kk) {
                printf("%d %d %d\n", y, x, kk);
                return 1;
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        int j = x;
        if (!(i == y && j == x)) {
            int t;
            if ((t = cell_get(self->cells[i][j])) == kk) {
                printf("%d %d %d\n", y, x, kk);
                return 1;
            }
        }
    }

    return 0;
}

static int sudoku_resolved(sudoku_t* self)
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            uint16_t k = self->cells[i][j];
            if (cell_get(k) == 0)
                return 0;
        }
    }
    return 1;
}


static int sudoku_simple_resolve(sudoku_t* self)
{
    int n;
    do {
        n = 0;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                uint16_t k = self->cells[i][j];
                if (cell_get(k) != 0)
                    continue;

                int kk = 1;
                while (k) {
                    if (k & 1) {
                        if (sudoku_test(self, i, j, kk)) {
                            n++;
                            self->cells[i][j] &= ~(1 << (kk - 1));
                            printf("disable %d at(%d, %d)\n", kk, i, j);
							if(self->cells[i][j] == 0) return 1;
                        }
                    }
                    k >>= 1;
                    kk++;
                }
            }
        }

    } while (n > 0);
	return 0;
}

static void sudoku_advance_resolve(sudoku_t* self) {

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            uint16_t k = self->cells[i][j];
            if (cell_get(k) == 0) {
				for(int nn = 1; nn <= 9; nn++) {
					if(k & (1 << (nn-1))){
						sudoku_t copy;
						memcpy(&copy, self, sizeof(sudoku_t));
						copy.cells[i][j] = 1 << (nn-1);

						if(sudoku_simple_resolve(&copy)) continue;
						if(sudoku_resolved(&copy)){
							memcpy(self, &copy, sizeof(sudoku_t));
							break;
						}
						else{
							sudoku_advance_resolve(&copy);
							if(sudoku_resolved(&copy)){
								memcpy(self, &copy, sizeof(sudoku_t));
								break;
							}
						}
					}
				}
				return ;
			}
        }
    }

}

void sudoku_resolve(sudoku_t* self)
{
    sudoku_simple_resolve(self);
    if (!sudoku_resolved(self)) {
		sudoku_advance_resolve(self);
    }
}

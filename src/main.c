#include <stdio.h>
#include <sudoku.h>

int main()
{
    FILE* fp = fdopen(0, "r");
    sudoku_t sudoku;
    sudoku_init_from_file(&sudoku, fp);
    sudoku_resolve(&sudoku);
    sudoku_print(&sudoku);
    return 0;
}

#include <curses.h>
#include <stdlib.h>

int grid_rows(void)
{
    // account for top and bottom border
    return LINES - 2;
}

int grid_cols(void)
{
    // account for left and right border
    return COLS - 2;
}

int **grid_create(void)
{
    int rows = grid_rows();
    int cols = grid_cols();

    int **grid = malloc(rows * sizeof(int *));

    for (int y = 0; y < rows; y++)
    {
        grid[y] = malloc(cols * sizeof(int));
        for (int x = 0; x < cols; x++)
        {
            grid[y][x] = rand() % 2;
        }
    }

    return grid;
}

void grid_free(int **grid)
{
    int rows = grid_rows();

    for (int y = 0; y < rows; y++)
    {
        free(grid[y]);
    }

    free(grid);
}

int **grid_next(int **grid)
{
    int rows = grid_rows();
    int cols = grid_cols();

    int **new_grid = grid_create();

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            int neighbours = 0;

            // count the alive neighbours
            for (int n_y = -1; n_y <= 1; n_y++)
            {
                for (int n_x = -1; n_x <= 1; n_x++)
                {
                    // skip the current cell
                    if (n_y == 0 && n_x == 0)
                    {
                        continue;
                    }

                    int neighbour_y = (y + n_y + rows) % rows;
                    int neighbour_x = (x + n_x + cols) % cols;

                    neighbours += grid[neighbour_y][neighbour_x];
                }
            }

            int alive = grid[y][x];

            new_grid[y][x] = (!alive && neighbours == 3) ? 1
                             : (alive && (neighbours < 2 || neighbours > 3))
                                 ? 0
                                 : alive;
        }
    }

    grid_free(grid);

    return new_grid;
}

void grid_draw(int **grid)
{
    int rows = grid_rows();
    int cols = grid_cols();

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            char c = grid[y][x] ? '#' : ' ';
            mvaddch(y + 1, x + 1, c);
        }
    }

    refresh();
}

int main(void)
{
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    box(stdscr, 0, 0);

    int **grid = grid_create();

    char i = '\0';
    while (i != 'q')
    {
        grid_draw(grid);
        grid = grid_next(grid);

        // ~30 fps
        napms(34);

        i = getch();
    }

    endwin();
    grid_free(grid);

    return 0;
}

#include <curses.h>
#include <stdlib.h>

int grid_rows()
{
    // account for top and bottom border
    return LINES - 2;
}

int grid_cols()
{
    // account for left and right border
    return COLS - 2;
}

int **grid_create()
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

            // top row
            if (y == 0)
            {
                // first column
                if (x == 0)
                {
                    neighbours += grid[y][x + 1];
                    neighbours += grid[y + 1][x];
                    neighbours += grid[y + 1][x + 1];
                }
                // last column
                else if (x == cols - 1)
                {
                    neighbours += grid[y + 1][x];
                    neighbours += grid[y + 1][x - 1];
                    neighbours += grid[y][x - 1];
                }
                else
                {
                    neighbours += grid[y][x - 1];
                    neighbours += grid[y][x + 1];
                    neighbours += grid[y + 1][x - 1];
                    neighbours += grid[y + 1][x];
                    neighbours += grid[y + 1][x + 1];
                }
            }
            // bottom row
            else if (y == rows - 1)
            {
                // first column
                if (x == 0)
                {
                    neighbours += grid[y - 1][x];
                    neighbours += grid[y - 1][x + 1];
                    neighbours += grid[y][x + 1];
                }
                // last column
                else if (x == cols - 1)
                {
                    neighbours += grid[y - 1][x];
                    neighbours += grid[y - 1][x - 1];
                    neighbours += grid[y][x - 1];
                }
                else
                {
                    neighbours += grid[y][x - 1];
                    neighbours += grid[y - 1][x - 1];
                    neighbours += grid[y - 1][x];
                    neighbours += grid[y - 1][x + 1];
                    neighbours += grid[y][x + 1];
                }
            }
            else
            {
                // first column
                if (x == 0)
                {
                    neighbours += grid[y - 1][x];
                    neighbours += grid[y - 1][x + 1];
                    neighbours += grid[y][x + 1];
                    neighbours += grid[y + 1][x + 1];
                    neighbours += grid[y + 1][x];
                }
                // last column
                else if (x == cols - 1)
                {
                    neighbours += grid[y - 1][x];
                    neighbours += grid[y - 1][x - 1];
                    neighbours += grid[y][x - 1];
                    neighbours += grid[y + 1][x - 1];
                    neighbours += grid[y + 1][x];
                }
                else
                {
                    neighbours += grid[y - 1][x - 1];
                    neighbours += grid[y - 1][x];
                    neighbours += grid[y - 1][x + 1];
                    neighbours += grid[y][x - 1];
                    neighbours += grid[y][x + 1];
                    neighbours += grid[y + 1][x - 1];
                    neighbours += grid[y + 1][x];
                    neighbours += grid[y + 1][x + 1];
                }
            }

            int alive = grid[y][x];

            new_grid[y][x] = (!alive && neighbours == 3) ? 1
                             : (alive && (neighbours < 2 || neighbours > 3))
                                 ? 0
                                 : alive;
        }
    }

    free(grid);

    return new_grid;
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

int main()
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

        napms(1000);

        i = getch();
    }

    endwin();
    grid_free(grid);

    return 0;
}

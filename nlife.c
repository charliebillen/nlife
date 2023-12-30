#include <curses.h>
#include <stdlib.h>
#include <time.h>

int rows;
int cols;

int **grid_create(int seed)
{
    srand(seed);

    int **grid = malloc(rows * sizeof(int *));

    for (int y = 0; y < rows; y++)
    {
        grid[y] = malloc(cols * sizeof(int));

        if (seed > 0)
        {
            // Randomly populate row
            for (int x = 0; x < cols; x++)
            {
                grid[y][x] = rand() % 2;
            }
        }
    }

    return grid;
}

void grid_free(int **grid)
{
    for (int y = 0; y < rows; y++)
    {
        free(grid[y]);
    }

    free(grid);
}

int **grid_next(int **grid)
{
    int **new_grid = grid_create(0);

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            /*
             * Count the living neighbours around [y,x], wrapping at the screen
             * edges
             *
             *   [y-1, x-1] [y-1, x] [y-1, x+1]
             *   [y,   x-1] [y,   x] [y,   x+1]
             *   [y+1, x-1] [y+1, x] [y+1, x+1]
             *
             */
            int live_neighbours = 0;

            for (int n_y = -1; n_y <= 1; n_y++)
            {
                for (int n_x = -1; n_x <= 1; n_x++)
                {
                    // Skip the current cell
                    if (n_y == 0 && n_x == 0)
                    {
                        continue;
                    }

                    int neighbour_y = (y + n_y + rows) % rows;
                    int neighbour_x = (x + n_x + cols) % cols;

                    live_neighbours += grid[neighbour_y][neighbour_x];
                }
            }

            int alive = grid[y][x];

            /*
             * Rules of Game of Life:
             * - If a dead cell has exactly 3 living neighbours it lives
             * - If a living cell has 2 or 3 living neighbours it lives
             * - Otherwise it dies
             */
            new_grid[y][x] =
                (!alive && live_neighbours == 3) ? 1
                : (alive && (live_neighbours < 2 || live_neighbours > 3))
                    ? 0
                    : alive;
        }
    }

    grid_free(grid);

    return new_grid;
}

void grid_draw(int **grid)
{
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

    // Account for the border on each side
    rows = LINES - 2;
    cols = COLS - 2;
    int **grid = grid_create(time(NULL));

    char i = '\0';
    while ((i = getch()) != 'q')
    {
        grid_draw(grid);
        grid = grid_next(grid);

        // ~24 FPS - Cinematic!
        napms(42);
    }

    endwin();
    grid_free(grid);

    return 0;
}

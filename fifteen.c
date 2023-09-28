/**************************************************************************
 * This work originally copyright David J. Malan of Harvard University, or-
 * iginally released under:
 * CC BY-NC-SA 3.0  http://creativecommons.org/licenses/by-nc-sa/3.0/
 * licensing.
 *
 * It has been adapted for use in csci 1730.  Per the share-alike
 * clause of this license, this document is also released under the
 * same license.
 **************************************************************************/
/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
int move(int tile);
int won(void);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("win!\n");
            break;
        }

        // prompt for move
        printf("Tile to move (0 to exit): ");
        int tile;
		scanf("%d", &tile);
		getchar();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(50000);
        }

        // sleep thread for animation's sake
        usleep(50000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(200000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
	int count = d*d - 1;

	//Adding the number in the array in reverse
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			board[i][j] = count;
			count--;
		}
	}

	// Switch 1 and 2 if the dimension is even
	if (d % 2 == 0) {
		board[d - 1][d - 2] = 2;
		board[d - 1][d - 3] = 1;
	}

	//In this case "-1" is a blank space
	board[d - 1][d - 1] = -1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			if (board[i][j] == -1) {
				printf("   ");
			} else if (board[i][j] < 10 && board[i][j] > 0) {
				printf(" %d ", board[i][j]);
			} else {
				printf("%d ", board[i][j]);
			}
		}
		printf("\n");
	}
}

/**
 * If tile borders empty space, moves tile and returns 1 (true), else
 * returns 0 (false).
 */
int move(int tile)
{
	//Checks for invalid tiles
	if (tile < 1 || tile > d*d - 1) {
		return 0;
	}

	int tileX = 0;
	int tileY = 0;
	int blankX = 0;
	int blankY = 0;

	//Finds the position of the tile
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			if (board[i][j] == tile) {
				tileX = i;
				tileY = j;
				break;
			}
		}
	}

	//Finds the position of the blank space
	for (int a = 0; a < d; a++) {
		for (int b = 0; b < d; b++) {
			if (board[a][b] == -1) {
				blankX = a;
				blankY = b;
				break;
			}
		}
	}

	//Makes sure that the tile and blank space position isn't the same
	if (tileX == blankX && tileY == blankY) {
		return 0;
	}

	//Checks if the blank space is adjacent to the tile and swaps if it is true
	if ((abs(tileX - blankX) == 0 || abs(tileX - blankX) == 1) &&
		(abs(tileY - blankY) == 0 || abs(tileY - blankY) == 1)) {
		board[tileX][tileY] = -1;
		board[blankX][blankY] = tile;
		return 1;
	}
	return 0;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
int won(void)
{
	int count = 1;
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			if ((i == d - 1) && (j == d - 1)) {
				if (board[i][j] != -1) {
					return 0;
				}
			} else {
				if (board[i][j] != count) {
					return 0;
				}
			}
			count++;
		}
	}
	return 1;
}

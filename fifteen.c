/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// variables to keep track of where the blank is
int xloc;
int yloc;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();
    
    xloc = d-1;   //defines x and y coordinates of the blank
    yloc = d-1;
    
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
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
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    bool even = false;
    int size = d*d;
    if (size % 2 == 0)
        even = true;
        
    for (int i = 0; i < d; i++)   
    {
        for (int j= 0; j < d; j++)
        {
            board[i][j] = size - 1;
            size -= 1;
        }       
    }
    
    if (even)
    {    
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
   for (int i = 0; i < d; i++)
   {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] > 9)
                printf("%i ", board[i][j]);
            else if (board[i][j] > 0)
                printf("%2i ",board[i][j]);
            else
                printf(" _");
        } 
   printf("\n");
   }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
 
 // starting blank locations information


bool move(int tile)
{
    int tilex;
    int tiley;
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)   
        {
            if (board[i][j]== tile)     //incrementally goes through the board looking for the location    
            {    
                tilex = i;      //of the tile requested and saves it to array
                tiley = j;
            }
        }
    }             
    //next step checks all posible locations of acceptable places for the variable to be
    if ((tile == board[xloc+1][yloc] && xloc < 3) ||(tile ==  board[xloc-1][yloc] && xloc > -1)|| 
    (tile ==  board[xloc][yloc+1] && yloc < 3)|| (tile == board[xloc][yloc-1] && yloc > -1))  
    {
        board[tilex][tiley] = 0;   //switches tiles on gameboard
        board[xloc][yloc] = tile;
          
        xloc = tilex;      //moves the blanks location in gloabal variable
        yloc = tiley;
        
        return true;
    }
    else 
        return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int counter = 1;  // variable to check all the values in the grid 
    
    for (int k = 0; k < d; k++)
    {
        for (int l = 0; l < d; l++)
        {              
            if (board[k][l] == 0)
                counter = 0;
                       
            if (board[k][l] != counter)  //checks if all the other values are where they are supposed to be                 
                return false; 
                          
        counter++;  //iterates counter
        }
    
    }
    
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}

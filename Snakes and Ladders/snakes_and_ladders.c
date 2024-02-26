/***************************/
/*         files           */
/***************************/
#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

/***************************/
/*         Define          */
/***************************/
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define EMPTY (-1)
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define BASE_NUMBER 10
#define GOOD_ARGS 3
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define FIRST_BIGGER 1
#define SECOND_BIGGER (-1)
#define EQUAL 0


/*********************************/
/*  snakes_and_ladders function  */
/*********************************/


/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    // Cell number 1-100
    int number;

    // ladder_to represents the jump of the ladder in case there is one from this square
    int ladder_to;

    // snake_to represents the jump of the snake in case there is one from this square
    int snake_to;

    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

static
/**
 * struct represents a Cell in the game board
 * @parm cell_node the cell that we are checking
 * @return true- is last , false - else
 */
bool check_if_last (const void *cell_node)
{
  Cell *seq = (Cell *) cell_node;
  int num = (int) seq->number;
  // if its the last cell of board
  if (num == BOARD_SIZE)
  {
    return true;
  }
  return false;
}
static
/**
 * struct represents a Cell in the game board
 * @parm cell_node the cell that we are printing
 */
void print_cell (const void *cell_node)
{
  // Cast void pointer to Cell
  Cell *seq = (Cell *) cell_node;
  int num = (int) seq->number;

  // print "[" Number "]"
  printf ("[");
  printf ("%d", num);
  printf ("]");

  /* Check what the path to next cell is*/

  // the cell contains a latter
  if (seq->ladder_to != EMPTY)
  {
    printf ("-ladder to %d -> ", seq->ladder_to);
  }

    // the cell contains a snake
  else if (seq->snake_to != EMPTY)
  {
    printf ("-snake to %d -> " ,seq->snake_to);
  }

    // the cell is not last
  else if (check_if_last (seq) == false)
  {
    printf (" -> ");
  }

  // the cell is last there for nothing will be printed after.

}

static
/**
 * struct represents a Cell in the game board
 * @parm cell_node the cell that we are freeing
 */
void free_cell (void *cell_node)
{
  // free the allocated memory of cell
  free (cell_node);
}
static
/**
 * struct represents a Cell in the game board
 * @parm cell_node the cell that we are copying
 * @return same cell in heap
 */
void *copy_cell (const void *cell_node)
{
  char *temp_src = (char *) (cell_node);
  size_t len = sizeof (Cell);
  // Allocate new memory on heap foe word pointer
  // Dynamic function
  char *new_cell = malloc (sizeof (Cell));

  // Check successful Allocation
  if (new_cell == NULL)
  {
    return NULL;
  }

  // copy cell from stack to heap byte by byte
  for (size_t i = 0; i < len; i++)
  {
    new_cell[i] = temp_src[i];
  }
  // cast from char to cell
  Cell *ret_cell = (Cell *) new_cell;
  return ret_cell;

}
static
/**
 * struct represents a Cell in the game board
 * @parm cell_node_1 the cell that we are compering
 * @parm cell_node_2 the cell that we are compering
 * @return int according to value (like any cmp in C)
 */
int cmp_cell (const void *cell_node_1, const void *cell_node_2)
{
  Cell *seq_1 = (Cell *) cell_node_1;
  int num_1 = (int) seq_1->number;

  Cell *seq_2 = (Cell *) cell_node_2;
  int num_2 = (int) seq_2->number;

  // Check witch num is bigger
  if (num_1 > num_2)
  {
    return FIRST_BIGGER;
  }
  else if (num_1 < num_2)
  {
    return SECOND_BIGGER;
  }
  return EQUAL;

}

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
  {
    free_database (database);
  }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    cells[i] = malloc (sizeof (Cell));
    if (cells[i] == NULL)
    {
      for (int j = 0; j < i; j++)
      {
        free (cells[j]);
      }
      handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
      return EXIT_FAILURE;
    }
    *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
  }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
  {
    int from = transitions[i][0];
    int to = transitions[i][1];
    if (from < to)
    {
      cells[from - 1]->ladder_to = to;
    }
    else
    {
      cells[from - 1]->snake_to = to;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    add_to_database (markov_chain, cells[i]);
  }

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    from_node = get_node_from_database (markov_chain, cells[i])->data;

    if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
    {
      index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
      to_node = get_node_from_database (markov_chain, cells[index_to])
          ->data;
      add_node_to_frequencies_list (from_node, to_node, markov_chain);
    }
    else
    {
      for (int j = 1; j <= DICE_MAX; j++)
      {
        index_to = ((Cell *) (from_node->markov_data))->number + j - 1;
        if (index_to >= BOARD_SIZE)
        {
          break;
        }
        to_node = get_node_from_database (markov_chain, cells[index_to])
            ->data;
        add_node_to_frequencies_list (from_node, to_node, markov_chain);
      }
    }
  }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    free (cells[i]);
  }
  return EXIT_SUCCESS;
}

static
/**
 * fills chain
 * @param markov_chain
 */
void fill_chain (MarkovChain *chain)
{
  // Allocate a pointer to the word print function
  chain->print_func = print_cell;

  // Allocate a pointer to the word compere function
  chain->comp_func = cmp_cell;

  // Allocate a pointer to the word free function
  chain->free_data = free_cell;

  // Allocate a pointer to the word copy function
  chain->copy_func = copy_cell;

  // Allocate a pointer to the function that checks if a ending word
  chain->is_last = check_if_last;

}

static
/**
 * prints all the path's
 * @param markov_chain
 * @param path_amount
 */
void print_path (MarkovChain *chain, int path_amount)
{
  // Every invitation is a tweet
  for (int i = 1; i <= path_amount; i++)
  {
    // first node
    MarkovNode *first_node = chain->database->first->data;

    // print the tweet number
    printf ("Random Walk %d: ", i);

    // print the tweet
    generate_tweet (chain, first_node, MAX_GENERATION_LENGTH);

    // Move down a line
    printf ("\n");

  }
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
// Check that argc are valid
  if (argc != GOOD_ARGS)
  {
    printf ("Usage: Valid number of inputs ");
    return EXIT_FAILURE;
  }

  // declare Variables
  unsigned int seed = strtol (argv[1], NULL, BASE_NUMBER);
  int path_amount = (int) strtol (argv[2], NULL, BASE_NUMBER);

  // Run seed
  srand (seed);

  // Start markov chain
  MarkovChain *chain = malloc (sizeof (MarkovChain));
  if (chain == NULL)
  {
    printf ("Allocation failure: Not enough memory");
    return EXIT_FAILURE;
  }
  // Fill all chain Variables
  fill_chain (chain);

  // Fill the data base
  int ans = fill_database (chain);

  // Generate tweets
  print_path (chain, path_amount);

  // static memory method
  free_database (&chain);
  return ans;
}




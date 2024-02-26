/***************************/
/*         files           */
/***************************/
#include "linked_list.h"
# include "markov_chain.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
# include <string.h> // For strtok()


/***************************/
/*         Define          */
/***************************/
#define WITH_WORD 5
#define WITH_OUT_WORD 4
#define BASE_NUMBER 10
# define MAX_WORD 100
# define MAX_LINE 1000
#define MAX_FILE (MAX_LINE * MAX_WORD)
#define MAX_WORD_IN_TWEET 20

/***************************/
/*  tweet markov function  */
/***************************/

static
bool check_if_last (const void *word)
{
  // Cast void pointer to char and get length
  char *str_word = (char *) (word);
  int len = (int) strlen (str_word);

  // if its the last word of tweet
  if (str_word[len - 1] == '.')
  {
    return true;
  }
  return false;
}

static
void print_word (const void *word)
{
  // Cast void pointer to char
  char *str_word = (char *) (word);

  if (check_if_last (str_word) == true)
  {
    printf ("%s", str_word);
  }
  else
  {
    // print word and space after word
    printf ("%s ", str_word);
  }
}
static
void free_word (void *word_ptr)
{
  // free the allocated memory of word
  free (word_ptr);
}

static
void *copy_word (const void *word)
{
  // Allocate new memory on heap foe word pointer
  // Dynamic function
  char *new_word = malloc (MAX_WORD);

  // Check successful Allocation
  if (new_word == NULL)
  {
    return NULL;
  }

  // copy word from stack to heap
  strcpy(new_word, word);

  return new_word;

}

/***************************/
/*      tweet function     */
/***************************/

static
bool add_to_frequencies_list (MarkovChain *chain, char *word, char *old_word)
{
  Node *first = get_node_from_database (chain, old_word);
  Node *second = get_node_from_database (chain, word);
  bool check = add_node_to_frequencies_list (first->data, second->data, chain);
  if (check == false)
  { return false; }
  return true;
}

static
bool fill_helper (MarkovChain *chain, char *word, char *old_word)
{
  // Adds new node to link list in data base and checks if succeeded
  Node *check = add_to_database (chain, word);
  if (check == NULL)
  { return false; }

  // checks if the word came after a word
  if (old_word != NULL && check_if_last (old_word) == false)
  {
    // Adds new word to the word before frequencies_list
    bool ans = add_to_frequencies_list (chain, word, old_word);
    if (ans == false)
    { return false; }

  }
  return true;
}
static
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  bool bol = true; // declare successes flag and counter
  int count = 0;
  char line[MAX_LINE] = {0}; // declare line string
// Run on all lines
  while (fgets (line, MAX_LINE, fp) != NULL && count < words_to_read)
  { // declaring a pointer to the first word in line
    char *word = strtok (line, " \t\n");
    char *old_word = NULL; // declaring a pointer to the last word in use
    while (word != NULL && count < words_to_read)  //go Through words in line
    {
      char *r_char = strchr (word, '\r'); // Remove carriage from word
      if (r_char != NULL)
      { *r_char = '\0'; }
      if (strlen (word) == 0)
      { // check if word is empty
        word = strtok (NULL, " \t\n"); // skip empty word and continue
        continue;
      }
      int len = (int) strlen (word); // allocate size and  memory for word
      char *new_word = malloc ((len + 1) * sizeof (char));
      if (new_word == NULL)
      {
        bol = false;
        break;
      }
      strcpy(new_word, word); // copy word from stack to heap
      // add word to data base
      bool ans = fill_helper (markov_chain, new_word, old_word);
      if (ans == false)
      {
        free (new_word);
        bol = false;
        break;
      }
      old_word = word;
      word = strtok (NULL, " \t\n");
      free (new_word);
      new_word = NULL;
      count++;
    }
  }
  if (bol == false) // if not all were true
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free_database (&markov_chain); // free all the data base
    markov_chain = NULL;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static
/**
 * Checks if all args all correct
 * @param argc Amount of arguments
 * @param path File link path
 * @return path success, Null otherwise
 */
FILE *check_arg (int argc, const char *path)
{
  // checks if argc amount correct
  if (argc != WITH_WORD && argc != WITH_OUT_WORD)
  {
    printf ("Usage: Valid number of inputs ");
    return NULL;
  }
  else
  {
    // checks if file path valid
    FILE *tweet = fopen (path, "r");
    if (tweet == NULL)
    {
      printf ("Error: Valid file");
      return NULL;
    }
    return tweet;
  }
}

static
/**
 * prints and generated the tweets
 * @param chain the filled markov chain
 * @param tweet_amount The amount of tweets print
 * @return 0 on success, 1 otherwise
 */
void print_tweets (MarkovChain *chain, int tweet_amount)
{
  // Every invitation is a tweet
  for (int i = 1; i <= tweet_amount; i++)
  {

    // print the tweet number
    printf ("Tweet %d: ", i);

    // print the tweet
    generate_tweet (chain, NULL, MAX_WORD_IN_TWEET);

    // Move down a line
    printf ("\n");

  }
}

static
void fill_chain (MarkovChain *chain)
{
  // Allocate a pointer to the word print function
  chain->print_func = print_word;

  // Allocate a pointer to the word compere function
  chain->comp_func = (comp_func) strcmp;

  // Allocate a pointer to the word free function
  chain->free_data = free_word;

  // Allocate a pointer to the word copy function
  chain->copy_func = copy_word;

  // Allocate a pointer to the function that checks if a ending word
  chain->is_last = check_if_last;

}

int main (int argc, char *argv[])
{
  // Check that argc are valid
  FILE *path = check_arg (argc, argv[3]);
  if (path == NULL)
  { return EXIT_FAILURE; }

  // declare Variables
  unsigned int seed = strtol (argv[1], NULL, BASE_NUMBER);
  srand (seed);
  int tweet_amount = (int) strtol (argv[2], NULL, BASE_NUMBER);
  int word_amount = MAX_FILE;
  if (argc == WITH_WORD)
  { word_amount = (int) strtol (argv[4], NULL, BASE_NUMBER); }

  // Start markov chain
  MarkovChain *chain = calloc (1, sizeof (MarkovChain));
  if (chain == NULL)
  {
    printf ("Allocation failure: Not enough memory");
    return EXIT_FAILURE;
  }
  // Fill all chain Variables
  fill_chain (chain);

  // Fill the data base
  int ans = fill_database (path, word_amount, chain);

  // Generate tweets
  print_tweets (chain, tweet_amount);

  // close file
  fclose (path);
  // static memory method
  free_database (&chain);
  return ans;
}


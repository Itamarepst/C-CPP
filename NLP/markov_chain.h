#ifndef _MARKOV_CHAIN_H_
#define _MARKOV_CHAIN_H_

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"

/**
 * insert Structs here
 */

typedef struct MarkovNodeFrequency MarkovNodeFrequency;

typedef struct MarkovChain
{
    LinkedList *database;
    // A pointer to the linked list that will hold all the data

} MarkovChain;

typedef struct MarkovNode
{
    char *word_str_ptr;
    // The word as a string

    MarkovNodeFrequency *frequencies_list;
    // pointer to the frequency list

    int length;
    // The amount of words in frequencies_list

} MarkovNode;

struct MarkovNodeFrequency
{
    MarkovNode *word_Node_ptr;
    // A pointer to a MarkovNode

    int frequency;
    // The amount of times the word (*MarkovNode -> data) appears
};



/**
* If word_str_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given word_str_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr);

/**
* Check if word_str_ptr is in database. If so, return the markov_node
 * wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr);

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool
add_node_to_frequencies_list (MarkovNode *first_node, MarkovNode *second_node);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_database (MarkovChain **ptr_chain);

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/** Receives markov_node and a name of the second to the markov_node .
 * checks if in thr frequencies list.
 * if it is return the place in arr of the second markov_node
 * else returns -1
 * @param *first_node - the node we are lokinf in
 * @param *word a string of the name on the second node
 * @return place/failure: place if the node was found, false if in
 * case of not.
 */
int check_in_frequencies_list (MarkovNode *first_node, char *word);

/** Receives name of a markov_node .
 * checks if it a word that ends a sentence.
 * if it is return true
 * else returns false
 * @param *word a string of the name on the node
 * @return bool
 */
bool check_if_last(char *word );
#endif /* _MARKOV_CHAIN_H_ */

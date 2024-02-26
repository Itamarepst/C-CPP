# include "markov_chain.h"
#include <string.h>

#define TRUE 0
# define ONE_OPTION 1
#define NOT_IN (-1)

MarkovNode *create_new_node (const void *data_ptr, copy_func markov_copy)
{
  // dynamic style - add_to_database will free.
  MarkovNode *new_markov_node = malloc (sizeof (*new_markov_node));

  if (new_markov_node != NULL)
  {
    //*malloc (elem_size * sizeof (char));*///

    // allocate memory and copy data from stack to heap
    void *new_data = markov_copy (data_ptr);

    if (new_data == NULL)
    {
      return NULL;
    }

    // Starting the new_markov_node data
    *new_markov_node = (MarkovNode) {new_data, NULL , 0};

    return new_markov_node;
  }
  return NULL;
}
/**
* Check if Markov Node data is in database. If so, return the node
 * otherwise return NULL.
 * @param markov_chain the chain of nodes - database
 * @param cur the first node to in the database
 * @param data_ptr the state to look for
 * @param len the length of the list
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *check_in_data_base (MarkovChain *markov_chain, Node *cur,
                          const void *data_ptr, int len)
{

  // Check if link list is empty
  if (cur == NULL)
  {
    return NULL;
  }

  // Counter for loops
  int count = 0;

  // Loop throw all nodes
  while (count < len)
  {
    // Checks if data are equal
    if (markov_chain->comp_func (cur->data->markov_data, data_ptr) == 0)
    {
      return cur;
    }
    // Moves to the next node
    cur = cur->next;
    count += 1;
  }
  return NULL;
}

LinkedList *start_data_base ()
{
  // start Link list
  LinkedList *data_base = malloc (sizeof (LinkedList));

  // memory check
  if (data_base == NULL)
  { return NULL; }

  // Connect the chain to link list
  data_base[0] = (LinkedList) {NULL, NULL, 0};
  return data_base;
}

Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  // check id no data allocated
  if (markov_chain->database == NULL)
  {
    markov_chain->database = start_data_base ();
    // if allocation failed
    if (markov_chain->database == NULL)
    {
      return NULL;
    }
  }
  // A pointer to database (LinkedList)
  LinkedList *lst = markov_chain->database;
  // A pointer to the first node, that will go over the list
  Node *head = lst->first;
  // The size of the list
  int len = lst->size;
  Node *check = check_in_data_base (markov_chain, head, data_ptr, len);
  // If data_ptr in markov_chain, return it's node
  if (check != NULL)
  {
    return check;
  }
    // else create new node ,add to end of markov_chain's database, return it.
  else
  {
    MarkovNode *new_markov_node = create_new_node (data_ptr,
                                                   markov_chain->copy_func);
    if (new_markov_node == NULL)
    {
      free (new_markov_node);
      return NULL;
    }
    int temp = add (lst, new_markov_node);
    // check if adding was successful
    if (temp == TRUE)
    { return lst->last; }
    else
    {
      // Free all the date
      free (new_markov_node);
      new_markov_node = NULL;
      return NULL;
    }
  }
}

Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  Node *check = check_in_data_base (markov_chain,
                                    markov_chain->database->first,
                                    data_ptr, markov_chain->database->size);

  // Return according to API
  return check;

}

int check_in_frequencies_list (MarkovNode *first_node, const void
*check_data, comp_func comp)
{
  // Checks if first
  if (first_node->frequencies_list == NULL)
  { return NOT_IN; }

  // start the runners
  int count = 0;
  MarkovNodeFrequency *cur = first_node->frequencies_list;

  // Loop throw the frequencies list
  while (count < first_node->length)
  {
    // Checks if data are equal
    if (comp (cur->markov_node->markov_data, check_data) == 0)
    {
      return count;
    }
    // Update runner and count
    cur++;
    count++;
  }
  return NOT_IN;
}
/**
 * Creates and adds node to frequencies_list
 * @param first_node The first node we are adding to
 * @param second_node the node we are adding
 * @return true on success, false otherwise
 */
bool new_frequencies_list (MarkovNode *first_node, MarkovNode
*second_node)
{
  int size = first_node->length;

  // Create new memory for the new node
  MarkovNodeFrequency *lst =
      calloc (size, sizeof (MarkovNodeFrequency));
  // Check allocation of memory
  if (lst == NULL)
  {
    return false;
  }
  else
  {
    // Add new node to the list
    first_node->frequencies_list = lst;
    first_node->frequencies_list[size - 1] = (MarkovNodeFrequency)
        {second_node, 1};
    return true;
  }

}
/**
 * adds memory to the lst , and adds the second node to the new memory
 * @param first_node The first node we are adding to
 * @param second_node the node we are adding
 * @return true on success, false otherwise
 */
bool extend_frequencies_list (MarkovNode *first_node, MarkovNode
*second_node)
{
  int len = first_node->length;

  // Extend memory for the new node
  MarkovNodeFrequency *temp = realloc (first_node->frequencies_list,
                                       sizeof (MarkovNodeFrequency) *
                                       len);
  // check if allocation correct
  if (temp == NULL)
  {
    /* check this function tht no need for free here */

    // Free up all memory
    for (int i = 0; i < len; i++)
    {
      free (first_node->frequencies_list->markov_node);
      first_node->frequencies_list += 1;
    }
    free (first_node->frequencies_list);
    first_node->frequencies_list = NULL;
    return false;
  }

  // Add new node to the list
  first_node->frequencies_list = temp;
  first_node->frequencies_list[len - 1] = (MarkovNodeFrequency)
      {second_node, 1};
  return true;

}

/**
 * Creates and adds node to frequencies_list
 * @param first_node The first node we are adding to
 * @param second_node the node we are adding
 * @return true on success, false otherwise
 */
bool add_node_to_frequencies_list (MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{
  int place = check_in_frequencies_list (first_node,
                                         second_node->markov_data,
                                         markov_chain->comp_func);
  // If already in list
  if (place != NOT_IN)
  {
    // Adds to frequency count
    first_node->frequencies_list[place].frequency += 1;
    return true;
  }
  else
    //else Add the second markov_node to the frequency list of the first
  {
    // Update length of list
    first_node->length += 1;

    // Checks if first
    if (first_node->frequencies_list == NULL)
    {
      // Allocate new memory and second node
      bool ans = new_frequencies_list (first_node, second_node);
      return ans;
    }
    else
    {
      bool ans = extend_frequencies_list (first_node, second_node);
      return ans;
    }
  }
}

void free_database (MarkovChain **markov_chain)
{
  free_data free_data = (*markov_chain)->free_data;
  //int size = (*ptr_chain)->database->size;
  Node *temp = NULL;
  // Free inner data
  Node *current = (*markov_chain)->database->first;
  while (current != NULL)
  {
    // Free the frequencies list
    if (current->data->frequencies_list != NULL)
    {
      free (current->data->frequencies_list);
      current->data->frequencies_list = NULL;
    }
    // Free the markov data
    if (current->data->markov_data != NULL)
    {
      free_data (current->data->markov_data);
      current->data->markov_data = NULL;
    }
    temp = current->next;
    // Free the markov node
    if (current->data != NULL)
    {
      free (current->data);
      current->data = NULL;
    }
    // Move to the next node and free the current node
    free (current);
    current = temp;
  }

  // Free the data base
  free ((*markov_chain)->database);
  (*markov_chain)->database = NULL;

  // Free the chain
  free ((*markov_chain));
  (*markov_chain) = NULL;
}

/**
 * goes to the number i node in lst
 * @param lst a link list of nodes
 * @param i the number node the method returns
 * @return lst[i]
 */
Node *go_to_node_number (LinkedList *lst, int i)
{
  // Declaring Node runner and counter
  Node *cur = lst->first;
  int count = 0;

  // Loop until counter at number
  while (count < i)
  {
    // Update runner and counter
    cur = cur->next;
    count++;
  }
  return cur;
}

int get_random_number (int max_number)
{
  return rand () % max_number;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int max_num = markov_chain->database->size;

  // Get a random number
  int i = get_random_number (max_num);

  // Loop until first Node is valid
  MarkovNode *start = NULL;
  while (start == NULL)
  {
    // goes to the node number i in list
    Node *temp = go_to_node_number (markov_chain->database, i);

    // if the markov data is not a ending Node
    if (markov_chain->is_last (((const void *) temp->data->markov_data)) ==
        false)
    {
      // Is not a ending Node - so we can return
      start = temp->data;
      return start;
    }
    else
      // get a new number
    { i = get_random_number (max_num); }
  }
  return NULL;
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  // Declaring values
  int size = state_struct_ptr->length;
  int total = 0;

  // sum al the frequency of node
  for (int i = 0; i < size; i++)
  {
    total += state_struct_ptr->frequencies_list[i].frequency;
  }
  // get random number
  int random_number = get_random_number (total);

  // check if there is only one possibility
  if (size == ONE_OPTION)
  { return state_struct_ptr->frequencies_list->markov_node; }

  // start pointer to next node
  MarkovNode *next_node = NULL;

  // generate the random number bt odds
  for (int j = 0; j < total; j++)
  {
    if (random_number < state_struct_ptr->frequencies_list[j].frequency)
    {
      next_node = state_struct_ptr->frequencies_list[j].markov_node;
      break;
    }
    random_number -= state_struct_ptr->frequencies_list[j].frequency;
  }
  return next_node;
}

void generate_tweet (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  // Starting the counter of tweets.
  int count = 0;

  //if NULL- choose a random markov_node
  while (first_node == NULL)
  { first_node = get_first_random_node (markov_chain); }

  // print the first node
  markov_chain->print_func (first_node->markov_data);
  count++;

  // Getting the second node
  MarkovNode *next = first_node;

  // Loop until the Max amount of Nodes printed
  while (count < max_length)
  {
    next = get_next_random_node (next);
    // if its the last node of sentence
    if (markov_chain->is_last (next->markov_data) == true)
    {
      markov_chain->print_func (next->markov_data);
      break;
    }
    else
    {
      // print the Node
      markov_chain->print_func (next->markov_data);

      // get the next node ,update counter
      //next = get_next_random_node (next);
      count++;
    }
  }
}
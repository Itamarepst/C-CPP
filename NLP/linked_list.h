#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
#include <stdlib.h> // For malloc()

typedef struct Node {
    struct MarkovNode *data;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *first;
    Node *last;
    int size;
} LinkedList;

/**
 * Add data to new markov_node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkedList *link_list, void *data);

#endif //_LINKEDLIST_H_


//טסט ראשון:
//Tweet 1: midterms with ponytails out in front and amazing mask its 95% of all day before.
//טסט שני:
//Tweet 1: than perfect.
//Tweet 2: better than perfect.
//טסט שלישי:
//Tweet 1: proudly! #justdoit.


//MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
//{
//  int total_occurrences = 0;
//  int frequency_length = state_struct_ptr->freq_length;
//  for (int i =0; i<frequency_length; i++)
//  {
//    total_occurrences+=state_struct_ptr->frequencies_list[i].frequency;
//  }
//  int rand_num = get_random_number(total_occurrences)+1;
//  if(frequency_length==1)
//  {
//    return state_struct_ptr->frequencies_list->markov_node;
//  }
//  MarkovNode *final_node = 0;
//  for(int j=0; j<frequency_length; j++)
//  {
//    if (rand_num<=state_struct_ptr->frequencies_list[j].frequency)
//    {
//      final_node = state_struct_ptr->frequencies_list[j].markov_node;
//      break;
//    }
//    rand_num-=state_struct_ptr->frequencies_list[j].frequency;
//  }
//  return final_node;
//}
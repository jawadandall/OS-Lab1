// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* mylist = (list_t *)malloc(sizeof(list_t)); 
  mylist->head = NULL;
  return mylist;  // Return the allocated list
}

void list_free(list_t *l) {
  if (l == NULL) {
    return;  // Null list
  }

  node_t *current = l->head;
  node_t *next_node;

  // Traverse the list and free each node
  while (current != NULL) {
    next_node = current->next;  // Store the next node
    free(current);              // Free the current node
    current = next_node;        // Move to the next node
  }

  // Finally, free the list structure itself
  free(l);
}

char * listToString(list_t *l) {
  if (l == NULL || l->head == NULL) {
    char* empty = (char *)malloc(sizeof(char) * 10);
    strcpy(empty, "NULL");
    return empty;  // Return "NULL" for an empty list
  }

  char* buf = (char *)malloc(sizeof(char) * 1024); // Buffer for storing the string
  char tbuf[20];
 // buf[0] = '\0';  // Initialize the buffer as an empty string

  node_t* curr = l->head;
  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    strcat(buf, tbuf);
    curr = curr->next;
  }
  strcat(buf, "NULL");  // Append "NULL" to the end of the string

  return buf;  // Return the list string
}


void list_print(list_t *l) {
  if (l == NULL || l->head == NULL) {
    printf("List is empty.\n");
    return;  // Handle null or empty list
  }

  node_t *current = l->head;

  // Traverse the list and print each element
  while (current != NULL) {
    printf("%d ", current->value);  // Print the value of the current node
    /*if (current->next != NULL) {
      printf(" ");  // Print a space if there's a next node
    }*/
    current = current->next;  // Move to the next node
  }
  printf("\n");  // Newline after printing the list
}

int list_length(list_t *l) {
  if (l == NULL || l->head == NULL) {
    return 0;  // Return 0 for a null or empty list
  }

  int length = 0;
  node_t *current = l->head;

  // Traverse the list and count the nodes
  while (current != NULL) {
    length++;  // Increment the length for each node
    current = current->next;
  }

  return length;
}

void list_add_to_back(list_t *l, elem value) {
  if (l == NULL) {
    return;  // Handle null list
  }

  // Allocate a new node
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  if (new_node == NULL) {
    return;  // Handle memory allocation failure
  }

  new_node->value = value;
  new_node->next = NULL;  // New node is the last, so next is NULL

  // If the list is empty, the new node becomes the head
  if (l->head == NULL) {
    l->head = new_node;
    return;
  }

  // Traverse the list to find the last node
  node_t *current = l->head;
  while (current->next != NULL) {
    current = current->next;
  }

  // Set the next of the last node to the new node
  current->next = new_node;
}

void list_add_to_front(list_t *l, elem value) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        return;  // Handle memory allocation failure
    }

    new_node->value = value;
    new_node->next = l->head;
    l->head = new_node;
}

void list_add_at_index(list_t *l, elem value, int index) {
  if (l == NULL || index < 1) {
    return;  // Handle null list or invalid index
  }

  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  if (new_node == NULL) {
    return;  // Handle memory allocation failure
  }
  new_node->value = value;
  new_node->next = NULL;

  // Special case: adding at index 1 (the new head)
  if (index == 1) {
    new_node->next = l->head;
    l->head = new_node;
    return;
  }

  // Traverse to the (index - 1)-th node
  node_t *current = l->head;
  int current_index = 1;

  while (current != NULL && current_index < index - 1) {
    current = current->next;
    current_index++;
  }

  // If index is larger than the list length, append to the end
  /*if (current == NULL) {
    list_add_to_back(l, value);
  }*/ 
  
  new_node->next = current->next;
  current->next = new_node;
  
}

elem list_remove_from_back(list_t *l) {
  if (l == NULL || l->head == NULL) {
    return -1;  // Handle null or empty list
  }

  node_t *current = l->head;
  elem removed_value;

  // Special case: only one element in the list
  if (current->next == NULL) {
    removed_value = current->value;
    free(current);
    l->head = NULL;
    return removed_value;
  }

  // Traverse the list to find the second-to-last node
  node_t *prev = NULL;
  while (current->next != NULL) {
    prev = current;
    current = current->next;
  }

  // Remove the last node
  removed_value = current->value;
  free(current);
  prev->next = NULL;  // Set the second-to-last node's next to NULL

  return removed_value;
}

elem list_remove_from_front(list_t *l) {
  if (l == NULL || l->head == NULL) {
    return -1;  // Handle null or empty list
  }

  node_t *old_head = l->head;
  elem removed_value = old_head->value;

  // Move the head to the next node
  l->head = old_head->next;

  // Free the old head node
  free(old_head);

  return removed_value;  // Return the value of the removed node
}

elem list_remove_at_index(list_t *l, int index) {
  if (l == NULL || l->head == NULL || index < 1) {
    return -1;  // Handle null list, empty list, or invalid index
  }

  node_t *current = l->head;
  elem removed_value;

  // Special case: removing the first element (index 1)
  if (index == 1) {
    removed_value = current->value;
    l->head = current->next;  // Move the head to the next node
    free(current);  // Free the old head node
    return removed_value;
  }

  // Traverse to the (index - 1)-th node
  node_t *prev = NULL;
  int current_index = 1;

  while (current != NULL && current_index < index) {
    prev = current;
    current = current->next;
    current_index++;
  }

  // If index is out of bounds, return -1
  if (current == NULL) {
    return -1;
  }

  // Remove the node at the specified index
  removed_value = current->value;
  prev->next = current->next;  // Adjust the next pointer of the previous node
  free(current);  // Free the node being removed

  return removed_value;
}

bool list_is_in(list_t *l, elem value) {
  if (l == NULL || l->head == NULL) {
    return false;  // Handle null or empty list
  }

  node_t *current = l->head;

  // Traverse the list to check if value exists
  while (current != NULL) {
    if (current->value == value) {
      return true;  // Value found
    }
    current = current->next;
  }

  return false;  // Value not found 
}

elem list_get_elem_at(list_t *l, int index) {
  if (l == NULL || l->head == NULL || index < 1) {
    return -1;  // Handle null, empty list, or invalid index
  }

  node_t *current = l->head;
  int current_index = 1;

  // Traverse the list until we reach the given index
  while (current != NULL) {
    if (current_index == index) {
      return current->value;  // Return the value at the given index
    }
    current_index++;
    current = current->next;
  }

  return -1;  // Index out of bounds
}

int list_get_index_of(list_t *l, elem value) {
  if (l == NULL || l->head == NULL) {
    return -1;  // Handle null or empty list
  }

  node_t *current = l->head;
  int index = 1;

  // Traverse the list to find the value
  while (current != NULL) {
    if (current->value == value) {
      return index;  // Return the index where the value is found
    }
    index++;
    current = current->next;
  }

  return -1;  // Value not found
}

#ifndef __POINTER_LIST_H__
#define __POINTER_LIST_H__

#include "base.h"

// Represents a single list node.
typedef struct Node {
    void* value;
    struct Node* next; // self-reference
} Node;

// Create a list node.
Node* new_node(void* value, Node* next);

// Returns a dynamically allocated string representation of the element, which the caller has to release.
typedef String (*ToStringFun)(void* element);

// Print the elements of the given list. Use to_string to print an element.
void print_list(Node* list, ToStringFun to_string);

// Print list followed by a newline. Use to_string to print an element.
void println_list(Node* list, ToStringFun to_string);

// Frees the element.
typedef void (*FreeFun)(void* element);

// Free all nodes of the list. Use free_element to free an element.
// If free_element is NULL, do not free the elements.
void free_list(Node* list, FreeFun free_element);

// Compares two elements.
typedef bool (*EqualFun)(void* element1, void* element2);

// Number of elements of the list.
int length_list(Node* list);

// Check whether list contains element. Use equal 
// to compare elements. If equal is NULL compare pointers.
bool contains_list(Node* list, void* element, EqualFun equal);

// Remove element at position index from list. Use free_element to release the element.
// If free_element is NULL, do not release the element.
Node* remove_list(Node* list, int index, FreeFun free_element);

// Prepend an element in front of the list.
Node* prepend_list(void* value, Node* list);

// Adds an element to the end of the list. Modifies the existing list.
Node* append_list(Node* list, void* value);

// Copies an element.
typedef void* (*CopyFun)(void* element);

// Copies the list. Use copy_element to copy each element.
// If copy_element is NULL, just assign the pointer to the original element.
Node* copy_list(Node* list, CopyFun copy_element);

// Insert value in list at index.
Node* insert_list(Node* list, int index, void* value);

// Returns =0 if x and y are equal, <0 if x smaller than y, >0 otherwise.
typedef int (*CompFun)(void* x, void*y);

// Insert value at the right position. Use cmp to compare elements.
Node* insert_ordered(Node* list, void* value, CompFun cmp);

// Reverse the origial (!) list.
Node* reverse_list(Node* list);

// Check whether element satisfies a predicate.
typedef bool (*FilterFun)(void* element, int i, void* x);

// Find the first element in list for which pred(element, i, x) is true.
void* find_list(Node* list, FilterFun pred, void* x);

///////////////////////////////////////////////////////////////////////////////

// Transform element into something else.
typedef void* (*MapFun)(void* element, int i, void* x);

// Map each list element to f(element, index, x).
Node* map_list(Node* list, MapFun f, void* x);

// Produce a list of those elements of list that satisfy the predicate.
Node* filter_list(Node* list, FilterFun predicate, void* x);

// First filter list using predicate and then map the filtered elements using map.
Node* filter_map_list(Node* list, FilterFun predicate, MapFun map, void* x);

// Aggregate a list: state is the intermediary aggregation result, element is the current element.
typedef void (*ReduceFun)(void* state, void* element, int index);

// Aggregate the list using f. Modifies state.
void reduce_list(Node* list, ReduceFun f, void* state);

#endif
